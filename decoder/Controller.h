/*
 * ActionReceiverMediator.h
 *
 *  Created on: 17.08.2016
 *      Author: sven
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#ifdef ESP8266
	#include <ESP8266WebServer.h>
#else
	#include <WebServer.h>
#endif

#include <DNSServer.h>
#include <LinkedList.h>
#include <map>

#include "CmdReceiverBase.h"
#include "CmdSenderBase.h"
#include "ILoop.h"
#include "INotify.h"
#include "ISettings.h"
#include "WebserviceCommandLogger.h"
#include "WebserviceDCCSniffer.h"
#include "InternalStatusAsString.h"
#include "InternalStatusAsJson.h"
#include "Connectors.h"
#include "LocData.h"

class CmdReceiverBase;
// forward declaration

class Controller {
public:
	typedef std::map<int, LocData*> Items;

	Controller();
	virtual ~Controller();
	void registerCmdSender(CmdSenderBase* base);
	void registerCmdReceiver(CmdReceiverBase* base);
	void registerNotify(INotify* base);
	void registerConnectors(Connectors* base);
	void registerLoop(ILoop* loop);
	void registerSettings(ISettings* loop);
	void registerStatus(IStatus* loop);

	LinkedList<ISettings*>* getSettings();
	void updateRequestList();
	void doLoops();

	// Notifications from Outside (via DCC, WLAN, ..)
	void notifyTurnout(int id, int direction, int source);
	LocData* notifyDCCSpeed(int id, int speed, int direction, int source);
	void notifyDCCFun(int id, int startbit, int stopbit, unsigned long value, int source);
	LocData* notifyDCCFun(int id, int bit, unsigned int value, int source);
	void notifyGPIOChange(int pin, int newvalue);

	void notifySpeeSteps(int id, int SpeedSteps);
	void enableAPModus();
	void getHTMLController();
	void getHTMLCfg();
	void sendContent(String s);
	void setRequest(String id, String key, String value);
	bool isEmergency();
	String getHostname();

	LocData* getLocData(int id);
	Items* getLocData();
	std::vector<int>* getLocDataKeys();
	
	TurnOutData* getTurnOutData(int id);

	String createDebugDiagramm();
	void emergencyStop(int source, bool enable);

	LinkedList<CmdSenderBase*>* getSender()  {
		return &sender;
	}

	WebserviceCommandLogger* cmdlogger;
	WebserviceDCCSniffer* dccSniffer;
	unsigned long longestLoop;
	
	String getInternalStatus(String modul, String key);
	String getInternalStatusAsJon(String modul, String key);
	void printInternalStatusAsJon();
	void internalStatusObjStatus(IInternalStatusCallback* cb, String modul, String key);

	LinkedList<INotify::requestInfo*>* getRrequestList();

	void sendSetTurnout(String id, String status);
    void sendSetSensor(uint16_t id, uint8_t status);
	void sendDCCSpeed(int id, int speed, int direction, int source);
	void sendDCCFun(int id, int bit, unsigned int value, int source);
	void sendPipeFilter(String dest, String key, String value);
	void collectAllInternalStatus(IInternalStatusCallback* cb, String modul, String key);

private:
	Items items;
	std::vector<int> itemskeys;
	typedef std::map<int, TurnOutData*> LocTurnOuts;
	LocTurnOuts turnoutinfo;

	bool EMERGENCYActive;
	LinkedList<CmdReceiverBase*> receiver = LinkedList<CmdReceiverBase*>();
	LinkedList<CmdSenderBase*> sender = LinkedList<CmdSenderBase*>();
	LinkedList<INotify*> actions = LinkedList<INotify*>();
	LinkedList<ILoop*> loops = LinkedList<ILoop*>();
	LinkedList<ISettings*> settings = LinkedList<ISettings*>();
	LinkedList<IStatus*> status = LinkedList<IStatus*>();
	LinkedList<unsigned long> nextRun = LinkedList<unsigned long>();
	LinkedList<INotify::requestInfo*> requestList = LinkedList<INotify::requestInfo*>();
	LinkedList<Connectors*> connectors = LinkedList<Connectors*>();
	long int lastTurnoutCmd[3];
	std::unique_ptr<DNSServer> dnsServer;
	void logLoop(unsigned long d);
	InternalStatusAsString statusAsString;
	InternalStatusAsJson statusAsJson;
	unsigned int l[100];
	unsigned long next;
};

#endif /* CONTROLLER_H_ */
