
//  ramImGui.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2017/05/01.
//  Copyright © 2017年 YCAMInterlab. All rights reserved.
//

#include "ramImGuiManager.h"

using namespace rdtk;

GuiManager* GuiManager::__instance = NULL;

GuiManager::GuiManager()
{
	//Load external font
//	ImGuiIO& io = ImGui::GetIO();
//	
//	if (io.Fonts->AddFontFromFileTTF(ofToDataPath("ARIALUNI.TTF").c_str(), 18.0f, NULL, io.Fonts->GetGlyphRangesChinese()))
//	{
//		unsigned char * pixels;
//		int width, height;
//		io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
//	}

	gui.setup();

	prefGui = ofPtr<PreferencesGui>(new PreferencesGui());
	addScene(ofPtr<SceneGui>(prefGui));
	
	switchGuiType(NONE);
}

GuiManager::~GuiManager()
{
}

void GuiManager::addActor(ofPtr<ActorGui> act)
{
	
}

void GuiManager::addScene(ofPtr<SceneGui> scn)
{
	sceneArr.push_back(scn);
}

void GuiManager::switchGuiType(GuiType type)
{
	switch(type) {
		case IMGUI:
			visible = true;
			GetGUI().setVisible(false);
			ofShowCursor();
			break;
		case OFXUI:
			visible = false;
			GetGUI().setVisible(true);
			ofShowCursor();
			break;
		case NONE:
			visible = false;
			GetGUI().setVisible(false);
			ofHideCursor();
			break;
	}
	currentGuiType = type;
}

void GuiManager::nextGuiType()
{
	switchGuiType((GuiType)(((int)currentGuiType + 1) % 3));
}


void GuiManager::keyPressed(ofKeyEventArgs & key)
{
	if (key.key == '\t')
	{
		nextGuiType();
	}
}

void GuiManager::draw()
{
	if (!visible) return;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.5,0.5,0.5,1.0));
	gui.begin();

	//CameraManager::instance().setEnableInteractiveCamera(!ImGui::IsWindowHovered());
	// WantCaptureMouse is better?  https://github.com/jvcleave/ofxImGui/issues/26
	CameraManager::instance().setEnableInteractiveCamera(!ImGui::GetIO().WantCaptureMouse);
	
	// Prevent ActorManager::onMouseReleased in order to ImGui based scene get value from ActorManager::getLastSelectedNodeIdentifer()
	ActorManager::instance().setEnableOnMouseRelease(!ImGui::GetIO().WantCaptureMouse);
	
	for (auto scn : sceneArr)
	{
		ImGui::PushID(scn->getName().c_str());
		scn->drawGuiBase();
		ImGui::PopID();
	}
	
	gui.end();
	ImGui::PopStyleColor();
}
