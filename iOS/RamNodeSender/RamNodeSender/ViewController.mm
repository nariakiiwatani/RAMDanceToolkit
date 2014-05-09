//
//  ViewController.m
//  RamNodeSender
//
//  Created by 神田竜 on 2014/05/09.
//  Copyright (c) 2014年 神田竜. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController


- (IBAction)hostDidEndOnExit:(id)sender
{
    NSLog(@"%@", self.hostTextField.text);
    [sender resignFirstResponder];
}

- (IBAction)portDidEndOnExit:(id)sender
{
    NSLog(@"%@", self.portTextField.text);
    [sender resignFirstResponder];
}


- (void)viewDidLoad
{
    
    [super viewDidLoad];
    
    _startTime = [[[NSDate alloc] init] timeIntervalSince1970];
    
	// Do any additional setup after loading the view, typically from a nib.
    OSCManager *manager = [[OSCManager alloc] init];
    manager.delegate = self;
        
    OSCOutPort *outPort = [manager createNewOutputToAddress:@"169.254.103.196" atPort:10000];
    
    _motionManager = [[CMMotionManager alloc] init];
    
    [_motionManager startDeviceMotionUpdatesToQueue:[NSOperationQueue currentQueue]
                                       withHandler:^(CMDeviceMotion *motion, NSError *error)
     {
         
         float dt = ([[[NSDate alloc] init] timeIntervalSince1970] - _startTime);
         
         Quaternion quat;
         quat.setEuler(motion.attitude.yaw,
                       motion.attitude.pitch,
                       motion.attitude.roll);
         
         //NSLog(@"%f",motion.attitude.yaw);
         
         float sinhalfangle = sqrt( quat.x * quat.x + quat.y * quat.y + quat.z * quat.z );
         
         float angle = 2.0 * atan2( sinhalfangle, quat.w );
         if (sinhalfangle) {
             quat.x = quat.x / sinhalfangle;
             quat.y = quat.y / sinhalfangle;
             quat.z = quat.z / sinhalfangle;
         } else {
             quat.x = 0.0;
             quat.y = 0.0;
             quat.z = 1.0;
         }
         
         angle = angle * 180.0 / 3.14159265;
         
         OSCMessage *message = [OSCMessage createWithAddress:@"/ram/rigid_body"];
         [message addString:@"iPhone"];
         [message addInt:1];
         [message addString:@"Neck"];
         [message addFloat:0.0];
         [message addFloat:0.0];
         [message addFloat:0.0];
         [message addFloat:angle];
         [message addFloat:quat.x];
         [message addFloat:quat.y];
         [message addFloat:quat.z];
         [message addFloat:dt];
         [outPort sendThisPacket:[OSCPacket createWithContent:message]];
        
         
         /*ofQuaternion ofQuat;
         ofQuat.set(quat.x, quat.y, quat.z, quat.w);
         
         ofVec3f axis;
         float angle;
         ofQuat.getRotate(angle, axis);
         
         ofxOscMessage m;
         m.setAddress( "/ram/rigid_body" );
         m.addStringArg( "iPhone" );
         m.addIntArg(1);
         m.addStringArg("Neck");
         m.addFloatArg(0.0);
         m.addFloatArg(0.0);
         m.addFloatArg(0.0);
         m.addFloatArg(angle);
         m.addFloatArg(axis.x);
         m.addFloatArg(axis.y);
         m.addFloatArg(axis.z);
         m.addFloatArg(ofGetElapsedTimef());
         sender.sendMessage( m );
         
         pitch = motion.attitude.pitch * 180 / M_PI;
         roll = motion.attitude.roll * 180 / M_PI;
         yaw = motion.attitude.yaw * 180 / M_PI;*/
         
     }];

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
