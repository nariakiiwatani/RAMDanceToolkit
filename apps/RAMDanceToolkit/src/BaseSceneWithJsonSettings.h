//
//  BaseSceneWithJsonSettings.h
//  RAMDanceToolkit
//
//  Created by Iwatani Nariaki on 2017/11/28.
//
//

#pragma once

#include "ramBaseScene.h"
#include "ofxJsonUtilsUtilsContainer.h"

class BaseSceneWithJsonSettings : public rdtk::BaseScene
{
public:
	BaseSceneWithJsonSettings() {
		settings_.setWillSaveListener([this](const ofJson &src, ofJson &dst) {
			dst = this->toJson();
		});
	}
	void onEnabled() {
		settings_.load("settings/scenes/"+getName()+".json");
		loadJson(settings_);
	}
	void onDisabled() {
		settings_.overwrite();
	}
protected:
	ofxJsonContainer<ofJson> settings_;
	virtual void loadJson(const ofJson &json){}
	virtual ofJson toJson() const{return ofJson();}
};
