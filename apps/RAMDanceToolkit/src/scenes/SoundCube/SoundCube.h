// 
// SoundCube.h - RAMDanceToolkit
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

#include "ramMain.h"
#include "BaseSceneWithJsonSettings.h"
#include "ofxJsonUtilsUtils.h"

class SoundCube : public BaseSceneWithJsonSettings
{
public:
	
	class Shape
	{
	public:
		
		ofColor color;
		shared_ptr<ofSoundPlayer> player;
		bool trigger_mode=false;
		bool visible=true;
		
		rdtk::CollisionEvent event;

		void set(int id, shared_ptr<rdtk::Primitive> obj)
		{
			this->id = id;
			this->obj = obj;
			
			event.setPrimitive(obj.get());
		}
		
		void loadSound(const string &path, bool trigger = false, bool loop = true)
		{
			player = make_shared<ofSoundPlayer>();
			player->load(path);
			player->setLoop(loop ? OF_LOOP_NORMAL : OF_LOOP_NONE);
			trigger_mode = trigger;
			
			volume = volume_t = 0;
			
			player->play();
			
			event.setTrigger(rdtk::RAM_TRIGGER_BOTH);
		}
		
		void draw(float fade = 0.1)
		{
			if (event.update())
			{
				if (event.getState())
				{
					if (trigger_mode) volume_t = 1;
					else volume_t = 0;
				}
				else
				{
					if (!trigger_mode) volume_t = 1;
					else volume_t = 0;
				}
			}
			
			ofPushStyle();
			
			if (event.getState())
			{
				alpha += (0 - alpha) * 0.1;
			}
			else
			{
				alpha += (1 - alpha) * 0.1;
			}
			
			volume += (volume_t - volume) * fade;
			player->setVolume(volume);
			
			if(visible)
			{
				ofSetColor(color, 127 + 127 * (1 - alpha));
				
				obj->transformGL();
				ofDrawBitmapString(ofToString(id), 0, 0);
				obj->restoreTransformGL();
				
				obj->draw();
			}
			
			ofPopStyle();
		}

	private:
		
		int id=-1;
		shared_ptr<rdtk::Primitive> obj;
		
		float alpha=0;
		float volume=0, volume_t=0;
	};
	
	string getName() const { return "SoundCube"; }
	
	bool fill;
	float line_width;
	
	float fade;
	bool show_box;
	
	void setupControlPanel()
	{
		fade = 0.5;
		
		rdtk::GetGUI().addSlider("line width", 0, 10, &line_width);
		rdtk::GetGUI().addSlider("fade", 0, 1, &fade);
		rdtk::GetGUI().addToggle("show box", &show_box);
	}
	
	void drawImGui()
	{
		ImGui::DragFloat("line width", &line_width, 0.1, 0, 10);
		ImGui::DragFloat("fade", &fade, 0.01, 0, 1);
		if(ImGui::Checkbox("show box", &show_box)) {
			for(auto &s : shapes) {
				if(s.shape) s.shape->visible = show_box;
			}
		}
		for(auto &s : shapes) {
			ImGui::PushID(&s);
			if(ImGui::TreeNode("shape")) {
				if(s.drawImGui()) {
					s.apply();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}

	JSON_FUNCS(line_width,fade,show_box,shapes);

	void setup()
	{
	}
	
	void update()
	{
	}
	
	void onEnabled()
	{
		BaseSceneWithJsonSettings::onEnabled();
		
		int i = 0;
		for(auto &s : shapes) {
			s.refresh();
		}
	}
	
	void onDisabled()
	{
		BaseSceneWithJsonSettings::onDisabled();
		clear();
	}
	
	void draw()
	{
		rdtk::BeginCamera();
		
		ofNoFill();
		ofPushStyle();
		ofSetLineWidth(line_width);
		
		if (show_box) ofDrawAxis(100);
		for(auto &s : shapes) {
			if(s.shape) s.shape->draw(fade);
		}

		ofPopStyle();
		rdtk::EndCamera();
	}
		
	void clear()
	{
		shapes.clear();
	}
	
protected:
	struct ShapeSettings {
		ofVec3f pos;
		ofVec3f rot;
		ofVec3f size;
		ofFloatColor color;
		string type;
		string sound;
		bool trigger, loop;
		bool need_refresh;
		int index;
		JSON_FUNCS(index,pos,rot,size,color,type,sound,trigger,loop);
		shared_ptr<rdtk::Primitive> primitive;
		shared_ptr<Shape> shape;
		bool drawImGui() {
			bool ret = false;
			ret |= ImGui::DragFloat3("pos", &pos[0]);
			ret |= ImGui::DragFloat3("rot", &rot[0]);
			need_refresh |= ImGui::DragFloat3("size", &size[0]);
			ret |= ImGui::ColorEdit4("color", &color[0]);
			char buf[256]={};
			strcpy(buf, type.c_str());
			if(ImGui::InputText("type", buf, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
				type = buf;
				need_refresh = true;
			}
			strcpy(buf, sound.c_str());
			if(ImGui::InputText("sound", buf, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
				sound = buf;
				need_refresh = true;
			}
			need_refresh |= ImGui::Checkbox("trigger", &trigger);
			need_refresh |= ImGui::Checkbox("loop", &loop);
			if(need_refresh && ImGui::Button("refresh") && refresh()) {
				need_refresh = false;
			}
		}
		void apply() {
			primitive->setPosition(pos);
			primitive->setOrientation(rot);
			primitive->updatePhysicsTransform();
			shape->color = color;
		}
		bool refresh() {
			if (type == "cube")
			{
				primitive = make_shared<rdtk::BoxPrimitive>(size);
			}
			else if (type == "pyramid")
			{
				primitive = make_shared<rdtk::PyramidPrimitive>(size.x);
			}
			else if (type == "sphere")
			{
				primitive = make_shared<rdtk::SpherePrimitive>(size.x);
			}
			else
			{
				ofLogError("Shape") << "invalid shape type";
				shape.reset();
				return false;
			}
			shape = make_shared<Shape>();
			shape->set(index, primitive);
			
			if (sound != "") {
				shape->loadSound(rdtk::ToResourcePath(sound), trigger, loop);
			}
			apply();
			return true;
		}
	};
	vector<ShapeSettings> shapes;
};
