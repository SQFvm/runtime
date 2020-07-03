#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <sstream>
#include <type_traits>
#include <algorithm>

namespace sqf
{
	namespace networking
	{
		template<class Stream>
		class streamable
		{
		public:
			enum byteorder
			{
				big_edian,
				little_edian
			};
		private:
			byteorder test_byteorder()
			{
				union {
					uint16_t i;
					char c[2];
				} byteordertest = { 0x0102 };
				return byteordertest.c[0] == 0x01 ? big_edian : little_edian;
			}
			Stream& m_stream;
			byteorder m_localbyteorder;
			byteorder m_expectedbyteorder;
		public:
			streamable(Stream& stream) : m_stream(stream), m_localbyteorder(test_byteorder()), m_expectedbyteorder(big_edian) {}
			streamable(Stream& stream, byteorder expected) : m_stream(stream), m_localbyteorder(test_byteorder()), m_expectedbyteorder(expected) {}
			template<typename T, size_t bytes>
			typename std::enable_if<std::is_integral<T>::value, T>::type read_bytes()
			{
				static_assert(sizeof(T) >= bytes, "Chosen datatype cannot hold expected bytes");
				T val;
				m_stream.read(((char*)(void*)&val) + (sizeof(T) - bytes), bytes);
				if (m_localbyteorder != m_expectedbyteorder)
				{
					std::reverse(((char*)(void*)&val), ((char*)(void*)&val) + sizeof(T));
				}
				if (sizeof(T) != bytes)
				{
					val >>= (sizeof(T) - bytes) * 8;
				}
				return val;
			}
			template<typename T>
			typename std::enable_if<std::is_arithmetic<T>::value, T>::type read()
			{
				T val;
				m_stream.read((char*)(void*)&val, sizeof(T));
				if (m_localbyteorder != m_expectedbyteorder)
				{
					std::reverse(((char*)(void*)&val), ((char*)(void*)&val) + sizeof(T));
				}
				return val;
			}
			template<typename T>
			typename std::enable_if<std::is_same<T, std::string>::value, T>::type read()
			{
				std::string val;
				auto len = read_size();
				val.resize(len);
				m_stream.read(&val[0], len);
				return val;
			}
			template<typename T, size_t Size>
			typename std::array<T, Size> read()
			{
				std::array<T, Size> val;
				for (auto& it : val)
				{
					it = read<T>();
				}
				return val;
			}
			template<template <typename, class> class C, typename T, typename A = std::allocator<T>>
			C<T, A> read()
			{ // ToDo: Increase performance by reading blocks
				C<T, A> cont;
				auto len = read_size();
				cont.reserve(len);
				for (size_t i = 0; i < len; i++)
				{
					cont.push_back(read<T>());
				}
				return cont;
			}
			void read() { read<int8_t>(); }

			size_t read_size()
			{
				auto size_prefix = read<int8_t>();
				if ((size_prefix & 0b10000000) == 0b00000000)
				{
					return read_bytes<int8_t, 1>();
				}
				else if ((size_prefix & 0b11000000) == 0b10000000)
				{
					return read_bytes<int16_t, 2>();
				}
				else if ((size_prefix & 0b11100000) == 0b11000000)
				{
					return read_bytes<int32_t, 3>();
				}
				else if ((size_prefix & 0b11110000) == 0b11100000)
				{
					return read_bytes<int32_t, 4>();
				}
				else if ((size_prefix & 0b11111000) == 0b11110000)
				{
					return read_bytes<int64_t, 5>();
				}
				else if ((size_prefix & 0b11111100) == 0b11111000)
				{
					return read_bytes<int64_t, 6>();
				}
				else if ((size_prefix & 0b11111110) == 0b11111100)
				{
					return read_bytes<int64_t, 7>();
				}
				else
				{
					return read_bytes<int64_t, 8>();
				}
			}


			template<typename T, size_t bytes>
			void write_bytes(typename std::enable_if<std::is_integral<T>::value, T>::type val)
			{
				static_assert(sizeof(T) >= bytes, "Chosen datatype cannot hold expected bytes");
				if (m_localbyteorder != m_expectedbyteorder)
				{
					std::reverse(((char*)(void*)&val), ((char*)(void*)&val) + sizeof(T));
				}
				m_stream.write(((const char*)(void*)&val) + (sizeof(T) - bytes), bytes);
			}
			template<typename T>
			void write(typename std::enable_if<std::is_arithmetic<T>::value, T>::type val)
			{
				if (m_localbyteorder != m_expectedbyteorder)
				{
					std::reverse(((char*)(void*)&val), ((char*)(void*)&val) + sizeof(T));
				}
				m_stream.write((const char*)(void*)&val, sizeof(T));
			}
			template<typename T>
			void write(typename std::enable_if<std::is_same<T, std::string>::value, T>::type val)
			{
				write_size(val.length());
				m_stream.write(&val[0], val.length());
			}
			template<typename T, size_t Size>
			typename void write(std::array<T, Size> val)
			{
				for (auto& it : val)
				{
					write<T>(it);
				}
			}
			template<template <typename, class> class C, typename T, typename A = std::allocator<T>>
			typename void write(C<T, A> val)
			{ // ToDo: Increase performance by writing blocks
				write_size(val.size());
				for (auto& it : val)
				{
					write<T>(it);
				}
			}
			void write() { write<int8_t>(0); }
			void write_size(size_t val)
			{
				if (val <= 0xFF)
				{
					write<int8_t>(0b00000000);
					write_bytes<int8_t, 1>(val);
				}
				else if (val <= 0xFFFF)
				{
					write<int8_t>(0b10000000);
					write_bytes<int16_t, 2>(val);
				}
				else if (val <= 0xFFFFFF)
				{
					write<int8_t>(0b11000000);
					write_bytes<int32_t, 3>(val);
				}
				else if (val <= 0xFFFFFFFF)
				{
					write<int8_t>(0b11100000);
					write_bytes<int32_t, 4>(val);
				}
				else if (val <= 0xFFFFFFFFFF)
				{
					write<int8_t>(0b11110000);
					write_bytes<int64_t, 5>(val);
				}
				else if (val <= 0xFFFFFFFFFFFF)
				{
					write<int8_t>(0b11111000);
					write_bytes<int64_t, 6>(val);
				}
				else if (val <= 0xFFFFFFFFFFFFFF)
				{
					write<int8_t>(0b11111100);
					write_bytes<int64_t, 7>(val);
				}
				else
				{
					write<int8_t>(0b11111110);
					write_bytes<int64_t, 8>(val);
				}
			}
		};
	}
}