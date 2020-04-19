#include "ConnectingScreen.hpp"
#include <SSD1306Wire.h>
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
/// Duration of a frame
constexpr unsigned long frameDuration = 60;
//---------------------------------------------------------------------
ConnectingScreen::ConnectingScreen(SSD1306Wire& display)
  : Screen(display), animation(0)
// Constructor
{
   forceDraw();
}
//---------------------------------------------------------------------
void ConnectingScreen::tick()
// Tick
{
  auto now = millis();
  auto diff = now - lastDraw;
  if (diff > frameDuration) {
    draw();
    lastDraw = now;
  }
}
//---------------------------------------------------------------------
void ConnectingScreen::forceDraw()
// Force draw
{
  draw();
  lastDraw = millis();
}
//---------------------------------------------------------------------
void ConnectingScreen::draw()
// Draw
{
  display.clear();

  animation.step(display);
  display.drawString(37, 8, "Connecting");

  display.display();
}
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------