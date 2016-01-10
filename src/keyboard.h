//
//  keyboard.h
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

#ifndef keyboard_h
#define keyboard_h

#include "ofButton.h"
#include <map>
#include <list>

const int C_COL_INCREMENT = 29;
const int C_ROW_INCREMENT = 40;
const int C_KEY_PADDING = 3;

const int OUTER_MIN_X = 10;
const int OUTER_MAX_X = 310;
const int OUTER_MAX_Y = 220;
const int OUTER_MIN_Y = 10; 
const int TEXT_BOX_OFFSET = 5;
const int TEXT_BOX_HEIGHT = 50;
const int CURSOR_TIMEOUT = 500;

class keyboard  {
	
public:
	
    keyboard(bool centeredText);
    
    void draw();
    void setupKey(int row, int col, const char* character);
    void pressIfValid(int x, int y);
    void release();
    void setMaxEntryCount(int count);
    const char* selectedText();
    void changeY(int deltaY);
    void setActive(bool activeStatus);
    void setProcessed(bool processedStatus);
    void setRotationSpeed(int newSpeed);
    bool needsProcessing();
    
    bool active;
    bool processed;
    
private:
    map<const char *, ofButton> keyBoardKeys;
    list<ofButton*> keyedText;
    ofButton *spaceButton;
    ofButton *deleteButton;
    ofButton *enterButton;
    ofButton *rotateButton;
    
    bool isActive;
    
    int cursorTimestamp;
    bool cursorOn;
    int rotationSpeed;
    bool rotating;
    int rotationPosition;
    int rotationTimestamp;
    int maxEntryCount;
    int y;
};

#endif
