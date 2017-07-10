/*
 * Webserver.h
 *
 *  Created on: 20.08.2016
 *      Author: sven
 */

#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <LinkedList.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FS.h>
#include "interfaceLoop.h"
#include "WebserviceBase.h"
#include <ESP8266mDNS.h>


class Controller;

class Webserver: public interfaceLoop {
public:
	Webserver(Controller* c);
	virtual int loop();
	virtual ~Webserver();
	static ESP8266WebServer* server;
	void addServices(WebserviceBase* base);


private:
	bool loadFromSPIFFS(String filepath);
	void handleNotFound();
	void handleRoot();
	void handleDel();
	void handleController();
	void handleCfg();
	void handleSet();
	void handleFilelist();
	void handleUpload();
	int lastWifiStatus = 6;
	File fsUploadFile;
	Controller* controll;
	LinkedList<WebserviceBase*> services = LinkedList<WebserviceBase*>();
	static ESP8266HTTPUpdateServer* httpUpdater;
};


#endif /* WEBSERVER_H_ */
