//
//  ramCommunicationManager.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/06/18.
//  Copyright (c) 2014 YCAMInterlab. All rights reserved.
//

#ifndef __RAMDanceToolkit__ramCommunicationManager__
#define __RAMDanceToolkit__ramCommunicationManager__

#include "ofMain.h"
#include "ofxOsc.h"
#include "ramConstants.h"
#include "ramUtils.h"
#include "ofxGui.h"

class ramCommunicationManager
{
public:

	void setup();
	void update();
	void draw();

	float getVelocity(string name);
	float getVelocity(int index);
	float getCC(string name,int ccNum);
	float getCC(int index,int ccNum);

	inline static ramCommunicationManager& instance()
	{
		if (__instance == NULL)
			__instance = new ramCommunicationManager;
		return* __instance;
	};

	void setupOscReceiver(int port) { oscReceiver.setup(port); }

private:
	int addInstrument(string name);
	static ramCommunicationManager *__instance;

	ramCommunicationManager() {};
	ramCommunicationManager(const ramCommunicationManager&){}
	ramCommunicationManager& operator=(const ramCommunicationManager&) {return *this; }
	~ramCommunicationManager() {};


	//Gui
	void refleshInstruments();
	ofxPanel mainPanel;
	vector<ofParameterGroup* > Instruments;


	//OSC
	ofxOscReceiver oscReceiver;
	void	updateWithOscMessage(const ofxOscMessage &m);


};

#endif /* defined(__RAMDanceToolkit__ramCommunicationManager__) */
