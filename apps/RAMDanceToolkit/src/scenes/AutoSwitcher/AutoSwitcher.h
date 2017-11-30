#pragma once

#include "imgui.h"
#include "BaseSceneWithJsonSettings.h"

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
	}

	void setup()
	{
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
private:
	void nextScene() {
		auto getScene = [this](int index) -> ofPtr<rdtk::BaseScene> {
			if(0 <= index && index < scenes_.size()) {
				return scenes_[index];
			}
			return nullptr;
		};
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
	
	std::vector<ofPtr<rdtk::BaseScene>> scenes_;
	int active_scene_index_=-1;
};
