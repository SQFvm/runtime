#pragma once
#include <mutex>
#include <queue>
#include <sstream>
#include <array>
#include <vector>
#include <thread>
#include <optional>
#include "networking.h"
#include "network_server.h"

namespace sqf
{
	namespace networking
	{
		class client
		{
			const size_t buffersize = 2024;
			char m_buffer[2024];
			bool m_die;

			SOCKET m_socket;
			std::stringstream m_builder;
			std::queue<std::string> m_messages_in;
			std::queue<std::string> m_messages_out;
			std::mutex m_mutex_in;
			std::mutex m_mutex_out;
			std::thread m_thread;
			bool m_alive;

			bool poll_queue();
			bool send_queue();
			static void THREAD_CLIENT_HANDLE(client* clnt);
		public:
			client(SOCKET socket)
				: m_die(false),
				m_socket(socket),
				m_builder(std::stringstream::out | std::stringstream::in | std::stringstream::binary),
				m_alive(false)
			{
				m_thread = std::thread([this]() { THREAD_CLIENT_HANDLE(this); });
			}
			~client() { stop(); }
			void stop();

			void send_message(const char* cstr) { send_message(std::string(cstr)); }
			void send_message(const std::string s) 
			{
				std::lock_guard<std::mutex> guard(m_mutex_out);
				this->m_messages_out.push(s);
			}
			std::optional<std::string> read_message(const std::string)
			{
				std::lock_guard<std::mutex> guard(m_mutex_in);
				if (this->m_messages_in.size() == 0)
				{
					return {};
				}
				auto res = this->m_messages_in.front();
				this->m_messages_in.pop();
				return res;
			}
		};
	}
}