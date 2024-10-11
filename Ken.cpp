//  Ken.cpp
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
//  Kaimana Ken character combo checks and animation calls. First released by ParadiseArcadeShop.com November, 2023
//  Characters are added into the character selection array in the main ino file. See that for info on how characters are selected
//
//  Created:  November, 2023    Paul 'pod' Denning
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
bool Ken::testForCharacterCombos() const
{
  //Super Dragon punch right
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) )
  {
    FlashAllSpeedIncreasing_Combo_Animation(RED);
    return true;
  }

  
  //Super kicks right
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Medium, 2, RED);  
    return true;
  }
    
  //Super kicks left
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Medium, 2, RED);  
    return true;
  }


  //Dragon punch right
  if( kaimana.switchHistoryTest( COMBO_DP_LEFT, DP_INPUT_COUNT, P1Array, 1, false ) )
  {
    FlashColour_Combo_Animation(RED, 250);
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_DP_LEFT, DP_INPUT_COUNT, P2Array, 1, false ) )
  {
    FlashColour_Combo_Animation(RED, 500);
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_DP_LEFT, DP_INPUT_COUNT, P3Array, 1, false ) )
  {
    FlashColour_Combo_Animation(RED, 750);
    return true;
  }
    
  //Dragon punch left
  if( kaimana.switchHistoryTest( COMBO_DP_RIGHT, DP_INPUT_COUNT, P1Array, 1, false ) )
  {
    FlashColour_Combo_Animation(RED, 250);
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_DP_RIGHT, DP_INPUT_COUNT, P2Array, 1, false ) )
  {
    FlashColour_Combo_Animation(RED, 500);
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_DP_RIGHT, DP_INPUT_COUNT, P3Array, 1, false ) )
  {
    FlashColour_Combo_Animation(RED, 750);
    return true;
  }


  //Fireball to the right
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Slow, 0, CYAN);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Medium, 0, CYAN);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Fast, 0, CYAN);  
    return true;
  }

  //Fireball to the left
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Slow, 0, CYAN);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Medium, 0, CYAN);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Fast, 0, CYAN);  
    return true;
  }


  //Hurricane kick (both sides)
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) )
  {
    Circle_OneColour_Combo_Animation(1, RED);  
    return true;
  }

  return false;
} 
