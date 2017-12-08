#pragma once
#include "ramMain.h"

/*-----------------------------------------------------------/
randomCubeUni.h
 
 github.com/YCAMInterlab
 github.com/azuremous
 
 add ofxBullet to {ofRoot}/addons folder
 add '../../../addons/ofxBullet/libs' to Header search path
 
 Created by Jung un Kim a.k.a azuremous on 10/27/17.
 /----------------------------------------------------------*/

#include "ofxBullet.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BaseSceneWithJsonSettings.h"
#include "ofxJsonUtilsUtils.h"

class randomCubeUni : public BaseSceneWithJsonSettings
{
private:
    ofVec3f visibleBox;
    
	ofTrueTypeFont infoFont;
    int touchId;
    ofColor touchIDColor;
	
	bool showDebug;
	ofRectangle rect;
	ofVec2f y_range;
	float box_size;

public:
    std::string getName() const { return "Rolling"; }
    
    void setup();
    void update();
    void draw();
	void drawHUD();
    void drawImGui();
    void onEnabled();
    
	JSON_FUNCS(rect,y_range,box_size,showDebug);
	
	void next();
};
