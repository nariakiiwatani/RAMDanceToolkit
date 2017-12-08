//
//  ramActor.h
//  RAM_KinectOsc
//
//  Created by Iwatani Nariaki on 2017/09/14.
//
//

#ifndef ramActor_h
#define ramActor_h

struct ramActor
{
public:
	enum Joint
	{
		HIPS,
		ABDOMEN,
		CHEST,
		NECK,
		HEAD,
		
		L_HIP,
		L_KNEE,
		L_ANKLE,
		L_TOE,
		
		R_HIP,
		R_KNEE,
		R_ANKLE,
		R_TOE,
		
		L_COLLAR,
		L_SHOULDER,
		L_ELBOW,
		L_WRIST,
		L_HAND,
		
		R_COLLAR,
		R_SHOULDER,
		R_ELBOW,
		R_WRIST,
		R_HAND,
		
		NUM_JOINTS
	};
	ofNode nodes_[NUM_JOINTS];

	static string getJointName(int joint)
	{
		string joint_name[NUM_JOINTS] =
		{
			"HIPS",
			"ABDOMEN",
			"CHEST",
			"NECK",
			"HEAD",
			"LEFT_HIP",
			"LEFT_KNEE",
			"LEFT_ANKLE",
			"LEFT_TOE",
			"RIGHT_HIP",
			"RIGHT_KNEE",
			"RIGHT_ANKLE",
			"RIGHT_TOE",
			"LEFT_COLLAR",
			"LEFT_SHOULDER",
			"LEFT_ELBOW",
			"LEFT_WRIST",
			"LEFT_HAND",
			"RIGHT_COLLAR",
			"RIGHT_SHOULDER",
			"RIGHT_ELBOW",
			"RIGHT_WRIST",
			"RIGHT_HAND"
		};
		
		return joint < NUM_JOINTS ? joint_name[joint] : "";
	}
	
	int parentJoint(int j) {
		switch(j) {
			case HIPS: return -1;
			case ABDOMEN: return HIPS;
			case CHEST: return ABDOMEN;
			case NECK: return CHEST;
			case HEAD: return NECK;

			case L_HIP: return HIPS;
			case L_KNEE: return L_HIP;
			case L_ANKLE: return L_KNEE;
			case L_TOE: return L_ANKLE;

			case R_HIP: return HIPS;
			case R_KNEE: return R_HIP;
			case R_ANKLE: return R_KNEE;
			case R_TOE: return R_ANKLE;

			case L_COLLAR: return CHEST;
			case L_SHOULDER: return L_COLLAR;
			case L_ELBOW: return L_SHOULDER;
			case L_WRIST: return L_ELBOW;
			case L_HAND: return L_WRIST;

			case R_COLLAR: return CHEST;
			case R_SHOULDER: return R_COLLAR;
			case R_ELBOW: return R_SHOULDER;
			case R_WRIST: return R_ELBOW;
			case R_HAND: return R_WRIST;

		}
		return -1;
	}
	void collection() {
		auto mid = [](ofNode &a, ofNode &b) {
			ofNode ret;
			ret.setGlobalPosition((a.getGlobalPosition()+b.getGlobalPosition())/2.f);
			ofQuaternion q = a.getGlobalOrientation();
			q.slerp(0.5f, q, b.getGlobalOrientation());
			ret.setGlobalOrientation(q);
			return ret;
		};
		nodes_[L_COLLAR] = mid(nodes_[CHEST], nodes_[L_SHOULDER]);
		nodes_[R_COLLAR] = mid(nodes_[CHEST], nodes_[R_SHOULDER]);
		
		for(int i = 0; i < NUM_JOINTS; ++i) {
			ofVec3f pos = nodes_[i].getPosition();
			pos.x *= -1;
			nodes_[i].setPosition(pos);
		}
		for(int i = 0; i < NUM_JOINTS; ++i) {
			int p = parentJoint(i);
			if(p >= 0) {
				nodes_[i].lookAt(nodes_[p]);
//				nodes_[i].rotate(180,ofVec3f(0,1,0));
			}
		}
	}
};

#endif /* ramActor_h */
