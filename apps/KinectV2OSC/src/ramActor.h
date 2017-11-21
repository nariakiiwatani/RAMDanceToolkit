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
}
};

#endif /* ramActor_h */
