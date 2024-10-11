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
//  Kaimana class based on original source released by ParadiseArcadeShop.com October 15, 2013
//
//  Created:  October 24, 2013    zonbipanda // gmail.com  -- Arduino 1.0.5 Support
//  Revised:  October 29, 2013    zonbipanda // gmail.com
//  Revised:  April   11, 2015    zonbipanda // gmail.com  -- Arduino 1.6.3 Support
//  Revised:  Nov     15, 2023    Paul 'pod' Denning -- Bug fixes, New j4/joystick pcb support, code cleanup, improved combo detection
//  Revised:  Mar     07, 2024    Paul 'pod' Denning -- Added static colour option for idle mode. Added fixed colour option for pressed mode. Added hold Idle colour instead of instant black for non-pressed. Can be tailored to be different for each character.
//  Revised:  Mar     22, 2024    Paul 'pod' Denning -- Added fade outs for button presses
//

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "kaimana.h"
#include "kaimana_custom.h"

Kaimana::Kaimana(void)
{
  // Kaimana Class Initialziation

  // set arduino led driver pin to output and low
  pinMode( PIN_LED, OUTPUT );
  digitalWrite( PIN_LED, LOW );

  // initialize random number seed with analog input #4 on port F1
  randomSeed(analogRead(4));

  // set arduino pins for input and enable internal pull up resistors
  for(int pinIndex = 0; pinIndex < SWITCH_COUNT; ++pinIndex)
    pinMode( switchPins[pinIndex],   INPUT_PULLUP );

  //init blend array
  for(int blendIndex = 0; blendIndex < LED_ENTRIES; ++blendIndex)
  {
    _ledBlend[blendIndex].LEDPin = ledList[blendIndex];
    _ledBlend[blendIndex].TimeSet = 0;
  }

  // initialize Switch History
  switchHistoryClear();
}

//Fade leds between 2 colours
bool Kaimana::blendLEDs(bool bForceEnd)
{
  unsigned long timeNow = millis();
  bool bOneStillGoing = false;
  
  for(int index = 0; index < LED_ENTRIES; ++index)
  {
    if(_ledBlend[index].TimeSet != 0)
    {
      bOneStillGoing = true;
      
      //Hold time elapsed?
      unsigned long timeBlendStart = _ledBlend[index].TimeSet + (unsigned long)_ledBlend[index].TimeToHold;
      if(bForceEnd || timeNow > timeBlendStart)
      {
        float blendPerc = (_ledBlend[index].TimeToBlend > 0) ? ((float)(timeNow - timeBlendStart) / (float)(_ledBlend[index].TimeToBlend)) : 1.0f;
        if(bForceEnd)
          blendPerc = 1.0f;

        //blend over?
        if(blendPerc >= 1.0f)
        {
          blendPerc = 1.0f;
          _ledBlend[index].TimeSet = 0;
        }
        
        //do blend
        setLED(_ledBlend[index].LEDPin, 
        (int)_ledBlend[index].SourceCol.r + (((int)_ledBlend[index].DestCol.r - (int)_ledBlend[index].SourceCol.r) * blendPerc), 
        (int)_ledBlend[index].SourceCol.g + (((int)_ledBlend[index].DestCol.g - (int)_ledBlend[index].SourceCol.g) * blendPerc), 
        (int)_ledBlend[index].SourceCol.b + (((int)_ledBlend[index].DestCol.b - (int)_ledBlend[index].SourceCol.b) * blendPerc), 
        true);
      }
    }
  }

  return bOneStillGoing;
}

// Sets LEDs to on in a globally defined brightness
void Kaimana::setLED(int index, int iR, int iG, int iB, bool bIsBlend, int holdTime, int fadeTime)
{
  if(index == 0xFF)
    return;
  
  int blendIndex = 0;
  for(; blendIndex < LED_ENTRIES; ++blendIndex)
  {
      if(_ledBlend[blendIndex].LEDPin == index)
        break;
  }

  //If its a blend then store away and exit
  if(blendIndex != LED_ENTRIES && bIsBlend == false)
  {
    if((holdTime > 0 || fadeTime > 0))
    {
      if(_ledBlend[blendIndex].TimeSet == 0 || 
         _ledBlend[blendIndex].DestCol.r != iR ||
         _ledBlend[blendIndex].DestCol.g != iG || 
         _ledBlend[blendIndex].DestCol.b != iB)
      {
        _ledBlend[blendIndex].SourceCol.r = _led[index].r;
        _ledBlend[blendIndex].SourceCol.g = _led[index].g;
        _ledBlend[blendIndex].SourceCol.b = _led[index].b;
        _ledBlend[blendIndex].DestCol.r = iR;
        _ledBlend[blendIndex].DestCol.g = iG;
        _ledBlend[blendIndex].DestCol.b = iB;
        _ledBlend[blendIndex].TimeSet = millis();
        _ledBlend[blendIndex].TimeToHold = holdTime;
        _ledBlend[blendIndex].TimeToBlend = fadeTime;
      }

      return;
    }
    else
    {
      _ledBlend[blendIndex].TimeSet = 0;
    }
  }
  
  int ledForThisSlot = LED_PER_BUTTON;
  if(index == LED_DOWN || index == LED_UP || index == LED_LEFT || index == LED_RIGHT)
    ledForThisSlot = LED_PER_JOYSTICK_DIRECTION;
    
  int ledIndex;
  for(ledIndex = 0; ledIndex < ledForThisSlot; ++ledIndex)
  {
    _led[index + ledIndex].r = iR;
    _led[index + ledIndex].g = iG;
    _led[index + ledIndex].b = iB;
  }   
}  

// Sets LEDs to on in a globally defined brightness
void Kaimana::setIndividualLED(int index, int iR, int iG, int iB)
{
  if(index == 0xFF)
    return;

  _led[index].r = iR;
  _led[index].g = iG;
  _led[index].b = iB;
}

void Kaimana::setALL(int iR, int iG, int iB)
{
  int index;

  //reset all blends
  for(int blendIndex = 0; blendIndex < LED_ENTRIES; ++blendIndex)
  {
    _ledBlend[blendIndex].TimeSet = 0;
  }
  
  // set all leds in the array to the RGB color passed to this function
  for(index=0;index<LED_COUNT;++index)
  {
    setIndividualLED( index, iR, iG, iB );
  }

  // update the leds with new/current colors in the array
  updateALL();
}


void Kaimana::updateALL(void)
{
  // update the leds with new/current colors and delay a little to avoid flickering (yea, it happens really, really fast!)
  // specific to ParadiseArcadeShop.com Kaimana board (PS360+LED)
  // port F0 drives LED data out
  //
  //  WS2811 timing based upon the threepixeldemo example by Alan Burlison ( alan@bleaklow.com )
  //    http://sourceforge.net/p/abavr/lib/ci/21a95bf1f6c437cac414e8efb0c80558e5c369db/tree/WS2811/
  //

  asm volatile( \
  /* initialise */ \
  "    cp %A[len], r1      ; check len > 0, return immediately if it is\n" \
  "    cpc %B[len], r1\n" \
  "    brne 1f\n" \
  "    rjmp 16f\n" \
  "1:  ld r18, Z+           ; load in first red byte to be output\n" \
  "    ld r19, Z+           ; load in first green byte to be output\n" \
  "    ld r20, Z+           ; load in first blue byte to be output\n" \
  "    ldi r27, 8           ; load inner loop counter\n" \
  "    in r26, __SREG__     ; timing-critical, so no interrupts\n" \
  "    cli\n" \
  /* green - loop over 8 bits */ \
  "2:  sbi  %[port], %[pin] ; pin lo -> hi\n" \
  "    sbrc r19, 7          ; test hi bit clear\n" \
  "    rjmp 3f              ; true, skip pin hi -> lo\n" \
  "    cbi  %[port], %[pin] ; false, pin hi -> lo\n" \
  "3:  sbrc r19, 7          ; equalise delay of both code paths\n" \
  "    rjmp 4f\n" \
  "4:  nop                  ; pulse timing delay\n" \
  "    nop\n" \
  "    nop\n" \
  "    nop\n" \
  "    nop\n" \
  "    nop\n" \
  "    lsl r19              ; shift to next bit\n" \
  "    dec r27              ; decrement loop counter\n" \
  "    cbi %[port], %[pin]  ; pin hi -> lo\n" \
  "    brne 2b\n            ; loop if required\n" \
  "    ldi r27, 7           ; reload inner loop counter\n" \
  /* red - loop over first 7 bits */ \
  "5:  sbi %[port], %[pin]  ; pin lo -> hi\n" \
  "    sbrc r18, 7          ; test hi bit clear\n" \
  "    rjmp 6f              ; true, skip pin hi -> lo\n" \
  "    cbi %[port], %[pin]  ; false, pin hi -> lo\n" \
  "6:  sbrc r18, 7          ; equalise delay of both code paths\n" \
  "    rjmp 7f\n" \
  "7:  nop                  ; pulse timing delay\n" \
  "    nop\n" \
  "    nop\n" \
  "    nop\n" \
  "    nop\n" \
  "    nop\n" \
  "    lsl r18              ; shift to next bit\n" \
  "    dec r27              ; decrement inner loop counter\n" \
  "    cbi %[port], %[pin]  ; pin hi -> lo\n" \
  "    brne 5b              ; inner loop, if required\n" \
  "    nop                  ; equalise delay of both code paths\n" \
  /* red, 8th bit - output & fetch next values */ \
  "    sbi %[port], %[pin]  ; pin lo -> hi\n" \
  "    sbrc r18, 7          ; test hi bit clear\n" \
  "    rjmp 8f              ; true, skip pin hi -> lo\n" \
  "    cbi %[port], %[pin]  ; false, pin hi -> lo\n" \
  "8:  sbrc r18, 7          ; equalise delay of both code paths\n" \
  "    rjmp 9f\n" \
  "9:  nop                  ; pulse timing delay\n" \
  "    nop\n" \
  "    nop\n" \
  "    ld r18, Z+           ; load next red byte\n" \
  "    ld r19, Z+           ; load next green byte\n" \
  "    ldi r27, 7           ; reload inner loop counter\n" \
  "    cbi %[port], %[pin]  ; pin hi -> lo\n" \
  "    nop                  ; pulse timing delay\n" \
  "    nop\n" \
  /* blue - loop over first 7 bits */ \
  "10:  sbi %[port], %[pin] ; pin lo -> hi\n" \
  "    sbrc r20, 7          ; test hi bit clear\n" \
  "    rjmp 11f             ; true, skip pin hi -> lo\n" \
  "    cbi %[port], %[pin]  ; false, pin hi -> lo\n" \
  "11: sbrc r20, 7          ; equalise delay of both code paths\n" \
  "    rjmp 12f\n" \
  "12: nop                  ; pulse timing delay\n" \
  "    nop\n" \
  "    nop\n" \
  "    nop\n" \
  "    nop\n" \
  "    nop\n" \
  "    lsl r20              ; shift to next bit\n" \
  "    dec r27              ; decrement inner loop counter\n" \
  "    cbi %[port], %[pin]  ; pin hi -> lo\n" \
  "    brne 10b             ; inner loop, if required\n" \
  "    nop                  ; equalise delay of both code paths\n" \
  /* blue, 8th bit -  output & handle outer loop */ \
  "    sbi %[port], %[pin]  ; pin lo -> hi\n" \
  "    sbrc r20, 7          ; test hi bit clear\n" \
  "    rjmp 13f             ; true, skip pin hi -> lo\n" \
  "    cbi %[port], %[pin]  ; false, pin hi -> lo\n" \
  "13: sbrc r20, 7          ; equalise delay of both code paths\n" \
  "    rjmp 14f\n" \
  "14: nop                  ; pulse timing delay\n" \
  "    nop\n" \
  "    ldi r27, 8           ; reload inner loop counter\n" \
  "    sbiw %A[len], 1      ; decrement outer loop counter\n" \
  "    breq 15f             ; exit outer loop if zero\n" \
  "    ld r20, Z+           ; load in next blue byte\n" \
  "    cbi %[port], %[pin]  ; pin hi -> lo\n" \
  "    rjmp 2b              ; outer loop, if required\n" \
  "15: nop                  ; pulse timing delay\n" \
  "    cbi %[port], %[pin]  ; pin hi -> lo\n" \
  "    nop                  ; pulse timing delay\n" \
  "    nop\n" \
  "    out __SREG__, r26    ; reenable interrupts\n" \
  "16:\n" \
  : \
  : [rgb] "z" (&_led), \
    [len] "w" (sizeof(_led)/sizeof(_led[0])), \
    [port] "I" (_SFR_IO_ADDR(PORTF)), \
    [pin] "I" (0) \
  : "r18", "r19", "r20", "r26", "r27", "cc", "memory" \
  );
}  



void Kaimana::switchHistoryClear(void)
{
  static int  i;

  _switchClearAwaitingAllRelease = true;
  
  // initialize Switch History with the None command
  for(i=0;i<SWITCH_HISTORY_MAX;++i)
    _switchHistory[i].Input = EIT_INPUT_NONE;
}

void  Kaimana::switchHistoryBeginFrame(void)
{
  _timeAtBeginningOfFrame = millis();
  
  for(int i = 0; i < EIT_INPUT_MAX; ++i)
    _InputsThisFrame[i] = false;
}

void  Kaimana::switchHistoryEndFrame(void)
{
  bool bAnyHeld = false;

  //check for all inputs released after a clear
  for(int inputIndex = 1 ; inputIndex < EIT_INPUT_MAX; ++inputIndex)
  {
    if(_InputsThisFrame[inputIndex])
      bAnyHeld = true;
  }
  if(bAnyHeld == false)
    _switchClearAwaitingAllRelease = false;
    
  for(int historyIndex = 0 ; historyIndex < SWITCH_HISTORY_MAX; ++historyIndex)
  {
    if(_switchHistory[historyIndex].bIsHeld && _InputsThisFrame[_switchHistory[historyIndex].Input] == false)
    {
      _switchHistory[historyIndex].bIsHeld = false;
      //This was a nasty bug to find. I originally just set the current time here. But that could be 1 to a few ms later than the input it was replaced by since this call is at the end of a loop
      //so when it came to input history tests for special moves you would sometimes end up seeing the time between the inputs as being -1 which since times are unsigned would mean it would wrap
      //and be a huge number and fail the time restraints!
      _switchHistory[historyIndex].TimeReleased = _timeAtBeginningOfFrame; 
    }
  }
}

void Kaimana::switchHistoryCreateNew(EInputTypes newValue)
{
  //Shift all entries up by 1
  for(int historyIndex = (SWITCH_HISTORY_MAX-1) ; historyIndex>0; --historyIndex)
  {
    _switchHistory[historyIndex].Input = _switchHistory[historyIndex-1].Input;
    _switchHistory[historyIndex].TimeSet = _switchHistory[historyIndex-1].TimeSet;
    _switchHistory[historyIndex].TimeReleased = _switchHistory[historyIndex-1].TimeReleased;
    _switchHistory[historyIndex].bIsHeld = _switchHistory[historyIndex-1].bIsHeld;
  }

  //Stick new input onto front of array and set the held start time
  _switchHistory[0].Input = newValue;
  _switchHistory[0].TimeSet = millis();
  _switchHistory[0].TimeReleased = 0;
  _switchHistory[0].bIsHeld = true;
}

void Kaimana::switchHistorySet(EInputTypes latestValue)
{
  //Store off that this input happened this frame
  _InputsThisFrame[latestValue] = true;

  //if we're after a special move then we want to make sure all inouts are released first before more logging.
  if(_switchClearAwaitingAllRelease)
    return;
  
  //loop through history for last occurance
  bool bRequiresNewEntry = true;
  for(int historyIndex = 0 ; historyIndex < SWITCH_HISTORY_MAX; ++historyIndex)
  {
    if(_switchHistory[historyIndex].Input == latestValue)
    {
      //If this entry says its being held then we do nothing. If it isnt being held then its expired and we need to make a new entry
      if(_switchHistory[historyIndex].bIsHeld)
      {
        bRequiresNewEntry = false;
      }

      //We found an entry, no need to search further
      break;
    }
  }

  //if we didnt find it or it was already released then we need a new entry
  if(bRequiresNewEntry)
    switchHistoryCreateNew(latestValue);     
}  

bool doesInputMatch(EInputTypes historyInput, EInputTypes comboInput, bool bIsChargeMove)
{
  if(historyInput == comboInput)
    return true;

  if(bIsChargeMove)
  {
    //Charge moves allow the surrounding directions to also count
    if(comboInput == EIT_Input_Left)
    {
      return historyInput == EIT_Input_UpLeft || historyInput == EIT_Input_DownLeft;
    }
    else if(comboInput == EIT_Input_Down)
    {
      return historyInput == EIT_Input_DownLeft || historyInput == EIT_Input_DownRight;
    }
    else if(comboInput == EIT_Input_Right)
    {
      return historyInput == EIT_Input_UpRight || historyInput == EIT_Input_DownRight;
    }
    else if(comboInput == EIT_Input_Up)
    {
      return historyInput == EIT_Input_UpLeft || historyInput == EIT_Input_UpRight;
    }
  }

  return false;
}

//todo : This is still a night mare for 360's and 720's. Need to add some kind of percentage of inputs hit or something. (like a rolling 2 out of 4 for any inputs over 6 inputs long
boolean Kaimana::switchHistoryTest(const EInputTypes* moveArray, int moveLength, const EInputTypes* triggerArray, int triggerLength, bool bIsChargeCombo)
{
  int lastIndexComboInputWasFound = -1;
  bool bHasFinishedDoingTriggerButtons = false;
  unsigned long timeNow = millis();
  unsigned long timeLastInputSet = millis();
  int numMissed = 0;
  bool bAllowMiss = moveLength >= COMBO_INPUT_COUNT_FOR_ONE_OUT_OF_TWO;

  //build one big array for easier processing
  EInputTypes comboArray[SWITCH_HISTORY_MAX];
  for(int index = 0; index < moveLength && index < SWITCH_HISTORY_MAX; ++index)
  {
    comboArray[index] = pgm_read_byte_near(&moveArray[index]);
  }
  for(int index = 0; index < triggerLength && moveLength + index < SWITCH_HISTORY_MAX; ++index)
  {
    comboArray[moveLength + index] = triggerArray[index];
  }
  int comboLength = moveLength + triggerLength;
  if(comboLength > SWITCH_HISTORY_MAX)
    comboLength = SWITCH_HISTORY_MAX;
    
  //Moves are specified in normal order. History is newest first. So start from the end of the combo and work back
  for(int comboIndex = comboLength - 1; comboIndex >= 0; --comboIndex)
  {
    //First see if the button(s) and the final movement that finish this combo routine have recently been pressed
    if(bHasFinishedDoingTriggerButtons == false)
    {
      //Is this a trigger button
      if((comboArray[comboIndex] >= EIT_Input_FirstAttackButton && comboArray[comboIndex] <= EIT_Input_LastAttackButton) == false)
      {
        bHasFinishedDoingTriggerButtons = true;
      }
   
      //In which case, we only care if it was pressed within the last COMBO_TRIGGER_INPUT_TIME_WINDOW
      bool bFoundTrigger = false;
      for(int historyIndex = 0; historyIndex < SWITCH_HISTORY_MAX; ++historyIndex)
      {
        //are we past the time window?
        if(timeNow - _switchHistory[historyIndex].TimeSet > COMBO_TRIGGER_INPUT_TIME_WINDOW)
        {
          return false; //we havent managed to find this trigger in time
        }
          
        if(doesInputMatch(_switchHistory[historyIndex].Input, comboArray[comboIndex], bIsChargeCombo))
        {
          //we found this trigger. Move onto the next one
          bFoundTrigger = true;
          break;
        }
      }
      
      if(bFoundTrigger == false)
      {
        return false; //we havent managed to find this trigger
      }
  
      //found trigger so move onto next combo entry
      continue;
    }
      
    //search history for next input
    bool bFoundInput = false;
    for(int historyIndex = lastIndexComboInputWasFound + 1; historyIndex < SWITCH_HISTORY_MAX; ++historyIndex)
    {
      if(doesInputMatch(_switchHistory[historyIndex].Input, comboArray[comboIndex], bIsChargeCombo))
      {
        bFoundInput = true;
        
        //Is this the last input and if so is it a charge input as they're special and 
        if(comboIndex == 0 && bIsChargeCombo)
        {
          //Its a charge move. In which case, this move must be held for CHARGE_COMBO_INPUT_TIME_WINDOW as well as released within COMBO_INPUT_TIME_WINDOW of the last one
          if(timeLastInputSet - _switchHistory[historyIndex].TimeReleased < COMBO_INPUT_TIME_WINDOW && _switchHistory[historyIndex].TimeReleased - _switchHistory[historyIndex].TimeSet > CHARGE_COMBO_INPUT_TIME_WINDOW)
          {
            //Clear the history to prevent retrigger if animation is really quick
            switchHistoryClear();
            return true; //success
          }
          else
          {
            return false; //released too early
          }
        }

        //regular input
        unsigned long timeToUse = _switchHistory[historyIndex].bIsHeld ?  _switchHistory[historyIndex].TimeSet : _switchHistory[historyIndex].TimeReleased;
        if(timeLastInputSet - timeToUse > COMBO_INPUT_TIME_WINDOW)
        {
          if(bAllowMiss)
          {
            //too long between inputs on this one. but might be ok to continue if this was an older input in the history
            numMissed++;
            if(numMissed == 2)
              return false; //too many missing
          }
          else
            return false; //too long between inputs
        }

        //Store this time and location and move onto next input
        timeLastInputSet = _switchHistory[historyIndex].TimeSet;
        lastIndexComboInputWasFound = historyIndex;
        numMissed = 0;

        break;
      }
    }

    if(bFoundInput == false)
    {
      if(bAllowMiss)
      {
        //didnt find this one. but might be ok to continue
        numMissed++;
        if(numMissed == 2)
          return false; //too many missing
      }
      else
        return false; //we havent found the input at all
    }
  }

  //if we get here we've found every single input! Combo passed
  //Clear the history to prevent retrigger if animation is really quick
  switchHistoryClear();
  //also clear any existing blends
  for(int blendIndex = 0; blendIndex < LED_ENTRIES; ++blendIndex)
  {
    _ledBlend[blendIndex].TimeSet = 0;
  }
  return true;
}
