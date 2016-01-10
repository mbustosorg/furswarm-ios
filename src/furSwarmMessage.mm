//
//  furSwarmMessage.mm
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see furSwarmMessage.h for documentation
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
#include "furSwarmMessage.h"
#include "furSwarmPatternConst.h"

//! Initialize object based on message type
furSwarmMessage::furSwarmMessage (uint8_t* data, uint8_t dataLength) {
    messageTypeId = (int) data[0];
    versionId = (int) data[1];
    latitude = 0;
    longitude = 0;
    switch (messageTypeId) {
        case 1: // Full heartbeat
            if (versionId >= 0x10) {
                memcpy (&frameCount, &data[2], sizeof(frameCount));
                memcpy (&currentPattern, &data[4], sizeof(currentPattern));
                memcpy (&batteryVoltage, &data[5], sizeof(batteryVoltage));
                memcpy (&frameRate, &data[7], sizeof(frameRate));
                memcpy (&memberType, &data[8], sizeof(memberType));                
                memcpy (&failedMessageCount, &data[9], sizeof(failedMessageCount));
                if (memberType == FS_TYPE_TOWER && dataLength >= 17) {
                    memcpy (&latitude, &data[11], sizeof(latitude));
                    memcpy (&longitude, &data[15], sizeof(longitude));
                }
            } else {
                memcpy (&frameCount, &data[2], sizeof(frameCount));
                memcpy (&currentPattern, &data[4], sizeof(currentPattern));
                memcpy (&batteryVoltage, &data[6], sizeof(batteryVoltage));
                memcpy (&frameRate, &data[8], sizeof(frameRate));
                memcpy (&memberType, &data[9], sizeof(memberType));
            }
            break;
        case 2: // Short heartbeat
            break;
    }
}