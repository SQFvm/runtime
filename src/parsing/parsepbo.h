#pragma once
#include <array>
#include <vector>
#include <string>
#include <iostream>


class PboProperty {
public:
    PboProperty() {}
    PboProperty(std::string k, std::string v): key(std::move(k)), value(std::move(v)) {}

    std::string key;
    std::string value;

    bool read(std::istream& in);
};

enum class PboEntryPackingMethod {
    none,
    version,
    compressed,
    encrypted
};

class PboEntry {
public:
    std::string name;

    uint32_t original_size = 0;
    uint32_t data_size = 0;
    uint32_t startOffset = 0;
    PboEntryPackingMethod method = PboEntryPackingMethod::none;

    void read(std::istream& in);
};

class PboReader;
class PboEntryBuffer : public std::streambuf {
    std::vector<char> buffer;
    const PboEntry& file;
    const PboReader& reader;
    //What position the character after the last character that's currently in our buffer, corresponds to in the pbofile
    //Meaning on next read, the first character read is that pos
    size_t bufferEndFilePos{0};
public:
    PboEntryBuffer(const PboReader& rd, const PboEntry& ent, uint32_t bufferSize = 4096u);

    void setBufferSize(size_t newSize);
    int underflow() override;
    std::streamsize xsgetn(char* _Ptr, std::streamsize _Count) override;
    pos_type seekoff(off_type, std::ios_base::seekdir, std::ios_base::openmode) override;
    pos_type seekpos(pos_type, std::ios_base::openmode) override;
    std::streamsize showmanyc() override;

    void setBuffersize(size_t newSize) {
        buffer.resize(newSize);
    }
};

class PboReader {
    friend class PboEntryBuffer;

    std::vector<PboEntry> files;
    std::vector<PboProperty> properties;
    uint32_t propertiesEnd{0};
    uint32_t headerEnd{0};
    std::istream& input;
    std::array<unsigned char, 20> hash;
    bool badHeader{false};
public:
    PboReader(std::istream &input) : input(input) {}
    void readHeaders();
    const auto& getFiles() const noexcept { return files; }
    PboEntryBuffer getFileBuffer(const PboEntry& ent) const {
        return PboEntryBuffer(*this, ent);
    }
    const auto& getProperties() const { return properties; }
    const auto& getHash() const { return hash; }
    //Broken 3DEN exported pbo with no header
    bool isBadHeader() const{ return badHeader; }
};

