//
//  xBeeControl.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see xBeeControl.h for documentation
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
#include "xBeeControl.h"
#include "TargetConditionals.h"

#define Endian32_Swap(value) \
((((uint32_t)((value) & 0x000000FF)) << 24) | \
(((uint32_t)((value) & 0x0000FF00)) << 8) | \
(((uint32_t)((value) & 0x00FF0000)) >> 8) | \
(((uint32_t)((value) & 0xFF000000)) >> 24))

using Poco::DateTime;

//! Create the control object
xBeeControl::xBeeControl() {
    setupRadio();
    sentCount = 0;
    sendFailures = 0;
    apiFailures = 0;
    timeoutFailures = 0;
    simulatorHearbeatTimestamp = 0;
    simulatorHearbeatTimestampInterval = 5;
    inNodeDiscovery = false;
}

//! Destructor
xBeeControl::~xBeeControl() {
}

//! Connect the radio
void xBeeControl::setupRadio() {
    //  Radio setup
#if (TARGET_IPHONE_SIMULATOR || !TARGET_OS_IPHONE)
    serial.listDevices();
    portName = "/dev/tty.usbserial-AH001572";
    serial.setup(portName, XBEE_BAUD_RATE); 
#else
    portName = "/dev/tty.iap";
    serial.setup(portName, XBEE_BAUD_RATE);
#endif
    xbeeRadio.setSerial(serial);
    /*
    xBeeCoordConfig XBconfiguration;
    XBconfiguration.setRadio (&xbeeRadio);
    bool isConnected = XBconfiguration.connected();

    bool needPersist = false;
    if (!isConnected) {
        for (int i = 0; i < numberOfBaudRates && !isConnected; i++) {
            serial.setup(0, baudRates[i]);
            xbeeRadio.setSerial(serial);
            isConnected = XBconfiguration.connected();
            if (isConnected && baudRates[i] != XBEE_BAUD_RATE) {
                XBconfiguration.setBaudRate(XBEE_BAUD_RATE);
                needPersist = true;
            }
        }        
        serial.setup(0, XBEE_BAUD_RATE);
        xbeeRadio.setSerial(serial);
    }
    if (XBEE_PAN != Endian32_Swap (XBconfiguration.panID()) || needPersist) {
        XBconfiguration.setPanID(Endian32_Swap (XBEE_PAN));
        XBconfiguration.persistConfiguration();
    }

    connected = isConnected;
    */
    connected = true;
}

//! Simulate receipt of collection of clients
void xBeeControl::receiveSimulatedMembers(int timestamp) {
    if (timestamp - simulatorHearbeatTimestamp > simulatorHearbeatTimestampInterval) {
        simulatorHearbeatTimestamp = timestamp;
        receiveMessage *latestMessage;
        uint8_t heartbeatPayload1[] = {
            0x01,       // Byte 0: Message Type ID (1 byte)
            0x11,       // Byte 1: Version ID (1 byte)
            0,          // Byte 2: Frame location (2 bytes)
            0,
            0x10,          // Byte 4: Current Pattern
            0x02,          // Byte 5: Battery Voltage (2 bytes)
            0x02,
            60,         // Byte 7: Frame Rate (1 byte)
            3,          // Byte 8: Member type
            0,          // Byte 9: Failed messages (2 bytes)
            0,
            0x14, 0xA8, 0x39, 0x00, // Byte 11: Latitude (4 bytes)
            0x07, 0x38, 0x45, 0xFF  // Byte 15: Longitude (4 bytes)
        };
        uint8_t heartbeatPayload2[] = {
            0x01,       // Byte 0: Message Type ID (1 byte)
            0x11,       // Byte 1: Version ID (1 byte)
            0,          // Byte 2: Frame location (2 bytes)
            0,
            0x10,       // Byte 4: Current Pattern
            0x02,          // Byte 5: Battery Voltage (2 bytes)
            0x01,
            60,         // Byte 7: Frame Rate (1 byte)
            3,          // Byte 8: Member type
            0,          // Byte 9: Failed messages (2 bytes)
            0,
            0x04, 0xA8, 0x39, 0x00, // Byte 11: Latitude (4 bytes)
            0x18, 0x39, 0x45, 0xFF  // Byte 15: Longitude (4 bytes)
        };
        uint8_t heartbeatPayload3[] = {
            0x01,       // Byte 0: Message Type ID (1 byte)
            0x11,       // Byte 1: Version ID (1 byte)
            0,          // Byte 2: Frame location (2 bytes)
            0,
            0x10,          // Byte 4: Current Pattern
            0x02,          // Byte 5: Battery Voltage (2 bytes)
            0x03,
            60,         // Byte 7: Frame Rate (1 byte)
            3,          // Byte 8: Member type
            0,          // Byte 9: Failed messages (2 bytes)
            0,
            0x14, 0xA8, 0x39, 0x00, // Byte 11: Latitude (4 bytes)
            0x39, 0x39, 0x45, 0xFF  // Byte 15: Longitude (4 bytes)
        };
        uint8_t heartbeatPayload4[] = {
            0x01,       // Byte 0: Message Type ID (1 byte)
            0x11,       // Byte 1: Version ID (1 byte)
            0,          // Byte 2: Frame location (2 bytes)
            0,
            0x10,          // Byte 4: Current Pattern
            0xFF,          // Byte 5: Battery Voltage (2 bytes)
            0x02,
            60,         // Byte 7: Frame Rate (1 byte)
            3,          // Byte 8: Member type
            0,          // Byte 9: Failed messages (2 bytes)
            0,
            0x24, 0xA8, 0x39, 0x00, // Byte 11: Latitude (4 bytes)
            0x39, 0x39, 0x45, 0xFF  // Byte 15: Longitude (4 bytes)
        };
        XBeeAddress64 address64 = XBeeAddress64(123, 123);
        latestMessage = new receiveMessage(1234, address64, heartbeatPayload1, sizeof(heartbeatPayload1), 0x0d);
        receiveQueue.push(*latestMessage);
        address64 = XBeeAddress64(123, 124);
        latestMessage = new receiveMessage(1235, address64, heartbeatPayload2, sizeof(heartbeatPayload2), 0x2b);
        receiveQueue.push(*latestMessage);
        address64 = XBeeAddress64(123, 125);
        latestMessage = new receiveMessage(1236, address64, heartbeatPayload3, sizeof(heartbeatPayload3), 0x10);
        receiveQueue.push(*latestMessage);
        address64 = XBeeAddress64(123, 126);
        latestMessage = new receiveMessage(1237, address64, heartbeatPayload4, sizeof(heartbeatPayload3), 0x10);
        receiveQueue.push(*latestMessage);
    }
}

//! Put `messageToSend' on the queue
void xBeeControl::pushBroadcast(broadcastMessage *messageToSend) {
    if (XBEE_THREADED) {
        lock();
    }
    if (broadcastQueue.size() > XBEE_QUEUE_LIMIT) {
        broadcastQueue.pop();
    }
    broadcastQueue.push (*messageToSend);
    if (XBEE_THREADED) {
        unlock();
    }
    if (!XBEE_THREADED) {
        processBroadcastQueue();
    }
}

//! Count of messages on the broadcast queue
int xBeeControl::broadcastQueueCount() {
    if (XBEE_THREADED) {
        lock();
    }
    int queueSize = broadcastQueue.size();
    if (XBEE_THREADED) {
        unlock();
    }
    return queueSize;
}

//! Process messages on the broadcast queue
void xBeeControl::processBroadcastQueue() {
    ZBTxRequest zbtxRequest;
    int frameId;
    bool sendStatus;
    broadcastMessage *message;
    if (XBEE_THREADED) {
        lock();
    }
    int broadcastSize = broadcastQueue.size();
    if (XBEE_THREADED) {
        unlock();
    }    
    while (broadcastSize > 0) {
        if (XBEE_THREADED) {
            lock();
        }
        message = &broadcastQueue.front();
        broadcastQueue.pop();
        broadcastSize = broadcastQueue.size();
        if (XBEE_THREADED) {
            unlock();
        }
        if (message->requestConfirmation) {
            frameId = 1;
        } else {
            frameId = 0;
        }
        zbtxRequest = ZBTxRequest(message->address64, message->address16, 0, 0, message->payload, message->payloadSize, frameId);
        sendStatus = sendTxRequest(zbtxRequest, frameId != 0);
        if (frameId != 0) {
            for (int i = 0; !sendStatus && i < FS_RESEND_LIMIT; ++i) {
                sendStatus = sendTxRequest(zbtxRequest, frameId != 0);
            }   
            if (FS_DEBUG_ACTIVITY) {
                if (!sendStatus) {  
                    cout << logTimestamp() << "Unable to send" << endl;    
                } else {
                    cout << logTimestamp() << "Sent" << endl;    
                }            
            }
        }
    }
}

//! Proceess an incoming message from the XBee
void xBeeControl::processIncoming() {
#if XBEE_SIMULATED
    receiveSimulatedMembers(time(0));
#else
    if (XBEE_THREADED) {
        lock();
    }
    xbeeRadio.readPacket();
    if (xbeeRadio.getResponse().isAvailable()) {
        int apiId = xbeeRadio.getResponse().getApiId();
        if (apiId == AT_COMMAND_RESPONSE) {
            processATResponse();
        } else if (apiId == ZB_RX_RESPONSE) {
            if (inNodeDiscovery) {
                if (FS_DEBUG_ACTIVITY) {
                    cout << logTimestamp() << "Discarding RX receipt because we are in nodeDiscovery" << endl;
                }
            } else {
                processRXResponse();
            }
        } else {
            if (FS_DEBUG_ACTIVITY) {
                cout << logTimestamp() << "Expected AT/ZB response but got " << (int) xbeeRadio.getResponse().getApiId() << endl;
            }
        }   
    } else {
        if (xbeeRadio.getResponse().isError()) {
            if (FS_DEBUG_ACTIVITY) {
                cout << logTimestamp() << "Error reading packet.  Error code: " << hex << (int) xbeeRadio.getResponse().getErrorCode() << endl;
            }
        } 
    }
    if (XBEE_THREADED) {
        unlock();
    }
#endif
}

//! Process an RXResponse from the XBee
void xBeeControl::processRXResponse() {
    ZBRxResponse rxResponse = ZBRxResponse();
    xbeeRadio.getResponse().getZBRxResponse(rxResponse);
    if (!rxResponse.isError()) {        
        uint8_t *payload = new uint8_t[rxResponse.getDataLength()];
        memcpy(payload, rxResponse.getData(), rxResponse.getDataLength());
        receiveMessage *latestMessage = new receiveMessage(rxResponse.getRemoteAddress16(), rxResponse.getRemoteAddress64(), 
                                                           payload, rxResponse.getDataLength(), lastRssi());
        receiveQueue.push(*latestMessage);
        delete[] payload;
    } else {
        if (FS_DEBUG_ACTIVITY) {
            cout << logTimestamp() << "Problem receiving ZB_RX_RESPONSE!" << endl;
        }
    }        
}

//!  Send a message, return status of send success
bool xBeeControl::sendTxRequest(ZBTxRequest &request, bool expectResponse) {
    ++sentCount;
    xbeeRadio.send(request);
    if (expectResponse) {
        if (xbeeRadio.readPacket(100)) {
            if (xbeeRadio.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
                ZBTxStatusResponse txStatus = ZBTxStatusResponse();
                xbeeRadio.getResponse().getZBTxStatusResponse(txStatus);
                if (txStatus.getDeliveryStatus() == SUCCESS) {
                    if (FS_DEBUG_ACTIVITY) {
                        cout << logTimestamp() << "Successful TX Status response" << endl;
                    }
                    return true;
                } else {
                    ++sendFailures;
                    if (FS_DEBUG_ACTIVITY) {
                        cout << logTimestamp() << "Failed to send message - " << sendFailures << "/" << sentCount << endl;
                    }
                }
            } else if (xbeeRadio.getResponse().isError()) {
                ++apiFailures;
                if (FS_DEBUG_ACTIVITY) {
                    cout << logTimestamp() << "TX Status response `is_error' - " << sendFailures << "/" << sentCount << endl;
                }
            }
        } else {
            ++timeoutFailures;
            if (FS_DEBUG_ACTIVITY) {
                cout << logTimestamp() << "TX Status response timed out - " << sendFailures << "/" << sentCount << endl;        
            }
        }
    } else {
        return true;
    }
    return false;
}

//! Perform a node discovery
void xBeeControl::nodeDiscover() {
    // DB command returns RSSI of last received message
    uint8_t atCommand[2];
    atCommand[0] = 'N';
    atCommand[1] = 'D';
    AtCommandRequest atRequest = AtCommandRequest(atCommand);
    xbeeRadio.send(atRequest);
}

//! Perform a network reset
void xBeeControl::resetNetwork() {
    uint8_t atCommand[3];
    atCommand[0] = 'N';
    atCommand[1] = 'R';
    uint8_t atCommandValues[1];
    atCommandValues[0] = 1;
    AtCommandRequest atRequest = AtCommandRequest(atCommand, atCommandValues, 1);
    xbeeRadio.send(atRequest);
}

//! Set `inNodeDiscovery' to control RX processing
void xBeeControl::setInNodeDiscovery(bool newInNodeDiscovery) {
    inNodeDiscovery = newInNodeDiscovery;
}

//! Determine the RSSI last received message
uint8_t xBeeControl::lastRssi() {
    // DB command returns RSSI of last received message
    uint8_t atCommand[2];
    atCommand[0] = 'D';
    atCommand[1] = 'B';
    AtCommandRequest atRequest = AtCommandRequest(atCommand);
    xbeeRadio.send(atRequest);    
    xbeeRadio.readPacket(20);
    if (xbeeRadio.getResponse().isAvailable()) {
        int apiId = xbeeRadio.getResponse().getApiId();
        if (apiId == AT_COMMAND_RESPONSE) {
            AtCommandResponse atResponse = AtCommandResponse();
            xbeeRadio.getResponse().getAtCommandResponse(atResponse);
            if (atResponse.isOk()) {
                if (atResponse.getValueLength() > 0) {
                    if (FS_DEBUG_ACTIVITY) {
                        cout << logTimestamp() << "RSSI is -" << (int) atResponse.getValue()[0] << "dB" << endl;
                    }
                    return atResponse.getValue()[0];
                } else {
                    if (FS_DEBUG_ACTIVITY) {
                        cout << logTimestamp() << "RSSI request response length > 1" << endl;                    
                    }
                }
            } 
            else {
                if (FS_DEBUG_ACTIVITY) {
                    cout << logTimestamp() << "RSSI request returned error code: " << (int) atResponse.getStatus() << endl;
                }
            }
        }
    }
    return 0;
}

//! Process and display an ATResponse from the XBee
void xBeeControl::processATResponse() {
    AtCommandResponse atResponse = AtCommandResponse();
    xbeeRadio.getResponse().getAtCommandResponse(atResponse);
    if (atResponse.isOk()) {
        if (FS_DEBUG_ACTIVITY) {
            cout << logTimestamp() << "Command [";
            cout << atResponse.getCommand()[0] << atResponse.getCommand()[1] << "] response" << endl;
            if (atResponse.getValueLength() > 0) {
                cout << "\tCommand value length is " << (int) atResponse.getValueLength() << endl;
                cout << "\tCommand value:" ;
                for (int i = 0; i < atResponse.getValueLength(); ++i) {
                    cout << " 0x" << hex << (int) atResponse.getValue()[i];
                }
                cout << dec << endl;
            }
        }
        if (atResponse.getCommand()[0] == 'N' && atResponse.getCommand()[1] == 'D') {
            // Collect incoming Node Discovery responses
            atndResponse *lastNDResponse = new atndResponse(atResponse.getValue(), atResponse.getValueLength());
            //lastNDResponse->setRssi(lastRssi());
            atndQueue.push(*lastNDResponse);
            if (FS_DEBUG_ACTIVITY) {
                cout << logTimestamp() << "Node Identifier: " << lastNDResponse->nodeIdentifier << endl;
            }
        }
    } 
    else {
        if (FS_DEBUG_ACTIVITY) {
            cout << logTimestamp() << "Command return error code: " << (int) atResponse.getStatus() << endl;
        }
    }
}

//! Timestamp for the log
string xBeeControl::logTimestamp() {
    DateTime now;
    stringstream ss;
    if (now.hour() < 10) {
        ss << "0";
    }
    ss << now.hour() << ":";
    if (now.minute() < 10) {
        ss << "0";
    }
    ss << now.minute() << ":";
    if (now.second() < 10) {
        ss << "0";
    }
    ss << now.second();
    if (now.millisecond() < 10) {
        ss << ".00";        
    } else if (now.millisecond() < 100) {
        ss << ".0";
    } else {
        ss << ".";
    }
    ss << now.millisecond();
    ss << " - ";
    return ss.str();
}
