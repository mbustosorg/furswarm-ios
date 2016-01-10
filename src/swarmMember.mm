//
//  swarmMember.mm
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see swarmMember.h for documentation
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
#include "swarmMember.h"

int swarmMember::indexCounter = 0;

//! Initialize the object and create history containers
swarmMember::swarmMember(uint16_t address16Value, XBeeAddress64 address64Value) {
    address16 = address16Value;
    address64 = address64Value;
    joinTime = 0;
    versionId = 0;
    frameCount = 0;
    lastTimestamp = 0;
    rssiHistory = new list<uint8_t>;
    indexCounter++;
    index = indexCounter;
}

//!  Destructor, clean up
swarmMember::~swarmMember() {
    delete rssiHistory;
}

//! Set the member's swarm join time to `newJoinTime'
void swarmMember::setJoinTime(int newJoinTime) {
    joinTime = newJoinTime;
    if (lastTimestamp < joinTime) {
        setLastTimestamp(joinTime);
    }
}

//! Set the member's firmware versionId to `newVersionId'
void swarmMember::setVersionId(int newVersionId) {
    versionId = newVersionId;
}

//! Set the member's current battery voltage to `newVoltage'
void swarmMember::setBatteryVoltage(uint16_t newVoltage) {
    batteryVoltage = newVoltage;
}

//! Set the member's current message failures count to `newMessageFailures'
void swarmMember::setMessageFailures(uint16_t newMessageFailures) {
    messageFailures = newMessageFailures;
}

//! Set the member's current frame rate to `newFrameRate'
void swarmMember::setFrameRate(int newFrameRate) {
    frameRate = newFrameRate;
}

//! Set the member type to `newType'
void swarmMember::setMemberType(int newType) {
    memberType = newType;
}

//! Set the member GPS position
void swarmMember::setLatLong(float newLatitude, float newLongitude) {
    latitude = newLatitude;
    longitude = newLongitude;
}

//! Set the member's lastTimestamp to `newLastTimestamp'
/*!
    Update the swarm join time if this is newer than the current `joinTime'
 */
void swarmMember::setLastTimestamp(int newLastTimestamp) {
    lastTimestamp = newLastTimestamp;
    if (newLastTimestamp < joinTime) {
        setJoinTime(lastTimestamp);
    }
}

//! Count of seconds since this member has joined the swarm
int swarmMember::secondsSinceJoin() {
    int life, lifeMod;
    life = lastTimestamp - joinTime;
    lifeMod = life % 1000;
    return ((life - lifeMod) / 1000);
}

//! Register an available RAM observation and add it to the history
void swarmMember::setFrameCount(short newFrameCount) {
    frameCount = newFrameCount;
}

//! Set the currently running pattern
void swarmMember::setCurrentPattern(int newPattern) {
    currentPattern = newPattern;
}

//! Register a signal strength observation and extend the history
void swarmMember::extendRssi(uint8_t latestRssi) {
    rssiHistory->push_front(latestRssi);
    if (rssiHistory->size() > HISTORY_COUNT) {
        rssiHistory->pop_back();
    }
}

//! The last observed RSSI observation
uint8_t swarmMember::lastRssiObservation() {
    return rssiHistory->front();
}

//! Inferred distance (ft) based on RSSI value (emperically determined)
double swarmMember::distance() {
    uint8_t lastRssi = lastRssiObservation();
    double inferredDistance = pow(10.0, 100.0 / ((double) -lastRssi + 130) - log10(4.0));
    
    if (inferredDistance < 1.0) {
        return 1.0;
    } else {
        return inferredDistance;
    }    
}

//! The last observed RSSI observation as a proportion of the range
uint8_t swarmMember::lastRssiObservationProportion() {
    // RSSI ranges from -26(0x1a)dBm -> -92(0x5c)dBm 
    // For good color variable we are doing -40bBm -> -90dBm
    // and scaling it to a range of 0 -> 255 * 300 / 360 (Violet)
    double inferredDistance = distance() * 2.0;
    if (inferredDistance > 255) {
        return 255;
    } else if (inferredDistance < 1) {
        return 1;
    } else {
        return inferredDistance;
    }
}

//! Is the member still in the network?
bool swarmMember::stillInNetwork(int timeStamp) {
    return (timeStamp - lastTimestamp) < FS_CONSIDERED_DROPPED;
}