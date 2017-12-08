/*-----------------------------------------------------------/
 randomCubeUni.cpp
 
 github.com/YCAMInterlab
 github.com/azuremous
 
 Created by Jung un Kim a.k.a azuremous on 10/27/17.
 /----------------------------------------------------------*/

#include "randomCubeUni.h"
const int newJointID[] = {
    0, //hip
    1, //abdomen
    2, //chest
    3, //neck
    4, //head
    
    5, //left hip
    6, //left knee
    7, //left ankle
    
    9, //right hip
    10, //right knee
    11, //right ankle
    
    14, //left collar
    15, //left shoulder
    16, //left elbow
    17, //left hand
    
    19, //right collar
    20, //right shoulder
    21, //right elbow
    22, //right hand
};

const string jointIDText[] = {
    "Hip",
    "Abdomen",
    "Chest",
    "Neck",
    "Head",
    
    "Left Hip",
    "Left Knee",
    "Left Toe",
    
    "Right Hip",
    "Right Knee",
    "Right Toe",
    
    "Left Collar",
    "Left Shoulder",
    "Left Elbow",
    "Left Hand",

    "Right Collar",
    "Right Shoulder",
    "Right Elbow",
    "Right Hand"
};

enum uni_joint{
    JOINT_HIPS              = 0,
    JOINT_ABDOMEN           = 1,
    JOINT_CHEST             = 2,
    JOINT_NECK              = 3,
    JOINT_HEAD              = 4,
    
    JOINT_LEFT_HIP          = 5,
    JOINT_LEFT_KNEE         = 6,
    JOINT_LEFT_ANKLE        = 7,
    //JOINT_LEFT_TOE          = 8,
    
    JOINT_RIGHT_HIP         = 9,
    JOINT_RIGHT_KNEE        = 10,
    JOINT_RIGHT_ANKLE       = 11,
    //JOINT_RIGHT_TOE         = 12,
    
    JOINT_LEFT_COLLAR       = 14,//13,
    JOINT_LEFT_SHOULDER     = 15,//14,
    JOINT_LEFT_ELBOW        = 16,//15,
    //JOINT_LEFT_WRIST        = 16,
    JOINT_LEFT_HAND         = 17,
    
    JOINT_RIGHT_COLLAR      = 19,//18,
    JOINT_RIGHT_SHOULDER    = 20,//19,
    JOINT_RIGHT_ELBOW       = 21,//20,
    //JOINT_RIGHT_WRIST       = 21,
    JOINT_RIGHT_HAND        = 22,
    
    NUM_JOINTS              = 23,//actual num of joint is 23 - 4 = 19
};

const ofColor tagColor[] = {
    ofColor(236, 1, 0), //0
    ofColor(236, 106, 12), //1
    ofColor(236, 217, 11), //2
    ofColor(153, 236, 12), //3
    ofColor(79, 236, 171), //4
    ofColor(23, 193, 236), //5
    ofColor(17, 38, 236), //6
    ofColor(144, 26, 236), //7
    ofColor(236, 168, 193), //8
    ofColor(236, 79, 171), //9
    ofColor(236, 109, 96), //10
    ofColor(223, 156, 236), //11
    ofColor(191, 213, 236), //12
    ofColor(176, 60, 54), //13
    ofColor(71, 38, 176), //14
    ofColor(40, 176, 110), //15
    ofColor(120, 139, 45), //16
    ofColor(112, 153, 255), //17
    ofColor(139, 34, 110), //18
    ofColor(16, 80, 0), //19
};



//--------------------------------------------------------------
void randomCubeUni::setup()
{
    auto & cam = rdtk::CameraManager::instance().getActiveCamera(); // <-- Get ramCamera like this
    infoFont.load(OF_TTF_SANS, 50);
}

//--------------------------------------------------------------
void randomCubeUni::update()
{
	const auto &nas = rdtk::ActorManager::instance().getAllNodeArrays();
	for(auto &na : nas) {
		if(!na.isActor()) {
			continue;
		}
		ofVec3f pos = na.getNode(newJointID[touchId]).getGlobalPosition();//
		
		float dis = pos.distance(visibleBox);
		if(dis <= box_size/2.f){
			next();
		}
	}
}

//--------------------------------------------------------------
void randomCubeUni::draw()
{
    auto & cam = rdtk::CameraManager::instance().getActiveCamera();
    cam.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofFill();
    ofSetColor(touchIDColor, 200);
    ofDrawBox(visibleBox, box_size);
    ofDisableAlphaBlending();
    ofPopStyle();
    
	if(showDebug) {
		ofVec3f center = ofVec3f(rect.getCenter().x, (y_range[0]+y_range[1])/2.f, rect.getCenter().y);
		ofVec3f size = ofVec3f(rect.getWidth(), y_range[1]-y_range[0], rect.getHeight());
		ofPushStyle();
		ofSetColor(ofColor::green);
		ofDrawBox(center, size.x,size.y,size.z);
		ofPopStyle();
	}
	cam.end();
}

void randomCubeUni::drawHUD(){
	ofPushStyle();
	ofSetColor(255, 0 , 0);
	infoFont.drawString(jointIDText[touchId], ofGetWidth()/2, 100);
	ofPopStyle();
}

//--------------------------------------------------------------
void randomCubeUni::drawImGui()
{
	ImGui::Checkbox("debug", &showDebug);
	if(ImGui::Button("next")) {
		next();
	}
	ofVec2f center = rect.getCenter();
	ofVec2f size = ofVec2f(rect.getWidth(), rect.getHeight());
	bool dirty = false;
	dirty |= ImGui::DragFloat2("center", &center[0]);
	dirty |= ImGui::DragFloat2("size", &size[0]);
	if(dirty) {
		rect.setFromCenter(center, size.x, size.y);
	}
	ImGui::DragFloatRange2("height", &y_range[0], &y_range[1], 1, 0, 1000);
	ImGui::DragFloat("box size", &box_size);
}

void randomCubeUni::onEnabled()
{
	BaseSceneWithJsonSettings::onEnabled();
	next();
}


void randomCubeUni::next()
{
	visibleBox = [this]() {
		ofVec3f center = ofVec3f(rect.getCenter().x, (y_range[0]+y_range[1])/2.f, rect.getCenter().y);
		ofVec3f size = ofVec3f(rect.getWidth(), y_range[1]-y_range[0], rect.getHeight());
		ofVec3f half_size = size/2.f;
		return center + ofVec3f(ofRandom(-half_size.x,half_size.x), ofRandom(-half_size.y,half_size.y), ofRandom(-half_size.z,half_size.x));;
	}();
	touchId = (int)ofRandom(0, 19);
	touchIDColor = tagColor[touchId];
}
