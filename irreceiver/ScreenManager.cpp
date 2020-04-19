#include "ScreenManager.hpp"
//---------------------------------------------------------------------
using namespace SoundTouch;
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
ScreenManager::ScreenManager(uint8_t sda, uint8_t scl, Device& device)
  : display(/*address*/0x3c, sda, scl, GEOMETRY_128_32), device(device)
// Constructor
{
}
//---------------------------------------------------------------------
void ScreenManager::begin()
// Begin
{
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);

  connectionStatusChanged(device, false);
  device.registerConnectionStatusChangedListener([this](Device& device, bool connected) {
    connectionStatusChanged(device, connected);
  });
  device.registerNowPlayingChangedListener([this](Device& device, NowPlayingChangeDescription changed) {
    nowPlayingChanged(device, changed);
  });
  device.registerVolumeChangedListener([this](Device& device, int16_t volume) {
    lastScreenCreated = millis();
    if (activeScreen) {
      activeScreen->~Screen();
    }
    activeScreen = &screens.emplace<VolumeScreen>(display, volume);
  });
}
//---------------------------------------------------------------------
void ScreenManager::nowPlayingChanged(Device& device, const NowPlayingChangeDescription& changed)
{
  if (activeScreen) {
    if (activeScreen->getScreenType() == ScreenType::NowPlayingScreen) {
      screens.get<NowPlayingScreen>().nowPlayingChanged(changed);
      return;
    }
    activeScreen->~Screen();
  }
  activeScreen = &screens.emplace<NowPlayingScreen>(display, device.getNowPlaying());
}
//---------------------------------------------------------------------
void ScreenManager::connectionStatusChanged(Device& device, bool connected)
// Connection status changed
{
  if (connected) {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0,0,"Connected");
    display.display();
  } else {
    if (activeScreen) {
      if (activeScreen->getScreenType() == ScreenType::ConnectingScreen) {
        return;
      }
      activeScreen->~Screen();
    }
    activeScreen = &screens.emplace<ConnectingScreen>(display);
  }
}
//---------------------------------------------------------------------
void ScreenManager::tick()
// Tick
{
  if (activeScreen) {
    if ((activeScreen->getScreenType() == ScreenType::VolumeScreen) && (millis() - lastScreenCreated) > 1200) {
      nowPlayingChanged(device, NowPlayingChangeDescription::NoChange);
    }
    activeScreen->tick();
  }
}
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------