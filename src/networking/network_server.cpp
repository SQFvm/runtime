#include "network_server.h"
#include <exception>

bool sqf::networking::network_server::client::poll_queue()
{
	std::lock_guard<std::mutex> guard(m_mutex_in);
	auto bytecount = recv(m_socket, m_buffer, buffersize, 0);
	bool flag = false;
	for (int i = 0; i < bytecount; i++)
	{
		if (m_buffer[i] == '\0')
		{
			flag = true;
			auto msg = m_builder.str();
			m_builder.str(std::string());
			m_messages_in.push(msg);
		}
		else
		{
			m_builder << m_buffer[i];
		}
	}
	return flag;
}
bool sqf::networking::network_server::client::send_queue()
{
	std::lock_guard<std::mutex> guard(m_mutex_out);
	bool flag = true;
	while (!m_messages_out.empty())
	{
		auto msg = m_messages_out.front();
		m_messages_out.pop();
		int res = send(m_socket, msg.c_str(), msg.length() + 1, 0);
		flag = res >= 0;
	}
	return flag;
}

sqf::networking::network_server::client::client(network_server* srv, SOCKET socket, SOCKETADDR sockaddr) : m_die(false), m_socket(socket), m_sockaddr(sockaddr), m_builder(std::stringstream::out|std::stringstream::in|std::stringstream::binary), m_alive(false)
{
	m_thread = std::thread([srv, this]() { THREAD_CLIENT_HANDLE(srv, this); });
}

void sqf::networking::network_server::client::stop()
{
	m_die = true;
	networking_close(m_socket);
	m_thread.join();
}

void sqf::networking::network_server::client::THREAD_CLIENT_HANDLE(network_server * srv, client * client)
{
	client->m_alive = true;
	bool flag = true;
	while (!srv->m_die && !client->m_die && flag)
	{
		if (networking_poll(&(client->m_socket), 100))
		{
			try { client->poll_queue(); }
			catch (const std::exception& err) {}
		}

		try { flag = client->send_queue(); }
		catch (const std::exception& err) { flag = false; }
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	try
	{
		std::lock_guard<std::mutex> guard(client->m_mutex_out);
		while (!client->m_messages_out.empty())
		{
			client->m_messages_out.pop();
		}
	}
	catch (const std::exception& err) {}
	networking_close(client->m_socket);
	client->m_alive = false;
}

void sqf::networking::network_server::THREAD_SERVER_ACCEPT(network_server * srv)
{
	while (!srv->m_die)
	{
		SOCKET sclient;
		SOCKETADDR aclient;
		if (!networking_server_accept_block(&srv->m_socket, &sclient, &aclient))
		{
			srv->push_client(std::make_shared<client>(srv, sclient, aclient));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


sqf::networking::network_server::network_server(unsigned short port)
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
		throw std::runtime_error("Coult not bind to provided port.");
	}
	m_thread = std::thread([this]() { THREAD_SERVER_ACCEPT(this); });
	//_currentThread.detach();
}

void sqf::networking::network_server::stop()
{
	m_die = true;
	networking_close(m_socket);
	for (auto& c : m_clients)
	{
		c->stop();
	}
	m_thread.join();
}