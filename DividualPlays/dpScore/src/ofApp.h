#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"
#include "dpScoreCommon.h"
#include "dpScoreSceneManager.h"
#include "dpScoreEvent.h"
#include "ofxMotioner.h"
#include "ofxEvent.h"

class ofApp final : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;
    void shutdown();
    
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void windowResized(int w, int h) override;
    
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    void guiEvent(ofxUIEventArgs &e);
    void onObjectReceived(dp::score::ObjectEventArgs& e);
    void onEventReceived(ofxEventMessage& e);
    
private:
    void generateFakeMeanData();
    void generateFakeVectorData();
    string makeInternalCameraUnitAddress(const string& addr);
    void changeSceneWithOsc(const ofxOscMessage& m);
    void updateCameraUnitOsc(const ofxOscMessage& m);
    
    void drawTitle(float t);
    void drawFps();
    void drawInvert();
    
    dp::score::SceneManager mSceneManager;
    ofxOscReceiver mOscReceiver;
    ofxOscMessage mCameraUnitMessageVector;
    ofxOscMessage mCameraUnitMessageVectorTotal;
    ofxOscMessage mCameraUnitMessageMean;
    ofxOscMessage mCameraUnitMessagePixelateR;
    ofxOscMessage mCameraUnitMessagePixelateG;
    ofxOscMessage mCameraUnitMessagePixelateB;
    bool mInvert{false};
    bool mDebugCamUnit{false};
    bool mFullscreen{false};
#ifdef DP_MASTER_HAKONIWA
    bool mShowFps{true};
#else
    bool mShowFps{false};
#endif
    bool mShowCursor{true};
    float mTimeSceneChanged{0.f};
#ifdef DEBUG
    const float mTitleDuration{1.f};
#else
    const float mTitleDuration{10.f};
#endif
    ofTrueTypeFont mFont;
    vector<string> mTitleNames;
    const float kTitleFontSize{150.f};
    
    vector<pair<string, string>> mTitleReplaceList;
    float mSensorScale{1.f};
    const float mSensorScaleMax{50.f};
};
