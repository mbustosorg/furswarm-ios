//
//  swarmMemberDisplay.mm
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see swarmMemberDisplay.h for documentation
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

#include <iostream>
#include "swarmMemberDisplay.h"
#include "furSwarmPatternConst.h"

int swarmMemberDisplay::color = MEMBER_INITIAL_COLOR;

//! Initialize the display object based on a swarmMember object
swarmMemberDisplay::swarmMemberDisplay(swarmMember* newMember, int x, int y, int radius) {
    member = newMember;
    heartBeatTimestamp = ofGetElapsedTimeMillis();
    
	font.load("DIN.otf", 10);
    
    ellipseX = x;
    ellipseY = y;
    ellipseRadius = (double)radius;
    theta = 0;
    currentEnergy = 1;
    swarmMemberDisplay::color = swarmMemberDisplay::color + MEMBER_COLOR_INCREMENT;
    memberColor = swarmMemberDisplay::color;
    targetLocation[0] = 0;
    targetLocation[1] = 0;
    velocity[0] = 0.0;
    velocity[1] = 0.0;
    goingToOrbit = false;
    goingToTarget = false;
    fixedSize = false;
    titleColor = 0;
    selected = false;
}

//! Toggle selected status
void swarmMemberDisplay::toggle() {
    selected = !selected;
}

//! Is this a valid point for the member?
bool swarmMemberDisplay::pointValid(int x, int y) {
    return x >= position[0] - DEFAULT_MEMBER_SIZE / 2 && x <= position[0] + DEFAULT_MEMBER_SIZE / 2 &&
        y >= position[1] - DEFAULT_MEMBER_SIZE / 2 && y <= position[1] + DEFAULT_MEMBER_SIZE / 2;
}

//! Display the swarmMember
void swarmMemberDisplay::display() {
    double lastRssi = lastRssiFactor();
    if (goingToTarget) {
        damping = 0.15;
        throttle = 1.0 / (60.0 * MIN(lastRssi, 0.70));
        animateMemberToTarget();
    } else if (goingToOrbit) {
        damping = 0.25;
        throttle = 1.0 / (50.0 * MIN(lastRssi, 0.65));
        setOrbitPosition(targetLocation, NULL);
        animateMemberToTarget();
    } else {
        animateMemberInOrbit();
    }
}

//! Animate `Current' to move from orbit to `targetLocation'
void swarmMemberDisplay::animateMemberToTarget() {
    setAnimationSettings();
    double acceleration[2];
    acceleration[0] = (targetLocation[0] - doublePosition[0]) * throttle; 
    acceleration[1] = (targetLocation[1] - doublePosition[1]) * throttle;   
    double accelerationLength = sqrt(acceleration[0] * acceleration[0] + acceleration[1] * acceleration [1]);
    bool atTarget = (!goingToOrbit && accelerationLength <= 0.0001) || (goingToOrbit && accelerationLength <= 0.07);
    if (!atTarget) {
        velocity[0] = acceleration[0] + velocity[0] - velocity[0] * damping;
        velocity[1] = acceleration[1] + velocity[1] - velocity[1] * damping;
        
        //cout << velocity[0] << ":" << velocity[1] << ":" << acceleration[0] << ":" << acceleration[1] << ":" << position[0] << ":" << position[1] << endl;
        doublePosition[0] = doublePosition[0] + velocity[0];
        doublePosition[1] = doublePosition[1] + velocity[1];
    } else if (goingToOrbit) {
        goingToOrbit = false;
    }
    position[0] = doublePosition[0];
    position[1] = doublePosition[1];
    //ofCircle(position[0], position[1], (int) (currentEnergy * RADIUS_FACTOR + animationStep * RADIUS_FACTOR));
    if (selected) {
        ofSetHexColor(0xAAAAAA);
        ofDrawCircle(position[0], position[1], DEFAULT_MEMBER_SIZE * 1.5);
        setColor();
    }
    ofDrawCircle(position[0], position[1], DEFAULT_MEMBER_SIZE);
    if (goingToTarget && atTarget) {
        if (member->stillInNetwork(ofGetElapsedTimeMillis())) {
            if (titleColor < FS_TITLE_COLOR) {
                titleColor += TITLE_COLOR_INCREMENT;
                titleColor = MIN(titleColor, FS_TITLE_COLOR);
            } else {
                titleColor = FS_TITLE_COLOR;
            }
        } else {
            if (titleColor < FS_MISSING_TITLE_COLOR) {
                titleColor += TITLE_COLOR_MISSING_INCREMENT;
                titleColor = MIN(titleColor, FS_MISSING_TITLE_COLOR);
            } else {
                titleColor = FS_MISSING_TITLE_COLOR;
            }
        }
        ofSetHexColor(titleColor);        
        font.drawString(memberInfoLabel().c_str(), position[0] + 30, position[1]);
    } else if (goingToOrbit && titleColor > 0) {
        titleColor -= TITLE_COLOR_INCREMENT;
        titleColor = MAX(titleColor, 0x000000);
        ofSetHexColor(titleColor);        
        font.drawString(memberInfoLabel().c_str(), position[0] + 30, position[1]);
    }
}

//! Current member battery voltage
float swarmMemberDisplay::batteryVoltage () {
    //if (member->memberType == FS_TYPE_TOWER) {
        return ((float) member->batteryVoltage) / 81.0;
    //} else {
    //    return ((float) member->batteryVoltage) / 1024.0 * (8.0 / (4.1 / 5.0));
    //}
}

string swarmMemberDisplay::memberInfoLabel() {
    stringstream ss;
    ss << "" << setprecision(2) << batteryVoltage() << "V ";
    switch (member->currentPattern) {
        case FS_ID_FULL_COLOR: 
            ss << FS_NAME_FULL_COLOR;
            break;
        case FS_ID_SPARKLE: 
            ss << FS_NAME_SPARKLE;
            break;
        case FS_ID_DESCEND: 
            ss << FS_NAME_DESCEND;
            break;
        case FS_ID_OFF: 
            ss << FS_NAME_OFF;
            break;
        case FS_ID_FLASH: 
            ss << FS_NAME_FLASH;
            break;
        case FS_ID_FIRE: 
            ss << FS_NAME_FIRE;
            break;
        case FS_ID_HEART: 
            ss << FS_NAME_HEART;
            break;
        case FS_ID_BREATHE: 
            ss << FS_NAME_BREATHE;
            break;
        case FS_ID_ORGANIC:
            ss << FS_NAME_ORGANIC;
            break;
        case FS_ID_CYLON: 
            ss << FS_NAME_CYLON;
            break;
        case FS_ID_DROP: 
            ss << FS_NAME_DROP;
            break;
        case FS_ID_CHARACTER: 
            ss << FS_NAME_CHARACTER;
            break;
        case FS_ID_CYLON_VERTICAL: 
            ss << FS_NAME_CYLON_VERTICAL;
            break;
        case FS_ID_CYLON_PONG: 
            ss << FS_NAME_CYLON_PONG;
            break;
        case FS_ID_SOUND_ACTIVATE: 
            ss << FS_NAME_SOUND_ACTIVATE;
            break;
        case FS_ID_PRISM:
            ss << FS_NAME_PRISM;
            break;
        case FS_ID_PRISM_DISTANCE:
            ss << FS_NAME_PRISM_DISTANCE;
            break;
        case FS_ID_MATRIX:
            ss << FS_NAME_MATRIX;
            break;
        case FS_ID_RAINBOW_CHASE:
            ss << FS_NAME_RAINBOW_CHASE;
            break;
        case FS_ID_RANDOM_FLASH:
            ss << FS_NAME_RANDOM_FLASH;
            break;
        case FS_ID_IMAGE_SCROLL:
            ss << FS_NAME_IMAGE_SCROLL;
            break;
        case FS_ID_STARFIELD: 
            ss << FS_NAME_STARFIELD;
            break;
        case FS_ID_SPIRAL:
            ss << FS_NAME_SPIRAL;
            break;
        case FS_ID_TILT:
            ss << FS_NAME_TILT;
            break;
        case FS_ID_SHAKE_SPARKLE:
            ss << FS_NAME_SHAKE_SPARKLE;
            break;
        case FS_ID_SPARKLER:
            ss << FS_NAME_SPARKLER;
            break;
        case FS_ID_GRASS_WAVE:
            ss << FS_NAME_GRASS_WAVE;
            break;
        case FS_ID_RADIO_TOWER:
            ss << FS_NAME_RADIO_TOWER;
            break;
        case FS_ID_BOUNCING_BALL:
            ss << FS_NAME_BOUNCING_BALL;
            break;
        case FS_ID_SPECTRUM_ANALYZER:
            ss << FS_NAME_SPECTRUM_ANALYZER;
            break;
        case FS_ID_FOREST_RUN:
            ss << FS_NAME_FOREST_RUN;
            break;
        case FS_ID_SEARCHING_EYE:
            ss << FS_NAME_SEARCHING_EYE;
            break;
        case FS_ID_BUBBLE_WAVE:
            ss << FS_NAME_BUBBLE_WAVE;
            break;
        case FS_ID_BROKEN:
            ss << FS_NAME_BROKEN;
            break;
        case FS_ID_PONG:
            ss << FS_NAME_PONG;
            break;
        case FS_ID_GIANT_SPECTRUM:
            ss << FS_NAME_GIANT_SPECTRUM;
            break;
        case FS_ID_FLAME:
            ss << FS_NAME_FLAME;
            break;
        case FS_ID_SHAKING:
            ss << FS_NAME_SHAKING;
            break;
        default:
            ss << member->currentPattern;
            break;
    }
    //ss << " -" << (int) member->lastRssiObservation() << "dBm";
    ss << endl;
    ss << (int) member->distance() << "ft";
    //ss << " " << (int) member->frameRate << "Hz ";
    /* Frame count
    char myStr[256];
    sprintf(myStr, "%d.\n", (uint) member->frameCount);
    int numDigits = strchr(myStr, '.') - myStr;
    int i, distance;
    
    for(i=0; myStr[i] != '\n'; i++) {
        if (myStr[i] != '.') ss << myStr[i];
        distance = numDigits - i - 1;
        if (distance > 0 && distance % 3 == 0) ss << ",";
    }
    ss << "frm";
     */
    //ss << " " << (int) member->messageFailures << "err";
    if (member->memberType == FS_TYPE_VEST) {
        ss << " V";
    } else if (member->memberType == FS_TYPE_HAT) {
            ss << " H";
    } else if (member->memberType == FS_TYPE_TOWER) {
            ss << " T";
    } 
    return ss.str();
}

//! Set color according to current battery voltage level
// Colors are colorbrewer.org
void swarmMemberDisplay::setColor() {
    float currentVoltage = batteryVoltage();
    if (currentVoltage < 6.2) {
        ofSetColor(165, 0, 38);
    } else if (currentVoltage < 6.8) {
        ofSetColor(215,48,39);
    } else if (currentVoltage < 7.0) {
        ofSetColor(244,109,67);
    } else if (currentVoltage < 7.05) {
        ofSetColor(253,174,97);
    } else if (currentVoltage < 7.1) {
        ofSetColor(254,224,139);
    } else if (currentVoltage < 7.15) {
        ofSetColor(255,255,191);
    } else if (currentVoltage < 7.4) {
        ofSetColor(217,239,139);
    } else if (currentVoltage < 7.6) {
        ofSetColor(166,217,106);
    } else if (currentVoltage < 7.8) {
        ofSetColor(102,189,99);
    } else if (currentVoltage < 8.0) {
        ofSetColor(26,152,80);
    } else {
        ofSetColor(0,104,55);
    }
}

//! Set the current settings for how to draw `Current'
void swarmMemberDisplay::setAnimationSettings() {
    if (!member->stillInNetwork(ofGetElapsedTimeMillis())) {
        if (newEnergy != 0) {
            newEnergy = 0;
            rssiChangeAnimationPercentage = ANIMATION_PERCENTAGE_INTERVAL;
        }
        ofSetHexColor(MEMBER_DROPPED_COLOR);
    } else {        
        if (fixedSize) {
            newEnergy = 0.3;
        } else if (newEnergy != ((double) (int) (lastRssiFactor() * 100.0) / 100.0)) {
            newEnergy = ((double) (int) (lastRssiFactor() * 100.0) / 100.0);
            rssiChangeAnimationPercentage = ANIMATION_PERCENTAGE_INTERVAL;
        }
        setColor();
    }
    animationStep = (newEnergy - currentEnergy) * rssiChangeAnimationPercentage;
    if (rssiChangeAnimationPercentage < 1.0) {
        rssiChangeAnimationPercentage += ANIMATION_PERCENTAGE_INTERVAL;
    } else if (rssiChangeAnimationPercentage >= 1.0) {
        currentEnergy = newEnergy;
    }
}

//! Set the proposed orbit position and velocity
void swarmMemberDisplay::setOrbitPosition(int oPosition[], double oVelocity[]) {
    theta = theta + currentEnergy * SPIN_FACTOR + animationStep * SPIN_FACTOR;
    int newPositionX = cos(theta / 180.0 * PI) * ellipseRadius * (1.0 - (currentEnergy + animationStep)) + ellipseX;
    int newPositionY = ellipseY - sin(theta / 180.0 * PI) * ellipseRadius * (1.0 - (currentEnergy + animationStep));
    if (newPositionX == ellipseX && newPositionY == ellipseY) {
        // Something's not right
        ++newPositionX;
    }
    if (oVelocity) {
        oVelocity[0] = (newPositionX - oPosition[0]);
        oVelocity[1] = (newPositionY - oPosition[1]);
    }
    oPosition[0] = newPositionX;
    oPosition[1] = newPositionY;
}

//! Animate `Current' in its orbit based on energy
void swarmMemberDisplay::animateMemberInOrbit() {
    setAnimationSettings();
    setOrbitPosition(position, velocity);
    doublePosition[0] = position[0];
    doublePosition[1] = position[1];
    //ofCircle(position[0], position[1], (int) (currentEnergy * RADIUS_FACTOR + animationStep * RADIUS_FACTOR));
    if (selected) {
        ofSetHexColor(0xAAAAAA);
        ofDrawCircle(position[0], position[1], DEFAULT_MEMBER_SIZE * 1.5);
        setColor();
    }
    ofDrawCircle(position[0], position[1], DEFAULT_MEMBER_SIZE);
}

//! Set the target location for `Current' to xTarget, yTarget
void swarmMemberDisplay::moveTo(int xTarget, int yTarget) {
    goingToTarget = true;
    goingToOrbit = false;
    targetLocation[0] = xTarget;
    targetLocation[1] = yTarget;
}

//! Get the member to start moving back into orbit
void swarmMemberDisplay::moveToOrbit() {
    goingToOrbit = true;
    goingToTarget = false;
    fixedSize = false;
}

//! Position the display at the relative distance location
void swarmMemberDisplay::moveToDistance() {
    fixedSize = true;
    moveTo(30, 480 - 90 - member->distance() * 2);
}

//! Does this member have lat/long information available?
bool swarmMemberDisplay::hasLatLong() {
    return member->latitude && member->longitude;
}

//! Display factor for last observed RSSI reading
double swarmMemberDisplay::lastRssiFactor() {
    // RSSI ranges from -26(0x1a)dBm -> -92(0x5c)dBm 
    uint8_t lastRssi = member->lastRssiObservation();
    uint8_t minRssi = 0x1a;
    uint8_t maxRssi = 0x5c;
    double factor = ((double) maxRssi - (double) lastRssi) / ((double) maxRssi - (double) minRssi);
    double inferredDistance = member->distance();
    factor = (130.0 - inferredDistance) / (130.0 - 1.0);
    factor /= 1.5;
    if (factor > 1.0) {
        return 1.0;
    } else if (factor < 0.05) {
        return 0.05;
    } else {
        return factor;
    }
}
