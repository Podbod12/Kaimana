//  DeeJay.h
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
//  Hand written DeeJay character profile. Its special moves are in DeeJay.cpp
//  Archived template, not compiled from here. To use it copy it (and its .cpp) into src/StreetFighter6/, then pick "C++: DeeJay" for a slot in the editor.
//
//  Created:  November, 2023    Paul 'pod' Denning
//

#ifndef __DeeJay_h__
#define __DeeJay_h__

#include "../../animations.h" // must come before Characters.h as they include each other
#include "../../Characters.h"

class DeeJay : public Character
{
  private:
 
  public:
    virtual bool testForCharacterCombos( ) const override;
    
    //Example for RainbowCircle idle mode. Out of idle will be GREEN if not pressed and ORANGE if pressed
    //virtual RGB_t notPressedStaticColour(int ledIndex) const override { return getRGB(ORANGE); }; //Original gi colour
    //virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(GREEN); }; //New gi colour
};

#endif
