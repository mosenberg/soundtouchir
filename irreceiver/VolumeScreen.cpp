#include "VolumeScreen.hpp"
#include "img/Images.hpp"
#include <SSD1306Wire.h>
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
void VolumeScreen::forceDraw()
// Force draw
{
  display.clear();

  display.drawXbm(0, 0, images::loudspeaker_width, images::loudspeaker_height, images::loudspeaker);
  display.setFont(ArialMT_Plain_24);
  display.drawString(40, 5, String(volume));

  display.display();
}
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------