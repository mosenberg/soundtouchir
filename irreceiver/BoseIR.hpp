#pragma once
//---------------------------------------------------------------------
#include <IRrecv.h>
#include <IRutils.h>
#include <IRremoteESP8266.h>
//---------------------------------------------------------------------
namespace BoseIR {
//---------------------------------------------------------------------
/// Button Codes as per the SoundTouch Remote documentation
enum class SoundTouchButton : uint64_t {
  Power = 0xcd32,
  BluetoothAux = 0x0ff0, 
  Preset1 = 0x1fe0,
  Preset2 = 0xef10, 
  Preset3 = 0x6f90, 
  Preset4 = 0xaf50, 
  Preset5 = 0x2fd0, 
  Preset6 = 0xcf30,
  VolumeUp = 0x3fc0, 
  VolumeDown = 0xbf40, 
  PlayPause = 0x27d8,
  // Not available on remote, but using API
  Pause,
  Prev = 0xe718, 
  Next = 0x6798, 
  ThumbsUp = 0xaa55, 
  ThumbsDown = 0xda25,
  // Not available on remote, but using API
  Mute,
  ShuffleOn,
  ShuffleOff,
  RepeatOff,
  RepeatAll,
  RepeatOne,
  INVALID = 0
};
//---------------------------------------------------------------------
/// Button codes as per the Wave Remote documentation
enum class WaveButton : uint64_t {
  Power = 0x32cd,
  BluetoothAux = 0xf00f, 
  Preset1 = 0xe01f,
  Preset2 = 0x10ef, 
  Preset3 = 0x906f, 
  Preset4 = 0x50af, 
  Preset5 = 0xd02f, 
  Preset6 = 0x30cf,
  VolumeUp = 0xc03f, 
  VolumeDown = 0x40bf, 
  PlayPause = 0xd827, 
  Prev = 0x18e7, 
  Next = 0x9867, 
  Sleep = 0x2ad5, 
  Mute = 0x807f,
  Radio = 0x609f,
  CD = 0xca35,
  StopEject = 0x58a7,
  TuneDown = 0xea15,
  TuneUp = 0x1ae5,
  TimeDown = 0x7986,
  TimeUp = 0x24db,
  PlayMode = 0x847b,
  Alarm1OnOff = 0x46bb,
  Alarm2OnOff = 0x46b9,
  AlarmReview = 0x45ba,
  INVALID = 0
};
//---------------------------------------------------------------------
/// Get human-readable string representation from the button
const char* buttonToString(SoundTouchButton b);
/// Get human-readable string representation from the button
const char* buttonToString(WaveButton b);
//---------------------------------------------------------------------
/// Decode a SoundTouch button from the specified decode_results.
/// If decoding was not successful, returns SoundTouchButton::INVALID
SoundTouchButton decodeSoundTouchFromIR(IRrecv& irrecv, const decode_results& results);
/// Decode a Wave button from the specified decode_results
WaveButton decodeWaveFromIR(IRrecv& irrecv, const decode_results& results);
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------
