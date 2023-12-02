//  Ryu.cc
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
//  Kaimana Zangief character combo checks and animation calls. First released by ParadiseArcadeShop.com November, 2023
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
void Gief::testForCharacterCombos()
{
  EInputTypes P1Array[] = {EIT_Input_P1};
  EInputTypes P2Array[] = {EIT_Input_P2};
  EInputTypes P3Array[] = {EIT_Input_P3};
  EInputTypes K1Array[] = {EIT_Input_K1};
  EInputTypes K2Array[] = {EIT_Input_K2};
  EInputTypes K3Array[] = {EIT_Input_K3};

  EInputTypes P1P2Array[] = {EIT_Input_P1, EIT_Input_P2};
  EInputTypes P1P3Array[] = {EIT_Input_P1, EIT_Input_P3};
  EInputTypes P2P3Array[] = {EIT_Input_P2, EIT_Input_P3};

  //Buster (both ways) 
  for(int index = 0; index < 3; ++index)
  {
    if( kaimana.switchHistoryTest( COMBO_630_LEFT_CLOCKWISE, SIXTHIRTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_630_UP_CLOCKWISE, SIXTHIRTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_630_RIGHT_CLOCKWISE, SIXTHIRTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_630_DOWN_CLOCKWISE, SIXTHIRTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_630_LEFT_ANTICLOCKWISE, SIXTHIRTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_630_UP_ANTICLOCKWISE, SIXTHIRTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_630_RIGHT_ANTICLOCKWISE, SIXTHIRTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_630_DOWN_ANTICLOCKWISE, SIXTHIRTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) )
    {
      CircleRGB_Combo_Animation(1);  
      FlashAllSpeedIncreasing_Combo_Animation(RED);
    }
  }
  
  //Cyclone Lariat (both sides)
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, P3Array, 1, false ))
    Circle_OneColour_Combo_Animation(4, RED);  

  
  //AerialSlam (both sides)
  if( kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_RIGHT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_DOUBLE_QUARTERCIRCLE_LEFT, DOUBLE_QUARTERCIRCLE_INPUT_COUNT, K3Array, 1, false ) )
  {
    WaveEffect_Combo_Animation(EFT_DownToUp, EFS_Medium, 1, GOLD);  
    WaveEffect_Combo_Animation(EFT_UpToDown, EFS_Medium, 1, GOLD);  
  }


  //Lariat
  if( kaimana.switchHistoryTest( {}, 0, TRIGGER_PPP, TRIPLEATTACK_TRIGGER_COUNT, false ) ||
      kaimana.switchHistoryTest( {}, 0, P1P2Array, 2, false ) ||
      kaimana.switchHistoryTest( {}, 0, P1P3Array, 2, false ) ||
      kaimana.switchHistoryTest( {}, 0, P2P3Array, 2, false ) )
    KnightRider_Combo_Animation(2, true, RED);  


  //bear grab to the right
  if( kaimana.switchHistoryTest( COMBO_HALFCIRCLE_RIGHT, HALFCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_HALFCIRCLE_RIGHT, HALFCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_HALFCIRCLE_RIGHT, HALFCIRCLE_INPUT_COUNT, K3Array, 1, false ) )
    Circle_OneColour_Combo_Animation(2, GOLD);  

  //bear grab to the left
  if( kaimana.switchHistoryTest( COMBO_HALFCIRCLE_LEFT, HALFCIRCLE_INPUT_COUNT, K1Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_HALFCIRCLE_LEFT, HALFCIRCLE_INPUT_COUNT, K2Array, 1, false ) ||
      kaimana.switchHistoryTest( COMBO_HALFCIRCLE_LEFT, HALFCIRCLE_INPUT_COUNT, K3Array, 1, false ) )
    Circle_OneColour_Combo_Animation(2, GOLD);  


  //piledriver 
  for(int index = 0; index < 3; ++index)
  {
    if( kaimana.switchHistoryTest( COMBO_270_LEFT_CLOCKWISE, TWOSEVENTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_270_UP_CLOCKWISE, TWOSEVENTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_270_RIGHT_CLOCKWISE, TWOSEVENTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_270_DOWN_CLOCKWISE, TWOSEVENTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_270_LEFT_ANTICLOCKWISE, TWOSEVENTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_270_UP_ANTICLOCKWISE, TWOSEVENTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_270_RIGHT_ANTICLOCKWISE, TWOSEVENTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) ||
        kaimana.switchHistoryTest( COMBO_270_DOWN_ANTICLOCKWISE, TWOSEVENTY_INPUT_COUNT, index == 0 ? P1Array : (index == 1 ? P2Array : P3Array), 1, false ) )
    {
      Circle_OneColour_Combo_Animation(2, RED);  
    }
  }
} 
