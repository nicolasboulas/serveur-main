#pragma once

#ifndef __serveur_h__ 
#define __serveur_h__ 

#include <winsock2.h>

#define _WINSOCK_DEPRECATED

#define _WINSOCK_DEPRECATED_NO_WARNINGS

class serveur;

struct thread_param {
	serveur* ser;
	SOCKET soc;
};


class serveur {
private:
	int           port;
	SOCKET	      ListeningSocket;
	bool          running;
	SOCKADDR_IN   ServerAddr;
	DWORD         ClientThread(SOCKET);
public:
	serveur(int);
	int                 init();
	int                 start();
	int                 pause();
	static DWORD WINAPI ThreadLauncher(void *p) {
		struct thread_param *Obj = reinterpret_cast<struct thread_param*>(p);
		serveur *s = Obj->ser;
		return s->ClientThread(Obj->soc);
	}
};



#endif