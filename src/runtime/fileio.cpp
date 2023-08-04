#include "fileio.h"

#include <fstream>
#include <vector>
#include <filesystem>

#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
#include <iostream>
#endif // DF__SQF_FILEIO__TRACE_REESOLVE


namespace {
    enum class byte_order_mark {
        NONE,
        UTF8,       // 0xEF 0xBB 0xBF
        UTF16BE,    // 0xFF 0xFF
        UTF16LE,    // 0xFF 0xFE
        UTF32BE,    // 0x00 0x00 0xFE 0xFF
        UTF32LE,    // 0xFF 0xFE 0x00 0x00
        UTF7,       // 0x2B 0x2F 0x76
        UTF1,       // 0xF7 0x64 0x4C
        UTFEBCDIC,  // 0xDD 0x73 0x66 0x73
        SCSU,       // 0x0E 0xFE 0xFF
        BOCU1,      // 0xFB 0xEE 0x28
        GB18030,    // 0x84 0x31 0x95 0x33
    };

    static bool begins_with(const std::vector<char> &buff, uint8_t b1, uint8_t b2) {
        if (buff.size() < 2)
            return false;
        return buff[0] == b1 && buff[1] == b2;
    }

    static bool begins_with(const std::vector<char> &buff, uint8_t b1, uint8_t b2, uint8_t b3) {
        if (buff.size() < 3)
            return false;
        return buff[0] == b1 && buff[1] == b2 && buff[2] == b3;
    }

    static bool begins_with(const std::vector<char> &buff, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
        if (buff.size() < 4)
            return false;
        return buff[0] == b1 && buff[1] == b2 && buff[2] == b3 && buff[3] == b4;
    }

    static byte_order_mark get_byte_order_mark(const std::vector<char> &buff) {
        if (begins_with(buff, 0x00, 0x00, 0xFE, 0xFF)) return byte_order_mark::UTF32BE;
        if (begins_with(buff, 0xFF, 0xFE, 0x00, 0x00)) return byte_order_mark::UTF32LE;
        if (begins_with(buff, 0xDD, 0x73, 0x66, 0x73)) return byte_order_mark::UTFEBCDIC;
        if (begins_with(buff, 0x84, 0x31, 0x95, 0x33)) return byte_order_mark::GB18030;
        if (begins_with(buff, 0xEF, 0xBB, 0xBF)) return byte_order_mark::UTF8;
        if (begins_with(buff, 0x2B, 0x2F, 0x76)) return byte_order_mark::UTF7;
        if (begins_with(buff, 0xF7, 0x64, 0x4C)) return byte_order_mark::UTF1;
        if (begins_with(buff, 0x0E, 0xFE, 0xFF)) return byte_order_mark::SCSU;
        if (begins_with(buff, 0xFB, 0xEE, 0x28)) return byte_order_mark::BOCU1;
        if (begins_with(buff, 0xFF, 0xFF)) return byte_order_mark::UTF16BE;
        if (begins_with(buff, 0xFF, 0xFE)) return byte_order_mark::UTF16LE;
        return byte_order_mark::NONE;
    }

    static int get_bom_skip(const std::vector<char> &buff) {
        if (buff.empty())
            return 0;
        // We are comparing against unsigned
        auto ubuff = reinterpret_cast<const unsigned char *>(buff.data());
        if (ubuff[0] == 0xEF && ubuff[1] == 0xBB && ubuff[2] == 0xBF) {
            //UTF-8
            return 3;
        } else if (ubuff[0] == 0xFE && ubuff[1] == 0xFF) {
            //UTF-16 (BE)
            return 2;
        } else if (ubuff[0] == 0xFF && ubuff[1] == 0xFE) {
            //UTF-16 (LE)
            return 2;
        } else if (ubuff[0] == 0x00 && ubuff[1] == 0x00 && ubuff[2] == 0xFF && ubuff[3] == 0xFF) {
            //UTF-32 (BE)
            return 2;
        } else if (ubuff[0] == 0xFF && ubuff[1] == 0xFF && ubuff[2] == 0x00 && ubuff[3] == 0x00) {
            //UTF-32 (LE)
            return 2;
        } else if (ubuff[0] == 0x2B && ubuff[1] == 0x2F && ubuff[2] == 0x76 &&
                   (ubuff[3] == 0x38 || ubuff[3] == 0x39 || ubuff[3] == 0x2B || ubuff[3] == 0x2F)) {
            //UTF-7
            return 4;
        } else if (ubuff[0] == 0xF7 && ubuff[1] == 0x64 && ubuff[2] == 0x4C) {
            //UTF-1
            return 3;
        } else if (ubuff[0] == 0xDD && ubuff[1] == 0x73 && ubuff[2] == 0x66 && ubuff[3] == 0x73) {
            //UTF-EBCDIC
            return 3;
        } else if (ubuff[0] == 0x0E && ubuff[1] == 0xFE && ubuff[2] == 0xFF) {
            //SCSU
            return 3;
        } else if (ubuff[0] == 0xFB && ubuff[1] == 0xEE && ubuff[2] == 0x28) {
            //BOCU-1
            if (ubuff[3] == 0xFF)
                return 4;
            return 3;
        } else if (ubuff[0] == 0x84 && ubuff[1] == 0x31 && ubuff[2] == 0x95 && ubuff[3] == 0x33) {
            //GB 18030
            return 3;
        }
        return 0;
    }

    static bool file_exists(std::string_view filename) {
        std::ifstream infile(filename.data());
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
        std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
            "        " <<
            "        " <<
            "    " << "\x1B[36mfile_exists\033[0m(\x1B[90m" << filename << "\033[0m) == \x1B[34m" << infile.good() << "\033[0m" << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
        return infile.good();
    }
}

std::optional<std::string> sqf::runtime::fileio::read_file_from_disk(std::string_view physical_path) {
    if (!file_exists(physical_path)) {
        return {};
    }
    std::ifstream file(physical_path.data(), std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        return {};
    }

    auto fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    switch (get_byte_order_mark(buffer)) {
        // Supported
        case byte_order_mark::NONE:
        case byte_order_mark::UTF8:
            break;

            // Pending support status (requires changes to the string type used to support UTF16/32)
        case byte_order_mark::UTF16BE:
        case byte_order_mark::UTF16LE:
        case byte_order_mark::UTF32BE:
        case byte_order_mark::UTF32LE:
            return {};

            // Unknown support status (assume works)
        case byte_order_mark::UTF7:
        case byte_order_mark::UTF1:
        case byte_order_mark::UTFEBCDIC:
        case byte_order_mark::SCSU:
        case byte_order_mark::BOCU1:
        case byte_order_mark::GB18030:
            break;
    }

    return std::string(buffer.begin() + get_bom_skip(buffer), buffer.end());
}

void sqf::runtime::fileio::add_mapping_auto(std::string_view phys) {
    const std::filesystem::path ignoreGit(".git");
    const std::filesystem::path ignoreSvn(".svn");
    const std::filesystem::path ignoreVisualStudioCode(".vscode");

    // Recursively search for pboprefix
    for (
            auto i = std::filesystem::recursive_directory_iterator(
                    phys, std::filesystem::directory_options::follow_directory_symlink);
            i != std::filesystem::recursive_directory_iterator();
            ++i
            ) {
        if (i->is_directory() && (i->path().filename() == ignoreGit || i->path().filename() == ignoreSvn ||
                                  i->path().filename() == ignoreVisualStudioCode)) {
            i.disable_recursion_pending(); // Don't recurse into that directory
            continue;
        }
        if (!i->is_regular_file()) {
            continue;
        }

        if (i->path().filename() == "$PBOPREFIX$") {
            std::ifstream prefixFile(i->path());
            std::string prefix;
            std::getline(prefixFile, prefix);
            prefixFile.close();
            add_mapping(prefix, i->path().parent_path().string());
        }
    }
}