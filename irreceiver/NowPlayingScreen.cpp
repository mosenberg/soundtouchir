#include "NowPlayingScreen.hpp"
#include <SSD1306Wire.h>
#include "SoundTouch.hpp"
#include "img/Images.hpp"
//---------------------------------------------------------------------
using namespace std;
using namespace SoundTouch;
//---------------------------------------------------------------------
constexpr unsigned long frameDuration = 60;
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
NowPlayingScreen::NowPlayingScreen(SSD1306Wire& display, const NowPlayingStatus& nowPlaying)
  : Screen(display), nowPlaying(nowPlaying), animation(0), slidingText(display, ArialMT_Plain_24, Rect{40, 5, 88, 27}, /*stepSize*/2)
// Constructor
{
  nowPlayingChanged(static_cast<NowPlayingChangeDescription>(~0)); // everything changed
  forceDraw();
}
//---------------------------------------------------------------------
void NowPlayingScreen::tick()
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
void NowPlayingScreen::forceDraw()
// Force draw
{
  draw();
}
//---------------------------------------------------------------------
static const uint8_t* getSourceImage(Source source)
// Get the source image
{
  switch (source) {
    case Source::Airplay: return images::sources::airplay;
    case Source::Bluetooth: return images::sources::bluetooth;
    case Source::TuneIn:
    case Source::Radioplayer: return images::sources::radio;
    case Source::Spotify: return images::sources::spotify;
    case Source::UPNP:
    case Source::StoredMusic: return images::sources::storedmusic;
    case Source::Aux: return images::sources::aux;
    case Source::Product: return images::sources::hdmi;
    case Source::Standby: return nullptr; // special case
    default: return images::sources::genericmusic;
  }
}
//---------------------------------------------------------------------
static const uint8_t* getPlayStatusImage(PlayStatus status)
// Get the image for the play status
{
  switch (status) {
    case PlayStatus::BUFFERING_STATE: return nullptr; // unreachable
    case PlayStatus::PLAY_STATE: return images::transportcontrol::play;
    case PlayStatus::PAUSE_STATE:
    case PlayStatus::STOP_STATE: return images::transportcontrol::pause;
  }
}
//---------------------------------------------------------------------
void NowPlayingScreen::nowPlayingChanged(NowPlayingChangeDescription change)
// Now playing changed
{
  if (change & NowPlayingChangeDescription::Song) {
    String s;
    s.reserve(sizeof(" - ") + nowPlaying.track.length() + nowPlaying.artist.length());
    if (nowPlaying.track.length() > 0) {
      s += nowPlaying.track;
    }
    if (nowPlaying.artist.length() > 0) {
      s += " - ";
      s += nowPlaying.artist;
    }
    slidingText.setText(move(s));
  }
  if (change & NowPlayingChangeDescription::Source) {
    if (nowPlaying.source == Source::Standby) {
      display.setContrast(10, 5, 0);
    } else {
      display.setContrast(100);
    }
  }
}
//---------------------------------------------------------------------
void NowPlayingScreen::draw()
// Draw
{
  display.clear();
  
  // Move the text (if any)
  slidingText.step();

  // Draw source image or buffering image
  if (nowPlaying.source == Source::Standby) {
    display.drawXbm(0, 0, images::hibernate_width, images::hibernate_height, images::hibernate);
    display.display();
    return;
  } else if (nowPlaying.playStatus == PlayStatus::BUFFERING_STATE) {
    animation.step(display);
    display.display();
    return;
  }

  display.drawXbm(0, 0, images::sources::width, images::sources::height, getSourceImage(nowPlaying.source));
  display.drawXbm(images::sources::width, 0, images::transportcontrol::width, images::transportcontrol::height, getPlayStatusImage(nowPlaying.playStatus));
  static_assert(images::sources::width + images::transportcontrol::width == 40, "didn't match expected width");
  static_assert(images::sources::height == images::transportcontrol::height, "didn't match expected width");

  // Draw repeat and shuffle status
  switch (nowPlaying.shuffleStatus) {
    case ShuffleStatus::SHUFFLE_OFF: break;
    case ShuffleStatus::SHUFFLE_ON:
      display.drawXbm(0, images::sources::height, images::playmode::shuffle_width, images::playmode::shuffle_height, images::playmode::shuffle);
      break;
  }
  switch (nowPlaying.repeatStatus) {
    case RepeatStatus::REPEAT_OFF: break;
    case RepeatStatus::REPEAT_ALL: {
      display.drawXbm(images::playmode::shuffle_width, images::sources::height, images::playmode::width, images::playmode::height, images::playmode::repeatall);
      break;
    }
    case RepeatStatus::REPEAT_ONE: {
      display.drawXbm(images::playmode::shuffle_width, images::sources::height, images::playmode::width, images::playmode::height, images::playmode::repeat_one);
      break;
    }
  }
  static_assert(images::playmode::shuffle_height == images::playmode::height, "");
  static_assert(images::playmode::shuffle_width + images::playmode::width == images::sources::width + images::transportcontrol::width, "");

  display.display();
}
//---------------------------------------------------------------------
}