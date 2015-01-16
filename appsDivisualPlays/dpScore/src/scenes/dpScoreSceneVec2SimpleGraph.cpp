//
//  dpScoreSceneVec2SimpleGraph.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneVec2SimpleGraph.h"

DP_SCORE_NAMESPACE_BEGIN

void SceneVec2SimpleGraph::initialize()
{
    dpDebugFunc();
    
    mBuffer.clear();
    mBuffer.assign(kW/mStep, ofVec2f::zero());
 
    mUICanvas = new ofxUICanvas();
    mUICanvas->setName(getName());
    mUICanvas->addLabel(getName());
    mUICanvas->addSpacer();
    mUICanvas->addIntSlider("Step", 1, 10, &mStep);
    mUICanvas->addSlider("Sensor Scale", 0.f, 2.f, &mSensorScale);
}

void SceneVec2SimpleGraph::shutDown()
{
    dpDebugFunc();
    
    if (mUICanvas) {
        delete mUICanvas;
        mUICanvas = nullptr;
    }
}

void SceneVec2SimpleGraph::enter()
{
    dpDebugFunc();
}

void SceneVec2SimpleGraph::exit()
{
    dpDebugFunc();
}

void SceneVec2SimpleGraph::update(ofxEventMessage& m)
{
    if (m.getAddress() == kOscAddrCaneraUnitVector) {
        if (m.getNumArgs() >= 2) {
            mVec.x = m.getArgAsFloat(0);
            mVec.y = m.getArgAsFloat(1);
            mBuffer.push_back(mVec);
            while (mBuffer.size()>kW/mStep-mStep*2) {
                mBuffer.pop_front();
            }
        }
    }
}

void SceneVec2SimpleGraph::draw()
{
    ofPushMatrix();
    
    ofPushMatrix();
    alignedTranslate(0.f, 20.f);
    alignedLine(10.f, 0.f, kW-10.f, 0.f);
    const float step{kW / 210.f};
    for (int i=0; i<=210; i++) {
        int height{5};
        if (i%10==0) height = 8;
        alignedLine(10.f+i*step, 0.f, 10.f+i*step, height);
    }
    ofPopMatrix();
    
    ofPushMatrix();
    alignedTranslate(0.f, kH-10.f);
    alignedLine(10.f, 0.f, kW-10.f, 0.f);
    for (int i=0; i<=210; i++) {
        int height{5};
        if (i%10==0) height = 8;
        alignedLine(10.f+i*step, 0.f, 10.f+i*step, -height);
    }
    ofPopMatrix();
    
    const float stepY = (kH - (10.f + 20.f + 40.f)) / 100.f;
    for (int i=0; i<=100; i++) {
        int width{5};
        if (i%10 == 0) width = 8;
        alignedLine(0.f, 40.f+i*stepY, width, 40.f+i*stepY);
    }
    
    ofSetColor(ofColor::white, 150);
    alignedTranslate(20.f, kH*0.5f+5.f);
    alignedLine(0.f, 0.f, kW, 0.f);
    
    const float halfH{(kH-50.f) * 0.5f};
    const float mult{halfH * mSensorScale};
    if (mBuffer.size()>=2) {
        ofSetColor(ofColor::white, 255);
        for (int i=0; i<mBuffer.size()-1; i++) {
            ofVec2f v0 = mBuffer.at(i);
            ofVec2f v1 = mBuffer.at(i+1);
            v0 *= mult;
            v1 *= mult;
            v0.x = clamp(v0.x, halfH);
            v0.y = clamp(v0.y, halfH);
            v1.x = clamp(v1.x, halfH);
            v1.y = clamp(v1.y, halfH);
            
            alignedLine(i*mStep, v0.x, (i+1)*mStep, v1.x);
            alignedLine(i*mStep, v0.y, (i+1)*mStep, v1.y);
        }
    }
    ofPopMatrix();
    
    ofPushMatrix();
    alignedTranslate(0.f, kH*0.5f+5.f);
    ofSetColor(color::kMain, 255);
    ofSetLineWidth(2.f);
    const ofVec2f v = mVec * mult;
    alignedLine(ofGetWidth()-14.f, v.x, ofGetWidth(), v.x);
    alignedLine(ofGetWidth()-14.f, v.y, ofGetWidth(), v.y);
    ofPopMatrix();
    
    stringstream ss;
    ss << "vec: " << mVec;
    ofSetColor(ofColor::white);
    ofDrawBitmapString(ss.str(), ofPoint(12.f, kH-20.f));
}

DP_SCORE_NAMESPACE_END