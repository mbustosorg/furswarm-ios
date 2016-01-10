//
//  swarmMemberDisplay.h
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Copyright (c) 2012, Mauricio Bustos
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, 
//  with or without modification, are permitted provided 
//  that the following conditions are met:
//
//  - Redistributions must not be for profit.  This software in source or binary form, 
//      with or without modification, cannot be included in anyproduct that is then sold.  
//      Please contact Mauricio Bustos m@bustos.org if you are interested in a for-profit license.
//  - Redistributions of source code or in binary form must reproduce the above copyright notice, 
//      this list of conditions and the following disclaimer in the documentation 
//      and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
//  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
//  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef furSwarmProj_swarmMemberDisplay_h
#define furSwarmProj_swarmMemberDisplay_h

#include "swarmMember.h"
#include "furSwarmConst.h"
#include "ofGraphics.h"
#include "ofTrueTypeFont.h"
#include "ofImage.h"

const int TITLE_X = 20;
const int TITLE_INDENT = 20;
const int DATA_COLUMN_X = 200;
const int PUMP_BUTTON_HEIGHT = 50;
const int PUMP_BUTTON_WIDTH = 70;
const int HEART_BEAT_INTERVAL = 750;
const int PUMP_BUTTON_OFFSET = 50;
const double RADIUS_FACTOR = 55.0;
const int DEFAULT_MEMBER_SIZE = 10;

#if (TARGET_IPHONE_SIMULATOR || !TARGET_OS_IPHONE)
const double ANIMATION_PERCENTAGE_INTERVAL = 0.005;
const double SPIN_FACTOR = 10.0;
const int TITLE_COLOR_INCREMENT = 0x0F0000;
const int TITLE_COLOR_MISSING_INCREMENT = 0x0F0F0F;
#else
const double ANIMATION_PERCENTAGE_INTERVAL = 0.010;
const double SPIN_FACTOR = 20.0;
const int TITLE_COLOR_INCREMENT = 0x0A0000;
const int TITLE_COLOR_MISSING_INCREMENT = 0x0A0A0A;
#endif

//! View object for a swarm member.
/*!
    Handles the expansion animation to
    reveal the swarm member details.
 */
class swarmMemberDisplay {
    
public: 
    
    swarmMemberDisplay(swarmMember* newMember, int x, int y, int radius);
    
    void display();
    void moveTo(int xTarget, int yTarget);
    void moveToDistance();
    void moveToOrbit();
    bool hasLatLong();
    void toggle();
    bool pointValid(int x, int y);
    int height;
    swarmMember* member;
    static int color;
    int position[2];
    bool selected;
    
private:
    
    ofTrueTypeFont font;
    double lastRssiFactor();
    double distance();
    float batteryVoltage();
    void setColor();
    int heartBeatTimestamp;
    
    //  Animation
    void setAnimationSettings();
    int thetaThrottle;    
    double expansionAnimationPercentage;
    double rssiChangeAnimationPercentage;
    double currentEnergy;
    double newEnergy;
    double animationStep;
    void animateMemberInOrbit();
    void animateMemberToTarget();
    void setOrbitPosition(int oPosition[], double oVelocity[]);
    bool goingToOrbit;
    bool goingToTarget;
    bool fixedSize;
    double damping, throttle;
    int titleColor;
    string memberInfoLabel();
    
    //  Position / size / color
    int ellipseX, ellipseY;
    double ellipseRadius;
    double theta;
    int memberColor;
    double doublePosition[2];
    double velocity[2];
    int targetLocation[2];
};

#endif
