//
//  keyboard.cpp
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Please see keyboard.h for documentation
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

#include "keyboard.h"
#include "ofUtils.h"

//! Create the keyboard object
keyboard::keyboard(bool centeredText){
	ofSetLineWidth(10);
    
    cursorTimestamp = 0;
    cursorOn = false;
    setupKey(1, 1, "Q");
    setupKey(1, 2, "W");
    setupKey(1, 3, "E");
    setupKey(1, 4, "R");
    setupKey(1, 5, "T");
    setupKey(1, 6, "Y");
    setupKey(1, 7, "U");
    setupKey(1, 8, "I");
    setupKey(1, 9, "O");
    setupKey(1, 10, "P");
    
    setupKey(2, 2, "A");
    setupKey(2, 3, "S");
    setupKey(2, 4, "D");
    setupKey(2, 5, "F");
    setupKey(2, 6, "G");
    setupKey(2, 7, "H");
    setupKey(2, 8, "J");
    setupKey(2, 9, "K");
    setupKey(2, 10, "L");
    
    setupKey(3, 3, "Z");
    setupKey(3, 4, "X");
    setupKey(3, 5, "C");
    setupKey(3, 6, "V");
    setupKey(3, 7, "B");
    setupKey(3, 8, "N");
    setupKey(3, 9, "M");

    deleteButton = new ofButton(48, C_ROW_INCREMENT * 3 + OUTER_MIN_Y + TEXT_BOX_HEIGHT + TEXT_BOX_OFFSET + C_ROW_INCREMENT / 2.5,
                                C_ROW_INCREMENT / 1.5, 63, 0x806080, "delete");
    deleteButton->setFontSize(14);
    spaceButton = new ofButton(123, C_ROW_INCREMENT * 3 + OUTER_MIN_Y + TEXT_BOX_HEIGHT + TEXT_BOX_OFFSET + C_ROW_INCREMENT / 2.5,
                               C_ROW_INCREMENT / 1.5, 63, 0x806080, "space");
    spaceButton->setFontSize(14);
    rotateButton = new ofButton(198, C_ROW_INCREMENT * 3 + OUTER_MIN_Y + TEXT_BOX_HEIGHT + TEXT_BOX_OFFSET + C_ROW_INCREMENT / 2.5,
                                  C_ROW_INCREMENT / 1.5, 63, 0x806080, "rotate");
    rotateButton->setFontSize(14);
    enterButton = new ofButton(273, C_ROW_INCREMENT * 3 + OUTER_MIN_Y + TEXT_BOX_HEIGHT + TEXT_BOX_OFFSET + C_ROW_INCREMENT / 2.5,
                               C_ROW_INCREMENT / 1.5, 63, 0x806080, "enter");
    enterButton->setFontSize(14);
    
    maxEntryCount = 10;
    rotationSpeed = 5000;
    rotationPosition = 0;
    rotating = false;
    y = 0;
    changeY(-OUTER_MAX_Y - 20);
}

//! Draw the keyboard
void keyboard::draw(){
    ofSetLineWidth(2);
    ofSetHexColor(0x909090);
    // Draw enclosing box
    ofDrawLine (OUTER_MIN_X, OUTER_MIN_Y + y, OUTER_MAX_X, OUTER_MIN_Y + y);
    ofDrawLine (OUTER_MAX_X, OUTER_MIN_Y + y, OUTER_MAX_X, OUTER_MAX_Y + y);
    ofDrawLine (OUTER_MAX_X, OUTER_MAX_Y + y, OUTER_MIN_X, OUTER_MAX_Y + y);
    ofDrawLine (OUTER_MIN_X, OUTER_MAX_Y + y, OUTER_MIN_X, OUTER_MIN_Y + y);
    
    // Draw text entry box
    ofDrawLine (OUTER_MIN_X + TEXT_BOX_OFFSET, OUTER_MIN_Y + TEXT_BOX_OFFSET + y, OUTER_MAX_X - TEXT_BOX_OFFSET, OUTER_MIN_Y + TEXT_BOX_OFFSET + y);
    ofDrawLine (OUTER_MAX_X - TEXT_BOX_OFFSET, OUTER_MIN_Y + TEXT_BOX_OFFSET + y, OUTER_MAX_X - TEXT_BOX_OFFSET, OUTER_MIN_Y + TEXT_BOX_HEIGHT + y);
    ofDrawLine (OUTER_MAX_X - TEXT_BOX_OFFSET, OUTER_MIN_Y + TEXT_BOX_HEIGHT + y, OUTER_MIN_X + TEXT_BOX_OFFSET, OUTER_MIN_Y + TEXT_BOX_HEIGHT + y);
    ofDrawLine (OUTER_MIN_X + TEXT_BOX_OFFSET, OUTER_MIN_Y + TEXT_BOX_OFFSET + y, OUTER_MIN_X + TEXT_BOX_OFFSET, OUTER_MIN_Y + TEXT_BOX_HEIGHT + y);
    
    if (ofGetElapsedTimeMillis() - cursorTimestamp > CURSOR_TIMEOUT) {
        cursorOn = !cursorOn;
        cursorTimestamp = ofGetElapsedTimeMillis();
    }
    if (cursorOn) {
        ofDrawLine (OUTER_MIN_X + TEXT_BOX_OFFSET + 3 + keyedText.size() * C_COL_INCREMENT,
                    OUTER_MIN_Y + TEXT_BOX_HEIGHT - 3 + y,
                    OUTER_MIN_X + TEXT_BOX_OFFSET + 3 + keyedText.size() * C_COL_INCREMENT + C_COL_INCREMENT,
                    OUTER_MIN_Y + TEXT_BOX_HEIGHT - 3 + y);
    }
    
    for (map<const char*, ofButton>::iterator it = keyBoardKeys.begin(); it != keyBoardKeys.end(); ++it) {
        (*it).second.draw();
    }    
    list<ofButton*>::iterator keyedTextIt;
    for(keyedTextIt = keyedText.begin(); keyedTextIt != keyedText.end(); ++keyedTextIt)
    {
        (*keyedTextIt)->draw();
    }
    deleteButton->draw();
    spaceButton->draw();
    rotateButton->draw();
    enterButton->draw();
}

//! The selected keyed text
const char* keyboard::selectedText() {
    string text;
    list<ofButton*>::iterator keyedTextIt;
    int index = 0;
    for(keyedTextIt = keyedText.begin(); keyedTextIt != keyedText.end(); ++keyedTextIt)
    {
        if (rotating) {
            if (index < rotationPosition) {
                text.append((*keyedTextIt)->label);
            } else {
                text.insert(index - rotationPosition, (*keyedTextIt)->label);                
            }
            ++index;
        } else {
            text.append((*keyedTextIt)->label);
        }
    }    
    cout << text << endl;
    return text.c_str();
}

//! Does the keyboard have data to be procesed?
bool keyboard::needsProcessing() {
    if (rotating) {
        if (ofGetElapsedTimeMillis() - rotationTimestamp > rotationSpeed) {
            rotationTimestamp = ofGetElapsedTimeMillis();
            rotationPosition++;
            if (rotationPosition >= keyedText.size()) {
                rotationPosition = 0;
            }
            return true;
        } else {
            return false;
        }
    } else {
        return !processed;
    }
}

//! Set `active' to `activeStatus', this is used to control drawing
void keyboard::setActive(bool activeStatus) {
    active = activeStatus;
}

//! Set `processed' to `processedStatus'
void keyboard::setProcessed(bool processedStatus) {
    processed = processedStatus;
}

//! Set `rotationSpeed' to `newSpeed'
void keyboard::setRotationSpeed(int newSpeed) {
    rotationSpeed = newSpeed * 3000 / 255;
    if (rotationSpeed < 100) {
        rotationSpeed = 100;
    }
}

//! Add `deltaY' to `y'
void keyboard::changeY(int deltaY) {
    y += deltaY;
    list<ofButton*>::iterator keyedTextIt;
    for(keyedTextIt = keyedText.begin(); keyedTextIt != keyedText.end(); ++keyedTextIt)
    {
        (*keyedTextIt)->changeY(deltaY);
    }    
    for (map<const char*, ofButton>::iterator it = keyBoardKeys.begin(); it != keyBoardKeys.end(); ++it) {
        (*it).second.changeY(deltaY);
    }            
    deleteButton->changeY(deltaY);
    spaceButton->changeY(deltaY);
    rotateButton->changeY(deltaY);
    enterButton->changeY(deltaY);
}

//! Press the key located by `x' and `y'
void keyboard::pressIfValid(int buttonX, int buttonY) {
    ofButton *currentButton;
    int x1, y1;
    x1 = OUTER_MIN_X + TEXT_BOX_OFFSET + 3 + keyedText.size() * C_COL_INCREMENT + C_COL_INCREMENT / 2;
    y1 = OUTER_MIN_Y + TEXT_BOX_HEIGHT - 3 - C_ROW_INCREMENT / 2 + y; 
    if (maxEntryCount > keyedText.size()) {
        for (map<const char*, ofButton>::iterator it = keyBoardKeys.begin(); it != keyBoardKeys.end(); ++it) {
            currentButton = &(*it).second;
            if (currentButton->pointValid(buttonX, buttonY)) {
                currentButton->press();
                ofButton* keyedButton = new ofButton(x1, y1, C_ROW_INCREMENT - 3, C_COL_INCREMENT - 3, 0x8080E0, currentButton->label);
                keyedButton->setFontSize(14);
                keyedText.push_back(keyedButton);
                processed = false;
            }
        }        
    }
    if (deleteButton->pointValid(buttonX, buttonY)) {
        deleteButton->press();
        keyedText.pop_back();
        processed = false;
    } else if (spaceButton->pointValid(buttonX, buttonY)) {
        spaceButton->press();
        ofButton* keyedButton = new ofButton(x1, y1, C_ROW_INCREMENT - 3, C_COL_INCREMENT - 3, 0x8080E0, " ");
        keyedText.push_back(keyedButton);
        processed = false;
    } else if (rotateButton->pointValid(buttonX, buttonY)) {
        rotateButton->press();
        rotating = not rotating;
        rotationTimestamp = ofGetElapsedTimeMillis();
    } else if (enterButton->pointValid(buttonX, buttonY)) {
        enterButton->press();
        processed = false;
    }
    cout << selectedText() << endl;
}

//! Set the maximum number of entries allowed
void keyboard::setMaxEntryCount(int count) {
    maxEntryCount = count;
}

//! Release all keys
void keyboard::release() {
    for (map<const char*, ofButton>::iterator it = keyBoardKeys.begin(); it != keyBoardKeys.end(); ++it) {
        (*it).second.release();
    }            
    deleteButton->release();
    spaceButton->release();
    rotateButton->release();
    enterButton->release();
}

//! Initialize key `character'
void keyboard::setupKey(int row, int col, const char* character)
{
    int x1, x2, y1;
    int rowOffset;
    x1 = C_COL_INCREMENT * col;
    x2 = C_COL_INCREMENT * col + C_COL_INCREMENT - C_KEY_PADDING;
    rowOffset = (row - 1) * (x2 - x1) / 2;
    x1 -= rowOffset;
    y1 = C_ROW_INCREMENT * (row - 1) + OUTER_MIN_Y + TEXT_BOX_HEIGHT + TEXT_BOX_OFFSET + C_ROW_INCREMENT / 2;
    
    ofButton* keyButton = new ofButton(x1, y1, C_ROW_INCREMENT - 3, C_COL_INCREMENT - 3, 0x808080, character);
    keyButton->setFontSize(14);
    keyBoardKeys.insert(std::pair<const char *, ofButton>(character, *keyButton));

}
