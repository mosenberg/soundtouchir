#pragma once
//---------------------------------------------------------------------
#include <Arduino.h>
#include "Screen.hpp"
#include "util/Defines.hpp"
#include <SSD1306Wire.h>
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
/// A rectangle
struct Rect {
  uint8_t x{0};
  uint8_t y{0};
  uint8_t width{0};
  uint8_t height{0};

  /// Constructor
  Rect() {}
  /// Constructor
  Rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) : x(x), y(y), width(width), height(height) { }

  /// Get the rightmost point
  uint8_t right() const { return x + width; }
  /// Get the bottom-most point
  uint8_t bottom() const { return y + height; }
};
//---------------------------------------------------------------------
/// Sliding text
class SlidingText {
  SSD1306Wire& display;
  /// The text
  String text;
  /// The width of the drawn text
  uint16_t textWidth{0};
  /// The font
  const uint8_t* font;
  /// The bounding box
  Rect boundingBox;
  /// The step size
  int8_t stepSize;
  /// The current shift of the text towards the left side
  uint16_t currPos;

  public:
  /// Constructor
  SlidingText(SSD1306Wire& display, const uint8_t* font, Rect boundingBox, int8_t stepSize);

  /// Get the text that is being animated
  const String& getText() const { return text; }
  /// Set new text
  void setText(String text);
  /// Set the bounding box
  void setBoundingBox(Rect newBounding) { boundingBox = std::move(newBounding); }
  /// Set the font
  void setFont(const uint8_t* font) { this->font = font; }

  /// Draw the next animation step
  void step();
};
//---------------------------------------------------------------------
class LoadingAnimation {
  /// An animated bar
  class AnimationBar {
    /// The current frame
    uint8_t currentFrame{0};
    /// The direction
    int8_t direction{1};
    /// The x offset
    uint8_t xOffs;

    public:
    /// Constructor
    AnimationBar() {}
    /// Constructor
    AnimationBar(uint8_t x, uint8_t startFrame) : currentFrame{startFrame}, xOffs{x} {}
    /// Draw a step, looks best when called every 60 ms
    void step(SSD1306Wire& display);
  };

  /// The number of animation bars
  static constexpr uint8_t barCount = 5;
  /// The animation bars
  AnimationBar bars[barCount];

  public:
  /// Constructor
  explicit LoadingAnimation(uint8_t x);

  /// Draw next animation phase, looks best when called every 60ms
  void step(SSD1306Wire& display);

  /// The width
  uint8_t width() const { return 32; }
  /// The height
  uint8_t height() const { return 32; }
};
//---------------------------------------------------------------------
}