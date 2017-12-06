//
//  Character.hpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 27/10/2017.
//


#pragma once

#include "ramMain.h"
#include "ofMain.h"
#include "BaseSceneWithJsonSettings.h"

class Character : public BaseSceneWithJsonSettings
{
public:
    Character();
    void setup();
    void reset();
    void setupControlPanel();
    void onPanelChanged(ofxUIEventArgs& e);
    void drawImGui();
    void update();
    void draw();
    string getName() const { return "Character"; }

private:
    ofTrueTypeFont font;
    map<int,vector<string>> characterSet;
    vector<string> subjects,verbs,objects;
    bool handsOnly;
    float maxScale,minScale;
    bool isChacterOn[4] = {false,false,false,false};

	void loadJson(const ofJson &json);
	ofJson toJson() const;
};
