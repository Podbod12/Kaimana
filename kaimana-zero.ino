#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "kaimana.h"
#include "kaimana_custom.h"
#include "animations.h"
#include "Characters.h"


// local function declarations
int  pollSwitches(void);
void setLEDRandomColor(int index);
int tourneypollSwitches(void);
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
  
  // initialize timeout value to now + some seconds
  ulTimeout = millis() + ( (unsigned long)IDLE_TIMEOUT_SECONDS * 1000 );

  // infinite loop of read switches, update LEDs and idle animation when necessary
  while(true)
  {
	  if (tournamentMode != true)
		{
			// active -- poll switches and update leds
			if( pollSwitches() != 0 )
			{
				// some switches were active so reset idle timeout to now + some seconds
				ulTimeout = millis() + ( (unsigned long)IDLE_TIMEOUT_SECONDS * 1000 );
			}
			else
			{
				// no switches active so test for start of idle timeout  
				if( millis() > ulTimeout )
				{
				  animation_idle(AllCharacters[selectedCharacter]);

          //Force set black when exiting idle (if applicable)
          if(AllCharacters[selectedCharacter]->turnNonHeldButtonsOff() || AllCharacters[selectedCharacter]->useStaticColourInIdle() == false)
            kaimana.setALL(BLACK);
				}  
			}
		}
	  else
	  {
		  if( tourneypollSwitches() != 0 )
  		{
  			// some switches were active so reset idle timeout to now + some seconds
  			ulTimeout = millis() + ( (unsigned long)IDLE_TIMEOUT_SECONDS * 1000 );
  		}
  	}    
  } 
}


// ==============================================================
//
//  local functions start here
//
// ==============================================================


// set LED to one of 8 predefined colors selected at random
//
void setLEDRandomColor(int index)
{
  int randomVal = random(0,NUM_RANDOM_COLORS);
  kaimana.setLED(index, randomColors[randomVal].r, randomColors[randomVal].g, randomColors[randomVal].b);
}

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
        if(AllCharacters[selectedCharacter]->useStaticColourWhenPressed())
        {
          RGB_t thisCol = AllCharacters[selectedCharacter]->pressedStaticColour();
          kaimana.setLED(led, thisCol.r, thisCol.g, thisCol.b);
        }      
        else //random colour
        {
          setLEDRandomColor(led);
        }
        iLED[led] = true;
      }
    }
    else
    {
        // switch is inactive
        if(AllCharacters[selectedCharacter]->turnNonHeldButtonsOff() || AllCharacters[selectedCharacter]->useStaticColourInIdle() == false)
          kaimana.setLED(led, BLACK, false, AllCharacters[selectedCharacter]->holdPressedButtonColourTimeInMS(),  AllCharacters[selectedCharacter]->fadePressedButtonColourTimeInMS());
        else
        {
          RGB_t thisCol = AllCharacters[selectedCharacter]->idleStaticColour();
          kaimana.setLED(led, thisCol.r, thisCol.g, thisCol.b, false, AllCharacters[selectedCharacter]->holdPressedButtonColourTimeInMS(),  AllCharacters[selectedCharacter]->fadePressedButtonColourTimeInMS());
        }
      
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

  boolean bResetToBlack = false;
  if(AllCharacters[selectedCharacter]->turnNonHeldButtonsOff() || AllCharacters[selectedCharacter]->useStaticColourInIdle() == false)
    bResetToBlack = true;

  //if using joystick pcb diagonals and it changes then we need to reset all directional lights
  if(bUpdateJoystickDiagonals)
  {
    RGB_t thisCol = AllCharacters[selectedCharacter]->idleStaticColour();

    int ledArray[] = {LED_UP, LED_LEFT, LED_RIGHT, LED_DOWN};
    for(int index = 0; index < 4; ++index)
    {
      if(bResetToBlack)
        kaimana.setLED(ledArray[index], BLACK);
      else
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
    RGB_t thisCol = AllCharacters[selectedCharacter]->idleStaticColour();
    
    if(bResetToBlack)
      kaimana.setIndividualLED(firstDirection, BLACK);
    else
      kaimana.setIndividualLED(firstDirection, thisCol.r, thisCol.g, thisCol.b);
    if(bResetToBlack)
      kaimana.setIndividualLED(firstDirection+1, BLACK);
    else
      kaimana.setIndividualLED(firstDirection+1, thisCol.r, thisCol.g, thisCol.b);
    if(bResetToBlack)
      kaimana.setIndividualLED(secondDirection+2, BLACK);
    else
      kaimana.setIndividualLED(secondDirection+2, thisCol.r, thisCol.g, thisCol.b);
    if(bResetToBlack)
      kaimana.setIndividualLED(secondDirection+3, BLACK);
    else
      kaimana.setIndividualLED(secondDirection+3, thisCol.r, thisCol.g, thisCol.b);
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

int pollSwitches(void)
{
  static int  iActiveSwitchCount = 0;
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
  if(digitalRead(PIN_HOME) == BUTTON_READ_CHECK)
  {
    // switch is active
    if(iLED[LED_HOME] == true)
    {
      //maintain color while switch is active
      //Add custom "when held" code here
      
	   //Button hold to change idle animation
	  	holdTimeout += 1;

  		//Button hold to start tourneymode		
  		if(holdTimeout == 1000)
  		{
  		  tournamentMode = true;
  		  tourneyModeActivate();
  		}
    }
    else
    {
      // select new color when switch is first activated
      setLEDRandomColor(LED_HOME);
      iLED[LED_HOME] = true;
    }
  }
  else if(LED_HOME != 0xFF)
  {
      // switch is inactive
      kaimana.setLED(LED_HOME, BLACK);
      iLED[LED_HOME] = false;	  	 
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
  AllCharacters[selectedCharacter]->testForCharacterCombos();

  kaimana.blendLEDs();

  // zero active switch counter
  iActiveSwitchCount = 0;
  
  // set LED color based on switch
  for(i=0;i<LED_COUNT;++i)
  {
    if(iLED[i] == true)
      ++iActiveSwitchCount;
  }  

  // update the leds with new/current colors in the array
  kaimana.updateALL();

  //Add a small delay to give the update function time to recover
  delay( MIN_LED_UPDATE_DELAY );

  // return number of active switches
  return(iActiveSwitchCount);
}  

int tourneypollSwitches(void)
{
  static int  iLED[LED_COUNT];
  static int  iActiveSwitchCount = 0;
  static int  i;
  static int  j;
  static int  firsttime;
  static uint16_t  joystickDirection;
  static uint16_t  switchActivity;

  // test switch and set LED based on result       // HOME = GUIDE
  if(!digitalRead(PIN_HOME))
  {
	  //Button hold to change idle animation
		holdTimeout += 1;
		//Button hold to end tourneymode		
		if(holdTimeout == 200)
		{
  		tournamentMode = false;
  		tourneyModeDeactivate();
		}
    
    ++iActiveSwitchCount;
	  
	  delay(10);	  
  }
  else
  {  	 
  	holdTimeout=0;
  }
   
  return(iActiveSwitchCount);
}

 
