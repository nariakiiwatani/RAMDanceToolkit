#pragma once

#include "imgui.h"
#include "BaseSceneWithJsonSettings.h"
#include "ofxJsonUtilsUtils.h"

class AutoSwitcher : public BaseSceneWithJsonSettings
{
public:
	void addScene(ofPtr<rdtk::BaseScene> scene) {
		scenes_.push_back(scene);
		scenes_enabled_.resize(scenes_.size());
		authors_[scene->getName()] = "";
	}
	string getName() const { return "AutoSwitcher"; }
	
	void drawImGui()
	{
		ImGui::SliderFloat("interval", &interval_, 0, 30);
		ImGui::SliderFloat("timer", &timer_, 0, interval_);
		ImGui::DragFloat2("string offset", &string_offset_[0]);
		ImGui::ColorEdit4("string color", &string_color_[0]);
		if(ImGui::TreeNode("on/off switch")) {
			for(int i = 0, num = scenes_.size(); i < num; ++i) {
				bool flag = scenes_enabled_[i];
				if(ImGui::Checkbox(scenes_[i]->getName().c_str(), &flag)) {
					scenes_enabled_[i] = flag;
				}
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("author")) {
			char buf[256]={};
			for(auto &a : authors_) {
				strcpy(buf, a.second.c_str());
				if(ImGui::InputText(a.first.c_str(), buf, 256)) {
					a.second = buf;
				}
			}
			ImGui::TreePop();
		}
	}
	
	JSON_FUNCS(interval_,string_offset_,string_color_,scenes_enabled_,authors_);

	void setup()
	{
		font_.load(rdtk::ToResourcePath(RAM_FONT_FILE),64,true,true,true);
	}
	
	void update()
	{
		float timef = ofGetLastFrameTime();
		timer_ += timef;
		if(timer_ >= interval_) {
			if(nextScene()) {
				timer_ = 0;
			}
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
			string str = scene->getName();
			ofRectangle rect = font_.getStringBoundingBox(str,0,0);
			font_.drawString(str, ofGetWidth()-rect.getWidth()+string_offset_.x, rect.getBottom()+string_offset_.y);
			string author = authors_[scene->getName()];
			if(author != "") {
				ofRectangle rect = font_.getStringBoundingBox(author,0,0);
				font_.drawString(author, ofGetWidth()-rect.getWidth()+string_offset_.x, rect.getBottom()+string_offset_.y+font_.getLineHeight());
			}
			ofPopStyle();
		}
	}
private:
	ofPtr<rdtk::BaseScene> getScene(int index) {
		if(0 <= index && index < scenes_.size()) {
			return scenes_enabled_[index] ? scenes_[index] : nullptr;
		}
		return nullptr;
	};
	bool nextScene() {
		if(auto scene = getScene(active_scene_index_)) {
			scene->setEnabled(false);
		}
		if(++active_scene_index_ >= scenes_.size()) {
			active_scene_index_ = 0;
		}
		if(auto scene = getScene(active_scene_index_)) {
			scene->setEnabled(true);
			return true;
		}
		return false;
	}
	
	float interval_=20;
	float timer_=0;
	ofVec2f string_offset_;
	ofFloatColor string_color_;
	
	std::vector<ofPtr<rdtk::BaseScene>> scenes_;
	std::vector<int> scenes_enabled_;
	std::map<std::string, std::string> authors_;
	int active_scene_index_=-1;
	ofTrueTypeFont font_;
};
