//
//  ViewController.h
//  RamNodeSender
//
//  Created by 神田竜 on 2014/05/09.
//  Copyright (c) 2014年 神田竜. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "VVOSC.h"
#import <CoreMotion/CoreMotion.h>
#include "Quaternion.h"

@interface ViewController : UIViewController
@property (weak, nonatomic) IBOutlet UITextField *hostTextField;
@property (weak, nonatomic) IBOutlet UITextField *portTextField;
@property CMMotionManager *motionManager;
@property NSTimeInterval startTime;

@end
