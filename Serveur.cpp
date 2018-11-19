#include <winsock2.h>
#include <iostream>

#include "Serveur.h"

#define _WINSOCK_DEPRECATED

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define _MAX_HOST_LENGTH_ 100

#define _WINSOCK_DEPRECATED_NO_WARNINGS


using namespace std;


serveur::serveur(int p) {
	port = p;
	running = false;
}

int serveur::init() {
	struct in_addr  MyAddress;
	struct hostent  *host;
	char HostName[_MAX_HOST_LENGTH_];
	WSADATA	      wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "WSAStartup a �chou� " << endl;
		return 1;
	}

	if (gethostname(HostName, _MAX_HOST_LENGTH_) == SOCKET_ERROR) {
		cerr << "gethostname() a rencontr� l'erreur " << WSAGetLastError() << endl;
		return 1;
	}

	if ((host = gethostbyname(HostName)) == NULL) {
		cerr << "gethostbyname() a rencontr� l'erreur " << WSAGetLastError() << endl;
		return 1;
	}

	memcpy(&MyAddress.s_addr, host->h_addr_list[0], sizeof(MyAddress.s_addr));

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(port);
	ServerAddr.sin_addr.s_addr = inet_addr(inet_ntoa(MyAddress));

	cout << "server correctement initialis�" << endl;
	return 0;
}

int serveur::start() {
	SOCKADDR_IN                 ClientAddr;
	int                         ClientAddrLen;
	HANDLE                      hProcessThread;
	SOCKET                      NewConnection;
	struct thread_param         p;

	if ((ListeningSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		cerr << "ne peut cr�er la socket. Erreur n� " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	if (bind(ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR) {
		cerr << "bind a �chou� avec l'erreur " << WSAGetLastError() << endl;
		cerr << "Le port est peut-�tre d�j� utilis� par un autre processus " << endl;
		closesocket(ListeningSocket);
		WSACleanup();
		return 1;
	}

	if (listen(ListeningSocket, 5) == SOCKET_ERROR) {
		cerr << "listen a �chou� avec l'erreur " << WSAGetLastError() << endl;
		closesocket(ListeningSocket);
		WSACleanup();
		return 1;
	}

	cout << "serveur d�marr� : � l'�coute du port " << port << endl;
	running = true;
	ClientAddrLen = sizeof(ClientAddr);

	while (running) {

		if ((NewConnection = accept(ListeningSocket, (SOCKADDR *)&ClientAddr, &ClientAddrLen)) == INVALID_SOCKET) {
			cerr << "accept a �chou� avec l'erreur " << WSAGetLastError() << endl;;
			closesocket(ListeningSocket);
			WSACleanup();
			return 1;
		}

		p.ser = this;
		p.soc = NewConnection;

		cout << "client connect� ::  IP : " << inet_ntoa(ClientAddr.sin_addr) << " ,port = " << ntohs(ClientAddr.sin_port) << endl;

		hProcessThread = CreateThread(NULL, 0, &serveur::ThreadLauncher, &p, 0, NULL);
		if (hProcessThread == NULL) {
			cerr << "CreateThread a �chou� avec l'erreur " << GetLastError() << endl;
		}
	}

	return 0;
}

int serveur::pause() {
	running = false;
	cout << "Serveur en pause" << endl;
	closesocket(ListeningSocket);
	return 0;
}

/* ======================================================================== */
/* ========================== thread proc ================================= */
/* ======================================================================== */


DWORD serveur::ClientThread(SOCKET soc) {
	cout << "thread client d�marr�" << endl;

	/*    A mettre ici : code relatif au protocole utilis�    */

	return 0;
}