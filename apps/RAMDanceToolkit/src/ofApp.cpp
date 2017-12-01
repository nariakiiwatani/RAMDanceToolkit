// 
// ofApp.cpp - RAMDanceToolkit
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

#include "ofApp.h"
#include "MirrorCamera.h"
#include "AutoSwitcher.h"

#pragma mark - RAMB
#include "Spiderman.h"
#include "VisualStudio.h"
#include "Paperman.h"

#pragma mark - UNI
#include "UniScene.h"
#include "randomCubeUni.h"

#pragma mark - MOZZI
#include "OrbitSystem.h"
#include "PlanetarySystem.h"

#pragma mark - KABOCHA
#include "Character.h"
#include "Lines.hpp"
#include "MakeSound.hpp"
#include "Moji.h"

#pragma mark - Regular
#include "MovingCam.h"
#include "LineDrawing.h"
#include "BigBox.h"
#include "Future.h"
#include "Donuts.h"
#include "Stamp.h"
#include "Expansion.h"
#include "Abacus.h"
#include "Particles.h"
#include "SoundCube.h"
#include "UpsideDown.h"
#include "HastyChase.h"
#include "ColorGrid.h"
#include "ThreePoints.h"
#include "FourPoints.h"
#include "Monster.h"
#include "Laban.h"
#include "Notation.h"
#if !defined (DEBUG) && !defined (_DEBUG) // exclude from debug build
#include "Kepler.h"
#include "Chain.h"
#endif

#pragma mark - oF methods
//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	/// ram setup
	// ------------------
	rdtk::Initialize(10000, true);
	
	/// scenes setup
	// ------------------
	rdtk::SceneManager& sceneManager = rdtk::SceneManager::instance();

	auto switcher = sceneManager.addScene<AutoSwitcher>();
	sceneManager.addScene<MirrorCamera>();
	
	switcher->addScene(sceneManager.addScene<Spiderman>());
	switcher->addScene(sceneManager.addScene<VisualStudio>());
	switcher->addScene(sceneManager.addScene<Paperman>());
	
	switcher->addScene(sceneManager.addScene<UniScene>());
	switcher->addScene(sceneManager.addScene<randomCubeUni>());
	
	switcher->addScene(sceneManager.addScene<OrbitSystem>());
	switcher->addScene(sceneManager.addScene<PlanetarySystem>());	

	switcher->addScene(sceneManager.addScene<Character>());	
	switcher->addScene(sceneManager.addScene<Lines>());
	switcher->addScene(sceneManager.addScene<MakeSound>());	
	switcher->addScene(sceneManager.addScene<Moji>());

	sceneManager.addScene<MovingCam>();
	sceneManager.addScene<LineDrawing>();
	sceneManager.addScene<BigBox>();
	sceneManager.addScene<Future>();
	sceneManager.addScene<Donuts>();
	sceneManager.addScene<Stamp>();
	sceneManager.addScene<Expansion>();
	// ignore win32
#ifndef TARGET_WIN32
	sceneManager.addScene<Particles>();
#endif
	sceneManager.addScene<Abacus>();
	sceneManager.addScene<SoundCube>();
	sceneManager.addScene<UpsideDown>();
	sceneManager.addScene<HastyChase>();
	sceneManager.addScene<ColorGrid>();
	sceneManager.addScene<ThreePoints>();
	sceneManager.addScene<FourPoints>();
	sceneManager.addScene<Monster>();
	sceneManager.addScene<Laban>();
	sceneManager.addScene<Notation>();
#if !defined (DEBUG) && !defined (_DEBUG) // exclude from debug build
	sceneManager.addScene<Kepler>();
	sceneManager.addScene<Chain>();
#endif
	switcher->enable();
}

//--------------------------------------------------------------
void ofApp::update()
{
	
}

//--------------------------------------------------------------
void ofApp::draw()
{
	
}



#pragma mark - ram methods
//--------------------------------------------------------------
void ofApp::drawActor(const rdtk::Actor &actor)
{
	
}

//--------------------------------------------------------------
void ofApp::drawRigid(const rdtk::RigidBody &rigid)
{
	
}

#pragma mark - ram Events

//--------------------------------------------------------------
void ofApp::onActorSetup(const rdtk::Actor &actor)
{
	
}

//--------------------------------------------------------------
void ofApp::onActorExit(const rdtk::Actor &actor)
{

}

//--------------------------------------------------------------
void ofApp::onRigidSetup(const rdtk::RigidBody &rigid)
{

}

//--------------------------------------------------------------
void ofApp::onRigidExit(const rdtk::RigidBody &rigid)
{

}



#pragma mark - of Event
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
	
}
