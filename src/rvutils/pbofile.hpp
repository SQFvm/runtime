#pragma once
#include <vector>
#include <filesystem>
#include <fstream>
#include <optional>
#include <utility>

namespace rvutils::pbo
{
    enum class packing_method
    {
        none,
        encrypted,
        compressed,
        version
    };
    struct file_descriptor
    {
        // Size as in the PBO
        size_t size;
        // Size as the PBO header states this file actually is.
        // May not be set for all packing methods
        size_t size_original;
        // The packing method used as stated in the header.
        packing_method packing;
        // The file name
        std::string name;
    };
    class pbofile
    {
        struct datablock
        {
            std::streampos start;
            std::streampos end;

            size_t length() const { return end - start; }
        };
        struct attribute_
        {
            std::string key;
            std::string value;

            datablock block;
            bool operator<(const attribute_& other) const { return block.start < other.block.start; }
            bool operator>(const attribute_& other) const { return block.end < other.block.end; }
            bool is_invalid() const
            {
                for (auto c : key)
                {
                    if (c != '?')
                    {
                        return false;
                    }
                }
                return !key.empty();
            }
            size_t bytes() const
            {
                return key.length() + 1 + value.length() + 1;
            }
        };
        struct header
        {
#if defined(_MSVC_LANG)
#pragma pack(push, 1)
            using bin = struct
            {
                char method[4];
                uint32_t size_original;
                uint32_t reserved;
                uint32_t timestamp;
                uint32_t size;
            };
#pragma pack(pop)
#else
            using bin = struct
            {
                char method[4];
                uint32_t size_original;
                uint32_t reserved;
                uint32_t timestamp;
                uint32_t size;
            } __attribute__((packed));
#endif
            std::string name;
            packing_method method;

            uint32_t size_original;
            uint32_t size;
            uint32_t timestamp;

            datablock block_entry;
            datablock block_data;
            bool operator<(const header& other) const { return block_data.start < other.block_data.start; }
            bool operator>(const header& other) const { return block_data.end < other.block_data.end; }
            bool is_invalid() const
            {
                for (auto c : name)
                {
                    if (c != '?')
                    {
                        return false;
                    }
                }
                return !name.empty();
            }
            size_t bytes() const
            {
                return sizeof(bin) + name.length() + 1;
            }
        };
    public:
        class reader
        {
            friend class pbofile;
            std::ifstream m_file;
            datablock m_block;
            bool m_good;
            file_descriptor m_file_descriptor;

            reader(const reader& copy) = delete;
            reader(const reader&& rcopy) = delete;
            bool initialize(std::filesystem::path path, const header& h)
            {
                m_file.open(path, std::ios_base::binary | std::ios_base::in);
                if (m_file.is_open() && m_file.good())
                {
                    m_block = h.block_data;
                    m_file.seekg(m_block.start);
                    m_good = true;
                    m_file_descriptor.name = h.name;
                    m_file_descriptor.packing = h.method;
                    m_file_descriptor.size = h.size;
                    m_file_descriptor.size_original = h.size_original;
                    return true;
                }
                else
                {
                    return false;
                }
            }
        public:
            reader() : m_good(false) { }
            file_descriptor descriptor() const { return m_file_descriptor; }
            bool good() const { return m_good; }
            size_t read(char* arr, std::streamsize bytes)
            {
                if (!good())
                { // Reader was not initialized proper
                    return 0;
                }
                auto pos = m_file.tellg();
                auto remaining = m_block.end - pos;
                if (remaining < 0)
                { // We already read everything readable
                    return 0;
                }
                remaining = remaining < bytes ? remaining : bytes;
                m_file.read(arr, remaining);
                return (size_t)remaining;
            }
            std::streampos tell()
            {
                auto pos = m_file.tellg();
                return pos - m_block.start;
            }
            void seek(std::streamoff offset, std::ios::seekdir dir)
            {
                switch (dir)
                {
                    case std::ios::beg:
                    {
                        if (offset + m_block.start > m_block.end)
                        {
                            m_file.seekg(m_block.end, std::ios::beg);
                        }
                        else if (offset + m_block.start < m_block.start)
                        {
                            m_file.seekg(m_block.start, std::ios::beg);
                        }
                        else
                        {
                            m_file.seekg(offset, std::ios::beg);
                        }
                    } break;
                    case std::ios::cur:
                    {
                        auto cur = m_file.tellg();
                        if (cur + offset > m_block.end)
                        {
                            m_file.seekg(m_block.end, std::ios::beg);
                        }
                        else if (cur + offset < m_block.start)
                        {
                            m_file.seekg(m_block.start, std::ios::beg);
                        }
                        else
                        {
                            m_file.seekg(offset, std::ios::cur);
                        }
                    } break;
                    case std::ios::end:
                    {
                        if (offset + m_block.end > m_block.end)
                        {
                            m_file.seekg(m_block.end, std::ios::beg);
                        }
                        else if (offset + m_block.end < m_block.start)
                        {
                            m_file.seekg(m_block.start, std::ios::beg);
                        }
                        else
                        {
                            m_file.seekg(offset, std::ios::end);
                        }
                    } break;
                }
            }
        };
        class writer
        {
            friend class pbofile;
            std::fstream m_file;
            bool m_good;
            header* m_header;
            bool initialize(pbofile* pbo, std::string_view name)
            {
                // Attempt to open the file
                m_file.open(pbo->m_path, std::ios_base::binary | std::ios_base::out | std::ios::in);
#if _DEBUG
                auto DBG_IS_OPEN = m_file.is_open();
                auto DBG_GOOD = m_file.good();
                auto DBG_EOF = m_file.eof();
                auto DBG_FAIL = m_file.fail();
                auto DBG_BAD = m_file.bad();
                auto DBG_RDSTATE = m_file.rdstate();
#endif
                if (!(m_file.is_open() && m_file.good()))
                {
                    return false;
                }
                auto oldcap = pbo->m_headers.capacity();
                // Find existing header
                auto iter = std::find_if(
                    pbo->m_headers.begin(),
                    pbo->m_headers.end(),
                    [name](header h) -> bool {
                    return h.name == name;
                });
                if (pbo->m_headers.end() != iter)
                {
                    // Create new header for copied data, rewrite data section to end
                    auto block_data_length = iter->block_data.end - iter->block_data.start;
                    std::vector<header>::iterator iter_created;
                    {
                        header created = *iter;
                        created.name = name;
                        created.timestamp = (uint32_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                        created.block_entry = { 0, 0 };
                        iter_created = pbo->push_back(m_file, created);
                    }

                    // Update possibly invalidated iterator
                    if (pbo->m_headers.capacity() != oldcap)
                    {
                        iter = std::find_if(
                            pbo->m_headers.begin(),
                            pbo->m_headers.end(),
                            [name](header& h) -> bool {
                            return h.name == name;
                        });
                    }

                    // Get EOF
                    m_file.seekg(0, std::ios::end);
                    auto eof = m_file.tellp();

                    // Copy data to end
                    copy<4096>(m_file, iter->block_data.start, iter->block_data.end, eof);

                    // Update created data
                    iter_created->block_data.start = eof;
                    iter_created->block_data.end = eof + (iter->block_data.end - iter->block_data.start);
                    pbo->write_header(m_file, *iter_created);

                    // Rename old header to represent empty section
                    std::transform(iter->name.begin(), iter->name.end(), iter->name.begin(), [](char c) -> char { return '?'; });
                    pbo->write_header(m_file, *iter);

                    m_header = &*iter_created;
                    // Set good to true and return true to indicate success.
                    m_good = true;
                    return true;
                }
                else
                {
                    // Create new header
                    header created;
                    created.name = name;
                    created.timestamp = (uint32_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    created.size = 0;
                    created.size_original = 0;
                    created.method = packing_method::none;
                    created.block_entry = { 0, 0 };
                    created.block_data = { 0, 0 };
                    m_header = &*pbo->push_back(m_file, created);


                    // Get EOF
                    m_file.seekg(0, std::ios::end);
                    auto eof = m_file.tellp();
                    m_header->block_data = { eof, eof };

                    // Set good to true and return true to indicate success.
                    m_good = true;
                    return true;
                }
            }
        public:
            std::streampos tell()
            {
                auto pos = m_file.tellg();
                return pos - m_header->block_data.start;
            }
            bool good() const { return m_good; }
            void seek(std::streamoff offset, std::ios::seekdir dir)
            {
                switch (dir)
                {
                    case std::ios::beg:
                    {
                        if (offset + m_header->block_data.start > m_header->block_data.end)
                        {
                            m_file.seekg(m_header->block_data.end, std::ios::beg);
                        }
                        else if (offset + m_header->block_data.start < m_header->block_data.start)
                        {
                            m_file.seekg(m_header->block_data.start, std::ios::beg);
                        }
                        else
                        {
                            m_file.seekg(offset, std::ios::beg);
                        }
                    } break;
                    case std::ios::cur:
                    {
                        auto cur = m_file.tellg();
                        if (cur + offset > m_header->block_data.end)
                        {
                            m_file.seekg(m_header->block_data.end, std::ios::beg);
                        }
                        else if (cur + offset < m_header->block_data.start)
                        {
                            m_file.seekg(m_header->block_data.start, std::ios::beg);
                        }
                        else
                        {
                            m_file.seekg(offset, std::ios::cur);
                        }
                    } break;
                    case std::ios::end:
                    {
                        if (offset + m_header->block_data.end > m_header->block_data.end)
                        {
                            m_file.seekg(m_header->block_data.end, std::ios::beg);
                        }
                        else if (offset + m_header->block_data.end < m_header->block_data.start)
                        {
                            m_file.seekg(m_header->block_data.start, std::ios::beg);
                        }
                        else
                        {
                            m_file.seekg(offset, std::ios::end);
                        }
                    } break;
                }
            }
            void write(const char* arr, std::streamsize bytes)
            {
                if (!good())
                { // Reader was not initialized proper
                    return;
                }
                m_file.write(arr, bytes);
                m_header->block_data.end += bytes;
                m_header->size += bytes;
                write_header(m_file, *m_header);
            }

            // Removes any data remaining for the current header.
            //
            // Remarks:
            // - Due to STL limitations, this cannot erase physical data
            //   The remaining data in this file thus remains unchanged
            //   and only `size_actual` is updated.
            void truncate()
            {
                m_header->block_data.end = m_file.tellp();
                m_header->size = m_header->block_data.length();
            }
            size_t original_size() const { return m_header->size_original; }
            void original_size(uint32_t bytes) { m_header->size_original = bytes; write_header(m_file, *m_header); }

            packing_method method() const { return m_header->method; }
            void method(packing_method m) { m_header->method = m; write_header(m_file, *m_header); }
        };
    private:
        std::filesystem::path m_path;
        std::vector<datablock> m_free_blocks;
        std::vector<header> m_headers;
        std::vector<attribute_> m_attributes;
        bool m_good;

        // Finds the '\0' character in a filestream and returns the characters required to reach it.
        // Always returns not null on success
        // Stream position will be reset on method exit.
        static size_t strlen(std::fstream& file)
        {
            const size_t buff_size = 256;
            char buff[buff_size];
            auto start_pos = file.tellg();
            file.seekg(0, std::ios::end);;
            auto eof = file.tellg();
            file.seekg(start_pos);
            int runs = 0;
            do
            {
                file.read(buff, buff_size);
                for (size_t i = 0; i < buff_size; i++)
                {
                    if (buff[i] == '\0')
                    {
                        file.clear();
                        file.seekg(start_pos);
                        return i + (runs * buff_size) + 1;
                    }
                }
                runs++;
            } while (file.tellg() < eof && !file.eof());
            file.seekg(start_pos);
            return -1;
        }

        // Reads a single c-styled string from the file stream.
        // Stream position will reset stream on error.
        static std::optional<std::string> read_string(std::fstream& file)
        {
            auto zero = strlen(file);

            if (zero == 0) { return {}; }
            if (zero == -1) { return {}; }
            std::vector<char> buff;
            buff.resize(zero);
            file.read(buff.data(), zero);
            buff.resize(zero - 1);
            return std::string{ buff.begin(), buff.end() };
        }
        // Writes a single c-styled string to the file stream.
        static void write_string(std::fstream& file, std::string_view view)
        {
            file.write(view.data(), view.length());
            file.write("\0", 1);
        }

        // Reads a single pbo attribute_ from the file.
        // Stream position will reset stream on error.
        static std::optional<attribute_> read_attribute(std::fstream& file)
        {
            auto start_pos = file.tellg();
            auto key = read_string(file);
            if (key.has_value() && !key->empty())
            {
                auto value = read_string(file);
                if (value.has_value())
                {
                    auto end_pos = file.tellg();
                    return attribute_{ *key, *value, { start_pos, end_pos } };
                }
                else
                {
                    file.seekg(start_pos);
                    return {};
                }
            }
            else
            {
                file.seekg(start_pos);
                return {};
            }
        }
        // Writes a single pbo attribute_ to the file stream where attribute_::data.start refers to
        // 
        // Remakrs:
        // - is_update: true
        //   - Auto-Reset on end
        //   - Auto-Seek to attribute_ on start
        static void write_attribute(std::fstream& file, const attribute_& actual, bool is_update = true)
        {
            auto cur = file.tellp();

            if (is_update)
            {
                file.seekp(actual.block.start);
            }

            write_string(file, actual.key);
            write_string(file, actual.value);
#ifdef _DEBUG
            file.flush();
#endif

            if (is_update)
            {
                file.seekp(cur);
            }
        }
        // Reads a single pbo header from the file.
        // Stream position will reset stream on error.
        static std::optional<header> read_header(std::fstream& file)
        {
            auto start_pos = file.tellg();

            // Cut off version of a3::header


            // Get filename
            auto name = read_string(file);
            if (!name.has_value()) { return {}; }
            header::bin data_mapped;

            // read in the whole data available into helper struct
            file.read(reinterpret_cast<char*>(&data_mapped), sizeof(header::bin));
            file.clear();


            header actual;

            actual.name = *name;

            if (data_mapped.method[3] == 'E' && data_mapped.method[2] == 'n' &&data_mapped.method[1] == 'c' && data_mapped.method[0] == 'r')
            { //encrypted
                actual.method = packing_method::encrypted;
            }
            else if (data_mapped.method[3] == 'C' && data_mapped.method[2] == 'p' && data_mapped.method[1] == 'r' && data_mapped.method[0] == 's')
            { //compressed
                actual.method = packing_method::compressed;
            }
            else if (data_mapped.method[3] == 'V' && data_mapped.method[2] == 'e' && data_mapped.method[1] == 'r' && data_mapped.method[0] == 's')
            { //Version
                actual.method = packing_method::version;
            }
            else
            {
                actual.method = packing_method::none;
            }

            actual.size_original = data_mapped.size_original;
            actual.size = data_mapped.size;
            actual.timestamp = data_mapped.timestamp;
            actual.block_entry.start = start_pos;
            actual.block_entry.end = file.tellg();
            return actual;
        }
        // Writes a single pbo header to the file stream where header::data_entry.start refers to.
        // 
        // Remakrs:
        // - is_update: true
        //   - Auto-Reset on end
        //   - Auto-Seek to header on start
        static void write_header(std::fstream& file, const header& actual, bool is_update = true)
        {
            auto cur = file.tellp();

            if (is_update)
            {
                file.seekp(actual.block_entry.start);
            }

            write_string(file, actual.name);

            header::bin data_mapped;
            data_mapped.reserved = 0;
            data_mapped.size = actual.size;
            data_mapped.size_original = actual.size_original;
            data_mapped.timestamp = actual.timestamp;

            switch (actual.method)
            {
                case packing_method::none:
                default:
                data_mapped.method[3] = '\0';
                data_mapped.method[2] = '\0';
                data_mapped.method[1] = '\0';
                data_mapped.method[0] = '\0';
                break;
                case packing_method::encrypted:
                data_mapped.method[3] = 'E';
                data_mapped.method[2] = 'n';
                data_mapped.method[1] = 'c';
                data_mapped.method[0] = 'r';
                break;
                case packing_method::version:
                data_mapped.method[3] = 'V';
                data_mapped.method[2] = 'e';
                data_mapped.method[1] = 'r';
                data_mapped.method[0] = 's';
                break;
                case packing_method::compressed:
                data_mapped.method[3] = 'C';
                data_mapped.method[2] = 'p';
                data_mapped.method[1] = 'r';
                data_mapped.method[0] = 's';
                break;
            }

            // write out the data from helper struct
            file.write(reinterpret_cast<char*>(&data_mapped), sizeof(header::bin));
#if _DEBUG
            file.flush();
#endif
            if (is_update)
            {
                file.seekp(cur);
            }
        }
        // Moves the data from A to B
        //
        // Remarks:
        // - If data overlaps, only half of buffsize is used
        template<size_t buffsize>
        static void copy(std::fstream& file, std::streampos old_start, std::streampos old_end, std::streampos new_start)
        {
            if (old_start == new_start) { return; }
            if (old_start == old_end) { return; }
            // Get current position so we can seek back later
            std::streampos cur = file.tellp();

            // Check if new start intersects with old data
            bool intersects = new_start >= old_start && old_end >= new_start;

            // Seek to start
            file.seekp(old_start, std::ios::beg);
            std::streampos source_cur = old_start;
            std::streampos target_cur = new_start;
            char buff[buffsize];

            if (intersects)
            {
                // Strategy:
                //   Only ever copy half of the data,
                //   read in other half ahead of time.
                bool flip = false;
                const size_t buffsize_half = buffsize / 2;

                // Read in initial
                size_t old_read_len = buffsize_half < (old_end - source_cur) ? buffsize_half : (old_end - source_cur);
                file.seekg(source_cur);
                file.read(buff, old_read_len);
                file.clear();
                source_cur = file.tellg();
                flip = true;

                while (source_cur < old_end)
                {
                    auto remaining = old_end - source_cur;
                    auto read_len = buffsize_half < remaining ? buffsize_half : remaining;

                    // Read in ahead, flipping the buffer part used each time
                    file.seekg(source_cur);
                    file.read(buff + ((flip ? 1 : 0) * buffsize_half), read_len);
                    file.clear();
                    source_cur = file.tellg();
                    flip = !flip;

                    // Write out current at end
                    file.seekp(target_cur);
                    file.write(buff + ((flip ? 1 : 0) * buffsize_half), old_read_len);
                    target_cur = file.tellp();

                    old_read_len = read_len;
                }

                // Write out last flip if needed
                if (old_read_len > 0)
                {
                    flip = !flip;
                    file.seekp(target_cur);
                    file.write(buff + ((flip ? 1 : 0) * buffsize_half), old_read_len);
                }
            }
            else
            {
                char buff[buffsize];
                std::streampos source_cur = old_start;
                while (source_cur < old_end)
                {
                    auto remaining = old_end - source_cur;
                    auto read_len = buffsize < remaining ? buffsize : remaining;

                    // Read in current
                    file.seekg(source_cur);
                    file.read(buff, read_len);
                    file.clear();
                    source_cur = file.tellg();

                    // Write out current at end
                    file.seekp(target_cur);
                    file.write(buff, read_len);
                    target_cur = file.tellp();
                }
            }

            // Seek back to where we have been
            file.seekp(cur);
#if _DEBUG
            file.flush();
#endif
        }
        // Rewrites the name of the provided header into being an empty-section name
        //
        // Remarks:
        // - Autoresets position to where it was before
        void make_empty_name(header h, std::fstream& file)
        {
            // Get current position so we can seek back later
            std::streampos cur = file.tellp();

            file.seekp(h.block_entry.start);
            h.name = std::string(h.name.length(), '?');
            file.write(h.name.data(), h.name.length());

            // Seek back to where we have been
            file.seekp(cur);
        }
        // Ensures that the datasection has at least the provided amount of bytes available.
        // If there are no headers (yet), method is returning immediate.
        //
        // Returns true on success
        [[nodiscard]] bool ensure_space_header(size_t bytes)
        {
            using namespace std::string_view_literals;
            if (m_headers.size() == 1) { return true; }
            size_t available = 0;
            // Check if additional data is required for the resize operation, inserting the empty header
            for (auto it = m_headers.begin(); it != m_headers.end() - /* empty header */ 1 && available < bytes; it++)
            {
                if (it->is_invalid())
                {
                    available += it->block_data.length();
                }
            }
            if (available < bytes)
            {
                std::fstream file(m_path, std::ios_base::binary | std::ios_base::in | std::ios_base::out);
                if (!file.is_open() && !file.good())
                {
                    return false;
                }
                ensure_space_header__free_space_for_headers(file, bytes, available);
                return true;
            }
            return true;
        }
        // Ensures that the datasection has at least the provided amount of bytes available.
        // If there are no headers (yet), method is returning immediate.
        void ensure_space_header(std::fstream& file, size_t bytes)
        {
            using namespace std::string_view_literals;
            if (m_headers.size() == 1) { return; }
            size_t available = 0;
            // Check if additional data is required for the resize operation, inserting the empty header
            for (auto it = m_headers.begin(); it != m_headers.end() - /* empty header */ 1 && available < bytes; it++)
            {
                if (it->is_invalid())
                {
                    available += it->block_data.length();
                }
            }
            if (available < bytes)
            {
                ensure_space_header__free_space_for_headers(file, bytes, available);
#if _DEBUG
                file.flush();
#endif
            }
        }
        // Helper method of ensure_space_header parameter overloads.
        // Not intended to be called by itself.
        void ensure_space_header__write_empty_section(std::fstream& file, std::streamsize freed)
        {
            using namespace std::string_view_literals;
            // Check if first header already is empty section
            if (m_headers.front().is_invalid())
            {
                m_headers.front().block_data.start -= std::streampos(freed);
                write_header(file, m_headers.front());
            }
            else
            {
                // Move all data by the header offset physically
                auto header_off = sizeof(header::bin) + "?????\0"sv.length();
                copy<4096>(file,
                    m_headers.begin()->block_entry.start,
                    (m_headers.end() - 2)->block_entry.end,
                    m_headers.begin()->block_entry.start + std::streampos(header_off));

                // Insert empty header to start
                header freed_section = { };
                freed_section.name = "?????";
                freed_section.block_data.start = m_headers.front().block_data.start - std::streampos(freed - header_off);
                freed_section.block_data.end = freed_section.block_data.start + std::streampos(freed - header_off);
                freed_section.block_entry = m_headers.front().block_entry;
                freed_section.method = packing_method::none;
                freed_section.size = uint32_t(freed_section.block_data.length());
                freed_section.timestamp = uint32_t(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
                m_headers.insert(m_headers.begin(), freed_section);
                write_header(file, freed_section);

                // Move all data by the header offset virtually
                for (auto it = m_headers.begin() + 1; it != m_headers.end(); it++)
                {
                    it->block_entry.start += header_off;
                    it->block_entry.end += header_off;
                    write_header(file, *it);
                }
            }
        }
        // Helper method of ensure_space_header parameter overloads.
        // Not intended to be called by itself.
        void ensure_space_header__free_space_for_headers(std::fstream& file, std::streamsize bytes, std::streamsize available)
        {
            using namespace std::string_view_literals;
            if (bytes < 1024) { bytes = 1024; }
            if (!m_headers.front().is_invalid())
            {
                // Add additional space requested for empty-section-header
                bytes += sizeof(header::bin) + "?????\0"sv.length();
                // Ensure we have at least one more header capacity available
                if (m_headers.size() == m_headers.capacity())
                {
                    m_headers.reserve(m_headers.size() * 2 + 1);
                }
            }
            // Count how many header-data moves are required
            std::streamsize freed_data = 0;
            for (auto it = m_headers.begin(); it != m_headers.end() - /* empty header */ 1 && freed_data < bytes; it++)
            {
                if (it->is_invalid())
                {
                    continue;
                }
                else
                {
                    freed_data += it->block_data.length();
                }
            }
            std::streamsize freed = 0;
            // Check if all data needs to be moved plus extra space or just some entries need to be moved to the end
            if (freed_data < bytes)
            {
                // Move everything by `size`
                copy<4096>(file,
                    m_headers.begin()->block_data.start,
                    (m_headers.end() - 2)->block_data.end,
                    m_headers.begin()->block_data.start + std::streampos(bytes));
                for (auto& it : m_headers)
                {
                    it.block_data.start += bytes;
                    it.block_data.end += bytes;
                    write_header(file, it);
                }
                freed = bytes;
                ensure_space_header__write_empty_section(file, freed);
            }
            else
            { // Just copy to end until we reached the required space
                auto iter = m_headers.begin();
                auto old_capacity = m_headers.capacity();
                auto eof = file.tellp();
                {
                    auto tmp = eof;
                    file.seekp(0, std::ios::end);
                    eof = file.tellp();
                    file.seekp(tmp);
                }

                // Skip all empty sections
                while (iter->is_invalid())
                {
                    if (iter->is_invalid()) { continue; }
                    iter++;
                }

                // Move to end until available is satisfied
                auto first_moved_header_iter = iter;
                while (available < bytes)
                {
                    // Append moved bytes to available
                    available += iter->block_data.length();
                    freed += iter->block_data.length();

                    // Copy data to end
                    copy<4096>(file, iter->block_data.start, iter->block_data.end, eof);

                    // Update block_data offsets
                    auto delta = eof - iter->block_data.start;
                    iter->block_data.start += delta;
                    iter->block_data.end += delta;

                    // Progress iterator
                    ++iter;
                }
                auto last_moved_header_iter = iter;
                
                // Set target to last header start (gets updated once at the very end too)
                auto move_delta_end = (m_headers.end() - 2)->block_entry.start - first_moved_header_iter->block_entry.start;
                auto move_delta_start = (last_moved_header_iter - 1)->block_entry.start - first_moved_header_iter->block_entry.start;

                // Rewrite the moved virtually to end
                for (; first_moved_header_iter != last_moved_header_iter; first_moved_header_iter++)
                {
                    first_moved_header_iter->block_entry.start += move_delta_end;
                    first_moved_header_iter->block_entry.end += move_delta_end;
                }
                // Rewrite the non-moved (but empty) to start
                for (; last_moved_header_iter != m_headers.end() - 1; last_moved_header_iter++)
                {
                    auto delta = last_moved_header_iter->block_entry.start - move_delta_start;
                    last_moved_header_iter->block_entry.start -= delta;
                    last_moved_header_iter->block_entry.end -= delta;
                }
                // Rewrite empty header to very end
                first_moved_header_iter--;
                auto move_target_empty = first_moved_header_iter->block_entry.end;
                m_headers.back().block_entry.end = move_target_empty + std::streamoff(m_headers.back().block_entry.length());
                m_headers.back().block_entry.start = move_target_empty;

                // Sort virtual representation
                std::sort(m_headers.begin(), m_headers.end() - /* empty header */ 1);

                // Invalidates iterators potentially
                ensure_space_header__write_empty_section(file, freed);
                if (m_headers.front().is_invalid())
                {
                    for (auto& it : m_headers)
                    {
                        write_header(file, it);
                    }
                }
            }
        }
        // Ensures that the attribute_ section has at least the provided amount of bytes available.
        // If there are no headers (yet), method is returning immediate.
        [[nodiscard]] bool ensure_space_attributes(size_t bytes)
        {
            using namespace std::string_view_literals;
            if (m_headers.size() == 1) { return true; }
            size_t available = 0;
            for (auto it = m_attributes.rbegin(); it != m_attributes.rend() && available < bytes; it++)
            {
                if (it->is_invalid())
                {
                    available += it->key.length();
                }
            }
            if (available < bytes)
            {
                std::fstream file(m_path, std::ios_base::binary | std::ios_base::in | std::ios_base::out);
                if (!file.is_open() && !file.good())
                {
                    return false;
                }
                ensure_space_attributes__free_space_for_attributes(file, bytes, available);
            }
            return true;
        }
        // Ensures that the attribute_ section has at least the provided amount of bytes available.
        [[nodiscard]] void ensure_space_attributes(std::fstream& file, size_t bytes)
        {
            using namespace std::string_view_literals;
            size_t available = 0;
            for (auto it = m_attributes.rbegin(); it != m_attributes.rend() && available < bytes; it++)
            {
                if (it->is_invalid())
                {
                    available += it->key.length();
                }
            }
            if (available < bytes)
            {
                ensure_space_attributes__free_space_for_attributes(file, bytes, available);
            }
        }
        // Helper method of ensure_space_attributes parameter overloads.
        // Not intended to be called by itself.
        [[nodiscard]] void ensure_space_attributes__free_space_for_attributes(std::fstream& file, std::streamsize bytes, std::streamsize available)
        {
            if (bytes < 1024) { bytes = 1024; }

            copy<8192>(file,
                m_headers.front().block_entry.start,
                m_headers.back().block_data.end,
                m_headers.front().block_entry.start + std::streampos(bytes));
            for (auto& it : m_headers)
            {
                it.block_data.start += bytes;
                it.block_data.end += bytes;
                it.block_entry.start += bytes;
                it.block_entry.end += bytes;
                write_header(file, it);
            }

            attribute_ created;
            created.value = "";
            created.key = std::string(bytes - /* terminating zeros */ 2, '?');
            created.block.start = m_attributes.back().block.start;
            created.block.end = m_attributes.back().block.start + std::streampos(bytes);
            m_attributes.insert(m_attributes.end() - 1, created);
            write_attribute(file, created);
            m_attributes.back().block.start = created.block.end;
            m_attributes.back().block.end = m_attributes.back().block.start + std::streamoff(1);
        }
        // Adds the header virtually and physically at the very end of the headers list.
        //
        // Returns true on success.
        // 
        // Remarks:
        // - Dataoffsets invalidate if moving data physically is required.
        [[nodiscard]] bool push_back(header& h)
        {
            std::fstream file(m_path, std::ios_base::binary | std::ios_base::in | std::ios_base::out);
            if (!file.is_open() && !file.good())
            {
                return false;
            }
            push_back(file, h);
            return true;
        }
        // Adds the header virtually and physically at the very end of the headers list.
        // 
        // Remarks:
        // - Dataoffsets invalidate if moving data physically is required.
        [[nodiscard]] std::vector<header>::iterator push_back(std::fstream& file, header& h)
        {
            // Ensure we have the space available to insert the header to end
            ensure_space_header(file, h.bytes());

            // If front is empty section, move its data back further
            size_t rem = h.bytes();
            for (auto it = m_headers.begin(); it != m_headers.end() && rem > 0 && it->is_invalid(); ++it)
            {
                if (it->size > 0)
                {
                    auto reduce = rem < it->size ? rem : it->size;
                    rem -= reduce;
                    it->block_data.start = it->block_data.start + std::streampos(reduce);
                    it->size = it->size - reduce;
                    write_header(file, *it);
                }
            }

            // Set block_entry
            h.block_entry.start = m_headers.back().block_entry.start;
            h.block_entry.end = h.block_entry.start + std::streamoff(h.bytes());

            // Write out created header
            write_header(file, h);

            // Update & write-out empty header
            m_headers.back().block_entry.start = h.block_entry.end;
            m_headers.back().block_entry.end = m_headers.back().block_entry.start + std::streampos(m_headers.back().bytes());
            write_header(file, m_headers.back());

            // Update created block_data
            {
                auto cur = file.tellp();
                file.seekp(0, std::ios::end);
                h.block_data.end = h.block_data.start = file.tellp();
                file.seekp(cur);
            }

            // Insert right before empty-header
            return m_headers.insert(m_headers.end() - /* empty header */ 1, h);
        }
        // Adds the header virtually and physically at the very end of the headers list.
        //
        // Returns true on success.
        // 
        // Remarks:
        // - Dataoffsets invalidate if moving data physically is required.
        [[nodiscard]] bool push_back(attribute_& m)
        {
            std::fstream file(m_path, std::ios_base::binary | std::ios_base::in | std::ios_base::out);
            if (!file.is_open() && !file.good())
            {
                return false;
            }
            push_back(file, m);
            return true;
        }
        // Adds the header virtually and physically at the very end of the headers list.
        // 
        // Remarks:
        // - Dataoffsets invalidate if moving data physically is required.
        [[nodiscard]] std::vector<attribute_>::iterator push_back(std::fstream& file, attribute_& m)
        {
            // Ensure we have the space available to insert the header to end
            ensure_space_attributes(file, m.bytes());

            // If back is empty section, move its data forward
            size_t rem = m.bytes();
            auto start = (m_attributes.end() - 2)->block.start;
            for (auto it = m_attributes.rbegin() + 1; it != m_attributes.rend() && rem > 0 && it->is_invalid(); ++it)
            {
                if (!it->key.empty())
                {
                    auto reduce = rem < it->key.length() ? rem : it->key.length();
                    rem -= reduce;
                    it->block.start = it->block.start + std::streampos(reduce);
                    it->key.resize(it->key.size() - reduce);
                }
                if (!it->value.empty() && rem > 0)
                {
                    auto reduce = rem < it->value.length() ? rem : it->value.length();
                    rem -= reduce;
                    it->block.start = it->block.start + std::streampos(reduce);
                    it->value.resize(it->value.size() - reduce);
                    write_attribute(file, *it);
                }
            }

            // Set block_entry
            m.block.start = start;
            m.block.end = start + std::streamoff(m.bytes());

            // Write out created header
            write_attribute(file, m);

            // Insert right before empty-section-attribute_
            return m_attributes.insert(m_attributes.end() - /* empty header */ 2, m);
        }
    public:
        pbofile() : m_good(false)
        {
        }
        pbofile(std::filesystem::path p) : m_good(false)
        {
            if (std::filesystem::exists(p))
            {
                open(p);
            }
            else
            {
                create(p);
            }
        }
        bool good() const { return m_good; }
        std::filesystem::path path() const { return m_path; }

        // // Will rearrange the files inside of the PBO, removing any free datablock
        // void defragment()
        // {
        //     // ToDo
        // }

        // Opens the provided PBO file
        void open(const std::filesystem::path &path)
        {
            std::fstream file(path, std::ios_base::binary | std::ios_base::in | std::ios_base::out);
            if (!file.is_open() && !file.good())
            {
                m_good = false;
                return;
            }
            m_path = path;
#if _DEBUG
            auto DBG_POS = file.tellg();
#endif

            // Read in version header
            std::optional<header> opt_header = read_header(file);
            if (!opt_header.has_value())
            {
                m_good = false;
                return;
            }
#if _DEBUG
            DBG_POS = file.tellg();
#endif


            // Read in attributes until we hit a "no value"
            std::optional<attribute_> opt_attribute;
            while ((opt_attribute = read_attribute(file)).has_value())
            {
                m_attributes.push_back(*opt_attribute);
            }
            attribute_ attribute_empty = {};
            attribute_empty.block.start = file.tellg();
            attribute_empty.block.end = attribute_empty.block.start + std::streamoff(1);
            m_attributes.push_back(attribute_empty);
#if _DEBUG
            DBG_POS = file.tellg();
#endif

            // Confirm we reached attributes end
            if (file.get() != '\0')
            { // we failed :(
                m_good = false;
                return;
            }
#if _DEBUG
            DBG_POS = file.tellg();
#endif


            // Read in headers until we hit a header with "no value"
            while ((opt_header = read_header(file)).has_value() && !opt_header->name.empty())
            {
                m_headers.push_back(*opt_header);
            }
            m_headers.push_back(*opt_header);
#if _DEBUG
            DBG_POS = file.tellg();
#endif


            auto offset = file.tellg();
            // Add data-sections to headers
            for (auto &it : m_headers)
            {
                it.block_data.start = offset;
                offset += it.size;
                it.block_data.end = offset;
            }

            // All fine here, end processing.
            m_good = true;
        }
        // Creates a new, empty PBO file
        void create(const std::filesystem::path& path)
        {
            std::fstream file(path, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
            if (!file.is_open() && !file.good())
            {
                m_good = false;
                return;
            }
            m_path = path;
            header version = {};
            version.method = packing_method::version;
            write_header(file, version, false);
#ifdef _DEBUG
            file.flush();
#endif

            attribute_ attribute_empty = {};
            attribute_empty.block.start = file.tellp();
            file.write("\0", 1);
#ifdef _DEBUG
            file.flush();
#endif

            header header_empty = {};
            header_empty.block_entry.start = attribute_empty.block.end = file.tellp();
            write_header(file, { }, false);
            header_empty.block_data.start = header_empty.block_data.end = header_empty.block_entry.end = file.tellp();
#ifdef _DEBUG
            file.flush();
#endif

            m_attributes.push_back(attribute_empty);
            m_headers.push_back(header_empty);

            m_good = true;
        }

        // Creates a new reader for the provided header file.
        [[nodiscard]] bool read(std::string_view filename, reader& out_reader) const
        {
            if (!good())
            {
                return false;
            }
            auto res = std::find_if(m_headers.begin(), m_headers.end(), [filename](header h) -> bool {
                return h.name == filename && !h.is_invalid();
            });
            if (res == m_headers.end())
            {
                return false;
            }
            return out_reader.initialize(m_path, *res);
        }
        // Creates a new writer, pointing at the end of this pbo for the provided file.
        // Behavior is undefined for more then one active writer.
        //
        // For existing files, actual action happening is:
        // 1. Copy old contents to the end.
        // 2. Replace header offsets.
        // 3. Create fake-header (header::empty_section) refering to the old contents.
        // 4. Seek writer to start of current file.
        [[nodiscard]] bool write(std::string_view name, writer& out_writer)
        {
            if (!good())
            {
                return false;
            }
            return out_writer.initialize(this, name);
        }

        // Returns all available attributes
        //
        // Remarks:
        // - Creates a new vector of the current attributes and returns it.
        std::vector<std::pair<std::string, std::string>> attributes() const
        {
            std::vector<std::pair<std::string, std::string>> pairs;
            pairs.reserve(m_attributes.size());
            for (auto it = m_attributes.begin(); it != m_attributes.end() - 1; ++it)
            {
                if (it->is_invalid())
                {
                    continue;
                }
                pairs.push_back(std::make_pair(it->key, it->value));
            }
            return pairs;
        }
        // Looks up a single attribute and returns it, if it exists.
        //
        // Remarks:
        // - If the attribute is not existing, empty optional will be returned
        std::optional<std::string> attribute(std::string_view key) const
        {
            auto res = std::find_if(m_attributes.begin(), m_attributes.end(), [key](attribute_ att) -> bool { return att.key == key && !att.is_invalid(); });
            if (res != m_attributes.end())
            {
                return res->value;
            }
            return {};
        }
        // Set a single attribute
        //
        // Remarks:
        // - Always appends attribute
        // - If attribute already exists, old attribute gets invalidated.
        // - Can invalidates any open writers/readers
        [[nodiscard]] bool attribute(std::string_view key, std::string_view value)
        {
            std::fstream file(m_path, std::ios_base::binary | std::ios_base::out | std::ios_base::in);
            if (!file.is_open() && !file.good())
            {
                m_good = false;
                return false;
            }

            auto res = std::find_if(m_attributes.begin(), m_attributes.end(), [key](attribute_ att) -> bool { return att.key == key; });
            if (res != m_attributes.end())
            {
                // invalidate old attribute
                std::transform(res->key.begin(), res->key.end(), res->key.begin(), [](char c) -> char { return '?'; });
                std::transform(res->value.begin(), res->value.end(), res->value.begin(), [](char c) -> char { return '?'; });
                write_attribute(file, *res);

                // Create attribute
                attribute_ att = {};
                att.key = key;
                att.value = value;
                push_back(file, att);
            }
            else
            {
                attribute_ att = {};
                att.key = key;
                att.value = value;
                push_back(file, att);
            }
            return true;
        }
        // Collects a list of all available files
        //
        // Remarks:
        // - Creates a new vector and returns it.
        std::vector<file_descriptor> files() const
        {
            std::vector<file_descriptor> descriptors;
            descriptors.reserve(m_headers.size());
            for (auto it = m_headers.begin(); it != m_headers.end() - 1; ++it)
            {
                if (it->is_invalid())
                {
                    continue;
                }
                file_descriptor descr = {};
                descr.name = it->name;
                descr.packing = it->method;
                descr.size = it->size_original;
                descr.size = it->size;
                descriptors.push_back(descr);
            }
            return descriptors;
        }

        // Truncates this pbofile by recreating it in another file.
        //
        // Remarks:
        // - Due to limitations with fstream, deleting from files is not possible.
        //   Sadly this limitation requires copying to reduce filesizes.
        bool copy_truncated(std::filesystem::path temporary) const
        {
            std::fstream copy(temporary, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
            if (!copy.is_open() && !copy.good())
            {
                return false;
            }
            std::fstream original(m_path, std::ios_base::binary | std::ios_base::out | std::ios_base::in);
            if (!original.is_open() && !original.good())
            {
                return false;
            }
            // Write version header
            header version = {};
            version.method = packing_method::version;
            write_header(copy, version, false);

            for (auto& att : m_attributes)
            {
                if (att.is_invalid() || att.block.length() == 1) { continue; }
                write_attribute(copy, att, false);
            }

            // Write attribute termination
            attribute_ attribute_empty = {};
            attribute_empty.block.start = copy.tellp();
            copy.write("\0", 1);

            for (auto& h : m_headers)
            {
                if (h.is_invalid()) { continue; }
                write_header(copy, h, false);
            }

            // Write header termination
            header header_empty = {};
            header_empty.block_entry.start = attribute_empty.block.end = copy.tellp();
            write_header(copy, { }, false);
            header_empty.block_data.start = header_empty.block_data.end = header_empty.block_entry.end = copy.tellp();

            for (auto& h : m_headers)
            {
                if (h.is_invalid()) { continue; }
                const size_t buffsize = 2048;
                char buff[buffsize] = {};
                auto pos = h.block_data.start;
                original.seekg(h.block_data.start);
                while (pos != h.block_data.end)
                {
                    auto remaining = h.block_data.end - pos;
                    remaining = remaining < buffsize ? remaining : buffsize;
                    original.read(buff, remaining);
                    copy.write(buff, remaining);
                    pos += remaining;
                }
            }
            return true;
        }
    };
}