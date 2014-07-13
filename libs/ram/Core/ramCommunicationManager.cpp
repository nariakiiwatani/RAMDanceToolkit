//
//  ramCommunicationManager.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/06/18.
//  Copyright (c) 2014 YCAMInterlab. All rights reserved.
//

#include "ramCommunicationManager.h"

ramCommunicationManager* ramCommunicationManager::__instance = NULL;

void ramCommunicationManager::setup(){

	mainPanel.setup();
	mainPanel.setName("Communicator");
	refleshInstruments();

	ofAddListener(newGUIEvent, this, &ramCommunicationManager::guiEvent);
	ofAddListener(ofEvents().keyPressed, this, &ramCommunicationManager::keyPressed);

	bVisible = true;
}

void ramCommunicationManager::update(){

	while (oscReceiver.hasWaitingMessages()){
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
		ramCommunicationManager::instance().updateWithOscMessage(m);
	}

}

void ramCommunicationManager::draw(){
	if (bVisible) mainPanel.draw();
}

void ramCommunicationManager::updateWithOscMessage(const ofxOscMessage &m){
	const std::string addr = m.getAddress();
	const std::string name = m.getArgAsString(0);

	if (addr.substr(0,17) != "/ram/communicate/"){
		return;
	}

	int index = 0;

	bool isExist = false;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
		{
			index   = i;
			isExist = true;
		}
	}

	if (!isExist) index = addInstrument(name);


	if (addr == RAM_OSC_ADDR_COMMUNICATE_NOTEON)
		Instruments[index]->getFloat("velocity") = m.getArgAsFloat(1);

	else if (addr == RAM_OSC_ADDR_COMMUNICATE_NOTEOFF)
		Instruments[index]->getFloat("velocity") = 0.0;

	else if (addr == RAM_OSC_ADDR_COMMUNICATE_CC){
		int ccNum = m.getNumArgs();

		for (int i = 0;i < ccNum;i++){
			string ccLabel = "cc" + ofToString(i);

			if (Instruments[index]->contains(ccLabel))
			{
				Instruments[index]->getFloat(ccLabel) = m.getArgAsFloat(i+1);
			}
			else
			{
				Instruments[index]->add((*new ofParameter<float>).set(ccLabel,
																	  m.getArgAsFloat(i+1),0.0,1.0));
				refleshInstruments();
			}

		}

	}

}

int ramCommunicationManager::addInstrument(string name){

	ofParameterGroup* newInst = new ofParameterGroup();

	newInst->setName(name);
	newInst->add((*new ofParameter<float>).set("velocity",0.0,0.0,1.0));

	Instruments.push_back(newInst);

	refleshInstruments();

	return Instruments.size() - 1;
}

void ramCommunicationManager::refleshInstruments(){
	mainPanel.clear();
	mainPanel.setName("Communicator");

	for (int i = 0;i < Instruments.size();i++){
		mainPanel.add(*Instruments[i]);
	}

	mainPanel.setPosition(ofGetWidth()-mainPanel.getWidth(),0);
	
}

float ramCommunicationManager::getVelocity(string name){
	int index = 0;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}

	return getVelocity(index);
}

float ramCommunicationManager::getVelocity(int index){
	if ((Instruments.size() == 0) || (index >= Instruments.size()))
		return 0.0;

	return Instruments[index]->getFloat("velocity");
}

float ramCommunicationManager::getCC(string name, int ccNum){
	int index = 0;
	for (int i = 0;i < Instruments.size();i++){
		if (name == Instruments[i]->getName())
			index = i;
	}

	return getCC(index,ccNum);
}

float ramCommunicationManager::getCC(int index, int ccNum){
	if ((Instruments.size() == 0) || (index >= Instruments.size()))
		return 0.0;

	if (Instruments[index]->contains("cc"+ofToString(ccNum)))
		return Instruments[index]->getFloat("cc"+ofToString(ccNum));

	return 0.0;
}

void ramCommunicationManager::guiEvent(ofxUIEventArgs &e){
	cout << "GUIEVVV" << endl;
	cout << e.widget->getName() << endl;
}

void ramCommunicationManager::keyPressed(ofKeyEventArgs &key){
	if (key.key == OF_KEY_TAB) bVisible ^= true;
}