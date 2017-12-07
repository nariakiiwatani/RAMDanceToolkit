#pragma once

#include "imgui.h"
#include "BaseSceneWithJsonSettings.h"
#include "ofxJsonUtilsUtils.h"

class AutoSwitcher : public BaseSceneWithJsonSettings
{
public:
	void addScene(ofPtr<rdtk::BaseScene> scene) {
		scenes_.push_back(scene);
	}
	string getName() const { return "AutoSwitcher"; }
	
	void drawImGui()
	{
		ImGui::SliderFloat("interval", &interval_, 0, 30);
		ImGui::SliderFloat("timer", &timer_, 0, interval_);
		ImGui::DragFloat2("string offset", &string_offset_[0]);
		ImGui::ColorEdit4("string color", &string_color_[0]);
	}
	
	JSON_FUNCS(interval_,string_offset_,string_color_);

	void setup()
	{
		font_.load(rdtk::ToResourcePath(RAM_FONT_FILE),64,true,true,true);
	}
	
	void update()
	{
		float timef = ofGetLastFrameTime();
		timer_ += timef;
		if(timer_ >= interval_) {
			nextScene();
			timer_ = 0;
		}
	}
	
	void draw()
	{
	}
	void drawHUD()
	{
		if(auto scene = getScene(active_scene_index_)) {
			ofPushStyle();
			ofSetColor(string_color_);
			ofRectangle rect = font_.getStringBoundingBox(scene->getName(),0,0);
			font_.drawString(scene->getName(), ofGetWidth()-rect.getWidth()+string_offset_.x, rect.getBottom()+string_offset_.y);
			ofPopStyle();
		}
	}
private:
	ofPtr<rdtk::BaseScene> getScene(int index) {
		if(0 <= index && index < scenes_.size()) {
			return scenes_[index];
		}
		return nullptr;
	};
	void nextScene() {
		if(auto scene = getScene(active_scene_index_)) {
			scene->setEnabled(false);
		}
		if(++active_scene_index_ >= scenes_.size()) {
			active_scene_index_ = 0;
		}
		if(auto scene = getScene(active_scene_index_)) {
			scene->setEnabled(true);
		}
	}
	
	float interval_=20;
	float timer_=0;
	ofVec2f string_offset_;
	ofFloatColor string_color_;
	
	std::vector<ofPtr<rdtk::BaseScene>> scenes_;
	int active_scene_index_=-1;
	ofTrueTypeFont font_;
};
