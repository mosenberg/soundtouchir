#pragma once
//---------------------------------------------------------------------
#include "Screen.hpp"
#include "UITools.hpp"
#include "util/Defines.hpp"
//---------------------------------------------------------------------
namespace SoundTouch { class NowPlayingStatus; enum class NowPlayingChangeDescription : uint8_t; }
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
class NowPlayingScreen final : public Screen {
  /// The now playing information
  const SoundTouch::NowPlayingStatus& nowPlaying;
  /// Last draw 
  unsigned long lastDraw{0};
  /// The loading animation
  LoadingAnimation animation;
  /// The sliding text
  SlidingText slidingText;

  public:
  /// Constructor
  NowPlayingScreen(SSD1306Wire& display, const SoundTouch::NowPlayingStatus& nowPlaying);
  /// Destructor
  ~NowPlayingScreen() { }

  /// Tick
  void tick() override;
  /// Force draw
  void forceDraw() override;
  /// Now playing information has changed
  void nowPlayingChanged(SoundTouch::NowPlayingChangeDescription);

  /// Get the screen type
  virtual ScreenType getScreenType() const override { return ScreenType::NowPlayingScreen; }

  private:
  /// Draw
  void draw();
};
//---------------------------------------------------------------------
}