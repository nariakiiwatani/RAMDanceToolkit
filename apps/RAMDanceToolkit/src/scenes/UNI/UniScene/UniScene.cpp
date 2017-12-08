#include "UniScene.h"

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#ifndef round
#define round(x) (x<0?ceil((x)-0.5):floor((x)+0.5))
#endif

bool UniScene::Wall::drawImGui()
{
	ofVec2f center = rect.getCenter();
	ofVec2f size = ofVec2f(rect.getWidth(), rect.getHeight());
	dirty |= ImGui::DragFloat2("center", &center[0]);
	dirty |= ImGui::DragFloat2("size", &size[0]);
	dirty |= ImGui::DragFloatRange2("height", &y_range[0], &y_range[1], 1, 0, 10000);
	if(dirty) {
		rect.setFromCenter(center, size.x, size.y);
	}
	return dirty && ImGui::Button("refresh");
}

void UniScene::Wall::refresh(ofxBulletWorldSoft &world)
{
	for(auto &box : boxes) {
		box->remove();
		box.reset();
	}
	boxes.resize(4);
	for(auto &box : boxes) {
		box = make_shared<ofxBulletBox>();
	}
	ofVec3f center = ofVec3f(rect.getCenter().x, (y_range[0]+y_range[1])/2.f, rect.getCenter().y);
	ofVec3f size = ofVec3f(rect.getWidth(), y_range[1]-y_range[0], rect.getHeight());
	ofVec3f half_size = size/2.f;
	float mass = 0;
	float size_min = 1;
	boxes[0]->create(world.world, ofVec3f(center.x+half_size.x, center.y, center.z), mass, size_min, size.y, size.z);
	boxes[1]->create(world.world, ofVec3f(center.x-half_size.x, center.y, center.z), mass, size_min, size.y, size.z);
	boxes[2]->create(world.world, ofVec3f(center.x, center.y, center.z+half_size.z), mass, size.x, size.y, size_min);
	boxes[3]->create(world.world, ofVec3f(center.x, center.y, center.z-half_size.z), mass, size.x, size.y, size_min);
	for(auto &box : boxes) {
		box->add();
	}
	dirty = false;
}

void UniScene::setup()
{
    //!
    actorName = "";
    sphere_size=255.;
    selected_node = 0;
    gavity = -98.0;
    auto & cam = rdtk::CameraManager::instance().getActiveCamera(); // <-- Get ramCamera like this
    world.setup();
    world.setGravity(ofVec3f(0., gavity, 0.));
    world.setCamera(&cam);
	
	ground.create(world.world, ofVec3f(0,0,0), 0.f, 2000,1,2000);
    ground.add();
	
    //!
    colors[0] = ofColor(15,197,138);
    colors[1] = ofColor(220, 0, 220);
    colors[2] = ofColor(220, 180, 60);
    colors[3] = ofColor(255, 20, 50);
    
	ofVec3f center = ofVec3f(wall.rect.getCenter().x, (wall.y_range[0]+wall.y_range[1])/2.f, wall.rect.getCenter().y);
	for (int i = 0;i<5;i++){
		sphere_mouse[i].create(world.world, center, 1.f, 10.f);
		sphere_mouse[i].add();
		
		sphere_mouse2[i].create(world.world, center, 1.f, 10.f);
		sphere_mouse2[i].add();
	}

    bDrawDebug	= true;
    bSpacebar	= false;
    bAddEllipsoid = true;
    bShapesNeedErase = false;
    
    for (int i=0;i<5;i++)
    {
        picker[i].setWorld(world.getWorld());
        
        picker2[i].setWorld(world.getWorld());
    }
    
}

void UniScene::update()
{
    //!
    if (ofGetKeyPressed('c')) { clearSpheres(); }
    world.update();
    
    //!
    auto & cam = rdtk::CameraManager::instance().getActiveCamera(); // <-- Get ramCamera like this
    mousePos = cam.screenToWorld( ofVec3f((float)ofGetMouseX(), (float)ofGetMouseY(), 0) );
    //  joints[0]->updatePivotPos( mousePos, 2.f );
    
    //    picker.updatePosition(btVector3(mousePos.x,mousePos.y,mousePos.z));
    
    for (int i=0;i<5;i++)
    {
        picker[i].attatchRigidBody(sphere_mouse[i].getRigidBody());
        picker2[i].attatchRigidBody(sphere_mouse2[i].getRigidBody());
    }
    //    sphere_mouse.applyForce(<#const ofVec3f &a_frc#>, <#const ofVec3f &a_rel_pos#>)
    //    sphere_mouse.
    //!
    
    bSpacebar = false;
    bShapesNeedErase = true;
    
    //! for debugging...
    
    selected_node = rdtk::ActorManager::instance().getLastSelectedNodeIdentifer().index;
    // if (selected_node < 0) selected_node = 0;
    selected_node=17;
    // int a=1;
}

void UniScene::draw()
{
    
	ofPushStyle();
//    glEnable( GL_DEPTH_TEST );
    rdtk::BeginCamera(); {
        ofSetLineWidth(1.f);
        if(bDrawDebug) world.drawDebug();
        
        //!
        ofSetColor(255, 255, 255);
        //   ofDrawSphere(mousePos, .15f);
        
        //!
//        ofEnableLighting();
//        light.enable();
        //    light.setPosition( mousePos );
//        light.setPosition( ofVec3f(0, 100, -1000) );
        
        //!
        ofSetColor(100, 100, 100);
        /*    ground->draw();
         wall[0].draw();
         wall[1].draw();
         wall[2].draw();*/
        
        ofNoFill();
        ofSetColor(255, 255, 255);
        for (int i =0;i<sphere.size();i++){
            sphere[i]->draw();
        }
        
        ofFill();
        
        //    uni.draw();
        ofSetColor(255,0,0);
        for (int i=0;i<5;i++){
            sphere_mouse[i].draw();
            sphere_mouse2[i].draw();
        }
//        light.disable();
//        ofDisableLighting();
    }
    
    rdtk::EndCamera();
//    glDisable(GL_DEPTH_TEST);
    ofPopStyle();
}

void UniScene::drawActor(const rdtk::Actor &actor)
{
    //   pushAll();
    

	ofPushStyle();
//    glEnable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
    

    
    for (int i=0; i<actor.getNumNode(); i++)
    {
        const rdtk::Node &node = actor.getNode(i);
        float jointSize = (i==rdtk::Actor::JOINT_HEAD) ? 6.0 : 3.0;
        
        node.beginTransform();
        ofSetColor(rdtk::Color::BLUE_LIGHT);
        ofNoFill();
        ofDrawBox(jointSize);
        
        if (i==selected_node) {
            ofSetColor(rdtk::Color::RED_LIGHT);
            ofDrawBox(jointSize*1.5f);
        }
        node.endTransform();
        
        if (node.hasParent())
        {
            ofSetColor(rdtk::Color::RED_LIGHT);
            ofDrawLine(node, *node.getParent());
        }
        const ofVec3f lhp = actor.getNode(17).getGlobalPosition();
        const ofVec3f rhp = actor.getNode(22).getGlobalPosition();
        const ofVec3f llp = actor.getNode(7).getGlobalPosition();
        const ofVec3f rlp = actor.getNode(11).getGlobalPosition();
        const ofVec3f hipp = actor.getNode(0).getGlobalPosition();
        
        if (i==0)
        {
            picker[0].updatePosition(btVector3(lhp.x, lhp.y, lhp.z));
            picker[1].updatePosition(btVector3(rhp.x, rhp.y, rhp.z));
            picker[2].updatePosition(btVector3(llp.x, llp.y, llp.z));
            picker[3].updatePosition(btVector3(rlp.x, rlp.y, rlp.z));
            picker[4].updatePosition(btVector3(hipp.x, hipp.y, hipp.z));
        }else if(i==1)
        {
            picker2[0].updatePosition(btVector3(lhp.x, lhp.y, lhp.z));
            picker2[1].updatePosition(btVector3(rhp.x, rhp.y, rhp.z));
            picker2[2].updatePosition(btVector3(llp.x, llp.y, llp.z));
            picker2[3].updatePosition(btVector3(rlp.x, rlp.y, rlp.z));
            picker2[4].updatePosition(btVector3(hipp.x, hipp.y, hipp.z));
        }
    }
	ofPopStyle();
    //    ofDrawSphere(lhp.x, lhp.y, lhp.z, 50.f);
    //    popAll();
}
void UniScene::drawImGui()
{
    //const char* label, float* v, float v_speed, float v_min, float v_max, const char* display_format, float power
	ImGui::Checkbox("draw debug", &bDrawDebug);
    ImGui::SliderFloat("Sphere size", &sphere_size, 100., 300.);
    if(ImGui::SliderFloat("gavity", &gavity, -980.0, 980.0)){
        world.setGravity(ofVec3f(0., gavity, 0.));
    }
	if(ImGui::SmallButton("add")){
		add();
	}
	ImGui::SameLine();
	if(ImGui::SmallButton("clear")){
		clearSpheres();
	}
	if(ImGui::TreeNode("wall")) {
		if(wall.drawImGui()) {
			wall.refresh(world);
		}
		ImGui::TreePop();
	}
}

void UniScene::onEnabled()
{
	clearSpheres();
	BaseSceneWithJsonSettings::onEnabled();
	world.setGravity(ofVec3f(0., gavity, 0.));
	wall.refresh(world);

	btVector3 center = btVector3(wall.rect.getCenter().x, (wall.y_range[0]+wall.y_range[1])/2.f, wall.rect.getCenter().y);
	for (int i = 0;i<5;i++){
		sphere_mouse[i].getCollisionObject()->getWorldTransform().setOrigin(center);
		sphere_mouse2[i].getCollisionObject()->getWorldTransform().setOrigin(center);
	}

	add();
}
void UniScene::add()
{
	ofVec3f random_pos = [this]() {
		ofVec3f center = ofVec3f(wall.rect.getCenter().x, (wall.y_range[0]+wall.y_range[1])/2.f, wall.rect.getCenter().y);
		ofVec3f size = ofVec3f(wall.rect.getWidth(), wall.y_range[1]-wall.y_range[0], wall.rect.getHeight());
		ofVec3f half_size = size/2.f;
		return center + ofVec3f(ofRandom(-half_size.x,half_size.x), half_size.y, ofRandom(-half_size.z,half_size.x));;
	}();
    sphere.push_back( new ofxBulletSphere());
	sphere[sphere.size()-1]->create(world.world, random_pos, 0.01f, sphere_size);
	sphere[sphere.size()-1]->add();
}

void UniScene::clearSpheres(){
	for(auto &s : sphere) {
		s->remove();
	}
    sphere.clear();
}

