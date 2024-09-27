//  kaimana.cpp
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
//  Revised:  Oct     25, 2024    Paul 'pod' Denning -- Fixed 2 overflow errors, fixed incorrect idle exit when not using leds on all buttons. Added Terry and Akuma characters
//

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "kaimana.h"
#include "kaimana_custom.h"
#include "animations.h"
#include "Characters.h"

void defaultStartup(void)
{
  kaimana.setALL( BLACK );
  delay( FAST_COLOR_DELAY );
  kaimana.setALL( RED );
  delay( FAST_COLOR_DELAY );
  kaimana.setALL( BLACK );
  delay( FAST_COLOR_DELAY );
  kaimana.setALL( GREEN );
  delay( FAST_COLOR_DELAY );
  kaimana.setALL( BLACK );
  delay( FAST_COLOR_DELAY );
  kaimana.setALL( BLUE );
  delay( FAST_COLOR_DELAY );
  kaimana.setALL( BLACK );
  delay( BOOT_COMPLETE_DELAY );  
} 

// Color Fade Animation when Idle
//
int animation_idle(const Character* currentCharacter)
{
  int  index;
  int  i;

  // set initial color to BLACK
  if(currentCharacter->turnNonHeldButtonsOff() || currentCharacter->useStaticColourInIdle() == false)
  {
    kaimana.setALL(BLACK);
  }

  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update

  while(true)
  {
    for(index=0;index<IDLE_SIZE;++index)
    {
      // update strip with new color
      for(i=0;i<LED_COUNT;++i)
      {
        if(currentCharacter->useStaticColourInIdle())
        {
          RGB_t thisCol = currentCharacter->idleStaticColour();
          kaimana.setIndividualLED(i, thisCol.r, thisCol.g, thisCol.b);    
        }
        else //cycle rgb
        {
          kaimana.setIndividualLED(
            i,
            pgm_read_byte_near(&colorCycleData[((index+IDLE_OFFSET_2+((LED_COUNT-i)*IDLE_OFFSET))%IDLE_SIZE)]),
            pgm_read_byte_near(&colorCycleData[((index+IDLE_OFFSET_1+((LED_COUNT-i)*IDLE_OFFSET))%IDLE_SIZE)]),
            pgm_read_byte_near(&colorCycleData[((index+IDLE_OFFSET_0+((LED_COUNT-i)*IDLE_OFFSET))%IDLE_SIZE)])
          );
        }
      }

      // update the leds with new/current colors in the array
      kaimana.updateALL();

      // test all switches and exit idle animation if active switch found
      for(i=0; i < LED_ENTRIES; ++i)
      {
        if( digitalRead(switchListForIdleExit[i]) == BUTTON_READ_CHECK)
          return(false);
      }

      // place test for switches here and use calculated timer not delay
      //
      delay( IDLE_ANIMATION_DELAY );
    }
  }
}

//############ TOURNAMENT MODE ANIMATIONS
void tourneyModeActivate(void)
{
	kaimana.setALL(RED);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K4, BLACK);
	kaimana.setLED(LED_P4, BLACK);
	kaimana.updateALL();
	delay( T_DELAY );	
	
	kaimana.setLED(LED_K3, BLACK);
	kaimana.setLED(LED_P3, BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K2, BLACK);
	kaimana.setLED(LED_P2, BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K1, BLACK);
	kaimana.setLED(LED_P1, BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
}

void tourneyModeDeactivate(void)
{
	kaimana.setALL(BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K1, RED);
	kaimana.setLED(LED_P1, RED);
	kaimana.updateALL();
	delay( T_DELAY );
		
	kaimana.setLED(LED_K2, RED);
	kaimana.setLED(LED_P2, RED);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K3, RED);
	kaimana.setLED(LED_P3, RED);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K4, RED );
	kaimana.setLED(LED_P4, RED);
	kaimana.updateALL();
	delay( T_DELAY );	

	kaimana.setALL(BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setALL(RED);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setALL(BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
}

//-----------------------------------------------------------------------------------
// Hadouken (Fireball) - A wave of colour from one side of the buttons to the other
//-----------------------------------------------------------------------------------
void WaveEffect_SetButton(int Counter, int Button, int Col_R, int Col_G, int Col_B)
{
  if( ((Counter) < FIREBALL_SIZE) && ((Counter) >= 0) )
  {
    kaimana.setLED(
      Button,
      (pgm_read_byte_near(&colorCycleData[(Counter)%FIREBALL_SIZE]) * Col_R) / 255,
      (pgm_read_byte_near(&colorCycleData[(Counter)%FIREBALL_SIZE]) * Col_G) / 255,
      (pgm_read_byte_near(&colorCycleData[(Counter)%FIREBALL_SIZE]) * Col_B) / 255
    );
  }
  else
  {
    kaimana.setLED( Button, BLACK );
  }
}

float UpdateWaveLoop(int& loopsLeft, int& counter, int& loopFrames)
{
  if(loopsLeft > 0 && counter < FIREBALL_LOOP_START)
  {
    loopFrames++;
    if(loopFrames >= FIREBALL_LOOP_FRAMES)
    {
      loopFrames = 0;
      loopsLeft--;
    }
    float loopValInRads = 6.2831 * (((float)loopFrames) / ((float)FIREBALL_LOOP_FRAMES));
    float cosResult = ((1.0f+cos(loopValInRads))/2.0f);
    return FIREBALL_LOOP_STROBE_LOW + ((1.0f - FIREBALL_LOOP_STROBE_LOW) * cosResult);
  }

  return 1.0f;
}

void WaveEffect_Combo_Animation(EWaveType WaveDirection, EWaveSpeed AnimSpeed, int loops, int Col_R, int Col_G, int Col_B)
{
  int  index;
  int  i;
  int  counter[FIREBALL_MAX_GROUPS];
  int  loopsLeft[FIREBALL_MAX_GROUPS];
  int  loopFrames[FIREBALL_MAX_GROUPS] = {0,0,0,0};
  float multiplier[FIREBALL_MAX_GROUPS] = { 1.0f, 1.0f, 1.0f, 1.0f};
  WaveButtonOrder WaveButtons[FIREBALL_MAX_BUTTONS];
  
  //init locals
  for(int index = 0; index < FIREBALL_MAX_GROUPS; ++index)
  {
    loopsLeft[index] = loops;
  }

  counter[0] = FIREBALL_SIZE-1;
  counter[1] = counter[0] + FIREBALL_OFFSET_1;
  counter[2] = counter[0] + FIREBALL_OFFSET_2;
  counter[3] = counter[0] + FIREBALL_OFFSET_3;

  kaimana.setALL( BLACK );
  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update

  //Generate button information
  if(WaveDirection == EFT_LeftToRight || WaveDirection == EFT_LeftToRightPunchOnly || WaveDirection == EFT_LeftToRightKickOnly)
  {
    WaveButtons[0].Button = LED_P1;
    WaveButtons[0].Group =  WaveDirection == EFT_LeftToRightKickOnly ? -1 : 0;
    WaveButtons[1].Button = LED_K1;
    WaveButtons[1].Group = WaveDirection == EFT_LeftToRightPunchOnly ? -1 : 0;
    WaveButtons[2].Button = LED_P2;
    WaveButtons[2].Group = WaveDirection == EFT_LeftToRightKickOnly ? -1 : 1;
    WaveButtons[3].Button = LED_K2;
    WaveButtons[3].Group = WaveDirection == EFT_LeftToRightPunchOnly ? -1 : 1;
    WaveButtons[4].Button = LED_P3;
    WaveButtons[4].Group = WaveDirection == EFT_LeftToRightKickOnly ? -1 : 2;
    WaveButtons[5].Button = LED_K3;
    WaveButtons[5].Group = WaveDirection == EFT_LeftToRightPunchOnly ? -1 : 2;
    WaveButtons[6].Button = LED_P4;
    WaveButtons[6].Group = WaveDirection == EFT_LeftToRightKickOnly ? -1 : 3;
    WaveButtons[7].Button = LED_K4;
    WaveButtons[7].Group = WaveDirection == EFT_LeftToRightPunchOnly ? -1 : 3;
  }
  else if(WaveDirection == EFT_RightToLeft || WaveDirection == EFT_RightToLeftPunchOnly || WaveDirection == EFT_RightToLeftKickOnly)
  {
    WaveButtons[0].Button = LED_P4;
    WaveButtons[0].Group = WaveDirection == EFT_RightToLeftKickOnly ? -1 : 0;
    WaveButtons[1].Button = LED_K4;
    WaveButtons[1].Group = WaveDirection == EFT_RightToLeftPunchOnly ? -1 : 0;
    WaveButtons[2].Button = LED_P3;
    WaveButtons[2].Group = WaveDirection == EFT_RightToLeftKickOnly ? -1 : 1;
    WaveButtons[3].Button = LED_K3;
    WaveButtons[3].Group = WaveDirection == EFT_RightToLeftPunchOnly ? -1 : 1;
    WaveButtons[4].Button = LED_P2;
    WaveButtons[4].Group = WaveDirection == EFT_RightToLeftKickOnly ? -1 : 2;
    WaveButtons[5].Button = LED_K2;
    WaveButtons[5].Group = WaveDirection == EFT_RightToLeftPunchOnly ? -1 : 2;
    WaveButtons[6].Button = LED_P1;
    WaveButtons[6].Group = WaveDirection == EFT_RightToLeftKickOnly ? -1 : 3;
    WaveButtons[7].Button = LED_K1;
    WaveButtons[7].Group = WaveDirection == EFT_RightToLeftPunchOnly ? -1 : 3;   
  }
  else
  {
    int group1 = WaveDirection == EFT_UpToDown ? 1 : 0;
    
    WaveButtons[0].Button = LED_K1;
    WaveButtons[0].Group = group1;
    WaveButtons[1].Button = LED_K2;
    WaveButtons[1].Group = group1;
    WaveButtons[2].Button = LED_K3;
    WaveButtons[2].Group = group1;
    WaveButtons[3].Button = LED_K4;
    WaveButtons[3].Group = group1;
    WaveButtons[4].Button = LED_P1;
    WaveButtons[4].Group = 1 - group1;
    WaveButtons[5].Button = LED_P2;
    WaveButtons[5].Group = 1 - group1;
    WaveButtons[6].Button = LED_P3;
    WaveButtons[6].Group = 1 - group1;
    WaveButtons[7].Button = LED_P4;
    WaveButtons[7].Group = 1 - group1;    
  }

  //Store off speed to advance at. used at the end of the loop
  int counterChangePerLoop = FIREBALL_SPEED_SLOW;
  if(AnimSpeed == EFS_Medium)
  {
    counterChangePerLoop = FIREBALL_SPEED_MEDIUM;
  }
  else if(AnimSpeed == EFS_Fast)
    counterChangePerLoop = FIREBALL_SPEED_FAST;

  while(counter[0] >=0)
  {
    //Progress loops (when they get there). Adjusts the intensity multiplier for the colour values
    for(int index = 0; index < FIREBALL_MAX_GROUPS; ++index)
      multiplier[index] = UpdateWaveLoop(loopsLeft[index], counter[index], loopFrames[index]);

    for(int index = 0; index < FIREBALL_MAX_BUTTONS; ++index)
    {
      //sanity check values
      if(WaveButtons[index].Group < 0 || WaveButtons[index].Group >= FIREBALL_MAX_GROUPS || WaveButtons[index].Button == 0xFF)
        continue;
      
      WaveEffect_SetButton(counter[WaveButtons[index].Group],
                           WaveButtons[index].Button,
                           Col_R * multiplier[WaveButtons[index].Group],
                           Col_G * multiplier[WaveButtons[index].Group],
                           Col_B * multiplier[WaveButtons[index].Group]);     
    }

    // update the leds with new/current colors in the array
    kaimana.updateALL();

    // slow down the fireball animation
    delayMicroseconds( FIREBALL_DELAY );

    for(int index = 0; index < FIREBALL_MAX_GROUPS; ++index)
    {
      if(loopsLeft[index] <= 0 || counter[index] > FIREBALL_LOOP_START)
        counter[index] -= counterChangePerLoop;
    }
  }

  kaimana.setALL( BLACK );
  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update
}

//-----------------------------------------------------------------------------------
// Knight rider along one Row
//-----------------------------------------------------------------------------------
void KnightRider_Combo_Animation(int loops, bool bTopRow, int Col_R, int Col_G, int Col_B)
{
  for(int index = 0; index < loops; ++index)
  {
    WaveEffect_Combo_Animation(bTopRow ? EFT_LeftToRightPunchOnly : EFT_LeftToRightKickOnly, EFS_Medium, 0, Col_R, Col_G, Col_B);
    WaveEffect_Combo_Animation(bTopRow ? EFT_RightToLeftPunchOnly : EFT_RightToLeftKickOnly, EFS_Medium, 0, Col_R, Col_G, Col_B);
  }
}

//-----------------------------------------------------------------------------------
// Light all buttons one colour for 2000ms
//-----------------------------------------------------------------------------------
void FlashColour_Combo_Animation(int Col_R, int Col_G, int Col_B, int FlashTime)
{
  kaimana.setALL(BLACK);
  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update

  kaimana.setALL(Col_R, Col_G, Col_B);
  delay(FlashTime);

  kaimana.setALL(BLACK);
  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update
}

//-----------------------------------------------------------------------------------
// Start from K and go anti-clockwise cycling through RGB
//-----------------------------------------------------------------------------------
void SingleCircleAnim(int R, int G, int B)
{
  kaimana.setLED( LED_K1, R, G, B );
  kaimana.updateALL();
  delay(25);
  kaimana.setLED( LED_K2, R, G, B );
  kaimana.updateALL();  
  delay(25);
  kaimana.setLED( LED_K3, R, G, B );
  kaimana.updateALL();
  delay(25);
  kaimana.setLED( LED_K4, R, G, B );
  kaimana.updateALL(); 
  delay(25);
  kaimana.setLED( LED_P4, R, G, B );
  kaimana.updateALL();
  delay(25);
  kaimana.setLED( LED_P3, R, G, B );
  kaimana.updateALL();
  delay(25);
  kaimana.setLED( LED_P2, R, G, B );
  kaimana.updateALL();
  delay(25);
  kaimana.setLED( LED_P1, R, G, B );
  kaimana.updateALL();
  delay(25);
}

void Circle_OneColour_Combo_Animation(int loops, int R, int G, int B)
{
  kaimana.setALL(BLACK);
  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update

  while(loops >= 0)
  {
    SingleCircleAnim(R, G, B);
    SingleCircleAnim(BLACK);
    loops--;
  }

  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update
}

void CircleRGB_Combo_Animation(int loops)
{

  kaimana.setALL(BLACK);
  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update

  while(loops >= 0)
  {
    SingleCircleAnim(RED);
    SingleCircleAnim(GREEN);
    SingleCircleAnim(BLUE);
    loops--;
  }
  SingleCircleAnim(BLACK);

  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update
}

//-----------------------------------------------------------------------------------
// Flash All a colour, increasing in speed
//-----------------------------------------------------------------------------------
#define FLASH_TIMING_NUM 7
const int flashTimings[NUM_RANDOM_COLORS] = { 100, 80, 60, 40, 20, 10, 5 };
void FlashAllSpeedIncreasing_Combo_Animation(int R, int G, int B)
{
  kaimana.setALL(BLACK);
  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update

  for(int flashTimeIndex = 0; flashTimeIndex < FLASH_TIMING_NUM; ++flashTimeIndex)
  {
    kaimana.setALL(R, G, B);
    delay(flashTimings[flashTimeIndex]);
    kaimana.setALL(BLACK);
    delay(flashTimings[flashTimeIndex]);
  }

  kaimana.setALL(BLACK);
  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update
}

//-----------------------------------------------------------------------------------
// Randomise buttons
//-----------------------------------------------------------------------------------
void Randomise_Combo_Animation(int NumFlash, int TimeLit, int DelayBetween,int R, int G, int B)
{
  kaimana.setALL(BLACK);
  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update

  if(TimeLit < MIN_LED_UPDATE_DELAY)
    TimeLit = MIN_LED_UPDATE_DELAY;
  if(DelayBetween < MIN_LED_UPDATE_DELAY)
    DelayBetween = MIN_LED_UPDATE_DELAY;    

  for(int flashIndex = 0; flashIndex < NumFlash; ++flashIndex)
  {
    switch(random(8))
    {
      case 0:
      {
        kaimana.setLED( LED_P1, R, G, B );      
      } break;
      case 1:
      {
        kaimana.setLED( LED_P2, R, G, B );      
      } break;
      case 2:
      {
        kaimana.setLED( LED_P3, R, G, B );      
      } break;
      case 3:
      {
        kaimana.setLED( LED_P4, R, G, B );      
      } break;
      case 4:
      {
        kaimana.setLED( LED_K1, R, G, B );      
      } break;
      case 5:
      {
        kaimana.setLED( LED_K2, R, G, B );      
      } break;
      case 6:
      {
        kaimana.setLED( LED_K3, R, G, B );      
      } break;
      default:
      {
        kaimana.setLED( LED_K4, R, G, B );      
      } break;
    }
    kaimana.updateALL();  
    delay(TimeLit);
    kaimana.setALL(BLACK);
    delay(DelayBetween);
  }
  
  delay( MIN_LED_UPDATE_DELAY ); // give leds time to update
}
