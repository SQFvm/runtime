#pragma once
#include "value.h"
#include "diagnostics/diag_info.h"

#include <vector>
#include <initializer_list>

namespace sqf::runtime
{
    enum class opcodes : uint8_t
    {
        nop,
        assign_to,
        assign_to_local,
        get_variable,
        call_nular,
        call_unary,
        call_binary,
        push,
        make_array,
        end_statement,

        meta_diagnostics = 100,
        meta_nil,
        meta_array,
        meta_code,
        meta_float,
        meta_string
    };
    class instruction_blob;
    class instruction_handle
    {
        const uint8_t* m_start;
        const uint8_t* m_end;
        const uint8_t* m_pc;
        friend class instruction_blob;
        instruction_handle(const instruction_blob& blob);
    public:
        instruction_handle(const instruction_handle& h) : m_start(h.m_start), m_end(h.m_end), m_pc(h.m_pc) { }

        bool eos() const { return m_pc >= m_end; }

        const uint8_t* pc() const { return m_pc; }
        const uint8_t* start() const { return m_start; }
        const uint8_t* end() const { return m_end; }

        template<typename T> T read() { return T::read(); }

        template<> uint8_t read<uint8_t>() { return *reinterpret_cast<const uint8_t*>(m_pc++); }
        template<> int8_t read<int8_t>() { return *reinterpret_cast<const int8_t*>(m_pc++); }
        template<> uint16_t read<uint16_t>() { return (static_cast<uint16_t>(read<uint8_t>())) | (static_cast<uint16_t>(read<uint8_t>()) << 8); }
        template<> int16_t read<int16_t>() { return (static_cast<int16_t>(read<uint8_t>())) | (static_cast<int16_t>(read<uint8_t>()) << 8); }
        template<> uint32_t read<uint32_t>() { return (static_cast<uint32_t>(read<uint16_t>())) | (static_cast<uint32_t>(read<uint16_t>()) << 16); }
        template<> int32_t read<int32_t>() { return (static_cast<int32_t>(read<uint16_t>())) | (static_cast<int32_t>(read<uint16_t>()) << 16); }
        template<> uint64_t read<uint64_t>() { return (static_cast<uint64_t>(read<uint32_t>())) | (static_cast<uint64_t>(read<uint32_t>()) << 32); }
        template<> int64_t read<int64_t>() { return (static_cast<int64_t>(read<uint32_t>())) | (static_cast<int64_t>(read<uint32_t>()) << 32); }
        template<> float read<float>() { auto val = read<uint32_t>(); return *reinterpret_cast<float*>(&val); }
        template<> double read<double>() { auto val = read<uint64_t>(); return *reinterpret_cast<double*>(&val); }
        template<> bool read<bool>() { return *m_pc++; }
        template<> opcodes read<opcodes>() { return static_cast<opcodes>(read<uint8_t>()); }

        template<> std::string_view read<std::string_view>()
        {
            auto length = read<uint64_t>();
            auto tmp = std::string_view(reinterpret_cast<const char*>(m_pc), length);
            m_pc += length;
            return tmp;
        }
    };

    /// <summary>
    /// A way to represent a "immutable" instruction set.
    /// </summary>
    class instruction_blob final
    {
    public:
        using iterator = std::vector<uint8_t>::const_iterator;
        using reverse_iterator = std::vector<uint8_t>::const_reverse_iterator;
    private:
        friend instruction_handle;
        std::vector<uint8_t> m_data;
        bool m_final;
    public:
        instruction_blob() noexcept :
            m_final(false)
        {}
        instruction_blob(std::initializer_list<uint8_t> initializer) noexcept :
            m_data(initializer.begin(), initializer.end()),
            m_final(true)
        {}
        instruction_blob(std::vector<uint8_t> instructions) noexcept :
            m_data(std::move(instructions)),
            m_final(true)
        {}
        template<typename Iterator>
        instruction_blob(Iterator start, Iterator end) noexcept :
            m_data(start, end),
            m_final(true)
        {}

        iterator begin() const noexcept { return m_data.begin(); }
        iterator end() const noexcept { return m_data.end(); }
        reverse_iterator rbegin() const noexcept { return m_data.rbegin(); }
        reverse_iterator rend() const noexcept { return m_data.rend(); }
        bool empty() const noexcept { return m_data.empty(); }
        size_t size() const noexcept { return m_data.size(); }
        bool final() const noexcept { return m_final; }
        void finalize() noexcept { m_final = true; }
        template<typename Iterator>
        void append(Iterator start, Iterator end)
        {
            m_data.insert(m_data.end(), start, end);
        }

        [[nodiscard]] instruction_handle create_handle() const noexcept { return { *this }; }

        template<typename T> void write(T value) { if (!m_final) { value.write(*this); } }

        template<> void write<uint8_t>(uint8_t value) { if (!m_final) { m_data.push_back(value); } }
        template<> void write<int8_t>(int8_t value) { if (!m_final) { m_data.push_back(value); } }
        template<> void write<uint16_t>(uint16_t value) { write<uint8_t>(value & 0xFF); write<uint8_t>((value >> 8) & 0xFF); }
        template<> void write<int16_t>(int16_t value) { write<int8_t>(value & 0xFF); write<int8_t>((value >> 8) & 0xFF); }
        template<> void write<uint32_t>(uint32_t value) { write<uint16_t>(value & 0xFFFF); write<uint16_t>((value >> 16) & 0xFFFF); }
        template<> void write<int32_t>(int32_t value) { write<int16_t>(value & 0xFFFF); write<int16_t>((value >> 16) & 0xFFFF); }
        template<> void write<uint64_t>(uint64_t value) { write<uint32_t>(value & 0xFFFFFFFF); write<uint32_t>((value >> 32) & 0xFFFFFFFF); }
        template<> void write<int64_t>(int64_t value) { write<int32_t>(value & 0xFFFFFFFF); write<int32_t>((value >> 32) & 0xFFFFFFFF); }
        template<> void write<float>(float value) { write<uint32_t>(*reinterpret_cast<uint32_t*>(&value)); }
        template<> void write<double>(double value) { write<uint64_t>(*reinterpret_cast<uint64_t*>(&value)); }
        template<> void write<bool>(bool value) { write<uint8_t>(value); }
        template<> void write<opcodes>(opcodes value) { write<uint8_t>(static_cast<uint8_t>(value)); }

        template<>
        void write<std::string_view>(std::string_view value)
        {
            if (!m_final)
            {
                write<uint64_t>(value.length());
                m_data.insert(m_data.end(), value.begin(), value.end());
            }
        }
    };
    instruction_handle::instruction_handle(const instruction_blob& blob) :
        m_start(blob.m_data.data()),
        m_end(blob.m_data.data() + blob.m_data.size()),
        m_pc(blob.m_data.data())
    {
    }


    namespace instructions
    {
        struct nop
        {
            std::string_view variable_name;
            void execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info) const noexcept { }
            static nop read(instruction_handle& h) noexcept { return { }; }
            void write(instruction_blob& b) const { b.write<opcodes>(opcodes::nop); }
        };
        struct assign_to
        {
            std::string_view variable_name;
            void execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const;
            static assign_to read(instruction_handle& h) { return { h.read<std::string_view>() }; }
            void write(instruction_blob& b) const
            {
                b.write<opcodes>(opcodes::assign_to);
                b.write<std::string_view>(variable_name);
            }
        };
        struct assign_to_local
        {
            std::string_view variable_name;
            void execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const;
            static assign_to_local read(instruction_handle& h) { return { h.read<std::string_view>() }; }
            void write(instruction_blob& b) const
            {
                b.write<opcodes>(opcodes::assign_to_local);
                b.write<std::string_view>(variable_name);
            }
        };
        struct get_variable
        {
            std::string_view variable_name;
            void execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const;
            static get_variable read(instruction_handle& h) { return { h.read<std::string_view>() }; }
            void write(instruction_blob& b) const
            {
                b.write<opcodes>(opcodes::get_variable);
                b.write<std::string_view>(variable_name);
            }
        };
        struct call_nular
        {
            std::string_view operator_name;
            void execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const;
            static call_nular read(instruction_handle& h) { return { h.read<std::string_view>() }; }
            void write(instruction_blob& b) const
            {
                b.write<opcodes>(opcodes::call_nular);
                b.write<std::string_view>(operator_name);
            }
        };
        struct call_unary
        {
            std::string_view operator_name;
            void execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const;
            static call_unary read(instruction_handle& h) { return { h.read<std::string_view>() }; }
            void write(instruction_blob& b) const
            {
                b.write<opcodes>(opcodes::call_unary);
                b.write<std::string_view>(operator_name);
            }
        };
        struct call_binary
        {
            std::string_view operator_name;
            void execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const;
            static call_binary read(instruction_handle& h) { return { h.read<std::string_view>() }; }
            void write(instruction_blob& b) const
            {
                b.write<opcodes>(opcodes::call_binary);
                b.write<std::string_view>(operator_name);
            }
        };
        struct push
        {
            value data;
            void execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const;
            static push read(instruction_handle& h);
            void write(instruction_blob& b) const;
        };
        struct make_array
        {
            size_t size;
            void execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const;
            static make_array read(instruction_handle& h) { return { (size_t)h.read<uint64_t>() }; }
            void write(instruction_blob& b) const
            {
                b.write<opcodes>(opcodes::make_array);
                b.write<uint64_t>(size);
            }
        };
        struct end_statement
        {
            void execute(sqf::runtime::runtime& vm, sqf::runtime::diagnostics::diag_info& diag_info) const;
            static end_statement read(instruction_handle& h) noexcept { return { }; }
            void write(instruction_blob& b) const
            {
                b.write<opcodes>(opcodes::end_statement);
            }
        };
    }
}