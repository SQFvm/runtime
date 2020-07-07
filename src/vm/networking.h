#pragma once


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if defined(_WIN32)
	/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")
#else
	/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#define INVALID_SOCKET -1
	typedef int SOCKET;
#endif
	typedef struct sockaddr_in SOCKETADDR;


	//https://stackoverflow.com/questions/28027937/cross-platform-sockets

	int networking_init(void);
	int networking_cleanup(void);

	int networking_initialized(int doinit, int uninit);
	int networking_close(SOCKET);
	// param1: The IP Address
	// param2: The PORT
	// param3: OUT parameter, the resulting socket
	// return: 0 (false) if everything is fine, 1 (true) if some error happened.
	int networking_create_client(const char*, const char*, SOCKET*);
	int networking_create_server(SOCKET *);
	int networking_server_bind(SOCKET*, unsigned short);
	int networking_server_listen(SOCKET*, int);
	int networking_poll(SOCKET* s, int timeoutms);
	int networking_server_accept_block(SOCKET*, SOCKET*, SOCKETADDR*);

#ifdef __cplusplus
}
#endif // __cplusplus
