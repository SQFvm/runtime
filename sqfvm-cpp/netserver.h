#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <queue>
#include "networking.h"

#if defined(_WIN32)
#elif defined(_linux)
#endif

class netserver
{
private:
	const unsigned short _port;
	SOCKET _socket;
	std::stringstream builder;
	std::thread _currentThread;
	std::queue<std::string> _messageQueueIn;
	std::queue<std::string> _messageQueueOut;
	bool _die;
	bool _accept;
	std::mutex _inLock;
	std::mutex _outLock;

#define BUFFSIZE 2048
	char buffer[BUFFSIZE];


	static void THREAD_METHOD(netserver* srv)
	{
		//ToDo: Accept outter loop

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
						catch (std::runtime_error err) {}
						srv->_inLock.unlock();
					}

					srv->_outLock.lock();
					try { flag = srv->send_queue(sclient); }
					catch (std::runtime_error err) { flag = false; }
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
				catch (std::runtime_error err) {}
				srv->_outLock.unlock();
				networking_close(sclient);
				srv->_accept = false;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	bool poll_queue(SOCKET client)
	{
		auto bytecount = recv(client, buffer, BUFFSIZE, 0);
		bool flag = false;
		for (int i = 0; i < bytecount; i++)
		{
			if (buffer[i] == '\0')
			{
				flag = true;
				auto msg = builder.str();
				builder.clear();
				_messageQueueIn.push(msg);
			}
			else
			{
				builder << buffer[i];
			}
		}
		return flag;
	}
	bool send_queue(SOCKET client)
	{
		bool flag = true;
		while (!_messageQueueOut.empty())
		{
			auto msg = _messageQueueOut.back();
			_messageQueueOut.pop();
			int res = send(client, msg.c_str(), msg.length() + 1, 0);
			flag = res >= 0;
		}
		return flag;
	}

public:
	netserver(unsigned short port) : _port(port), builder(), _accept(false)
	{
		if (networking_create_server(&_socket))
		{
			throw new std::runtime_error("Coult not create server-socket.");
		}
		if (networking_server_bind(&_socket, port))
		{
			networking_close(_socket);
			throw new std::runtime_error("Coult not bind to provided port.");
		}
		if (networking_server_listen(&_socket, 3))
		{
			networking_close(_socket);
			throw new std::runtime_error("Coult not bind to provided port.");
		}
		_currentThread = std::thread(THREAD_METHOD, this);
		_currentThread.detach();
	}
	~netserver()
	{
		force_close();
	}


	std::string next_message(void) { _inLock.lock(); auto str = _messageQueueIn.back(); _messageQueueIn.pop(); _inLock.unlock(); return str; }
	bool has_message(void) { return !_messageQueueIn.empty(); }
	void request_close(void) { _die = true; }
	void force_close(void) { _die = true; networking_close(_socket); _currentThread.join(); }
	void wait_accept(void) {
		while (!_accept) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	bool is_accept(void) { return _accept; }

	void push_message(std::string str) { _outLock.lock(); _messageQueueOut.push(str); _outLock.unlock(); }
};