#include "parsepbo.h"
#include <algorithm>

bool is_garbage(PboEntry entry) {
    if (entry.method != PboEntryPackingMethod::none && entry.method != PboEntryPackingMethod::compressed)
        return true;

    bool garbageName = std::any_of(entry.name.begin(), entry.name.end(), [](char c) {
        return 
            c <= ' ' ||
            c == '"' ||
            c == '*' ||
            c == ':' ||
            c == '<' ||
            c == '>' ||
            c == '?' ||
            c == '/' ||
            c == '|';
    });

    return garbageName;
}

bool PboProperty::read(std::istream& in) {
    std::getline(in, key, '\0');
    if (key.empty()) return false; //We tried to read the end element of the property list
    std::getline(in, value, '\0');
    return true;
}

void PboEntry::read(std::istream& in) {
    struct {
        uint32_t method;
        uint32_t originalsize;
        uint32_t reserved;
        uint32_t timestamp;
        uint32_t datasize;
    } header {};

    std::getline(in, name, '\0');
    in.read(reinterpret_cast<char*>(&header), sizeof(header));

    method = PboEntryPackingMethod::none;

    if (header.method == 'Encr') { //encrypted
        method = PboEntryPackingMethod::encrypted;
    }
    if (header.method == 'Cprs') { //compressed
        method = PboEntryPackingMethod::compressed;
        //#TODO cannot read these files
    }
    if (header.method == 'Vers') { //Version
        method = PboEntryPackingMethod::version;
    }

    data_size = header.datasize;
    if (method == PboEntryPackingMethod::compressed)
        original_size = header.originalsize;
    else
        original_size = header.datasize;
}


PboEntryBuffer::
PboEntryBuffer(const PboReader& rd, const PboEntry& ent, uint32_t bufferSize): buffer(std::min(ent.original_size, bufferSize)), file(ent),
                                                                               reader(rd) {
    char* start = &buffer.front();
    setg(start, start, start);

    //if (ent.method == PboEntryPackingMethod::compressed) {
    //
    //    rd.input.seekg(file.startOffset);
    //
    //    buffer.resize(ent.original_size);
    //
    //
    //    lzss_decomp(rd.input, buffer, ent.original_size);
    //    bufferEndFilePos = ent.original_size;
    //    setg(&buffer.front(), &buffer.front(), &buffer.front() + ent.original_size);
    //}
}

void PboEntryBuffer::setBufferSize(size_t newSize) {
    if (file.method == PboEntryPackingMethod::compressed) return; //Already max size
    size_t dataLeft = egptr() - gptr();
    auto bufferOffset = gptr() - &buffer.front(); //Where we currently are inside the buffer
    auto bufferStartInFile = bufferEndFilePos - (dataLeft + bufferOffset); //at which offset in the PboEntry file our buffer starts
    bufferEndFilePos = bufferStartInFile; //Back to start.
    setg(&buffer.front(), &buffer.front(), &buffer.front()); //no data available

    buffer.clear(); //So we don't copy on realloc.
    buffer.resize(newSize);
}

int PboEntryBuffer::underflow() {
    if (gptr() < egptr()) // buffer not exhausted
        return traits_type::to_int_type(*gptr());

    reader.input.seekg(file.startOffset + bufferEndFilePos);
    size_t sizeLeft = file.original_size - bufferEndFilePos;
    if (sizeLeft == 0) return std::char_traits<char>::eof();

    auto sizeToRead = std::min(sizeLeft, buffer.size());
    reader.input.read(buffer.data(), sizeToRead);
    bufferEndFilePos += sizeToRead;

    setg(&buffer.front(), &buffer.front(), &buffer.front() + sizeToRead);

    return std::char_traits<char>::to_int_type(*this->gptr());
}

std::streamsize PboEntryBuffer::xsgetn(char* _Ptr, std::streamsize _Count) {
    // get _Count characters from stream
    const int64_t _Start_count = _Count;

    while (_Count) {
        size_t dataLeft = egptr() - gptr();
        if (dataLeft == 0) {
            reader.input.seekg(file.startOffset + bufferEndFilePos);
            size_t sizeLeft = file.original_size - bufferEndFilePos;
            if (sizeLeft == 0) break; //EOF
            auto sizeToRead = std::min(sizeLeft, buffer.size());
            reader.input.read(buffer.data(), sizeToRead);
            bufferEndFilePos += sizeToRead;

            setg(&buffer.front(), &buffer.front(), &buffer.front() + sizeToRead);

            dataLeft = std::min(sizeToRead, (size_t)_Count);
        } else
            dataLeft = std::min(dataLeft, (size_t)_Count);

        std::copy(gptr(), gptr() + dataLeft, _Ptr);
        _Ptr += dataLeft;
        _Count -= dataLeft;
        gbump(dataLeft);
    }

    return (_Start_count - _Count);
}

std::basic_streambuf<char>::pos_type PboEntryBuffer::seekoff(off_type offs, std::ios_base::seekdir dir, std::ios_base::openmode mode) {
    auto test = egptr();
    auto test2 = gptr();


    switch (dir) {
        case std::ios_base::beg: {
            //#TODO negative offs is error


            size_t dataLeft = egptr() - gptr();
            auto bufferOffset = gptr() - &buffer.front(); //Where we currently are inside the buffer
            auto bufferStartInFile = bufferEndFilePos - (dataLeft + bufferOffset); //at which offset in the PboEntry file our buffer starts

            //offset is still inside buffer
            if (bufferStartInFile <= offs && bufferEndFilePos > offs) { 
                auto curFilePos = (bufferEndFilePos - dataLeft);

                int64_t offsetToCurPos = offs - static_cast<int64_t>(curFilePos);
                gbump(offsetToCurPos); //Jump inside buffer till we find offs
                return offs;
            }

            //We are outside of buffer. Just reset and exit
            bufferEndFilePos = offs;
            setg(&buffer.front(), &buffer.front(), &buffer.front()); //no data available
            return bufferEndFilePos;

        }

        break;
        case std::ios_base::cur: {
                size_t dataLeft = egptr() - gptr();
                auto curFilePos = (bufferEndFilePos - dataLeft);

                if (offs == 0) return curFilePos;

                if (dataLeft == 0) {
                    bufferEndFilePos += offs;
                    return bufferEndFilePos;
                }


                if (offs > 0 && dataLeft > offs) { // offset is still inside buffer
                    gbump(offs);
                    return curFilePos + offs;
                }
                if (offs > 0) { //offset is outside of buffer
                    bufferEndFilePos = curFilePos + offs;
                    setg(&buffer.front(), &buffer.front(), &buffer.front()); //no data available
                    return bufferEndFilePos;
                }

                if (offs < 0) {

                    auto bufferOffset = gptr() - &buffer.front(); //Where we currently are inside the buffer
                    if (bufferOffset >= -offs) {//offset is still in buffer
                        gbump(offs);
                        return bufferOffset + offs;
                    }

                    bufferEndFilePos = curFilePos + offs;
                    setg(&buffer.front(), &buffer.front(), &buffer.front()); //no data available
                    return bufferEndFilePos;
                }
            }
        break;
        case std::ios_base::end:
            //#TODO positive offs is error
            bufferEndFilePos = file.original_size + offs;
            setg(&buffer.front(), &buffer.front(), &buffer.front()); //no data available
            return bufferEndFilePos;
        break;
    }
    return -1; //#TODO this is error
}

std::basic_streambuf<char>::pos_type PboEntryBuffer::seekpos(pos_type offs, std::ios_base::openmode mode) {
    return seekoff(offs, std::ios_base::beg, mode);
}

std::streamsize PboEntryBuffer::showmanyc() {
    //How many characters are left
    size_t dataLeft = egptr() - gptr();
    
    return (file.original_size - bufferEndFilePos) + dataLeft;
}

void PboReader::readHeaders() {
    PboEntry intro;
    intro.read(input);

    //#TODO check stuff and throw if error
    //filename is empty
    //packing method is vers
    //time is 0
    //datasize is 0

    //header ignores startoffset and uncompressed size

    if (intro.method == PboEntryPackingMethod::none) {//Broken 3den exported pbo
        input.seekg(0, std::istream::beg); //Seek back to start
        badHeader = true;
    } else {   
        PboProperty prop;
        while (prop.read(input)) {
            properties.emplace_back(std::move(prop));
        }
        //When prop's last read "failed" we just finished reading the terminator of the properties
        propertiesEnd = input.tellg();
    }


    PboEntry entry;

    while (entry.read(input), !entry.name.empty()) {
        files.emplace_back(std::move(entry));
    }
    //We just read the last terminating entry header too.
    headerEnd = input.tellg();

    size_t curPos = headerEnd;
    for (auto& it : files) {
        it.startOffset = curPos;
        curPos += it.data_size;
    }
    auto fileEnd = curPos;
    input.seekg(fileEnd+1, std::istream::beg);
    input.read(reinterpret_cast<char*>(hash.data()), 20); //After end there is checksum 20 bytes.
}
