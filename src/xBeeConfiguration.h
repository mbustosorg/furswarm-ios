//
//  xBeeConfiguration.h
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

#ifndef xBeeConfiguration_h
#define xBeeConfiguration_h

#include "XBee/XBee.h"

const uint32_t baudRates[] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
const uint8_t numberOfBaudRates = 8;
const int c_sendAndGetAckWait = 500;

//! XBee configuration class
/*! 
 *  Class to facilitate programmatic
 *  configuration of XBee radios.
 */
class xBeeConfiguration {
	
public:
    xBeeConfiguration(); 
    ~xBeeConfiguration();

    void setRadio(XBee* anXbeeRadio); 

    bool connected();
    bool persistConfiguration();
    
    uint32_t panID();
    uint32_t baudRate();

    bool setPanID(uint32_t newPanID);
    bool setBaudRate(uint32_t newBaudRate);
    
private:
    uint8_t indexForBaudRate(uint32_t desiredRate);
    bool sendAndGetAck(AtCommandRequest atRequest);
    virtual void outputMessage(const char * newMessage, bool withEndl);
    virtual void outputInteger(int newInt, bool withEndl);
    
    //  XBee Attributes & Commands
    XBee*        xbeeRadio;
};

#endif
