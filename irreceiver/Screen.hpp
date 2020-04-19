#pragma once
//---------------------------------------------------------------------
class SSD1306Wire;
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
enum class ScreenType : unsigned char {
  ConnectingScreen, NowPlayingScreen, VolumeScreen
 };
//---------------------------------------------------------------------
/// A screen
class Screen {
  protected:
  /// The display
  SSD1306Wire& display;

  public:
  /// Constructor
  Screen(SSD1306Wire& display) : display(display) {}
  /// Desturctor
  virtual ~Screen() {}


  /// Update the screen
  virtual void tick() = 0;
  /// Force drawing the screen
  virtual void forceDraw() = 0;

  /// Get the type of the screen
  virtual ScreenType getScreenType() const = 0;
};
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------