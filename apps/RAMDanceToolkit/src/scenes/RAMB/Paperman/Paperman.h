//
//  boids.hpp
//  RAMDanceToolkit
//
//  Created by NAOYA IWAMOTO on 2017/10/26.
//
//

#pragma once

#include "ramMain.h"
#include "BaseSceneWithJsonSettings.h"

struct Plane {
    int index;
    ofColor col;
    ofVec3f previousPos;
    ofVec3f currentPos;
    ofVec3f vel;
    ofMesh pathLines;
    deque<ofVec3f> pathVertices;
};

class Paperman : public BaseSceneWithJsonSettings
{
public:
    
    Paperman();
    void setup();
    void update();
    void draw();
    void drawImGui();
    string getName() const { return "Paperman"; }
    
    void audioOut(float * output, int bufferSize, int nChannels);
    
    void modelingPlane(ofMesh& mesh);
    void setScalePlane(const float scale);
    void addPlane();
    void removePlane();
    void resetPos();
    
    bool checkSetPort;

	void onEnable();
	void loadJson(const ofJson &json);
	ofJson createJson();

private:
    bool mIsAddPlane;
    bool mIsRemovePlane;
    bool mIsResetPos;
    
    bool mIsControlVelocity;
    bool mIsControlDirection;
    bool mEnableSound;
    
    int mPlayingMethod;
    int mManualControlMethod;
    
    float mTimestep;
    float mTrackDistance;
    
    ofMesh mMesh;
    vector<Plane> mPlanes;
    rdtk::MotionExtractor mEx;  // for picking the joint

    void updateAuto();
    void updateManual();
    void checkNumPlaneWithActor();
    
    // sound
    vector<float> mWEAudioL, mWEAudioR;
};
