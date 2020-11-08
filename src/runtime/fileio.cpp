#include "fileio.h"

#include <fstream>
#include <vector>
#include <filesystem>

#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
#include <iostream>
#endif // DF__SQF_FILEIO__TRACE_REESOLVE

static int get_bom_skip(const std::vector<char>& buff)
{
    if (buff.empty())
        return 0;
    // We are comparing against unsigned
    auto ubuff = reinterpret_cast<const unsigned char*>(buff.data());
    if (ubuff[0] == 0xEF && ubuff[1] == 0xBB && ubuff[2] == 0xBF)
    {
        //UTF-8
        return 3;
    }
    else if (ubuff[0] == 0xFE && ubuff[1] == 0xFF)
    {
        //UTF-16 (BE)
        return 2;
    }
    else if (ubuff[0] == 0xFE && ubuff[1] == 0xFE)
    {
        //UTF-16 (LE)
        return 2;
    }
    else if (ubuff[0] == 0x00 && ubuff[1] == 0x00 && ubuff[2] == 0xFF && ubuff[3] == 0xFF)
    {
        //UTF-32 (BE)
        return 2;
    }
    else if (ubuff[0] == 0xFF && ubuff[1] == 0xFF && ubuff[2] == 0x00 && ubuff[3] == 0x00)
    {
        //UTF-32 (LE)
        return 2;
    }
    else if (ubuff[0] == 0x2B && ubuff[1] == 0x2F && ubuff[2] == 0x76 &&
        (ubuff[3] == 0x38 || ubuff[3] == 0x39 || ubuff[3] == 0x2B || ubuff[3] == 0x2F))
    {
        //UTF-7
        return 4;
    }
    else if (ubuff[0] == 0xF7 && ubuff[1] == 0x64 && ubuff[2] == 0x4C)
    {
        //UTF-1
        return 3;
    }
    else if (ubuff[0] == 0xDD && ubuff[1] == 0x73 && ubuff[2] == 0x66 && ubuff[3] == 0x73)
    {
        //UTF-EBCDIC
        return 3;
    }
    else if (ubuff[0] == 0x0E && ubuff[1] == 0xFE && ubuff[2] == 0xFF)
    {
        //SCSU
        return 3;
    }
    else if (ubuff[0] == 0xFB && ubuff[1] == 0xEE && ubuff[2] == 0x28)
    {
        //BOCU-1
        if (ubuff[3] == 0xFF)
            return 4;
        return 3;
    }
    else if (ubuff[0] == 0x84 && ubuff[1] == 0x31 && ubuff[2] == 0x95 && ubuff[3] == 0x33)
    {
        //GB 18030
        return 3;
    }
    return 0;
}
static bool file_exists(std::string_view filename)
{
    std::ifstream infile(filename.data());
#ifdef DF__SQF_FILEIO__TRACE_REESOLVE
    std::cout << "\x1B[33m[FILEIO ASSERT]\033[0m" <<
        "        " <<
        "        " <<
        "    " << "\x1B[36mfile_exists\033[0m(\x1B[90m" << filename << "\033[0m) == \x1B[34m" << infile.good() << "\033[0m" << std::endl;
#endif // DF__SQF_FILEIO__TRACE_REESOLVE
    return infile.good();
}
std::optional<std::string> sqf::runtime::fileio::read_file_from_disk(std::string_view physical_path)
{
    if (!file_exists(physical_path))
    {
        return {};
    }
    std::ifstream file(physical_path.data(), std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        return {};
    }

    auto fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return std::string(buffer.begin() + get_bom_skip(buffer), buffer.end());
}

void sqf::runtime::fileio::add_mapping_auto(std::string_view phys)
{
    const std::filesystem::path ignoreGit(".git");
    const std::filesystem::path ignoreSvn(".svn");
    const std::filesystem::path ignoreVisualStudioCode(".vscode");

    // Recursively search for pboprefix
    for (
        auto i = std::filesystem::recursive_directory_iterator(
                    phys, std::filesystem::directory_options::follow_directory_symlink);
        i != std::filesystem::recursive_directory_iterator();
        ++i
        )
    {
        if (i->is_directory() && (i->path().filename() == ignoreGit || i->path().filename() == ignoreSvn || i->path().filename() == ignoreVisualStudioCode))
        {
            i.disable_recursion_pending(); // Don't recurse into that directory
            continue;
        }
        if (!i->is_regular_file())
        {
            continue;
        }

        if (i->path().filename() == "$PBOPREFIX$")
        {
            std::ifstream prefixFile(i->path());
            std::string prefix;
            std::getline(prefixFile, prefix);
            prefixFile.close();
            add_mapping(prefix, i->path().parent_path().string());
        }
    }
}