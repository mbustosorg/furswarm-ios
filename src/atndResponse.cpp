//
//  atndResponse.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see atndResponse.h for documentation
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
#include "atndResponse.h"

//! Create message ATND response object
atndResponse::atndResponse(uint8_t *newPayload, int newPayloadLength) {
    
    address16 = (uint16_t)((newPayload[0] << 8) + newPayload[1]);
    address64 = XBeeAddress64((uint32_t)((newPayload[2] << 24) + (newPayload[3] << 16) + (newPayload[4] << 8) + newPayload[5]),
                              (uint32_t)((newPayload[6] << 24) + (newPayload[7] << 16) + (newPayload[8] << 8) + newPayload[9]));

    int i = niStartByte;
    for (; newPayload[i] != 0; ++i) {
        nodeIdentifier[i - niStartByte] = (char) newPayload[i];
    }
    for (; i < niStartByte + niLength; ++i) {
        nodeIdentifier[i - niStartByte] = 0;
    }    
}

//! Destructor
atndResponse::~atndResponse() {
}

//! Set the RSSI value for this command
void atndResponse::setRssi(int newRssi) {
    rssi = newRssi;
}