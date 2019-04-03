#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <mutex>
#include <queue>
#include <sstream>
#include "../networking.h"

namespace sqf
{
	namespace networking
	{
		class network_server
		{
		public:
			class client
			{
				const size_t buffersize = 2024;
				char m_buffer[2024];
				bool m_die;

				SOCKET m_socket;
				SOCKETADDR m_sockaddr;
				std::stringstream m_builder;
				std::queue<std::string> m_messages_in;
				std::queue<std::string> m_messages_out;
				std::mutex m_mutex_in;
				std::mutex m_mutex_out;
				std::thread m_thread;
				bool m_alive;

				bool poll_queue();
				bool send_queue();
				static void THREAD_CLIENT_HANDLE(network_server* srv, client* client);
			public:
				client(network_server* srv, SOCKET socket, SOCKETADDR sockaddr);
				~client() { stop(); }
				void stop();
			};
		private:
			static void THREAD_SERVER_ACCEPT(network_server* srv);

			SOCKET m_socket;
			std::thread m_thread;
			bool m_die;
			bool m_accept;
			std::mutex m_clients_mutex;
			std::vector<std::shared_ptr<client>> m_clients;

			void push_client(std::shared_ptr<client> c)
			{
				std::lock_guard<std::mutex> guard(m_clients_mutex);
				m_clients.push_back(c);
			}
			void remove_client(std::shared_ptr<client> c)
			{
				std::lock_guard<std::mutex> guard(m_clients_mutex);
				auto res = std::find_if(m_clients.begin(), m_clients.end(), [c](std::shared_ptr<client> it) -> bool { return it.get() == c.get(); });
				if (res == m_clients.end())
				{
					return;
				}
				std::remove(m_clients.begin(), m_clients.end(), res);
			}
		public:
			network_server(unsigned short port);
			~network_server() { stop(); }

			std::vector<std::shared_ptr<client>> clients()
			{
				std::lock_guard<std::mutex> guard(m_clients_mutex);
				auto copy = std::vector<std::shared_ptr<client>>(m_clients);
				return m_clients;
			}
			void stop();
		};
	}
}