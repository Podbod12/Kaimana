//  Akuma.cpp
//
//  Copyright 2023 Paradise Arcade Shop, ParadiseArcadeShop.com  
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
//  Kaimana Akuma character combo checks and animation calls. First released by ParadiseArcadeShop.com November, 2023
//  Characters are added into the character selection array in the main ino file. See that for info on how characters are selected
//
//  Created:  Oct, 2024    Paul 'pod' Denning
//

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "kaimana.h"
#include "kaimana_custom.h"
#include "animations.h"
#include "Characters.h"

// Define combo sequences here and corresponding animations. Most complex moves should go first (eg, supers, ultras)
bool Akuma::testForCharacterCombos() const
{
  EInputTypes K1K2Array[] = {EIT_Input_K1, EIT_Input_K2};
  EInputTypes K1K3Array[] = {EIT_Input_K1, EIT_Input_K3};
  EInputTypes K2K3Array[] = {EIT_Input_K2, EIT_Input_K3};

  EInputTypes P3K1Array[] = {EIT_Input_P3, EIT_Input_K1};

  EInputTypes LeftArray[] = {EIT_Input_Left};
  EInputTypes RightArray[] = {EIT_Input_Right};

  //Shun goku satsu left
  if( kaimana.switchHistoryTest( HELL_MURDER_INPUT_LEFT, HELL_MURDER_INPUT_COUNT, P3K1Array, 2, false ))
  {
    Randomise_Combo_Animation(6, 120, 60, RED);
    Randomise_Combo_Animation(12, 70, 45, RED);
    Randomise_Combo_Animation(24, 30, 30, RED);
    FlashColour_Combo_Animation(RED, 750);
    return true;
  }
 
  //Shun goku satsu Right
  if( kaimana.switchHistoryTest( HELL_MURDER_INPUT_RIGHT, HELL_MURDER_INPUT_COUNT, P3K1Array, 2, false ))
  {
    Randomise_Combo_Animation(6, 120, 60, RED);
    Randomise_Combo_Animation(12, 70, 45, RED);
    Randomise_Combo_Animation(24, 30, 30, RED);
    FlashColour_Combo_Animation(RED, 750);
    return true;
  }
  
  
  //Super Dragon punch (both sides)
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ))
  {
    FlashAllSpeedIncreasing_Combo_Animation(RED);
    return true;
  }

  
  //Super fireball right
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Medium, 2, PURPLE);  
    return true;
  }
    
  //Super fireball left
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Medium, 2, PURPLE);  
    return true;
  }


  //Teleport Left
  if( kaimana.switchHistoryTest( LeftArray, 1, TRIGGER_KKK, TRIPLEATTACK_TRIGGER_COUNT, false ) ||
      kaimana.switchHistoryTest( LeftArray, 1, K1K2Array, 2, false ) ||
      kaimana.switchHistoryTest( LeftArray, 1, K1K3Array, 2, false ) ||
      kaimana.switchHistoryTest( LeftArray, 1, K2K3Array, 2, false ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Slow, 0, DARKGREY);  
    return true;
  }

  //Teleport Right
  if( kaimana.switchHistoryTest( RightArray, 1, TRIGGER_KKK, TRIPLEATTACK_TRIGGER_COUNT, false ) ||
      kaimana.switchHistoryTest( RightArray, 1, K1K2Array, 2, false ) ||
      kaimana.switchHistoryTest( RightArray, 1, K1K3Array, 2, false ) ||
      kaimana.switchHistoryTest( RightArray, 1, K2K3Array, 2, false ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Slow, 0, DARKGREY);  
    return true;
  }

  
  //Dragon punch right
  if( kaimana.switchHistoryTest( COMBO_DP_LEFT, DP_INPUT_COUNT, P1Array, 1, false ) )
  {
    FlashColour_Combo_Animation(WHITE, 250);
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_DP_LEFT, DP_INPUT_COUNT, P2Array, 1, false ) )
  {
    FlashColour_Combo_Animation(WHITE, 500);
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_DP_LEFT, DP_INPUT_COUNT, P3Array, 1, false ) )
  {
    FlashColour_Combo_Animation(WHITE, 750);
    return true;
  }
    
  //Dragon punch left
  if( kaimana.switchHistoryTest( COMBO_DP_RIGHT, DP_INPUT_COUNT, P1Array, 1, false ) )
  {
    FlashColour_Combo_Animation(WHITE, 250);
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_DP_RIGHT, DP_INPUT_COUNT, P2Array, 1, false ) )
  {
    FlashColour_Combo_Animation(WHITE, 500);
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_DP_RIGHT, DP_INPUT_COUNT, P3Array, 1, false ) )
  {
    FlashColour_Combo_Animation(WHITE, 750);
    return true;
  }

  //Fireball to the right
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Slow, 0, PURPLE);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Medium, 0, PURPLE);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Fast, 0, PURPLE);  
    return true;
  }
  //Fireball to the left
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Slow, 0, PURPLE);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Medium, 0, PURPLE);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Fast, 0, PURPLE);  
    return true;
  }

  //Hurricane kick
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) )
  {
    Circle_OneColour_Combo_Animation(1, WHITE);  
    return true;
  }

  return false;
} 
