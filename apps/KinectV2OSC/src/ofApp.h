#pragma once

#include "ofMain.h"
#include "ofxKinect2.h"
#include "ofxOscSender.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		private:
		ofxKinect2::Device device_;
		ofxKinect2::BodyStream body_;
		ofxOscSender sender_;
	private:
		ofxImGui::Gui gui_;
		ofEasyCam preview_camera_;
		ofCamera virtual_camera_;

		bool view_from_virtual_camera_ = false;
		float preview_scale_ = 100;
		void save();
		void load();

		int max_num_player_=1;
		ofRectangle valid_area_;

		int osc_port_;
		std::string osc_host_;
};
