#pragma once
#include "ramMain.h"


/*
 add ofxBullet to {ofRoot}/addons folder
 add '../../../addons/ofxBullet/libs' to Header search path
 */
#include "ofxBullet.h"
#include "btPicker.h"
#include "BaseSceneWithJsonSettings.h"
#include "ofxJsonUtilsUtils.h"

class UniScene : public BaseSceneWithJsonSettings
{
public:
    std::string getName() const { return "Floating"; }
    
    void setup();
    void update();
    void draw();
    void drawImGui();
    void drawActor(const rdtk::Actor &actor);
    
	void onEnabled();
	JSON_FUNCS(bDrawDebug,sphere_size,gavity,wall);
private:
    
    // Bullet example code from SoftBodyExample
    ofxBulletWorldSoft world;

	struct Wall {
		ofRectangle rect;
		ofVec2f y_range;
		bool dirty=false;
		JSON_FUNCS(rect,y_range);
		bool drawImGui();
		void refresh(ofxBulletWorldSoft &world);
		vector<shared_ptr<ofxBulletBox>> boxes;
	};
	Wall wall;
	ofxBulletBox ground;
    
    vector<ofxBulletSphere*> sphere;
    ofxBulletSphere sphere_mouse[5];
    ofxBulletSphere sphere_mouse2[5];
    
    ofColor colors[4];
    
    btPicker picker[5];
    btPicker picker2[5];
    
    bool bDrawDebug;
    bool bSpacebar;
    bool bAddEllipsoid;
    bool bShapesNeedErase;
    
    ofLight light;
    ofVec3f	 mousePos;
    string actorName;
    
    // for debugging...
    void add();
    void clearSpheres();
    float sphere_size;
    int selected_node;
    float gavity;
};
