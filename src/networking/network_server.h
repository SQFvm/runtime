#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <mutex>
#include <queue>
#include <thread>
#include <sstream>
#include "networking.h"

namespace sqf
{
	namespace networking
	{
		class client;
		class server
		{
		private:
			static void THREAD_SERVER_ACCEPT(server* srv);

			SOCKET m_socket;
			std::thread m_thread;
			bool m_die;
			std::mutex m_clients_mutex;
			std::vector<std::shared_ptr<client>> m_clients;

			void push_client(std::shared_ptr<client> c);
			void remove_client(std::shared_ptr<client> c);
		public:
			server(unsigned short port);
			~server() { stop(); }

			std::vector<std::shared_ptr<client>> clients()
			{
				std::lock_guard<std::mutex> guard(m_clients_mutex);
				auto copy = std::vector<std::shared_ptr<client>>(m_clients);
				return m_clients;
			}
			void stop();

			void send_message(const char* cstr) { send_message(std::string(cstr)); }
			void send_message(const std::string s);
		};
	}
}