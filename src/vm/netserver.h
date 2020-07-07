#pragma once
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

	static void THREAD_METHOD(netserver* srv);
	bool poll_queue(SOCKET client);
	bool send_queue(SOCKET client);

public:
	netserver(unsigned short port);
	~netserver();

	std::string next_message() { _inLock.lock(); auto str = _messageQueueIn.back(); _messageQueueIn.pop(); _inLock.unlock(); return str; }
	bool has_message() { return !_messageQueueIn.empty(); }
	void request_close() { _die = true; }
	void force_close();
	void wait_accept();
	bool is_accept() { return _accept; }

	void push_message(std::string str) { _outLock.lock(); _messageQueueOut.push(str); _outLock.unlock(); }
};