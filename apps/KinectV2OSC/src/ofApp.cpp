#include "ofApp.h"
#include "ramActor.h"
#include "ofxJsonUtils.h"

namespace {
	int kinectToRam(int joint_id) {
		switch (joint_id) {
		case JointType_SpineBase:		return ramActor::HIPS;
		case JointType_SpineMid:		return ramActor::ABDOMEN;
		case JointType_Neck:			return ramActor::NECK;
		case JointType_Head:			return ramActor::HEAD;
		case JointType_ShoulderLeft:	return ramActor::L_SHOULDER;
		case JointType_ElbowLeft:		return ramActor::L_ELBOW;
		case JointType_WristLeft:		return ramActor::L_WRIST;
		case JointType_HandLeft:		return ramActor::L_HAND;
		case JointType_ShoulderRight:	return ramActor::R_SHOULDER;
		case JointType_ElbowRight:		return ramActor::R_ELBOW;
		case JointType_WristRight:		return ramActor::R_WRIST;
		case JointType_HandRight:		return ramActor::R_HAND;
		case JointType_HipLeft:			return ramActor::L_HIP;
		case JointType_KneeLeft:		return ramActor::L_KNEE;
		case JointType_AnkleLeft:		return ramActor::L_ANKLE;
		case JointType_FootLeft:		return ramActor::L_TOE;
		case JointType_HipRight:		return ramActor::R_HIP;
		case JointType_KneeRight:		return ramActor::R_KNEE;
		case JointType_AnkleRight:		return ramActor::R_ANKLE;
		case JointType_FootRight:		return ramActor::R_TOE;
		case JointType_SpineShoulder:	return ramActor::CHEST;
		}
		return -1;
	}
	bool shouldUseJoint(int joint_id) {
		return kinectToRam(joint_id) != -1;
	}
}
void ofApp::save()
{
	ofJson json = {
		{"matrix", ofToString(virtual_camera_.getGlobalTransformMatrix())},
		{"num", max_num_player_},
		{"area",ofToString(valid_area_)},
		{"host",osc_host_},
		{"port",osc_port_}
	};
	ofxJsonUtils::writeToFile("settings.json", json, 4);
}
void ofApp::load()
{
	ofJson json = ofxJsonUtils::loadFromFile("settings.json");
	if(json.is_null()) {
		return;
	}
	virtual_camera_.setTransformMatrix(ofFromString<ofMatrix4x4>(json["matrix"]));
	max_num_player_ = json["num"];
	valid_area_ = ofFromString<ofRectangle>(json["area"]);

	osc_host_ = json["host"];
	osc_port_ = json["port"];
	sender_.setup(osc_host_, osc_port_);

}

//--------------------------------------------------------------
void ofApp::setup(){
	if (device_.setup() && body_.setup(device_)) {
		body_.open();
	}
	gui_.setup();
	ofBackground(0);

	load();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (!ImGui::IsMouseHoveringAnyWindow()) {
		preview_camera_.enableMouseInput();
	}
	else {
		preview_camera_.disableMouseInput();
	}


	device_.update();
	auto bodies = body_.getBodies();
	int valid_num = 0;
	for (auto &b : bodies) {
		ofVec2f pos_2d = [this](CameraSpacePoint pos) {
			ofMatrix4x4 mat = virtual_camera_.getModelViewMatrix();
			ofPoint p = mat.preMult(ofPoint(pos.X,pos.Y,pos.Z)*preview_scale_);
			return ofVec2f(p.x,p.z);
		}(b.getJoint(JointType_SpineBase).Position);
		if(!valid_area_.inside(pos_2d)) {
			continue;
		}
		if(++valid_num > max_num_player_) {
			break;
		}
		float scale = 100;
		ramActor actor;
		for (int i = 0; i < JointType_Count; ++i) {
			if (!shouldUseJoint(i)) {
				continue;
			}
			const Joint &j = b.getJoint(i);
			ofVec3f pos;
			pos.x = j.Position.X;
			pos.y = j.Position.Y;
			pos.z = j.Position.Z;
			pos *= scale;
			ofQuaternion ori;
			ori.x() = 
			ori.y() = 
			ori.z() = 0;
			ori.w() = 1;
			int ram_joint = kinectToRam(i);
			actor.nodes_[ram_joint].setPosition(pos);
			actor.nodes_[ram_joint].setOrientation(ori);
		}
		actor.collection();

		ofxOscMessage m;
		m.setAddress("/ram/skeleton");
		m.addStringArg(ofToHex(b.getId()));
		m.addIntArg(ramActor::NUM_JOINTS);
		for (int i = 0; i < ramActor::NUM_JOINTS; ++i) {
			m.addStringArg(ramActor::getJointName(i));
			auto mat = actor.nodes_[i].getGlobalTransformMatrix();
			mat.postMult(virtual_camera_.getGlobalTransformMatrix().getInverse());
			auto pos = mat.getTranslation();
			auto ori = mat.getRotate();
			m.addFloatArg(pos.x);
			m.addFloatArg(pos.y);
			m.addFloatArg(pos.z);
			m.addFloatArg(ori.x());
			m.addFloatArg(ori.y());
			m.addFloatArg(ori.z());
			m.addFloatArg(ori.w());
		}
		m.addFloatArg(ofGetElapsedTimef());
		sender_.sendMessage(m);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (view_from_virtual_camera_) {
		virtual_camera_.begin();
	}
	else {
		preview_camera_.begin();
	}
	virtual_camera_.transformGL();
	ofPushMatrix();
	ofRotateZ(90);
	ofDrawGridPlane(50);
	ofPopMatrix();
	ofPushMatrix();
	ofRotateX(90);
	ofPushStyle();
	ofSetColor(ofColor::yellow, 128);
	ofDrawRectangle(valid_area_);
	ofPopStyle();
	ofPopMatrix();
	ofDrawAxis(100);
	virtual_camera_.restoreTransformGL();

	ofPushStyle();
	auto bodies = body_.getBodies();
	int valid_num = 0;
	for (auto &b : bodies) {
		ofVec2f pos_2d = [this](CameraSpacePoint pos) {
			ofMatrix4x4 mat = virtual_camera_.getModelViewMatrix();
			ofPoint p = mat.preMult(ofPoint(pos.X,pos.Y,pos.Z)*preview_scale_);
			return ofVec2f(p.x,p.z);
		}(b.getJoint(JointType_SpineBase).Position);
		if(valid_area_.inside(pos_2d) && ++valid_num <= max_num_player_) {
			ofSetColor(ofColor::green);
		}
		else {
			ofSetColor(ofColor::red, 64);
		}
		for(int i = 0, num = b.getNumJoints(); i < num; ++i) {
			ofPoint p = ofPoint(b.getJoint(i).Position.X, b.getJoint(i).Position.Y, b.getJoint(i).Position.Z)*preview_scale_;
			ofDrawBox(p,5);
		}
	}
	ofPopStyle();
	if (view_from_virtual_camera_) {
		virtual_camera_.end();
	}
	else {
		preview_camera_.end();
	}


	gui_.begin();
	if (ImGui::Begin("Settings")) {
		if (ImGui::Button("Save")) {
			save();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			load();
		}
		if(ImGui::TreeNode("camera")) {
			ofVec3f move(0,0,0);
			if (ImGui::DragFloat3("Move", &move[0], 0.01f)) {
				virtual_camera_.move(move);
			}
			float roll(0);
			if (ImGui::DragFloat("Roll", &roll, 0.01f)) {
				virtual_camera_.roll(roll);
			}
			float pitch(0);
			if (ImGui::DragFloat("Pitch", &pitch, 0.01f)) {
				virtual_camera_.tilt(pitch);
			}
			float yaw(0);
			if (ImGui::DragFloat("Yaw", &yaw, 0.01f)) {
				virtual_camera_.pan(yaw);
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("clipping")) {
			ImGui::SliderInt("player", &max_num_player_, 0, 6);
			ofVec2f center = valid_area_.getCenter();
			ofVec2f size = ofVec2f(valid_area_.getWidth(),valid_area_.getHeight());
			if(ImGui::DragFloat2("center", &center[0]) || ImGui::DragFloat2("size", &size[0])) {
				valid_area_.setFromCenter(center, size.x, size.y);
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("osc")) {
			char buf[256]={};
			strcpy(buf,osc_host_.c_str());
			if(ImGui::InputText("host", buf, 256, ImGuiInputTextFlags_EnterReturnsTrue)
			|| ImGui::InputInt("port", &osc_port_, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {
				osc_host_ = buf;
				sender_.setup(osc_host_, osc_port_);
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
	gui_.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 's': 
		save();
		break;
	case 'l': 
		load();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
