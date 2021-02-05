import os
import subprocess

cba_git_hash = 'ba5c8ea6228a44615172fadb5d8fcdfb5bc0f03e'

def replace_in_file(file, old, new):
    fin = open(file, "rt")
    data = fin.read()
    fin.close()
    data = data.replace(old, new)

    fout = open(file, "wt")
    fout.write(data)
    fout.close()

current_dir = os.path.dirname(os.path.realpath(__file__))
cba_dir = os.path.join(current_dir, 'cba_a3')
a3_dir = os.path.join(cba_dir, 'include', 'a3')
addons_dir = os.path.join(cba_dir, 'addons')

if os.path.exists('cba_a3'):
    subprocess.call(['git', '-C', cba_dir, 'fetch'])
else:
    subprocess.call(['git', 'clone', 'https://github.com/CBATeam/CBA_A3.git', cba_dir])

subprocess.call(['git', '-C', cba_dir, 'reset', '--hard', cba_git_hash])

# Enable diag_log instead of CBA_fnc_log
replace_in_file(os.path.join(addons_dir, 'main', 'script_mod.hpp'), '#define PREFIX cba', '#define PREFIX cba\n#define DEBUG_SYNCHRONOUS')

# Change hashes tests to call instead of spawn
replace_in_file(os.path.join(addons_dir, 'hashes', 'test.sqf'), '0 spawn', 'call')

# Disable tests currently not working
replace_in_file(os.path.join(addons_dir, 'arrays', 'test.sqf'), '"findTypeName", ', '')
replace_in_file(os.path.join(addons_dir, 'common', 'test.sqf'), '"config", ', '')
replace_in_file(os.path.join(addons_dir, 'common', 'test.sqf'), '"inventory", ', '')
replace_in_file(os.path.join(addons_dir, 'common', 'test.sqf'), '"position", ', '')
replace_in_file(os.path.join(addons_dir, 'common', 'test.sqf'), '"weaponComponents", ', '')
replace_in_file(os.path.join(addons_dir, 'diagnostic', 'test.sqf'), ', "parameters"', '')
replace_in_file(os.path.join(addons_dir, 'events', 'test.sqf'), '"globalEventJIP"', '')
replace_in_file(os.path.join(addons_dir, 'hashes', 'test.sqf'), '"parseJSON", ', '')
replace_in_file(os.path.join(addons_dir, 'network', 'test.sqf'), '"network"', '')

# Dummy workarounds for commands not yet implemented
replace_in_file(os.path.join(addons_dir, 'arrays', 'fnc_findNull.sqf'), 'controlNull, ', '')
replace_in_file(os.path.join(addons_dir, 'arrays', 'fnc_findNull.sqf'), 'displayNull, ', '')
replace_in_file(os.path.join(addons_dir, 'arrays', 'fnc_findNull.sqf'), 'locationNull, ', '')
replace_in_file(os.path.join(addons_dir, 'arrays', 'fnc_findNull.sqf'), 'taskNull, ', '')
replace_in_file(os.path.join(addons_dir, 'arrays', 'fnc_findNull.sqf'), 'teamMemberNull, ', '')
replace_in_file(os.path.join(addons_dir, 'arrays', 'test_findNil.sqf'), 'displayNull', 'missionNamespace')
replace_in_file(os.path.join(addons_dir, 'arrays', 'test_findNull.sqf'), 'displayNull', 'missionNamespace')
replace_in_file(os.path.join(addons_dir, 'arrays', 'test_findTypeName.sqf'), 'controlNull, ', '')
replace_in_file(os.path.join(addons_dir, 'arrays', 'test_findTypeName.sqf'), 'displayNull, ', '')
replace_in_file(os.path.join(addons_dir, 'arrays', 'test_findTypeName.sqf'), 'locationNull, ', '')
replace_in_file(os.path.join(addons_dir, 'arrays', 'test_findTypeName.sqf'), 'taskNull, ', '')
replace_in_file(os.path.join(addons_dir, 'common', 'fnc_createNamespace.sqf'), 'createLocation ["CBA_NamespaceDummy", DUMMY_POSITION, 0, 0]', '"CBA_NamespaceDummy" createVehicle DUMMY_POSITION')
replace_in_file(os.path.join(addons_dir, 'common', 'fnc_createNamespace.sqf'), 'createSimpleObject ["CBA_NamespaceDummy", DUMMY_POSITION]', '"CBA_NamespaceDummy" createVehicle DUMMY_POSITION')
replace_in_file(os.path.join(addons_dir, 'common', 'test_macro_is_x.sqf'), 'controlNull', '""')
replace_in_file(os.path.join(addons_dir, 'common', 'test_macro_is_x.sqf'), 'displayNull', '""')
replace_in_file(os.path.join(addons_dir, 'common', 'test_macro_is_x.sqf'), 'locationNull', '""')
replace_in_file(os.path.join(addons_dir, 'common', 'test_macro_is_x.sqf'), 'missionconfigfile', 'configFile')
replace_in_file(os.path.join(addons_dir, 'diagnostic', 'fnc_error.sqf'), 'QGVAR(Error) cutRsc [QGVAR(Error), "PLAIN"];', '')
replace_in_file(os.path.join(addons_dir, 'diagnostic', 'fnc_error.sqf'), '_control ctrlSetStructuredText composeText _compose;', '')
replace_in_file(os.path.join(addons_dir, 'main', 'script_macros_common.hpp'), 'controlNull', '""')
replace_in_file(os.path.join(addons_dir, 'main', 'script_macros_common.hpp'), 'displayNull', '""')
replace_in_file(os.path.join(addons_dir, 'main', 'script_macros_common.hpp'), 'locationNull', '""')
replace_in_file(os.path.join(addons_dir, 'strings', 'fnc_formatNumber.sqf'), 'localize', '')
replace_in_file(os.path.join(addons_dir, 'vectors', 'fnc_vectAdd.sqf'), 'BIS_fnc_vectorAdd', '{}')
replace_in_file(os.path.join(addons_dir, 'vectors', 'fnc_vectDot.sqf'), 'BIS_fnc_dotProduct', '{}')

sqfvm_binary = 'sqfvm'
sqfvm_test_script = os.path.join(current_dir, 'cba_a3.sqf')

sqfvm = [sqfvm_binary, '--automated']

# Map a3 and cba folders into Arma prefixes
sqfvm.extend(['--virtual', '{}|\\a3'.format(a3_dir)])
sqfvm.extend(['--virtual', '{}|\\x\\cba\\addons'.format(addons_dir)])

# Load addon configs
for addon in os.listdir(addons_dir):
  sqfvm.extend(['--input-config', '{}/{}/config.cpp'.format(addons_dir, addon)])

sqfvm.extend(['--input-sqf', sqfvm_test_script])

def run(cmd):
    proc = subprocess.Popen(cmd, stdout = subprocess.PIPE, stderr = subprocess.STDOUT, universal_newlines = True)

    for stdout_line in iter(proc.stdout.readline, ""):
        yield stdout_line.strip()

    proc.stdout.close()
    return_code = proc.wait()

    if return_code:
        raise subprocess.CalledProcessError(return_code, cmd)

tests_passed = []
tests_failed = []

for line in run(sqfvm):
    if "Test OK" in line:
        tests_passed.append(line)
    if "Test FAIL" in line:
        tests_failed.append(line)

    if "defined twice" not in line and "Unexpected IFDEF" not in line and "The provided inherited config node was not located on current or upper levels" not in line:
        print(line)

if len(tests_failed):
    print('==== Failing tests ====')
    for test in tests_failed:
        print(test)

print('''
==== CBA A3 tests summary ====
Tests passed: %d
Tests failed: %d
==============================
''' % (len(tests_passed), len(tests_failed)))
