#pragma once
//---------------------------------------------------------------------
#include "Screen.hpp"
#include "UITools.hpp"
#include "util/Defines.hpp"
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
/// The screen that is displayed when establishing connections
class ConnectingScreen final : public Screen {
  /// When did we draw the last time
  unsigned long lastDraw{0};
  /// The animation
  LoadingAnimation animation;

  public:
  /// Constructor
  explicit ConnectingScreen(SSD1306Wire& display);
  /// Destructor
  ~ConnectingScreen() {}
  
  /// Tick
  void tick() override;
  /// Force draw
  void forceDraw() override;
  /// Get the screen type
  ScreenType getScreenType() const override { return ScreenType::ConnectingScreen; }

  private:
  /// Draw
  void draw();
};
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------