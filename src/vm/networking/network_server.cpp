#include "network_server.h"
#include "network_client.h"
#include "../networking.h"
#include <exception>



void sqf::networking::server::THREAD_SERVER_ACCEPT(server * srv)
{
	while (!srv->m_die)
	{
		SOCKET sclient;
		SOCKETADDR aclient;
		if (!networking_server_accept_block(&srv->m_socket, &sclient, &aclient))
		{
			srv->push_client(std::make_shared<client>(sclient));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


void sqf::networking::server::push_client(std::shared_ptr<client> c)
{
	std::lock_guard<std::mutex> guard(m_clients_mutex);
	m_clients.push_back(c);
}

void sqf::networking::server::remove_client(std::shared_ptr<client> c)
{
	std::lock_guard<std::mutex> guard(m_clients_mutex);
	auto res = std::find_if(m_clients.begin(), m_clients.end(), [c](std::shared_ptr<client> it) -> bool { return it.get() == c.get(); });
	if (res == m_clients.end())
	{
		return;
	}
	m_clients.erase(res);
}

sqf::networking::server::server(unsigned short port) : m_die(false)
{
	if (networking_create_server(&m_socket))
	{
		throw std::runtime_error("Coult not create server-socket.");
	}
	if (networking_server_bind(&m_socket, port))
	{
		networking_close(m_socket);
		throw std::runtime_error("Coult not bind to provided port.");
	}
	if (networking_server_listen(&m_socket, 3))
	{
		networking_close(m_socket);
		throw std::runtime_error("Coult not start listening.");
	}
	m_thread = std::thread([this]() { THREAD_SERVER_ACCEPT(this); });
	//_currentThread.detach();
}

void sqf::networking::server::stop()
{
	m_die = true;
	networking_close(m_socket);
	for (auto& c : m_clients)
	{
		c->stop();
	}
	m_thread.join();
}

void sqf::networking::server::send_message(const std::string s)
{
	for (auto it : m_clients)
	{
		it->send_message(s);
	}
}
