#include "BoseIR.hpp"
//---------------------------------------------------------------------
namespace BoseIR {
//---------------------------------------------------------------------
const char* buttonToString(SoundTouchButton b)
// Get human-readable string representation from the button
{
  switch (b) {
    case SoundTouchButton::Power: return "Power";
    case SoundTouchButton::BluetoothAux: return "BluetoothAux";
    case SoundTouchButton::Preset1: return "Preset1";
    case SoundTouchButton::Preset2: return "Preset2";
    case SoundTouchButton::Preset3: return "Preset3";
    case SoundTouchButton::Preset4: return "Preset4";
    case SoundTouchButton::Preset5: return "Preset5";
    case SoundTouchButton::Preset6: return "Preset6";
    case SoundTouchButton::VolumeUp: return "VolumeUp";
    case SoundTouchButton::VolumeDown: return "VolumeDown";
    case SoundTouchButton::PlayPause: return "PlayPause";
    case SoundTouchButton::Prev: return "Prev";
    case SoundTouchButton::Next: return "Next";
    case SoundTouchButton::ThumbsUp: return "ThumbsUp";
    case SoundTouchButton::ThumbsDown: return "ThumbsDown";
    default: return "INVALID";
  }
}
//---------------------------------------------------------------------
const char* buttonToString(WaveButton b)
// Get human-readable string representation from the button
{
  switch (b) {
    case WaveButton::Power: return "Power"; 
    case WaveButton::BluetoothAux: return "BluetoothAux"; 
    case WaveButton::Preset1: return "Preset1"; 
    case WaveButton::Preset2: return "Preset2"; 
    case WaveButton::Preset3: return "Preset3"; 
    case WaveButton::Preset4: return "Preset4"; 
    case WaveButton::Preset5: return "Preset5"; 
    case WaveButton::Preset6: return "Preset6"; 
    case WaveButton::VolumeUp: return "VolumeUp"; 
    case WaveButton::VolumeDown: return "VolumeDown"; 
    case WaveButton::PlayPause: return "PlayPause"; 
    case WaveButton::Prev: return "Prev"; 
    case WaveButton::Next: return "Next"; 
    case WaveButton::Sleep: return "Sleep"; 
    case WaveButton::Mute: return "Mute"; 
    case WaveButton::Radio: return "Radio"; 
    case WaveButton::CD: return "CD"; 
    case WaveButton::StopEject: return "StopEject"; 
    case WaveButton::TuneDown: return "TuneDown"; 
    case WaveButton::TuneUp: return "TuneUp"; 
    case WaveButton::TimeDown: return "TimeDown"; 
    case WaveButton::TimeUp: return "TimeUp"; 
    case WaveButton::PlayMode: return "PlayMode"; 
    case WaveButton::Alarm1OnOff: return "Alarm1OnOff"; 
    case WaveButton::Alarm2OnOff: return "Alarm2OnOff"; 
    case WaveButton::AlarmReview: return "AlarmReview"; 
    case WaveButton::INVALID: return "INVALID"; 
  }
}
//---------------------------------------------------------------------
static SoundTouchButton sanitizeSoundTouchButton(uint64_t b)
// Ensure the specified button code is a soundtouch button.
{
  switch(static_cast<SoundTouchButton>(b)) {
    case SoundTouchButton::Power: return SoundTouchButton::Power;
    case SoundTouchButton::BluetoothAux: return SoundTouchButton::BluetoothAux;
    case SoundTouchButton::Preset1: return SoundTouchButton::Preset1;
    case SoundTouchButton::Preset2: return SoundTouchButton::Preset2;
    case SoundTouchButton::Preset3: return SoundTouchButton::Preset3;
    case SoundTouchButton::Preset4: return SoundTouchButton::Preset4;
    case SoundTouchButton::Preset5: return SoundTouchButton::Preset5;
    case SoundTouchButton::Preset6: return SoundTouchButton::Preset6;
    case SoundTouchButton::VolumeUp: return SoundTouchButton::VolumeUp;
    case SoundTouchButton::VolumeDown: return SoundTouchButton::VolumeDown;
    case SoundTouchButton::PlayPause: return SoundTouchButton::PlayPause;
    case SoundTouchButton::Prev: return SoundTouchButton::Prev;
    case SoundTouchButton::Next: return SoundTouchButton::Next;
    case SoundTouchButton::ThumbsUp: return SoundTouchButton::ThumbsUp;
    case SoundTouchButton::ThumbsDown: return SoundTouchButton::ThumbsDown;
    default: return SoundTouchButton::INVALID;
  }
}
//---------------------------------------------------------------------
static WaveButton sanitizeWaveButton(uint64_t b)
// Ensure the specified button code is a soundtouch button.
{
  switch(static_cast<WaveButton>(b)) {
    case WaveButton::Power: return WaveButton::Power; 
    case WaveButton::BluetoothAux: return WaveButton::BluetoothAux; 
    case WaveButton::Preset1: return WaveButton::Preset1; 
    case WaveButton::Preset2: return WaveButton::Preset2; 
    case WaveButton::Preset3: return WaveButton::Preset3; 
    case WaveButton::Preset4: return WaveButton::Preset4; 
    case WaveButton::Preset5: return WaveButton::Preset5; 
    case WaveButton::Preset6: return WaveButton::Preset6; 
    case WaveButton::VolumeUp: return WaveButton::VolumeUp; 
    case WaveButton::VolumeDown: return WaveButton::VolumeDown; 
    case WaveButton::PlayPause: return WaveButton::PlayPause; 
    case WaveButton::Prev: return WaveButton::Prev; 
    case WaveButton::Next: return WaveButton::Next; 
    case WaveButton::Sleep: return WaveButton::Sleep; 
    case WaveButton::Mute: return WaveButton::Mute; 
    case WaveButton::Radio: return WaveButton::Radio; 
    case WaveButton::CD: return WaveButton::CD; 
    case WaveButton::StopEject: return WaveButton::StopEject; 
    case WaveButton::TuneDown: return WaveButton::TuneDown; 
    case WaveButton::TuneUp: return WaveButton::TuneUp; 
    case WaveButton::TimeDown: return WaveButton::TimeDown; 
    case WaveButton::TimeUp: return WaveButton::TimeUp; 
    case WaveButton::PlayMode: return WaveButton::PlayMode; 
    case WaveButton::Alarm1OnOff: return WaveButton::Alarm1OnOff; 
    case WaveButton::Alarm2OnOff: return WaveButton::Alarm2OnOff; 
    case WaveButton::AlarmReview: return WaveButton::AlarmReview; 
    default: return WaveButton::INVALID;
  }
}
//---------------------------------------------------------------------
SoundTouchButton decodeSoundTouchFromIR(IRrecv& irrecv, const decode_results& results)
// Decode a SoundTouch button from the specified decode_results
{
  if (results.rawlen-1 < 35)
    return SoundTouchButton::INVALID;
  uint64_t decoded;
  // SoundTouch remote signals start with a Header-Mark of 1000 us High and 1500 us Low.
  // Followed by the data, where a 1 bit has 500 us High and 1500 us Low and a 
  // 0 bit has 500 us High and 500 us Low.
  // The transmission ends with a 500 us High signal.
  // The message has a total length of 16 bits, i.e. 2 bytes.
  uint16_t hadMatch = irrecv.matchGeneric(results.rawbuf+1, &decoded, 
                                          /*remaining:*/ results.rawlen-1,/*nbits*/ 16, 
                                          /*hdrmark*/ 1000, /*hdrspace*/ 1500, 
                                          /*onemark*/ 500, /*onespace*/ 1500, 
                                          /*zeromark*/ 500, /*zerospace*/ 500, 
                                          /*footemark*/ 500, /*footerspace*/ 0,
                                          /*footerMatchAtLeast*/ false, 
                                          /*tolerance*/ kUseDefTol, /*excess*/0, 
                                          /*msbfirst*/true);
  if (hadMatch == 0)
    return SoundTouchButton::INVALID;
  return sanitizeSoundTouchButton(decoded);
}
//---------------------------------------------------------------------
WaveButton decodeWaveFromIR(IRrecv& irrecv, const decode_results& results)
// Decode a Wave button from the specified decode_results
{
  if (results.rawlen-1 < 35)
    return WaveButton::INVALID;
  uint64_t decoded;
  // SoundTouch remote signals start with a Header-Mark of 1000 us High and 1500 us Low.
  // Followed by the data, where a 1 bit has 500 us High and 1500 us Low and a 
  // 0 bit has 500 us High and 500 us Low.
  // The transmission ends with a 500 us High signal.
  // The message has a total length of 16 bits, i.e. 2 bytes.
  uint16_t hadMatch = irrecv.matchGeneric(results.rawbuf+1, &decoded, 
                                          /*remaining:*/ results.rawlen-1,/*nbits*/ 16, 
                                          /*hdrmark*/ 1000, /*hdrspace*/ 1500, 
                                          /*onemark*/ 500, /*onespace*/ 1500, 
                                          /*zeromark*/ 500, /*zerospace*/ 500, 
                                          /*footemark*/ 500, /*footerspace*/ 0,
                                          /*footerMatchAtLeast*/ false, 
                                          /*tolerance*/ kUseDefTol, /*excess*/0, 
                                          /*msbfirst*/true);
  if (hadMatch == 0)
    return WaveButton::INVALID;
  return sanitizeWaveButton(decoded);
}
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------

