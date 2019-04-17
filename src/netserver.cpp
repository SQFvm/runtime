#include "netserver.h"

void netserver::THREAD_METHOD(netserver* srv)
{
	while (!srv->_die)
	{
		SOCKET sclient;
		SOCKETADDR aclient;
		if (!networking_server_accept_block(&srv->_socket, &sclient, &aclient))
		{
			bool flag = true;
			srv->_accept = true;
			while (!srv->_die && flag)
			{
				if (networking_poll(&sclient, 100))
				{
					srv->_inLock.lock();
					try { srv->poll_queue(sclient); }
					catch (const std::runtime_error& /*err*/) {}
					srv->_inLock.unlock();
				}

				srv->_outLock.lock();
				try { flag = srv->send_queue(sclient); }
				catch (const std::runtime_error& /*err*/) { flag = false; }
				srv->_outLock.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			srv->_outLock.lock();
			try
			{
				while (!srv->_messageQueueOut.empty())
				{
					srv->_messageQueueOut.pop();
				}
			}
			catch (const std::runtime_error& /*err*/) {}
			srv->_outLock.unlock();
			networking_close(sclient);
			srv->_accept = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
bool netserver::poll_queue(SOCKET client)
{
	auto bytecount = recv(client, buffer, BUFFSIZE, 0);
	bool flag = false;
	for (int i = 0; i < bytecount; i++)
	{
		if (buffer[i] == '\0')
		{
			flag = true;
			auto msg = builder.str();
			builder.str(std::string());
			_messageQueueIn.push(msg);
		}
		else
		{
			builder << buffer[i];
		}
	}
	return flag;
}
bool netserver::send_queue(SOCKET client)
{
	bool flag = true;
	while (!_messageQueueOut.empty())
	{
		auto msg = _messageQueueOut.front();
		_messageQueueOut.pop();
		int res = send(client, msg.c_str(), static_cast<int>(msg.length()) + 1, 0);
		flag = res >= 0;
	}
	return flag;
}

netserver::netserver(unsigned short port) : _port(port), _die(false), _accept(false)
{
	if (networking_create_server(&_socket))
	{
		throw std::runtime_error("Coult not create server-socket.");
	}
	if (networking_server_bind(&_socket, port))
	{
		networking_close(_socket);
		throw std::runtime_error("Coult not bind to provided port.");
	}
	if (networking_server_listen(&_socket, 3))
	{
		networking_close(_socket);
		throw std::runtime_error("Coult not bind to provided port.");
	}
	_currentThread = std::thread([this]() { THREAD_METHOD(this); });
	//_currentThread.detach();
}
netserver::~netserver()
{
	force_close();
}


void netserver::force_close() {
	_die = true;
	networking_close(_socket);
	_currentThread.join();
}
void netserver::wait_accept() {
	while (!_accept)
	{
		if (_die)
		{
			throw std::runtime_error("server died");
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
