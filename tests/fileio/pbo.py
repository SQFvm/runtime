import pathlib
import struct
import subprocess
import sys
import tempfile
import unittest

VM = str(pathlib.Path(sys.argv.pop(1)).resolve())

def write_pbo(path, files, prefix=r"a3\ui_f", extra_size=0):
    header = b"\0" + struct.pack("<5I", 0x56657273, 0, 0, 0, 0)
    if prefix is not None:
        header += b"prefix\0" + prefix.encode("ascii") + b"\0"
    header += b"\0"
    body = b""
    for name, content in files.items():
        payload = content.encode("ascii")
        header += name.encode("ascii") + b"\0" + struct.pack("<5I", 0, 0, 0, 0, len(payload) + extra_size)
        body += payload
    path.write_bytes(header + b"\0" + struct.pack("<5I", 0, 0, 0, 0, 0) + body)
    return path


class PboTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory(prefix="sqfvm-pbo-")
        self.addCleanup(self.directory.cleanup)
        self.root = pathlib.Path(self.directory.name)
        self.pbo = write_pbo(self.root / "fixture.pbo", {
            r"hpp\defineDIKCodes.inc": "#define DIK_ESCAPE 1\n",
            r"hpp\Base.inc": "#define PBO_VALUE 42\n",
            r"hpp\Empty.inc": "",
            r"hpp\Outer.inc": '#include "sub\\Inner.inc"\n',
            r"hpp\sub\Inner.inc": '#include "..\\Base.inc"\n',
            r"hpp\Rooted.inc": '#include "\\A3\\UI_F\\HPP\\BASE.INC"\n',
            r"hpp\Cycle.inc": '#include "./CYCLE.INC"\n',
            r"hpp\Missing.inc": '#include "DoesNotExist.inc"\n',
            r"text\Message.txt": "PBO contents",
        })

    def run_vm(self, code, archives=None, mappings=()):
        command = [VM, "-a", "--suppress-welcome", "--no-work-print", "--no-load-executable-dir"]
        for archive in [self.pbo] if archives is None else archives:
            command += ["--input-pbo", str(archive)]
        for mapping in mappings:
            command += ["-v", mapping]
        return subprocess.run(command + ["--sqf", code], cwd=self.root,
            capture_output=True, text=True, timeout=5)

    def assert_success(self, process, expected):
        output = process.stdout + process.stderr
        self.assertEqual(process.returncode, 0, output)
        self.assertNotRegex(output, r"\[(?:FAT|ERR|WRN)\]")
        self.assertIn("[DIAG_LOG] " + str(expected), output)

    def test_rooted_unrooted_mixed_case_and_separators(self):
        for path in [r"a3\ui_f\hpp\defineDIKCodes.inc", r"\a3\ui_f\hpp\defineDIKCodes.inc",
                     "a3/ui_f/hpp/defineDIKCodes.inc", "/a3/ui_f/hpp/defineDIKCodes.inc",
                     r"\A3\UI_F\HPP\DEFINEDIKCODES.INC", "///A3/UI_F/HPP/defineDIKCodes.inc",
                     "/a3/ui_f/hpp/./sub/../defineDIKCodes.inc"]:
            with self.subTest(path=path):
                self.assert_success(self.run_vm('#include "' + path + '"\ndiag_log DIK_ESCAPE;'), 1)

    def test_loadfile_uses_original_member_name(self):
        for path in [r"\A3\UI_F\TEXT\MESSAGE.TXT", "a3/ui_f/text/Message.txt"]:
            with self.subTest(path=path):
                self.assert_success(self.run_vm('diag_log loadFile "' + path + '";'), "PBO contents")

    def test_nested_relative_includes_in_one_archive(self):
        self.assert_success(self.run_vm('#include "\\a3\\ui_f\\hpp\\Outer.inc"\ndiag_log PBO_VALUE;'), 42)

    def test_nested_rooted_include_in_one_archive(self):
        self.assert_success(self.run_vm('#include "\\a3\\ui_f\\hpp\\Rooted.inc"\ndiag_log PBO_VALUE;'), 42)

    def test_empty_member_is_valid(self):
        self.assert_success(self.run_vm('#include "\\a3\\ui_f\\hpp\\Empty.inc"\ndiag_log 42;'), 42)

    def test_real_include_cycle_is_still_rejected(self):
        result = self.run_vm('#include "\\a3\\ui_f\\hpp\\Cycle.inc"\ndiag_log 42;')
        self.assertNotEqual(result.returncode, 0, result.stdout)
        self.assertIn("[ERR]", result.stdout)
        self.assertNotIn("[DIAG_LOG] 42", result.stdout)

    def test_missing_nested_include_is_not_an_empty_success(self):
        result = self.run_vm('#include "\\a3\\ui_f\\hpp\\Missing.inc"\ndiag_log 42;')
        self.assertNotEqual(result.returncode, 0, result.stdout)
        self.assertIn("DoesNotExist.inc", result.stdout)
        self.assertNotIn("[DIAG_LOG] 42", result.stdout)

    def test_prefix_must_match_a_complete_component(self):
        result = self.run_vm('#include "\\a3\\ui_f_other\\hpp\\Base.inc"\ndiag_log 42;')
        self.assertNotEqual(result.returncode, 0, result.stdout)
        self.assertNotIn("[DIAG_LOG] 42", result.stdout)

    def test_several_archives_can_share_a_prefix(self):
        second = write_pbo(self.root / "second.pbo", {r"hpp\Second.inc": '#include "Base.inc"\n'})
        self.assert_success(self.run_vm('#include "\\a3\\ui_f\\hpp\\Second.inc"\ndiag_log PBO_VALUE;',
            archives=[self.pbo, second]), 42)

    def test_forward_slash_prefix(self):
        archive = write_pbo(self.root / "forward.pbo", {"hpp/Forward.inc": "#define PBO_VALUE 42\n"}, prefix="/a3/forward/")
        self.assert_success(self.run_vm('#include "\\A3\\FORWARD\\HPP\\FORWARD.INC"\ndiag_log PBO_VALUE;', archives=[archive]), 42)

    def test_missing_archive_fails_even_without_an_include(self):
        missing = self.root / "missing.pbo"
        result = self.run_vm('diag_log 42;', archives=[missing])
        self.assertNotEqual(result.returncode, 0, result.stdout)
        self.assertNotIn("[DIAG_LOG] 42", result.stdout)
        self.assertFalse(missing.exists(), "Mounting a missing PBO must not create it")

    def test_archive_without_prefix_is_not_silently_ignored(self):
        archive = write_pbo(self.root / "no-prefix.pbo", {"empty.inc": ""}, prefix=None)
        result = self.run_vm('diag_log 42;', archives=[archive])
        self.assertNotEqual(result.returncode, 0, result.stdout)

    def test_truncated_include_reports_member_and_fails(self):
        archive = write_pbo(self.root / "truncated.pbo", {"broken.inc": ""}, extra_size=100)
        result = self.run_vm('#include "\\a3\\ui_f\\broken.inc"\ndiag_log 42;', archives=[archive])
        self.assertNotEqual(result.returncode, 0, result.stdout)
        self.assertIn("broken.inc", result.stdout + result.stderr)
        self.assertNotIn("[DIAG_LOG] 42", result.stdout)

    def test_truncated_runtime_read_cannot_exit_successfully(self):
        archive = write_pbo(self.root / "truncated.pbo", {"broken.inc": ""}, extra_size=100)
        result = self.run_vm('diag_log loadFile "\\a3\\ui_f\\broken.inc"; exitcode__ 0;', archives=[archive])
        self.assertNotEqual(result.returncode, 0, result.stdout)
        self.assertIn("[FAT]", result.stdout)
        self.assertIn("broken.inc", result.stdout)

    def test_directory_mapping_continues_to_resolve_files(self):
        (self.root / "Local.inc").write_text("#define LOCAL_VALUE 7\n", encoding="ascii")
        self.assert_success(self.run_vm('#include "\\local\\Local.inc"\ndiag_log LOCAL_VALUE;',
            mappings=[str(self.root) + "|/local"]), 7)


if __name__ == "__main__":
    unittest.main()
