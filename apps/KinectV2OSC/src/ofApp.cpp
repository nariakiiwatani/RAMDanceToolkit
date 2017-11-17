#include "ofApp.h"
#include "ramActor.h"

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

//--------------------------------------------------------------
void ofApp::setup(){
	if (device_.setup() && body_.setup(device_)) {
		body_.open();
	}
	sender_.setup("localhost", 10000);
}

//--------------------------------------------------------------
void ofApp::update(){
	device_.update();
	auto bodies = body_.getBodies();
	for (auto &b : bodies) {
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
			auto &node = actor.nodes_[i];
			auto pos = node.getGlobalPosition();
			auto ori = node.getGlobalOrientation();
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
	body_.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
