//
//  xBeeControl.h
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

#ifndef furSwarm_xBeeControl_h
#define furSwarm_xBeeControl_h

#include "XBee/XBee.h"
#include "broadcastMessage.h"
#include "receiveMessage.h"
#include "atndResponse.h"
#include "Poco/DateTime.h"
#include "ofThread.h"
#include "ofAppRunner.h"
#include <queue>
#include "xBeeCoordConfig.h"

#define XBEE_THREADED (true)
#define XBEE_SIMULATED (false)

const int FS_FRAME_COUNT = 1;
const int FS_RESEND_LIMIT = 4;
const int XBEE_BAUD_RATE = 57600;
const uint32_t XBEE_PAN = 0x2015;
const int XBEE_QUEUE_LIMIT = 50;

//! XBee controller class
/*!
 *  Control interface for the XBee API.
 *  Provides threading capability for better
 *  processing control.
 */
class xBeeControl : public ofThread {
	
public:
    xBeeControl(); 
    ~xBeeControl();
    
    queue<broadcastMessage> broadcastQueue;
    int currentQueueStep = 0;
    queue<receiveMessage> receiveQueue;    
    queue<atndResponse> atndQueue;    
    
    void processIncoming();
    void pushBroadcast(broadcastMessage *messageToSend);
    void nodeDiscover();
    void resetNetwork();
    string logTimestamp();
    string portName;
    bool bluetoothConnection = false;
    void setInNodeDiscovery(bool newInNodeDiscovery);
    int broadcastQueueCount();
    int sentCount;
    int sendFailures, apiFailures, timeoutFailures;

    //! Thread function implementation
    void threadedFunction() {
        setupRadio();
        while (isThreadRunning()) {
            processIncoming();
            processBroadcastQueue();
        }
    }
    
    bool connected;

private:
    void setupRadio();
    void processBroadcastQueue();
    uint8_t lastRssi();
    void receiveSimulatedMembers(int timestamp);

    queue<receiveMessage> simReceiveQueue;    

    //  XBee Attributes & Commands
    ofSerial	serial;
    XBee        xbeeRadio;
    bool inNodeDiscovery;
    
    int simulatorHearbeatTimestamp, simulatorHearbeatTimestampInterval;
    
    void broadcastPayload(uint8_t payload[], int payloadSize);
    bool sendTxRequest(ZBTxRequest &request, bool expectResponse);
    void processResponse();
    void processTransparentRequest();
    void processATResponse();
    void processRXResponse();
};

#endif