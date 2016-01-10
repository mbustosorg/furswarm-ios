//
//  swarmMember.h
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

#ifndef furSwarmProj_swarmMember_h
#define furSwarmProj_swarmMember_h

#include "furSwarmConst.h"
#include "XBee/XBee.h"
#include <list>

//! Model for a swarm member.
/*!
    This object contains
    details for memory and signal strength observations.
*/
class swarmMember {
	
public: 
    
    swarmMember(uint16_t address16Value, XBeeAddress64 address64Value);
    ~swarmMember();
    
    void setJoinTime(int newJoinTime);
    void setVersionId(int newVersionId);
    void setBatteryVoltage(uint16_t newVoltage);
    void setMessageFailures(uint16_t newMessageFailures);
    void setFrameCount(short newFrameCount);
    void setLastTimestamp(int newLastTimestamp);
    void setCurrentPattern(int newPattern);
    void setFrameRate(int newFrameRate);
    void setMemberType(int newType);
    void setLatLong(float newLatitude, float newLongitude);
    int secondsSinceJoin();
    short lastFrameCountObservation();
    void extendRssi(uint8_t latestRssi);
    uint8_t lastRssiObservation();
    uint8_t lastRssiObservationProportion();
    bool stillInNetwork(int timeStamp);
    double distance();
    
    int joinTime, lastTimestamp;
    int versionId; //! Currently running firmware version id
    int currentPattern;
    uint16_t batteryVoltage;
    uint16_t messageFailures;
    uint16_t frameCount;
    int frameRate;
    uint8_t memberType;
    
    uint16_t address16;
    XBeeAddress64 address64;
    
    int index;
    static int indexCounter;

    float latitude;
    float longitude;
    
private:
    
    std::list<uint8_t> *rssiHistory;
    
};

#endif
