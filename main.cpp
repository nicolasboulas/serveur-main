#include <iostream>
#include "Serveur.h"

using namespace std;

int main()
{
	serveur *MyServer = new serveur(12345);
	if (MyServer->init() != 0) {
		cerr << "ne peut initialiser le serveur" << endl;
		return 1;
	}

	if (MyServer->start() != 0) {
		cerr << "ne peut démarrer le serveur" << endl;
		return 1;
	}

	return 0;
}






