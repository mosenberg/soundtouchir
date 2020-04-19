#pragma once
//---------------------------------------------------------------------
#include "Screen.hpp"
#include "UITools.hpp"
#include "util/Defines.hpp"
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
/// The screen that is displayed when establishing connections
class VolumeScreen final : public Screen {
  /// The volume
  int16_t volume;

  public:
  /// Constructor
  explicit VolumeScreen(SSD1306Wire& display, int16_t volume) : Screen(display), volume(volume) { forceDraw(); }
  /// Destructor
  ~VolumeScreen() {}
  
  /// Tick
  void tick() override { }
  /// Force draw
  void forceDraw() override;
  /// Get the screen type
  ScreenType getScreenType() const override { return ScreenType::VolumeScreen; }
};
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------