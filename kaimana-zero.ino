#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "kaimana.h"
#include "kaimana_custom.h"
#include "animations.h"
#include "Characters.h"


// local function declarations
bool pollSwitches(void);
void setLEDRandomColor(int index);
bool tourneypollSwitches(void);
boolean tournamentMode = false;
int holdTimeout = 0;

//global state of all LED inputs being on or off (todo : wasteful, look into new way to store this or share with input history)
bool iLED[LED_COUNT];

// ParadiseArcadeShop.com Kaimana features initialzied when Kaimana class instantiated
Kaimana kaimana;

//Characters
Ryu ryu;
Ken ken;
Guile guile;
Chun chun;
Gief gief;
Blanka blanka;
Honda honda;
Dhalsim sim;

//DeeJay deeJay;
//Marisa marisa;
//Akuma akuma;
//Terry terry;
//SF2 sf2;

const Character* AllCharacters[NUM_CHARACTERS] = { &ryu, &honda, &blanka, &guile, &ken, &chun, &gief, &sim };
int8_t selectedCharacter = 0;

// ==============================================================
//
//  init and main loop functions start here
//
// ==============================================================

// the setup routine runs first and once each time power is applied to the Kaimana board
void setup() 
{                
  // light up all leds at boot to demonstrate everything is functional
	defaultStartup();
}


// the loop routine repeats indefinitely and executes immediately following the setup() function
void loop() 
{
  unsigned long  ulTimeout;
  
  // go straight into idle
  ulTimeout = 0;

  // infinite loop of read switches, update LEDs and idle animation when necessary
  while(true)
  {
	  if (tournamentMode != true)
		{
			// active -- poll switches and update leds
			if( pollSwitches() )
			{
				// some switches were active so reset idle timeout to now + some seconds
        //also force first frame to correct colours
        if(millis() > ulTimeout)
        {
          kaimana.blendLEDs(true);
        }

				ulTimeout = millis() + ( (unsigned long)IDLE_TIMEOUT_SECONDS * 1000 );
			}
		
			if (tournamentMode != true) //retest here as it can change in the poll function
			{
				// if there been enough time with no LEDS set by pollSwitches then do idle animation this frame
				if( millis() > ulTimeout )
				{
				  animation_idle(AllCharacters[selectedCharacter]);
				}  

        // update the leds with new/current colors in the array
        kaimana.updateALL();

        //Add a small delay to give the update function time to recover
        delay( MIN_LED_UPDATE_DELAY );
			}
		}
	  else
	  {
      if(tourneypollSwitches())
      {
        ulTimeout = 0;        
      }
  	}    
  } 
}


// ==============================================================
//
//  local functions start here
//
// ==============================================================

//See if an attack button is held, record it and set a random colour for that button if newly pressed
bool checkButtonPressedAndSetNewColourIfSo(int pin, int led)
{
  bool bHeld = digitalRead(pin) == BUTTON_READ_CHECK;
  
  // test switch and set LED based on result
  if(led != 0xFF)
  {
    if(bHeld)
    {
      // switch is active
      if(iLED[led] == true)
      {
        //maintain color while switch is active
        //Add custom "when held" code here
      }
      else
      {
        // select new color when switch is first activated
        RGB_t thisCol = AllCharacters[selectedCharacter]->pressedStaticColour(led);
        kaimana.setLED(led, thisCol.r, thisCol.g, thisCol.b);
        iLED[led] = true;
      }
    }
    else
    {
        // switch is inactive
        RGB_t thisCol = AllCharacters[selectedCharacter]->notPressedStaticColour(led);
        kaimana.setLED(led, thisCol.r, thisCol.g, thisCol.b, false, AllCharacters[selectedCharacter]->holdPressedButtonColourTimeInMS(),  AllCharacters[selectedCharacter]->fadePressedButtonColourTimeInMS());
      
        iLED[led] = false;
    }
  }
    
  return bHeld;
}

//Check movement directions, record it and set a random colour for that button if newly pressed
void updateMovementHeldDirections()
{
  static EInputTypes joystickLastFrame = EIT_INPUT_NONE;
  
  bool bUpHeld = digitalRead(PIN_UP) == BUTTON_READ_CHECK;
  bool bLeftHeld = digitalRead(PIN_LEFT) == BUTTON_READ_CHECK;
  bool bRightHeld = digitalRead(PIN_RIGHT) == BUTTON_READ_CHECK;
  bool bDownHeld = digitalRead(PIN_DOWN) == BUTTON_READ_CHECK;

  EInputTypes joystickThisFrame = EIT_INPUT_NONE;

  //This code is designed for joysticks. If you're on leverless you might want to edit it to your SOCD prefference
  if(bUpHeld && bLeftHeld)
    joystickThisFrame = EIT_Input_UpLeft;
  else if(bDownHeld && bLeftHeld)
    joystickThisFrame = EIT_Input_DownLeft;
  else if(bDownHeld && bRightHeld)
    joystickThisFrame = EIT_Input_DownRight;
  else if(bUpHeld && bRightHeld)
    joystickThisFrame = EIT_Input_UpRight;
  else if(bUpHeld)
    joystickThisFrame = EIT_Input_Up;
  else if(bDownHeld)
    joystickThisFrame = EIT_Input_Down;
  else if(bLeftHeld)
    joystickThisFrame = EIT_Input_Left;
  else if(bRightHeld)
    joystickThisFrame = EIT_Input_Right;
    
  if(joystickThisFrame != EIT_INPUT_NONE)
    kaimana.switchHistorySet(joystickThisFrame);
    
  //Do joystick pcb true diagonals
  bool bUpdateJoystickDiagonals = false;
  int firstDirection;
  int secondDirection;
  if(USING_JOYSTICK_LED_PCB_AND_WANT_PROPER_DIAGONALS && LED_PER_JOYSTICK_DIRECTION == 4 && LED_LEFT != 0xFF && LED_RIGHT != 0xFF && LED_UP != 0xFF && LED_DOWN != 0xFF) //Double check we've set 4 leds per direction
  {
    //NB. Leds are done in a anti-clockwise direction
    bUpdateJoystickDiagonals = joystickThisFrame != joystickLastFrame;
    if(joystickThisFrame == EIT_Input_UpLeft)
    {
      firstDirection = LED_UP;
      secondDirection = LED_LEFT;
    }
    else if(joystickThisFrame == EIT_Input_DownLeft)
    {
      firstDirection = LED_LEFT;
      secondDirection = LED_DOWN;
    }
    else if(joystickThisFrame == EIT_Input_DownRight)
    {
      firstDirection = LED_DOWN;
      secondDirection = LED_RIGHT;
    }
    else if(joystickThisFrame == EIT_Input_UpRight)
    {
      firstDirection = LED_RIGHT;
      secondDirection = LED_UP;
    }
  }

  //if using joystick pcb diagonals and it changes then we need to reset all directional lights
  if(bUpdateJoystickDiagonals)
  {
    int ledArray[] = {LED_UP, LED_LEFT, LED_RIGHT, LED_DOWN};
    for(int index = 0; index < 4; ++index)
    {
      RGB_t thisCol = AllCharacters[selectedCharacter]->notPressedStaticColour(ledArray[index]);
      kaimana.setLED(ledArray[index], thisCol.r, thisCol.g, thisCol.b);
      iLED[ledArray[index]] = false;
    }
  }

  //Check for button down and set rgb colour
  checkButtonPressedAndSetNewColourIfSo(PIN_UP, LED_UP);
  checkButtonPressedAndSetNewColourIfSo(PIN_LEFT, LED_LEFT);
  checkButtonPressedAndSetNewColourIfSo(PIN_RIGHT, LED_RIGHT);
  checkButtonPressedAndSetNewColourIfSo(PIN_DOWN, LED_DOWN);

  //if using joystick pcb diagonals then now remove the 4 unneeded lights
  if(bUpdateJoystickDiagonals && (joystickThisFrame == EIT_Input_UpLeft || joystickThisFrame == EIT_Input_UpRight || joystickThisFrame == EIT_Input_DownLeft || joystickThisFrame == EIT_Input_DownRight))
  {
    RGB_t firstCol = AllCharacters[selectedCharacter]->notPressedStaticColour(firstDirection);
    RGB_t secondCol = AllCharacters[selectedCharacter]->notPressedStaticColour(secondDirection);
   
    kaimana.setIndividualLED(firstDirection, firstCol.r, firstCol.g, firstCol.b);
    kaimana.setIndividualLED(firstDirection+1, firstCol.r, firstCol.g, firstCol.b);
    kaimana.setIndividualLED(secondDirection+2, secondCol.r, secondCol.g, secondCol.b);
    kaimana.setIndividualLED(secondDirection+3, secondCol.r, secondCol.g, secondCol.b);
  }
      
  joystickLastFrame = joystickThisFrame;
}

bool testForCharacterChange(void)
{
  static unsigned long ChangeComboHeldStart = 0;
  bool bChangeComboHeld = digitalRead(PIN_P1) == BUTTON_READ_CHECK &&
                          digitalRead(PIN_P4) == BUTTON_READ_CHECK &&
                          digitalRead(PIN_K1) == BUTTON_READ_CHECK &&
                          digitalRead(PIN_K4) == BUTTON_READ_CHECK;

  delay( MIN_LED_UPDATE_DELAY );

  //Have we held it long enough?
  if(bChangeComboHeld)
  {
    if(ChangeComboHeldStart == 0)
      ChangeComboHeldStart = millis();

    if(millis() -  ChangeComboHeldStart >= 1000) //1 second
    {
      //Enter change mode.
      kaimana.setALL(BLACK);
      delay( MIN_LED_UPDATE_DELAY ); // give leds time to update
      
      kaimana.setALL(WHITE);
      delay( MIN_LED_UPDATE_DELAY ); // give leds time to update

      //Wait for player to remove fingers from combo keys
      while(bChangeComboHeld)
      {
        bChangeComboHeld = digitalRead(PIN_P1) == BUTTON_READ_CHECK ||
                          digitalRead(PIN_P4) == BUTTON_READ_CHECK ||
                          digitalRead(PIN_K1) == BUTTON_READ_CHECK ||
                          digitalRead(PIN_K4) == BUTTON_READ_CHECK;
        
        delay( MIN_LED_UPDATE_DELAY );
      }

      kaimana.setALL(RED);
      delay( MIN_LED_UPDATE_DELAY ); // give leds time to update
      
      selectedCharacter = -1;
      while(selectedCharacter < 0)
      {
        if(digitalRead(PIN_P1) == BUTTON_READ_CHECK)
          selectedCharacter = 0;
        else if(digitalRead(PIN_P2) == BUTTON_READ_CHECK)
          selectedCharacter = 1;
        else if(digitalRead(PIN_P3) == BUTTON_READ_CHECK)
          selectedCharacter = 2;
        else if(digitalRead(PIN_P4) == BUTTON_READ_CHECK)
          selectedCharacter = 3;
        else if(digitalRead(PIN_K1) == BUTTON_READ_CHECK)
          selectedCharacter = 4;
        else if(digitalRead(PIN_K2) == BUTTON_READ_CHECK)
          selectedCharacter = 5;
        else if(digitalRead(PIN_K3) == BUTTON_READ_CHECK)
          selectedCharacter = 6;
        else if(digitalRead(PIN_K4) == BUTTON_READ_CHECK)
          selectedCharacter = 7;
          
        delay( MIN_LED_UPDATE_DELAY );
      }

      kaimana.setALL(BLACK);
      delay( MIN_LED_UPDATE_DELAY ); // give leds time to update

      kaimana.switchHistoryClear();

      return true;
    }
  }
  else
  {
    ChangeComboHeldStart = 0;
  }

  return false;
}

bool pollSwitches(void)
{
  static int  i = 0;  
  static int  j = 0;  
  static int  firsttime = 1;
  static uint16_t  joystickDirection = 0;
  static uint16_t  switchActivity = 0;
  static uint16_t  irrellivantActivity = 0;

  // reset LED status
  if (firsttime == 1)
  {
    for(i=0;i<LED_COUNT;++i)
    {
      iLED[i] = false;
      firsttime = 0;
    }
  }

  // test switch and set LED based on result       // HOME = GUIDE
  // tests if we should switch into or out of tourney mode
  if(digitalRead(PIN_TO_SWITCH_TO_TOURNEY_MODE) == BUTTON_READ_CHECK)
  {
    // switch is active
    if(PIN_TO_SWITCH_TO_TOURNEY_MODE_LED != 0xFF)
    {
      if(iLED[PIN_TO_SWITCH_TO_TOURNEY_MODE_LED] == true)
      {
        //maintain color while switch is active
        //Add custom "when held" code here
      }
      else
      {
        // select new color when switch is first activated
        setLEDRandomColor(LED_HOME);
        iLED[PIN_TO_SWITCH_TO_TOURNEY_MODE_LED] = true;
      }
    }
    
    //Button hold to change idle animation
    holdTimeout += 1;

    //Button hold to start tourneymode    
    if(holdTimeout == 500)
    {
      tournamentMode = true;
      tourneyModeActivate();
      return true;
    }
  }
  else
  {
      // switch is inactive
      if(LED_HOME != 0xFF)
      {
        kaimana.setLED(PIN_TO_SWITCH_TO_TOURNEY_MODE_LED, BLACK);
        iLED[PIN_TO_SWITCH_TO_TOURNEY_MODE_LED] = false;	  	 
      }
      
	    holdTimeout=0;
  }

  //Do Character change
  if(testForCharacterChange())
  {
    return 1; //make sure Idle timer resets
  }

  // test switchs and set LED based on result    
  kaimana.switchHistoryBeginFrame();

  checkButtonPressedAndSetNewColourIfSo(PIN_SELECT, LED_SELECT);// SELECT = BACK
  checkButtonPressedAndSetNewColourIfSo(PIN_START, LED_START);
 
  if(checkButtonPressedAndSetNewColourIfSo(PIN_P1, LED_P1))
      kaimana.switchHistorySet(EIT_Input_P1);
  if(checkButtonPressedAndSetNewColourIfSo(PIN_P2, LED_P2))
      kaimana.switchHistorySet(EIT_Input_P2);
  if(checkButtonPressedAndSetNewColourIfSo(PIN_P3, LED_P3))
      kaimana.switchHistorySet(EIT_Input_P3);
  if(checkButtonPressedAndSetNewColourIfSo(PIN_P4, LED_P4))
      kaimana.switchHistorySet(EIT_Input_P4);
  if(checkButtonPressedAndSetNewColourIfSo(PIN_K1, LED_K1))
      kaimana.switchHistorySet(EIT_Input_K1);
  if(checkButtonPressedAndSetNewColourIfSo(PIN_K2, LED_K2))
      kaimana.switchHistorySet(EIT_Input_K2);
  if(checkButtonPressedAndSetNewColourIfSo(PIN_K3, LED_K3))
      kaimana.switchHistorySet(EIT_Input_K3);
  if(checkButtonPressedAndSetNewColourIfSo(PIN_K4, LED_K4))
      kaimana.switchHistorySet(EIT_Input_K4);

  updateMovementHeldDirections();

  kaimana.switchHistoryEndFrame();
  
  //Do selected Character combos/Special moves
  bool bSpecialFired = false;
  if(AllCharacters[selectedCharacter]->testForCharacterCombos())
  {
    bSpecialFired = true;
    
    //reset all light storage and colours
    for(i=0;i<LED_COUNT;++i)
    {
      iLED[i] = false;
    }
    for(i=0;i<LED_ENTRIES;++i)
    {
      RGB_t thisCol = AllCharacters[selectedCharacter]->notPressedStaticColour(ledList[i]);
      kaimana.setLED(ledList[i], thisCol.r, thisCol.g, thisCol.b);
    }        
  }

  //blend the leds. this will return true if theres at least one blend still going so
  //return true here to make sure the idle animation doesnt kick in.
  kaimana.blendLEDs();
  
  // test all led's and return true if any set (will stop/delay idle animation)
  for(i=0;i<LED_COUNT;++i)
  {
    if(iLED[i])
      return true;
  }

  return bSpecialFired;
}  

bool tourneypollSwitches(void)
{
  // test switch and set LED based on result       // HOME = GUIDE
  if(!digitalRead(PIN_TO_SWITCH_TO_TOURNEY_MODE))
  {
	  //Button hold to change idle animation
		holdTimeout += 1;
		//Button hold to end tourneymode		
		if(holdTimeout == 1000)
		{
  		tournamentMode = false;
  		tourneyModeDeactivate();
      return true;
		}
 
	  delay(10);	  
  }
  else
  {  	 
  	holdTimeout=0;
  }

  return false;
}

 
