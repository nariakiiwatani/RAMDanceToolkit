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
#include "ramOscManager.h"
#include "ramConstants.h"
#include "ramUtils.h"
#include "ofxGui.h"
#include "ofxUI.h"

class ramCommunicationManager
{
public:

	void setup(ramOscManager* oscMan);
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

private:
	int addInstrument(string name);
	static ramCommunicationManager *__instance;

	ramCommunicationManager() {};
	ramCommunicationManager(const ramCommunicationManager&){}
	ramCommunicationManager& operator=(const ramCommunicationManager&) {return *this; }
	~ramCommunicationManager() {};

	bool bVisible;

	//Gui
	void						refleshInstruments();
	vector<ofParameterGroup*>	Instruments;

	vector<ofxUISlider*>			velocities;
	vector<vector<ofxUISlider*> >	ccs;
	ofxUICanvas						UIcanvas;

	//OSC
	ramOscReceiveTag	oscReceiver;
	void			updateWithOscMessage(const ofxOscMessage &m);

	ofEvent<ofxUIEventArgs> newGUIEvent;
	void guiEvent(ofxUIEventArgs &e);
	void keyPressed(ofKeyEventArgs &key);
};

#endif /* defined(__RAMDanceToolkit__ramCommunicationManager__) */
