//  animations.h
//
//  Copyright 2013 Paradise Arcade Shop, ParadiseArcadeShop.com  
//  All rights reserved.  Use is subject to license terms.
//
//  Code is provided for entertainment purposes and use with the Kaimana controller.
//  Code may be copied, modified, resused with this Copyright notice.
//  No commercial use without written permission from Paradise Arcade Shop.
//
//  Paradise Arcade Shop Kaimana LED Driver Board
//  Initial Release October 15, 2013
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
//  Kaimana animations based on original source released by ParadiseArcadeShop.com October 15, 2013
//
//  Created:  October 24, 2013    zonbipanda // gmail.com  -- Arduino 1.0.5 Support
//  Revised:  October 29, 2013    zonbipanda // gmail.com
//  Revised:  April   11, 2015    zonbipanda // gmail.com  -- Arduino 1.6.3 Support
//  Revised:  Nov     15, 2023    Paul 'pod' Denning -- Bug fixes, New j4/joystick pcb support, code cleanup, improved combo detection
//  Revised:  Mar     07, 2024    Paul 'pod' Denning -- Added static colour option for idle mode. Added fixed colour option for pressed mode. Added hold Idle colour instead of instant black for non-pressed. Can be tailored to be different for each character.
//

#ifndef __animations_h__
#define __animations_h__

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "kaimana.h"
#include "kaimana_custom.h"
#include "Characters.h"

extern Kaimana kaimana;

#define IDLE_SIZE           768    // size of animation array
#define IDLE_OFFSET_2       512    // used to create animation -- see code
#define IDLE_OFFSET_1       256    // used to create animation -- see code
#define IDLE_OFFSET_0         0    // used to create animation -- see code
#define IDLE_OFFSET          12    // used to create animation -- see code

enum EWaveSpeed
{
  EFS_Slow,
  EFS_Medium,
  EFS_Fast,
};

enum EWaveType
{
  EFT_LeftToRight,
  EFT_LeftToRightPunchOnly,
  EFT_LeftToRightKickOnly,
  EFT_RightToLeft,
  EFT_RightToLeftPunchOnly,
  EFT_RightToLeftKickOnly,
  EFT_DownToUp,
  EFT_UpToDown,
};

#define FIREBALL_SIZE             768    // size of animation array
#define FIREBALL_OFFSET_3         288    // used to create animation -- see code
#define FIREBALL_OFFSET_2         192    // used to create animation -- see code
#define FIREBALL_OFFSET_1         96     // used to create animation -- see code
#define FIREBALL_DELAY            350    // value in microseconds
#define FIREBALL_LOOP_START       512    // For supers we want to strobe the fireball a bit. Begin loop at this Frame in the fireball table
#define FIREBALL_LOOP_FRAMES      64     // For supers we want to strobe the fireball a bit. loop for this many frames
#define FIREBALL_LOOP_STROBE_LOW  0.5f   // For supers we want to strobe the fireball a bit. at the low intensity multiply by this
#define FIREBALL_SPEED_SLOW   2
#define FIREBALL_SPEED_MEDIUM 4
#define FIREBALL_SPEED_FAST   6

// WaveButton structure.
typedef struct __attribute__ ((__packed__)) {
    int Button;
    int Group; //Used to group buttons together. EG if P1 and K2 are both Group 0 then they will both light up at the same time. Must not be bigger than 4 - see WaveEffect_Combo_Animation() definition
} WaveButtonOrder;

#define FIREBALL_MAX_GROUPS   4
#define FIREBALL_MAX_BUTTONS  8

void WaveEffect_Combo_Animation(EWaveType WaveDirection, EWaveSpeed AnimSpeed, int loops, int Col_R, int Col_G, int Col_B);    // Fireball effect - A wave of colour from one side of the buttons to the other
void FlashColour_Combo_Animation(int Col_R, int Col_G, int Col_B, int FlashTime);       // Light all buttons one colour for FlashTime in ms
void CircleRGB_Combo_Animation(int loops);                                              //Start from K and go anti-clockwise cycling through RGB
void Circle_OneColour_Combo_Animation(int loops, int R, int G, int B);                  //Start from K and go anti-clockwise cycling through One colour
void FlashAllSpeedIncreasing_Combo_Animation(int R, int G, int B);                      // Flash All a colour, increasing in speed
void KnightRider_Combo_Animation(int loops, bool bTopRow, int Col_R, int Col_G, int Col_B); //strobe left to right and back
void Randomise_Combo_Animation(int NumFlash, int TimeLit, int DelayBetween,int R, int G, int B); //randomly set a led for a specific time

// other function prototypes

int animation_idle(const Character* currentCharacter);
void showStartup(void);
void defaultStartup(void);
void tourneyModeActivate(void);
void tourneyModeDeactivate(void);

#endif
