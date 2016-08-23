// 
// Donuts.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "ramActor.h"

class Donuts : public ramBaseScene
{
	
	ofxUIToggle *mToggles[ramActor::NUM_JOINTS];
	bool mNodeVisibility[ramActor::NUM_JOINTS];

	float mNumDuplicate;
	float mRadius;
	float mScale;
	float mBoxSize;
	ofVec3f mTranslate;
	bool mShowActor;
	bool mResetAll;
	bool mToggleAll;
	
	int mNumDuplicates;
	float mRadian;
	
public:
	
	
	void setupControlPanel()
	{
		
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
		
		ramGetGUI().addSection("Original me");
		ramGetGUI().addSlider("Scale", 0.1, 10.0, &mScale);
		ramGetGUI().addSlider("Position: X", -300.0, 300.0, &mTranslate.x);
		ramGetGUI().addSlider("Position: Z", -300.0, 300.0, &mTranslate.z);
		
		ramGetGUI().addSection("Duplicated me");
		ramGetGUI().addSlider("Duplicate", 1, 200, &mNumDuplicate);
		ramGetGUI().addSlider("Radius", 10.0, 1000.0, &mRadius);
		ramGetGUI().addSlider("Box Size", 1.0, 1000.0, &mBoxSize);
		
		ramGetGUI().addSection("Reset, Toggle visibility");
		panel->addButton("Reset all settings", false, 30, 30);
		ramGetGUI().addToggle("Show Actor", &mShowActor);
		ramGetGUI().addToggle("Toggle All", &mToggleAll);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = (i == ramActor::JOINT_RIGHT_HAND || i == ramActor::JOINT_LEFT_HAND);
			mToggles[i] = panel->addToggle(ramActor::getJointName(i), &mNodeVisibility[i], 8, 8);
		}
		
		clear();
		ofAddListener(panel->newGUIEvent, this, &Donuts::onValueChanged);
		
#endif
	}

	
	void setup()
	{
		
	}
	
	void update()
	{
		mNumDuplicate = floor(mNumDuplicate);
		mRadian = 2 * M_PI / mNumDuplicate;
	}
	
	void drawDonuts(const ramNodeArray &nodeArray)
	{
		ofPushStyle();
		ofNoFill();
		
		if (mShowActor)
		{
			for (int n=0; n<mNumDuplicate; n++)
			{
				ofColor c1 = ramColor::RED_DEEP;
				ofColor c2 = ramColor::BLUE_LIGHT;
				c1.g += n*3;
				c2.b += n*3;
				
				float x = cos( mRadian * n ) * mRadius;
				float z = sin( mRadian * n ) * mRadius;
				
				ofPushMatrix();
				ofTranslate(x, 0, z);
				if( n==0 ) ofTranslate(mTranslate.x, mTranslate.y, mTranslate.z);
				
				ofRotateY(360/mNumDuplicate * n);
				if( n==0 ) ofScale(mScale, mScale, mScale);
				

				ofPushStyle();
				for(int i=0; i<nodeArray.getNumNode(); i++)
				{
					const ramNode& node = nodeArray.getNode(i);
					
					node.beginTransform();
					ofSetColor(c1);
					ofDrawBox( i==ramActor::JOINT_HEAD ? 8 : 5);
					node.endTransform();
					
					if (node.hasParent())
					{
						ofSetColor(c2);
						ofDrawLine(node, *node.getParent());
					}
				}
				ofPopStyle();
				
				ofPopMatrix();
			}
		}
		
		else
		{
			for (int index=0; index<nodeArray.getNumNode(); index++)
			{
				if (mNodeVisibility[index] == false) continue;
				
				for (int n=0; n<mNumDuplicate; n++)
				{
					const ramNode &node = nodeArray.getNode(index);
					
					ofPushMatrix();
					{
						ofColor c1 = ramColor::RED_DEEP;
						ofColor c2 = ramColor::BLUE_LIGHT;
						c1.g += n*3;
						c2.b += n*3;
						
						float x = cos( mRadian * n ) * mRadius;
						float z = sin( mRadian * n ) * mRadius;
						
						ofMatrix4x4 m = node.getOrientationQuat();
						glMultMatrixf(m.getPtr());
						
						ofTranslate(x, 0, z);
						if( n==0 ) ofTranslate(mTranslate.x, mTranslate.y, mTranslate.z);
						
						ofRotateY(360/mNumDuplicate * n);
						if( n==0 ) ofScale(mScale, mScale, mScale);
						
						node.beginTransform();
						ofSetColor(c2);
						ofDrawBox(mBoxSize);
						node.endTransform();
					}
					ofPopMatrix();
				}
			}
		}
		
		
		ofPopStyle();

	}
	
	void draw()
	{

	}
	
	void drawActor(const ramActor& actor)
	{
		drawDonuts( actor );
	}
	
	void drawRigid(const ramRigidBody &rigid)
	{
		drawDonuts( rigid );
	}
	
	void onValueChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
		
		if (name == "Toggle All")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			setAllVisiblity(t->getValue());
		}
		
		if (name == "Reset all settings")
		{
			clear();
		}
	}
	
	void clear()
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = (i == ramActor::JOINT_RIGHT_HAND || i == ramActor::JOINT_LEFT_HAND);
			mToggles[i]->setValue(mNodeVisibility[i]);
		}
		
		mNumDuplicate = 20;
		mRadius = 110;
		mScale = 1.0;
		mBoxSize = 3.0;
		mTranslate = ofVec3f::zero();
		mShowActor = false;
	}
	
	string getName() const { return "Donuts"; }
	
	
	void setAllVisiblity(bool b)
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			mToggles[i]->setValue(b);
	}
};

