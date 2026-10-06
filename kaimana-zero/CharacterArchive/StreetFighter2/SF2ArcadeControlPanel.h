//  SF2ArcadeControlPanel.h
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
//  Hand written example profile with no special moves, so there is no .cpp for it.
//  Archived template, not compiled from here. To use it copy it into src/StreetFighter2/, then pick "C++: SF2ArcadeControlPanel" for a slot in the editor.
//
//  Created:  November, 2023    Paul 'pod' Denning
//

#ifndef __SF2ArcadeControlPanel_h__
#define __SF2ArcadeControlPanel_h__

#include "../../animations.h" // must come before Characters.h as they include each other
#include "../../Characters.h"

//This is an example profile thats not actually a real character but shows how to customise individual button colours. In this case, they match the original sf2 machine and turn green when pressed
class SF2ArcadeControlPanel : public Character
{
  private:
 
  public:
    virtual EIdleType getIdleAnimationType() const override { return EIT_Disabled; }
  
    virtual RGB_t notPressedStaticColour(int ledIndex) const override
    {
      if(ledIndex == LED_P1 || ledIndex == LED_K1)
        return getRGB(RED);
       if(ledIndex == LED_P2 || ledIndex == LED_K2)
        return getRGB(WHITE);
      if(ledIndex == LED_P3 || ledIndex == LED_K3)
        return getRGB(BLUE);

      //all other buttons and directions
      return getRGB(YELLOW); 
    }; 
    
    virtual RGB_t pressedStaticColour(int ledIndex) const override { return getRGB(GREEN); };
};

#endif
