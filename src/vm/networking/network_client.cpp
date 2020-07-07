#include "network_client.h"
#include "../networking.h"

bool sqf::networking::client::poll_queue()
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

bool sqf::networking::client::send_queue()
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

void sqf::networking::client::THREAD_CLIENT_HANDLE(client* clnt)
{
	clnt->m_alive = true;
	bool flag = true;
	while (!clnt->m_die && flag)
	{
		if (networking_poll(&(clnt->m_socket), 100))
		{
			try { clnt->poll_queue(); }
			catch (const std::exception & err) {}
		}

		try { flag = clnt->send_queue(); }
		catch (const std::exception & err) { flag = false; }
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	try
	{
		std::lock_guard<std::mutex> guard(clnt->m_mutex_out);
		while (!clnt->m_messages_out.empty())
		{
			clnt->m_messages_out.pop();
		}
	}
	catch (const std::exception & err) {}
	networking_close(clnt->m_socket);
	clnt->m_alive = false;
}

void sqf::networking::client::stop()
{
	m_die = true;
	networking_close(m_socket);
	m_thread.join();
}
