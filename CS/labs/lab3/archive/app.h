#ifndef APP_H
#define APP_H

#include "spx.h"
#include "ipx.h"


void start_app(enum appType app, enum connType conn) {

	if (conn == IPX) {

		if (app == Server)
			_ipx_server();
		else
			_ipx_client();

	}
	else {

		if (app == Server)
			_spx_server();
		else
			_spx_client();

	}
	system("pause");

}

#endif // !APP_H