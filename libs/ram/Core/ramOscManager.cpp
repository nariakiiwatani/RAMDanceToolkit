//
//  ramOscManager.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/07/14.
//  Copyright (c) 2014年 YCAMInterlab. All rights reserved.
//

#include "ramOscManager.h"

ramOscManager* ramOscManager::__instance = NULL;

void ramOscManager::setup(int receivePort){

	receiver.setup(receivePort);

}

void ramOscManager::addSenderTag(int port, string address){

	senderlist.push_back(ramOscSendTag(port,address));

}

void ramOscManager::addReceiverTag(ramOscReceiveTag* ptr){

	receiverList.push_back(ptr);

}

void ramOscManager::update(){

	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		for (int i = 0;i < receiverList.size();i++){

			if (m.getAddress().substr(0,receiverList[i]->address.length()) ==
				receiverList[i]->address){

				receiverList[i]->addMessage(m);

			}

		}

	}
}