#include "networking.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#ifndef WIN32
#include"poll.h"
#endif // ! WIN32




int networking_init()
{
	if (networking_initialized(1, 0))
	{
		return 1;
	}
#ifdef WIN32
	WSADATA wsa_data;
	return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
	return 0;
#endif
}

int networking_cleanup()
{
	if (!networking_initialized(0, 1))
	{
		return 1;
	}
#ifdef WIN32
	return WSACleanup();
#else
	return 0;
#endif
}

int networking_initialized(int doinit, int uninit)
{
	static unsigned char flag = 0;
	if (uninit)
	{
		if (!flag)
		{
			return 1;
		}
		flag = 0;
		return 0;
	}
	else
	{
		if (flag)
		{
			return 1;
		}
		if (doinit)
		{
			flag = ~0;
		}
		return 0;
	}
}
int networking_close(SOCKET sock)
{
	int status;

#ifdef WIN32
	status = shutdown(sock, SD_BOTH);
	if (status == 0) { status = closesocket(sock); }
#else
	status = shutdown(sock, SHUT_RDWR);
	if (status == 0) { status = close(sock); }
#endif

	return status;

}
int networking_create_client(const char* ip, const char* port, SOCKET* outSocket)
{
	struct addrinfo *ptr = NULL, hints;
	int res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	res = getaddrinfo(ip, port, &hints, &ptr);
	if (res)
	{
		return 1;
	}
	*outSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	
	if (*outSocket == INVALID_SOCKET)
	{
		*outSocket = INVALID_SOCKET;
		freeaddrinfo(ptr);
		return 2;
	}
	
	res = connect(*outSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (res)
	{
		freeaddrinfo(ptr);
		return 3;
	}
	freeaddrinfo(ptr);
	return 0;
}
int networking_create_server(SOCKET* outSocket)
{
	*outSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (*outSocket == INVALID_SOCKET)
	{
		*outSocket = INVALID_SOCKET;
		return 1;
	}
	return 0;
}

int networking_server_listen(SOCKET* s, int conqueue)
{
	int res = listen(*s, conqueue);
#ifdef WIN32
	if (res)
	{
		res = WSAGetLastError();
	}
#endif
	return res;
}
int networking_poll(SOCKET* s, int timeoutms)
{
#ifdef WIN32
	WSAPOLLFD poll_set[1];
	poll_set[0].fd = *s;
	poll_set[0].events = POLLRDBAND | POLLRDNORM;
	return WSAPoll((LPWSAPOLLFD)&poll_set, 1, timeoutms) > 0;
#else
	struct pollfd poll_set[1];
	poll_set[0].fd = *s;
	poll_set[0].events = POLLIN;
	return poll(poll_set, 1, timeoutms) > 0;
#endif
}
int networking_server_bind(SOCKET* s, unsigned short port)
{
	int res;
	SOCKETADDR server;
	server.sin_family = AF_INET;
#ifdef WIN32
	server.sin_addr.S_un.S_addr = INADDR_ANY;
#else
	server.sin_addr.s_addr = INADDR_ANY;
#endif
	server.sin_port = htons(port);
	res = bind(*s, (struct sockaddr *)&server, sizeof(server));
	return res;
}
int networking_server_accept_block(SOCKET* s, SOCKET* out, SOCKETADDR* clientout)
{
	socklen_t len = sizeof(SOCKETADDR);
	*out = accept(*s, (struct sockaddr*)&clientout, &len);
#ifdef WIN32
	return *out != INVALID_SOCKET ? 0 : WSAGetLastError();
#else
	return *out != INVALID_SOCKET;
#endif
}
#ifdef __cplusplus
}
#endif // __cplusplus
