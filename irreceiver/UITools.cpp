#include "UITools.hpp"
#include <SSD1306Wire.h>
#include <cstdlib>
#include <memory>
//---------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
SlidingText::SlidingText(SSD1306Wire& display, const uint8_t* font, Rect boundingBox, int8_t stepSize)
  : display(display), font(font), boundingBox(move(boundingBox)), stepSize(stepSize), currPos(1)
// Constructor
{
}
//---------------------------------------------------------------------
void SlidingText::setText(String text)
{
  this->text = move(text);
  // We call the getStringWidth method that takes pointers, because it doesn't do utf-8 conversion.
  // We already did that in the XML-parser, so don't do it twice.
  textWidth = display.getStringWidth(this->text.c_str(), this->text.length());
  currPos = 1;
}
//---------------------------------------------------------------------
void SlidingText::step()
// Draw next step
{
  if (textWidth == 0) return;

  display.setFont(font);

  int32_t stringPos = static_cast<int32_t>(boundingBox.right()) - currPos;
  display.drawStringInternal(stringPos, boundingBox.y, text.c_str(), text.length(), textWidth);

  currPos += stepSize;
  if (currPos >= textWidth + (2 * display.getWidth())) {
    currPos = 1;
  }

  display.setColor(OLEDDISPLAY_COLOR::BLACK);
  // This is a hack, because it clears only the left side - I don't need anything else right now in my project
  display.fillRect(0, 0, boundingBox.x, display.height());

  display.setColor(OLEDDISPLAY_COLOR::WHITE);
}
//---------------------------------------------------------------------
constexpr size_t amplitudesCount = 8;
constexpr float amplitudes[amplitudesCount] = {1, 0.96, 0.85, 0.69, 0.5, 0.3, 0.14, 0};
constexpr uint8_t minHeight = 10;
constexpr uint8_t maxHeight = 28;
constexpr uint8_t topOffset = 2;
constexpr uint8_t screenHeight = 32;
//---------------------------------------------------------------------
void LoadingAnimation::AnimationBar::step(SSD1306Wire& display)
// Step
{
  auto ampl = amplitudes[currentFrame];
  if (currentFrame == 0 && direction == -1) {
    direction = 1;
  } else if (currentFrame == amplitudesCount - 1) {
    direction = -1;
  }
  currentFrame += direction;

  auto currentHeight = minHeight + (maxHeight - minHeight) * ampl;
  auto y = topOffset + (maxHeight - currentHeight)/2;

  display.drawRect(xOffs, y, 2, currentHeight);
}
//---------------------------------------------------------------------
LoadingAnimation::LoadingAnimation(uint8_t x)
{
  new (&bars[0]) AnimationBar(x + 1, 0);
  new (&bars[1]) AnimationBar(x + 8, 3);
  new (&bars[2]) AnimationBar(x + 15, 7);
  new (&bars[3]) AnimationBar(x + 23, 3);
  new (&bars[4]) AnimationBar(x + 29, 0);
}
//---------------------------------------------------------------------
void LoadingAnimation::step(SSD1306Wire& display)
{
  for (uint8_t i = 0; i < barCount; ++i) {
    bars[i].step(display);
  }
}
//---------------------------------------------------------------------
}