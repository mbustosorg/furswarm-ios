//
//  furSwarmConst.h
//
//  $Date$
//  $Rev$
//  $Author$
//
//  Description: System constants for the furSwarm system
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

#ifndef furSwarmProj_furSwarmConst_h
#define furSwarmProj_furSwarmConst_h

#define FS_HEARTBEAT (0x01)
#define FS_HEARTBEAT_VERSION_BYTE (0x02)

#define FS_HEARTBEAT_REQUEST (0x02)

#define HISTORY_COUNT (10) //! Upper count of observations (eg. RSSI or memory) available for display 
#define FS_CONSIDERED_DROPPED (30000) //! Milliseconds since the last timestamp when we consider you out of network

#define FS_APP_BACKGROUND (0x404040)
#define FS_APP_BACKGROUND_R (40)
#define FS_APP_BACKGROUND_G (40)
#define FS_APP_BACKGROUND_B (40)

#define FS_TITLE_COLOR (0xFF0000)
#define FS_MISSING_TITLE_COLOR (0xA0A0A0)
#define FS_DATA_COLOR (0xFF55FF)

#define FS_DEBUG_INTERACTION (TARGET_IPHONE_SIMULATOR || !TARGET_OS_IPHONE)
#define FS_DEBUG_ACTIVITY (TARGET_IPHONE_SIMULATOR || !TARGET_OS_IPHONE)
#define FS_DEBUG_ACTION (TARGET_IPHONE_SIMULATOR || !TARGET_OS_IPHONE)

#define MAIN_BUTTON_X (160)
#define MAIN_BUTTON_Y (180)
#define MAIN_BUTTON_RADIUS (30)
#define MEMBER_RADIUS (160)

#define FS_ELLIPSE_BUTTON (1)
#define FS_RECT_BUTTON (2)

#define MEMBER_INITIAL_COLOR (0x205080)
#define MEMBER_COLOR_INCREMENT (0x202020)
#define MEMBER_DROPPED_COLOR (0x404040)

#define FS_COLOR_FULL_COLOR (0x000000)
#define FS_COLOR_SPARKLE (0xff0000)
#define FS_COLOR_DESCEND (0x00ff00)
#define FS_COLOR_OFF (0xff9000)
#define FS_COLOR_FLASH (0x0000ff)
#define FS_COLOR_FIRE (0xff009f)
#define FS_COLOR_HEART (0xff0000)
#define FS_COLOR_BREATHE (0x00a0a0)
#define FS_COLOR_ORGANIC (0xffffff)
#define FS_COLOR_CYLON (0xff90ff)
#define FS_COLOR_DROP (0x00faf5)
#define FS_COLOR_CHARACTER (0x800a05)
#define FS_COLOR_CYLON_VERTICAL (0x808080)
#define FS_COLOR_CYLON_PONG (0x8080FF)
#define FS_COLOR_BREATHE_EVOLVE (0x80FF80)
#define FS_COLOR_WORD (0xFF8080)
#define FS_COLOR_SOUND_ACTIVATE (0xFFFF80)
#define FS_COLOR_ANIMATE_1 (0xff00ff)
#define FS_COLOR_PRISM (0x909090)
#define FS_COLOR_PRISM_DISTANCE (0x90FF90)
#define FS_COLOR_MATRIX (0x00FF00)
#define FS_COLOR_RAINBOW_CHASE (0xAAFFAA)
#define FS_COLOR_RANDOM_FLASH (0xFFFFAA)
#define FS_COLOR_IMAGE_SCROLL (0xFFFF00)
#define FS_COLOR_STARFIELD (0xffffff)
#define FS_COLOR_SPIRAL (0xff00ff)
#define FS_COLOR_GRASS_WAVE (0xff00ff)
#define FS_COLOR_SPARKLER (0xffff00)
#define FS_COLOR_TILT (0xff00ff)
#define FS_COLOR_RADIO_TOWER (0x654321)
#define FS_COLOR_BOUNCING_BALL (0x234567)
#define FS_COLOR_SPECTRUM_ANALYZER (0x654234)
#define FS_COLOR_FOREST_RUN (0x123765)
#define FS_COLOR_SEARCHING_EYE (0x62A4A6)
#define FS_COLOR_BUBBLE_WAVE (0xA7A321)
#define FS_COLOR_BROKEN (0xEEEEEE)
#define FS_COLOR_PONG (0x00EE00)
#define FS_COLOR_GIANT_SPECTRUM (0x00EEEE)
#define FS_COLOR_FLAME (0x0000EE)
#define FS_COLOR_CANDLE (0xEE00EE)
#define FS_COLOR_POOF_1 (0xAAAAAA)
#define FS_COLOR_POOF_2 (0xAAAAAA)

#define FS_SLIDER_HEIGHT (10)
#define FS_SLIDER_WIDTH (175)

#define FS_ROW_1 (40)
#define FS_ROW_2 (80)
#define FS_ROW_3 (120)
#define FS_ROW_4 (160)
#define FS_ROW_5 (200)
#define FS_ROW_6 (240)
#define FS_ROW_7 (280)
#define FS_ROW_8 (320)
#define FS_ROW_9 (360)
#define FS_ROW_10 (400)
#define FS_ROW_11 (440)
#define FS_COL_1 (40)
#define FS_COL_2 (120)
#define FS_COL_3 (200)
#define FS_COL_4 (280)
#define FS_CB_HEIGHT (30)
#define FS_CB_WIDTH (60)

#define FS_BREATHE_DELAY (500)

#endif
