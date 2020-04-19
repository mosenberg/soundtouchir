#pragma once
//---------------------------------------------------------------------
#include <SSD1306Wire.h>
#include "SoundTouch.hpp"
#include "ConnectingScreen.hpp"
#include "NowPlayingScreen.hpp"
#include "VolumeScreen.hpp"
#include "util/Variant.hpp"
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
class ScreenManager {
  /// The display class
  SSD1306Wire  display;
  /// The device
  SoundTouch::Device& device;

  using Screens = Variant<ConnectingScreen, NowPlayingScreen, VolumeScreen>;
  /// The screens
  Screens screens;
  /// The active screen
  Screen* activeScreen{nullptr};
  /// Time when the last screen was set
  unsigned long lastScreenCreated{0};

  public:
  /// Constructor
  ScreenManager(uint8_t sda, uint8_t scl, SoundTouch::Device& device);

  /// Setup the display
  void begin();

  public:
  /// Now playing has changed
  void nowPlayingChanged(SoundTouch::Device&, const SoundTouch::NowPlayingChangeDescription&);
  /// Volume has changed
  void volumeChanged(SoundTouch::Device&, int16_t volume);
  /// Connection status has changed
  void connectionStatusChanged(SoundTouch::Device&, bool connected);

  /// Called from loop() to update screen
  void tick();
};
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------