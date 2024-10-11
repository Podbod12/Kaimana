//  Chun.cpp
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
//  Kaimana Chun-li character combo checks and animation calls. First released by ParadiseArcadeShop.com November, 2023
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
bool Chun::testForCharacterCombos() const
{
  //Critical (both sides)
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false )||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ))
  {
    FlashAllSpeedIncreasing_Combo_Animation(BLUE);
    return true;
  }
  
  
  //Super fireball right
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Medium, 2, CYAN);  
    return true;
  }
    
  //Super fireball left
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Medium, 2, CYAN);  
    return true;
  }


  //Hundred kicks (both sides)
  if( kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_RIGHT, QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_QUARTERCIRCLE_LEFT, QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) )
  {
    for(int index = 0; index < 8; ++index)
      FlashColour_Combo_Animation(BLUE, 100);
    return true;
  }


  //Spinning Bird kick 
  if( kaimana.switchHistoryTest( COMBO_CHARGE_DOWN_UP, CHARGE_INPUT_COUNT, K1Array, 1, true ) ||
      kaimana.switchHistoryTest( COMBO_CHARGE_DOWN_UP, CHARGE_INPUT_COUNT, K2Array, 1, true ) ||
      kaimana.switchHistoryTest( COMBO_CHARGE_DOWN_UP, CHARGE_INPUT_COUNT, K3Array, 1, true ) )
  {
    KnightRider_Combo_Animation(2, false, BLUE);  
    return true;
  }

  //Fireball Right
  if( kaimana.switchHistoryTest( COMBO_CHARGE_LEFT_RIGHT, CHARGE_INPUT_COUNT, P1Array, 1, true ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Slow, 0, BLUE);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_CHARGE_LEFT_RIGHT, CHARGE_INPUT_COUNT, P2Array, 1, true ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Medium, 0, BLUE);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_CHARGE_LEFT_RIGHT, CHARGE_INPUT_COUNT, P3Array, 1, true ) )
  {
    WaveEffect_Combo_Animation(EFT_LeftToRight, EFS_Fast, 0, BLUE);  
    return true;
  }

  //Fireball left
  if( kaimana.switchHistoryTest( COMBO_CHARGE_RIGHT_LEFT, CHARGE_INPUT_COUNT, P1Array, 1, true ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Slow, 0, BLUE);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_CHARGE_RIGHT_LEFT, CHARGE_INPUT_COUNT, P2Array, 1, true ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Medium, 0, BLUE);  
    return true;
  }
  if( kaimana.switchHistoryTest( COMBO_CHARGE_RIGHT_LEFT, CHARGE_INPUT_COUNT, P3Array, 1, true ) )
  {
    WaveEffect_Combo_Animation(EFT_RightToLeft, EFS_Fast, 0, BLUE);  
    return true;
  }

  return false;
} 
