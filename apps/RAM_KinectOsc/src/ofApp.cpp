#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	receiver_.setup(12345);
	sender_.setup("localhost", 10000);
}

namespace {
	
	enum KinectJointType {
		JointType_SpineBase= 0,
		JointType_SpineMid= 1,
		JointType_Neck= 2,
		JointType_Head= 3,
		JointType_ShoulderLeft= 4,
		JointType_ElbowLeft= 5,
		JointType_WristLeft= 6,
		JointType_HandLeft= 7,
		JointType_ShoulderRight= 8,
		JointType_ElbowRight= 9,
		JointType_WristRight= 10,
		JointType_HandRight= 11,
		JointType_HipLeft= 12,
		JointType_KneeLeft= 13,
		JointType_AnkleLeft= 14,
		JointType_FootLeft= 15,
		JointType_HipRight= 16,
		JointType_KneeRight= 17,
		JointType_AnkleRight= 18,
		JointType_FootRight= 19,
		JointType_SpineShoulder= 20,
		JointType_HandTipLeft= 21,
		JointType_ThumbLeft= 22,
		JointType_HandTipRight= 23,
		JointType_ThumbRight= 24,
		JointType_Count= ( JointType_ThumbRight + 1 ) 
	};
	int kinectToRam(int joint_id) {
		switch(joint_id) {
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
void ofApp::update(){
	while(receiver_.hasWaitingMessages()) {
		ofxOscMessage msg;
		if(receiver_.getNextMessage(msg)) {
			const string &address = msg.getAddress();
			if(address == "/bodies") {
				float scale = 100;
				ramActor actor;
				for(int i = 0, num = msg.getArgAsInt32(1); i < num; ++i) {
					if(!shouldUseJoint(i)) {
						continue;
					}
					int msg_index = 2+i*7;
					ofVec3f pos;
					pos.x = msg.getArgAsFloat(msg_index++);
					pos.y = msg.getArgAsFloat(msg_index++);
					pos.z = msg.getArgAsFloat(msg_index++);
					pos *= scale;
					ofQuaternion ori;
					ori.x() = msg.getArgAsFloat(msg_index++);
					ori.y() = msg.getArgAsFloat(msg_index++);
					ori.z() = msg.getArgAsFloat(msg_index++);
					ori.w() = msg.getArgAsFloat(msg_index++);
					int ram_joint = kinectToRam(i);
					actor.nodes_[ram_joint].setPosition(pos);
					actor.nodes_[ram_joint].setOrientation(ori);
				}
				actor.collection();
				ofxOscMessage msg_send;
				msg_send.setAddress("/ram/skeleton");
				msg_send.addStringArg(msg.getArgAsString(0));
				msg_send.addIntArg(ramActor::NUM_JOINTS);
				for(int i = 0; i < ramActor::NUM_JOINTS; ++i) {
					msg_send.addStringArg(ramActor::getJointName(i));
					auto &node = actor.nodes_[i];
					auto pos = node.getGlobalPosition();
					auto ori = node.getGlobalOrientation();
					msg_send.addFloatArg(pos.x);
					msg_send.addFloatArg(pos.y);
					msg_send.addFloatArg(pos.z);
					msg_send.addFloatArg(ori.x());
					msg_send.addFloatArg(ori.y());
					msg_send.addFloatArg(ori.z());
					msg_send.addFloatArg(ori.w());
				}
				sender_.sendMessage(msg_send);
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

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
