#pragma once
#include "Arduino.h"
#include <functional>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include "XmlReader.hpp"
#include <type_traits>
//---------------------------------------------------------------------
namespace BoseIR { enum class SoundTouchButton : uint64_t; }
//---------------------------------------------------------------------
namespace SoundTouch {
//---------------------------------------------------------------------
class Device;
struct NowPlayingStatus;
//---------------------------------------------------------------------
/// Sources available for a SoundTouch device
enum class Source : unsigned char {
  /// System is in standby mode
  Standby,
  /// Auxiliary input
  Aux,
  /// Bluetooth
  Bluetooth,
  /// Sources that are built into the product, further specified by a BuiltinSourceAccount
  Product,
  /// Radioplayer music service
  Radioplayer,
  /// TuneIn music service
  TuneIn,
  /// Spotify
  Spotify,
  /// UPNP stream
  UPNP,
  /// Music Library from a NAS volume
  StoredMusic,
  /// Notification is being played
  Notification,
  /// Airplay
  Airplay,
  /// The selected source is not valid
  INVALID_SOURCE
};
//---------------------------------------------------------------------
/// Get source from its string representation
bool getSource(hox::Span v, Source& result);
const char* sourceToString(Source s);
//---------------------------------------------------------------------
/// Builtin source accounts when Source equals Product
enum class BuiltinSourceAccount : unsigned char {
  HDMI_1, HDMI_2, HDMI_3, HDMI_4, HDMI_5, HDMI_6, TV, Optical_1, Optical_2,
  Analog_1, Analog_2, Coax_1, Coax_2
};
//---------------------------------------------------------------------
/// Get a BuiltinSourceAccount from the specified string
bool getBuiltinSourceAccount(hox::Span v, BuiltinSourceAccount& result);
//---------------------------------------------------------------------
/// The type of stream that is playing
enum class StreamStatus : unsigned char {
  TRACK_ONDEMAND, RADIO_STREAMING, RADIO_TRACKS, NO_TRANSPORT_CONTROLS
};
//---------------------------------------------------------------------
/// A repeat status
enum class RepeatStatus : unsigned char {
  REPEAT_OFF, REPEAT_ALL, REPEAT_ONE
};
//---------------------------------------------------------------------
bool getRepeatStatus(hox::Span v, RepeatStatus& result);
//---------------------------------------------------------------------
/// A shuffle status
enum class ShuffleStatus : unsigned char {
  SHUFFLE_OFF, SHUFFLE_ON
};
//---------------------------------------------------------------------
bool getShuffleStatus(hox::Span v, ShuffleStatus& result);
const char* shuffleStatusToString(ShuffleStatus s);
//---------------------------------------------------------------------
/// A play status
enum class PlayStatus : unsigned char {
  PLAY_STATE, PAUSE_STATE, STOP_STATE, BUFFERING_STATE
};
//---------------------------------------------------------------------
bool getPlayStatus(hox::Span vtr, PlayStatus& result);
const char* playStatusToString(PlayStatus s);
//---------------------------------------------------------------------
/// Describes what has changed in a NowPlaying update
enum class NowPlayingChangeDescription : uint8_t {
  NoChange = 0,
  /// Groups source and source account
  Source = 1 << 0,
  /// Groups track, artist, album and station name
  Song = 1 << 1,
  StreamStatus = 1 << 2,
  RepeatStatus = 1 << 3,
  ShuffleStatus = 1 << 4,
  PlayStatus = 1 << 5
};
//---------------------------------------------------------------------
inline NowPlayingChangeDescription operator|(NowPlayingChangeDescription lhs, NowPlayingChangeDescription rhs) { 
  using VT = uint8_t;
  return static_cast<NowPlayingChangeDescription>(static_cast<VT>(lhs) | static_cast<VT>(rhs)); 
}
inline bool operator&(NowPlayingChangeDescription lhs, NowPlayingChangeDescription rhs) { 
  using VT = uint8_t;
  return (static_cast<VT>(lhs) & static_cast<VT>(rhs)) != 0; 
}
//---------------------------------------------------------------------
/// Status about what's currently playing
struct NowPlayingStatus {
  /// The source
  Source source = Source::Standby;
  /// The source account
  /// \remarks If source equals Product, this can be parsed using getBuiltinSourceAccount
  String sourceAccount;
  /// The track that is playing
  String track;
  /// The artist that is playing
  String artist;
  /// The album tht is playing
  String album;
  /// The station that is playing
  String stationName;
  /// The stream status
  StreamStatus streamStatus;
  /// The repeat status
  RepeatStatus repeatStatus;
  /// The shuffle status
  ShuffleStatus shuffleStatus;
  /// The play status
  PlayStatus playStatus;
};
//---------------------------------------------------------------------
using NowPlayingChangedListener = std::function<void(Device&, NowPlayingChangeDescription)>;
using ConnectionStatusChangedListener = std::function<void(Device&,bool connected)>;
using VolumeChangedListener = std::function<void(Device&,int16_t volume)>;
//---------------------------------------------------------------------
/// A SoundTouch device
class Device {
  /// The URL to the device
  const char* url;
  /// Listener when NowPlaying has changed
  NowPlayingChangedListener nowPlayingChangedListener;
  /// Listener when connection status changed
  ConnectionStatusChangedListener connectionStatusChangedListener;
  /// Listener when volume changed
  VolumeChangedListener volumeChangedListener;

  /// The websocket client
  WebSocketsClient websocket;
  
  public:  
  /// Constructor
  explicit Device(const char* url);

  /// To be called in setup()
  bool begin();
  /// To be called in loop() when websockets are registered
  void run();

  /// Send a button command
  /// \remarks The following buttons are not supported: VolumeUp, VolumeDown, BluetoothAux
  void sendButtonClick(BoseIR::SoundTouchButton b);
  
  /// Change device volume by the specified delta
  /// \return The new volume that was sent to the device
  int16_t changeVolumeDelta(int16_t delta);
  /// Set the volume to the specified value
  void setVolume(uint16_t value);
  /// Get the volume
  int16_t getVolume() const { return currentVolume; }
  
  /// Select the specified source
  void changeSource(Source source);
  /// Select the specified built-in source, available for Lifestyle systems
  void changeSource(BuiltinSourceAccount bsa);

  /// Register a listener for the NowPlayingChanged event.
  /// \remarks If a listener is alredy registered, it will be overridden
  void registerNowPlayingChangedListener(NowPlayingChangedListener&& l) { nowPlayingChangedListener = std::move(l); }
  /// Register a listener for the ConnectionStatusChanged event.
  /// \remarks If a listener is alredy registered, it will be overridden
  void registerConnectionStatusChangedListener(ConnectionStatusChangedListener&& l) { connectionStatusChangedListener = std::move(l); }
  /// Register a listener for the VolumeChanged event
  /// \remarks If a listener is alredy registered, it will be overridden
  void registerVolumeChangedListener(VolumeChangedListener&& l) { volumeChangedListener = std::move(l); }

  /// Returns true if the websocket is connected
  bool isConnected() const { return connected; }

  const NowPlayingStatus& getNowPlaying() const { return nowPlaying; }

  // Debugging
  String debugMessage;
  String debugMessageCmd;

  private:
  // Internal callback on websocket event
  void websocketEvent(WStype_t type, uint8_t* payload, size_t length);
  // Handle updates
  void handleUpdates(hox::XmlDocument doc);
  void handleNowPlayingUpdated(hox::XmlDocument doc);
  void handleVolumeUpdated(hox::XmlDocument doc);
  // Messages that are responses to requests sent earlier, e.g. /volume
  void handleMessage(hox::XmlDocument doc);

  /// Request volume by sending a request to the device
  /// \remarks Updates currentVolume
  void requestVolume();
  /// Request now playing information
  /// \remarks Updates nowPlaying
  void requestNowPlaying();

  /// The current NowPlayingStatus
  NowPlayingStatus nowPlaying;
  /// The current volume
  int16_t currentVolume;
  /// Is the device connected?
  bool connected = false;
};
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------
