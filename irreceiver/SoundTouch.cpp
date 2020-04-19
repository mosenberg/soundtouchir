#include "SoundTouch.hpp"
#include "LeanStringStream.hpp"
#include "BoseIR.hpp"
#include "XmlReader.hpp"
#include <algorithm>
#include <type_traits>
//---------------------------------------------------------------------
using namespace hox;
using namespace std;
using namespace BoseIR;
//---------------------------------------------------------------------
namespace SoundTouch {
//---------------------------------------------------------------------
bool getSource(Span v, Source& result)
// Get source
{
  if (v=="STANDBY") {
    result = Source::Standby;
    return true;
  } else if (v=="AUX") {
    result = Source::Aux;
    return true;
  } else if (v=="BLUETOOTH") {
    result = Source::Bluetooth;
    return true;
  } else if (v=="PRODUCT") {
    result = Source::Product;
    return true;
  } else if (v=="TUNEIN") {
    result = Source::TuneIn;
    return true;
  } else if (v=="SPOTIFY") {
    result = Source::Spotify;
    return true;
  } else if (v=="UPNP") {
    result = Source::UPNP;
    return true;
  } else if (v=="STORED_MUSIC") {
    result = Source::StoredMusic;
    return true;
  } else if (v=="NOTIFICATION") {
    result = Source::Notification;
    return true;
  } else if (v=="INVALID_SOURCE") {
    result = Source::INVALID_SOURCE;
    return true;
  } else if (v=="AIRPLAY") {
    result = Source::Airplay;
    return true;
  } else if (v=="RADIOPLAYER") {
    result = Source::Radioplayer;
    return true;
  }
  return false;
}
//---------------------------------------------------------------------
const char* sourceToString(Source s) 
{
  switch (s) {
    case Source::Standby: return "Standby";
    case Source::Aux: return "Aux"; 
    case Source::Bluetooth: return "Bluetooth"; 
    case Source::Product: return "Product"; 
    case Source::Radioplayer: return "Radioplayer"; 
    case Source::TuneIn: return "TuneIn"; 
    case Source::Spotify: return "Spotify"; 
    case Source::UPNP: return "UPNP"; 
    case Source::StoredMusic: return "StoredMusic"; 
    case Source::Notification: return "Notification"; 
    case Source::Airplay: return "Airplay"; 
    case Source::INVALID_SOURCE: return "INVALID_SOURCE"; 
  }
}
//---------------------------------------------------------------------
bool getRepeatStatus(Span v, RepeatStatus& result)
// Get repeat status
{
  if (v=="REPEAT_OFF") {
    result = RepeatStatus::REPEAT_OFF;
    return true;
  } else if (v=="REPEAT_ALL") {
    result = RepeatStatus::REPEAT_ALL;
    return true;
  } else if (v=="REPEAT_ONE") {
    result = RepeatStatus::REPEAT_ONE;
    return true;
  }
  return false;
}
//---------------------------------------------------------------------
bool getShuffleStatus(Span v, ShuffleStatus& result)
// Get shuffle status
{
  if (v=="SHUFFLE_OFF") {
    result = ShuffleStatus::SHUFFLE_OFF;
    return true;
  } else if (v=="SHUFFLE_ON") {
    result = ShuffleStatus::SHUFFLE_ON;
    return true;
  }
  return false;
}
//---------------------------------------------------------------------
const char* shuffleStatusToString(ShuffleStatus s)
{
  switch (s) {
    case ShuffleStatus::SHUFFLE_ON: return "SHUFFLE_ON";
    case ShuffleStatus::SHUFFLE_OFF: return "SHUFFLE_OFF";
  }
}
//---------------------------------------------------------------------
bool getPlayStatus(Span v, PlayStatus& result)
// Get repeat status
{
  if (v=="PLAY_STATE") {
    result = PlayStatus::PLAY_STATE;
    return true;
  } else if (v=="PAUSE_STATE") {
    result = PlayStatus::PAUSE_STATE;
    return true;
  } else if (v=="STOP_STATE") {
    result = PlayStatus::STOP_STATE;
    return true;
  } else if (v=="BUFFERING_STATE") {
    result = PlayStatus::BUFFERING_STATE;
    return true;
  }
  return false;
}
//---------------------------------------------------------------------
const char* playStatusToString(PlayStatus s)
{
  switch (s)
  {
    case PlayStatus::PLAY_STATE: return "PLAY_STATE";
    case PlayStatus::PAUSE_STATE: return "PAUSE_STATE";
    case PlayStatus::STOP_STATE: return "STOP_STATE";
    case PlayStatus::BUFFERING_STATE: return "BUFFERING_STATE";
  }
}
//---------------------------------------------------------------------
template<typename T>
struct InvokeBodyGeneratorHelper {
  T fn;
  InvokeBodyGeneratorHelper(T&& fn) : fn(fn) {}
  void operator()(LeanStringStreamBase& lss) {
    lss << "<body>";
    fn(lss);
    lss << "</body>";
  }
};
//---------------------------------------------------------------------
template<>
struct InvokeBodyGeneratorHelper<nullptr_t> {
  InvokeBodyGeneratorHelper(void*) {}
  void operator()(LeanStringStreamBase&) {}
};
//---------------------------------------------------------------------
template<unsigned N = 0, typename Fun = nullptr_t>
static String requestUrl(WebSocketsClient& client, const char* url, const char* method = "GET", Fun&& body = nullptr)
{
  static uint32_t requestCounter = 0;
  LeanStringStream<256 + N> lss;
  lss << "<msg>";
  lss << "<header";
  lss << " deviceID=\"" << "B7AB7A" << "\""; //TODO
  lss << " url=\"" << url << "\"";
  lss << " method=\"" << method << "\"";
  lss << ">";
  lss << "<request requestID=\"" << requestCounter++ << "\">";
  lss << "<info type=\"new\"/>";
  lss << "</request>";
  lss << "</header>";
  InvokeBodyGeneratorHelper<Fun>(std::forward<Fun>(body))(lss);
  lss << "</msg>";
  client.sendTXT(lss.data(), lss.length());
  return String(lss.data());
}
//---------------------------------------------------------------------
Device::Device(const char* url)
  : url(url)
// Constructor
{
  
}
//---------------------------------------------------------------------
bool Device::begin()
// To be called in setup to register websockets
{
  websocket.begin(url, 8080, "/", "gabbo");

  websocket.onEvent([this] (WStype_t type, uint8_t* payload, size_t length) {
    websocketEvent(type, payload, length);
  });

  // Try reconnection every 5 sec if connection failed once
  websocket.setReconnectInterval(5000);
  
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  websocket.enableHeartbeat(15000, 3000, 2);
}
//---------------------------------------------------------------------
void Device::run()
// To be called in loop() when websockets are registered
{
  websocket.loop();
}
//---------------------------------------------------------------------
void Device::websocketEvent(WStype_t type, uint8_t* payload, size_t length)
// Callback on websocket event
{
  switch(type) {
    case WStype_DISCONNECTED:
      connected = false;
      if (connectionStatusChangedListener) {
        connectionStatusChangedListener(*this, false);
      }
      debugMessage = "Disconnected";
      break;
    case WStype_CONNECTED:
      connected = true;
      if (connectionStatusChangedListener) {
        connectionStatusChangedListener(*this, true);
      }
      debugMessage = "Connected";
      requestVolume();
      requestNowPlaying();
      break;
    case WStype_TEXT: {
      debugMessage = "Received text:\n";
      Span data(reinterpret_cast<char*>(payload), length);
      debugMessage += data.toString();
      debugMessage += "\n";
      // Check what we received
      XmlDocument xd(data);
      if (xd.getName() == "updates") {
        handleUpdates(xd.getBody());
      } else if (xd.getName() == "msg") {
        handleMessage(xd.getBody());
      }
      break;
    }
    case WStype_BIN:
    case WStype_PING:
    case WStype_PONG:
      break;
    }
}
//---------------------------------------------------------------------
void Device::handleUpdates(XmlDocument doc)
{
  debugMessage += "\nUpdate: ";
  debugMessage += doc.getName().toString();
  if (doc.getName() == "nowPlayingUpdated") {
    handleNowPlayingUpdated(doc.getBody());
  } else if (doc.getName() == "volumeUpdated") {
    handleVolumeUpdated(doc.getBody());
  }
}
//---------------------------------------------------------------------
void Device::handleMessage(XmlDocument doc)
{
  XmlDocument header = doc;
  XmlDocument body = header.getSibling("body");

  Span url = header.getAttribute("url");
  if (url) {
    debugMessage += "\nMessage: ";
    debugMessage += url.toString();
    if (url == "volume") {
      handleVolumeUpdated(body.getBody());
    } else if (url == "now_playing") {
      handleNowPlayingUpdated(body.getBody());
    }
  } else {
    debugMessage += "\nMessage: <unknown>";
  }
}
//---------------------------------------------------------------------
template<typename T>
struct ChangeHelper {
  static bool isChange(const T& v) { return true; }
  static T&& extractValue(T&& v) { return forward<T>(v); }
};
//---------------------------------------------------------------------
template<>
struct ChangeHelper<pair<bool, String>> {
  static bool isChange(const pair<bool, String>& v) { return v.first; }
  static String extractValue(pair<bool, String>&& v) { return move(v.second); }
};
//---------------------------------------------------------------------
/// Record change if needed
template<typename T, typename U>
static void recordChange(T& target, U&& newValue, NowPlayingChangeDescription& changed, NowPlayingChangeDescription tag) {
  if (!ChangeHelper<U>::isChange(newValue)) return;
  auto v = ChangeHelper<U>::extractValue(forward<U>(newValue));
  if (target != v) {
    target = move(v);
    changed = changed | tag;
  }
}
//---------------------------------------------------------------------
void Device::handleNowPlayingUpdated(XmlDocument doc)
{
  auto getAttr = [&](const char* name) {
    Span attr = doc.getAttribute(name);
    if (attr) {
      return attr.toString();
    } else {
      return String();
    }
  };
  XmlDocument body = doc.getBody();
  auto getElem = [&](const char* name)->pair<bool, String> {
    XmlDocument x = body.getSibling(name);
    if (x) {
      return {true, x.getBodyAsText()};
    } else {
      return {false, {}};
    }
  };
  auto getElemRaw = [&](const char* name) {
    XmlDocument x = body.getSibling(name);
    if (x) {
      return x.getBodyAsTextRaw();
    } else {
      return Span();
    }
  };

  NowPlayingChangeDescription changed = NowPlayingChangeDescription::NoChange;

  Span source = doc.getAttribute("source");
  if (source) {
    Source newSource;
    if (getSource(source, newSource)) {
      if (newSource == Source::Standby) {
        // Special case: If we receive standby, let's hard-clear everything
        nowPlaying = NowPlayingStatus{};
        nowPlaying.source = Source::Standby;
        changed = static_cast<NowPlayingChangeDescription>(~0);
        if (nowPlayingChangedListener) {
          nowPlayingChangedListener(*this, changed);
        }
        return;
      }
      recordChange(nowPlaying.source, newSource, changed, NowPlayingChangeDescription::Source);
    }
  }
  recordChange(nowPlaying.sourceAccount, getAttr("sourceAccount"), changed, NowPlayingChangeDescription::Source);
  
  recordChange(nowPlaying.track, getElem("track"), changed, NowPlayingChangeDescription::Song);
  recordChange(nowPlaying.album, getElem("album"), changed, NowPlayingChangeDescription::Song);
  recordChange(nowPlaying.artist, getElem("artist"), changed, NowPlayingChangeDescription::Song);
  recordChange(nowPlaying.stationName, getElem("stationName"), changed, NowPlayingChangeDescription::Song);

  Span repeatStatus = getElemRaw("repeatSetting");
  if (repeatStatus) {
    RepeatStatus newRepeatStatus;
    if (getRepeatStatus(repeatStatus, newRepeatStatus)) {
      recordChange(nowPlaying.repeatStatus, newRepeatStatus, changed, NowPlayingChangeDescription::RepeatStatus);
    }
  }
  Span shuffleStatus = getElemRaw("shuffleSetting");
  if (shuffleStatus) {
    ShuffleStatus newShuffleStatus;
    debugMessage += shuffleStatus.toString();
    if (getShuffleStatus(shuffleStatus, newShuffleStatus)) {
      recordChange(nowPlaying.shuffleStatus, newShuffleStatus, changed, NowPlayingChangeDescription::ShuffleStatus);
    }
  }
  Span playStatus = getElemRaw("playStatus");
  if (playStatus) {
    PlayStatus newPlayStatus;
    if (getPlayStatus(playStatus, newPlayStatus)) {
      recordChange(nowPlaying.playStatus, newPlayStatus, changed, NowPlayingChangeDescription::PlayStatus);
    }
  }
  if (nowPlayingChangedListener) {
    nowPlayingChangedListener(*this, changed);
  }
}
//---------------------------------------------------------------------
void Device::handleVolumeUpdated(XmlDocument doc)
{
  XmlDocument body = doc.getBody();
  auto getElem = [&](const char* name) {
    XmlDocument x = body.getSibling(name);
    if (x) {
      return x.getBodyAsTextRaw();
    } else {
      return Span();
    }
  };

  Span targetVolume = getElem("targetvolume");
  if (targetVolume) {
    debugMessage += "\ntargetvolume: ";
    debugMessage += targetVolume.toString();
    currentVolume = targetVolume.toString().toInt();
  }
  if (volumeChangedListener) {
    volumeChangedListener(*this, currentVolume);
  }
}
//---------------------------------------------------------------------
/// Get the name of the key in the API
static const char* getKeyFromButton(SoundTouchButton b) {
  switch (b) {
    case SoundTouchButton::Power: return "POWER";
    case SoundTouchButton::Preset1: return "PRESET_1";
    case SoundTouchButton::Preset2: return "PRESET_2";
    case SoundTouchButton::Preset3: return "PRESET_3";
    case SoundTouchButton::Preset4: return "PRESET_4";
    case SoundTouchButton::Preset5: return "PRESET_5";
    case SoundTouchButton::Preset6: return "PRESET_6";
    case SoundTouchButton::PlayPause: return "PLAY_PAUSE";
    case SoundTouchButton::Pause: return "PAUSE";
    case SoundTouchButton::Prev: return "PREV_TRACK";
    case SoundTouchButton::Next: return "NEXT_TRACK";
    case SoundTouchButton::ThumbsUp: return "THUMBS_UP";
    case SoundTouchButton::ThumbsDown: return "THUMBS_DOWN";
    case SoundTouchButton::Mute: return "MUTE";
    case SoundTouchButton::ShuffleOff: return "SHUFFLE_OFF";
    case SoundTouchButton::ShuffleOn: return "SHUFFLE_ON";
    case SoundTouchButton::RepeatOff: return "REPEAT_OFF";
    case SoundTouchButton::RepeatOne: return "REPEAT_ONE";
    case SoundTouchButton::RepeatAll: return "REPEAT_ALL";
    case SoundTouchButton::BluetoothAux:
    case SoundTouchButton::VolumeUp:
    case SoundTouchButton::VolumeDown:
      // Those are not supported via the button API
      return nullptr;
  }
}
//---------------------------------------------------------------------
/// Get the state that we must send according to the API to get the desired effect
/// of a button press on a remote control
static const char* getKeyStateFromButton(SoundTouchButton b) {
  switch (b) {
    case SoundTouchButton::Preset1:
    case SoundTouchButton::Preset2:
    case SoundTouchButton::Preset3:
    case SoundTouchButton::Preset4:
    case SoundTouchButton::Preset5:
    case SoundTouchButton::Preset6:
      // Presets are selected when the key is released, otherwise they would be saved
      return "release";
    case SoundTouchButton::PlayPause:
    case SoundTouchButton::Prev:
    case SoundTouchButton::Next:
    case SoundTouchButton::ThumbsUp:
    case SoundTouchButton::ThumbsDown:
    case SoundTouchButton::Mute:
    case SoundTouchButton::Pause:
    case SoundTouchButton::ShuffleOn:
    case SoundTouchButton::ShuffleOff:
    case SoundTouchButton::RepeatOff:
    case SoundTouchButton::RepeatAll:
    case SoundTouchButton::RepeatOne:
      // Those are selected when the button is pressed
      return "press";
    case SoundTouchButton::Power:
      // Power must send a press AND release message.. who knows why
      return "";
    case SoundTouchButton::BluetoothAux:
    case SoundTouchButton::VolumeUp:
    case SoundTouchButton::VolumeDown:
      // Cannot be triggered through the button API
      return "";
  }
}
//---------------------------------------------------------------------
void Device::sendButtonClick(SoundTouchButton b)
// Send a button command
{
  const unsigned maxRequestSize = 96;
  auto sendKey = [&] (const char* state, const char* key) {
    debugMessageCmd = requestUrl<maxRequestSize>(websocket, "key", "POST", [&](LeanStringStreamBase& lss) {
      lss << "<key state=\"" << state << "\" sender=\"Gabbo\">" << key << "</key>";
    });
  };
  switch (b) {
    case SoundTouchButton::Preset1:
    case SoundTouchButton::Preset2:
    case SoundTouchButton::Preset3:
    case SoundTouchButton::Preset4:
    case SoundTouchButton::Preset5:
    case SoundTouchButton::Preset6:
    case SoundTouchButton::PlayPause:
    case SoundTouchButton::Prev:
    case SoundTouchButton::Next:
    case SoundTouchButton::ThumbsUp:
    case SoundTouchButton::ThumbsDown:
    case SoundTouchButton::Mute:
    case SoundTouchButton::Pause:
    case SoundTouchButton::ShuffleOn:
    case SoundTouchButton::ShuffleOff:
    case SoundTouchButton::RepeatOff:
    case SoundTouchButton::RepeatAll:
    case SoundTouchButton::RepeatOne:
      // Normal buttons
      sendKey(getKeyStateFromButton(b), getKeyFromButton(b));
      return;
    case SoundTouchButton::Power:
      // Requires press+release in rapid succession
      sendKey("press", getKeyFromButton(b));
      sendKey("release", getKeyFromButton(b));
      return;
    case SoundTouchButton::BluetoothAux:
    case SoundTouchButton::VolumeUp:
    case SoundTouchButton::VolumeDown:
      // Those buttons are not supported
      return;
  }
}
//---------------------------------------------------------------------
int16_t Device::changeVolumeDelta(int16_t delta)
// Change device volume by the specified delta
{
  int16_t newVolume = std::min(100, std::max(0, currentVolume + delta));
  setVolume(newVolume);
  return newVolume;
}
//---------------------------------------------------------------------
void Device::setVolume(uint16_t value)
// Set the volume to the specified value
{
  const unsigned maxSize = 72;
  debugMessageCmd = requestUrl<maxSize>(websocket, "volume", "POST", [=](LeanStringStreamBase& lss) {
    lss << "<volume>" << value << "</volume>";
  });
  currentVolume = value;
}
//---------------------------------------------------------------------
void Device::requestVolume()
{
  requestUrl<>(websocket, "volume");
}
//---------------------------------------------------------------------
void Device::requestNowPlaying()
{
  requestUrl<>(websocket, "now_playing");
}
//---------------------------------------------------------------------
void Device::changeSource(Source source)
// Select the specified source
{
  
}
//---------------------------------------------------------------------
void Device::changeSource(BuiltinSourceAccount bsa)
// Select the specified built-in source, available for Lifestyle systems
{
  
}
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------
