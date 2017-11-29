#pragma once

#include "ramMain.h"
#include "ramCameraManager.h"
#include "imgui.h"
#include "BaseSceneWithJsonSettings.h"

class MirrorCamera : public BaseSceneWithJsonSettings
{
public:
	ofCamera *camera_=nullptr;
	int my_camera_index_=0;
	ofVec3f scale_=ofVec3f(1,1,1);
	int active_camera_cache_ = 0;
	ofxOscReceiver osc_;
	float mirror_width_=300;
	struct TestNode {
		ofVec3f pos = ofVec3f(0,160,100);
		ofQuaternion ori;
		bool enabled=false;
	} test_;
	string getName() const { return "MirrorCamera"; }
	
	void drawImGui()
	{
		ImGui::DragFloat("mirror width", &mirror_width_);
		ImGui::DragFloat3("scale", &scale_[0], 0.01f);
		if(ImGui::TreeNode("Test")) {
			ImGui::Checkbox("enable", &test_.enabled);
			ImGui::DragFloat3("pos", &test_.pos[0]);
			ImGui::DragFloat4("orientation", &test_.ori[0], 0.01f,-1,1);
			ImGui::TreePop();
		}
	}
	
	void onEnabled() {
		auto &cm = rdtk::CameraManager::instance();
		active_camera_cache_ = cm.getActiveCameraIndex();
		cm.setActiveCamera(my_camera_index_);
		BaseSceneWithJsonSettings::onEnabled();
	}
	
	void onDisabled() {
		auto &cm = rdtk::CameraManager::instance();
		cm.setActiveCamera(active_camera_cache_);
		BaseSceneWithJsonSettings::onDisabled();
	}

	void setup()
	{
		auto &cm = rdtk::CameraManager::instance();
		my_camera_index_ = cm.getNumCameras();
		camera_ = cm.createCamera<ofCamera>();
	}
	
	void update()
	{
		ofVec3f actor_pos;
		if([this](ofVec3f &actor_pos) {
			if(test_.enabled) {
				actor_pos = test_.pos*scale_;
				return true;
			}
			const rdtk::Actor *actor = nullptr;
			size_t num_node_arrays = rdtk::ActorManager::instance().getNumNodeArray();
			for(int i = 0; i < num_node_arrays; ++i) {
				const rdtk::NodeArray &node = rdtk::ActorManager::instance().getNodeArray(i);
				if(node.isActor()) {
					actor = static_cast<const rdtk::Actor*>(&node);
				}
			}
			if(actor) {
				actor_pos = actor->getNode(rdtk::Actor::JOINT_HEAD).getGlobalPosition();
				return true;
			}
			return false;
		}(actor_pos)) {
			ofVec3f camera_pos = actor_pos;
			camera_pos.z = -actor_pos.z;
			camera_->setLensOffset(ofVec2f(0,0));
			camera_->setPosition(camera_pos);
			camera_->lookAt(actor_pos);
			ofVec3f origin_screen_pos = camera_->worldToScreen(ofVec3f(0,0,0));
			float lens_offset = ofMap(origin_screen_pos.x, 0, ofGetWidth(), -1, 1);
			camera_->setLensOffset(ofVec2f(lens_offset,0));
			{
				ofVec2f a = ofVec2f(-mirror_width_/2.f,0) - ofVec2f(camera_pos.x,camera_pos.z);
				ofVec2f b = ofVec2f(mirror_width_/2.f,0) - ofVec2f(camera_pos.x,camera_pos.z);
				camera_->setFov(b.angle(a));
			}
		}
	}
	
	
	void draw()
	{
		rdtk::BeginCamera();
		
		ofNoFill();
		ofPushStyle();
		if(test_.enabled) {
			ofDrawSphere(test_.pos*scale_, 20);
		}
		ofPopStyle();
		rdtk::EndCamera();
	}
};
