//---------------------------------------------------------------------
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//---------------------------------------------------------------------
#include "BoseIR.hpp"
#include "SoundTouch.hpp"
#include "ScreenManager.hpp"
#include "config/Config.hpp"
//---------------------------------------------------------------------
using namespace BoseIR;
//---------------------------------------------------------------------
/// The IR receiver.
IRrecv irrecv(/*pin*/0, /*bufferSize*/ 64, /*timeoutMs*/ kTimeoutMs, false);
/// The global decode results
decode_results results_buf;
//---------------------------------------------------------------------
/// WiFI-Credentials
const char* ssid  = config::ssid;
const char* password = config::password;
/// The global webserver
ESP8266WebServer server(80);
//---------------------------------------------------------------------
/// The name of this ESP device to be used for the mDNS
const char* mDNSName = config::mDNSName;
/// The URL of the SoundTouch device to use.
/// ToDo: Make this configurable in the EEEPROM.
const char* deviceURL = config::deviceURL;
/// The SoundTouch device
SoundTouch::Device stDevice(deviceURL);
//---------------------------------------------------------------------
hox::ScreenManager screenManager(/*sda*/ 3, /*scl*/ 2, stDevice);
//---------------------------------------------------------------------
/// The button that was last read so we can post it to web clients.
WaveButton lastButton = WaveButton::INVALID;
//---------------------------------------------------------------------
/// Handle webclients connecting to the "/" root-URL of this ESP
void handleRoot() {
  String m;
  m = "Last Button: ";
  m += buttonToString(lastButton);
  m += "\nNow Playing";
  m += "\n    Status: " + String(SoundTouch::playStatusToString(stDevice.getNowPlaying().playStatus));
  m += "\n    Source: " + String(SoundTouch::sourceToString(stDevice.getNowPlaying().source));
  m += "\n    Artist: " + stDevice.getNowPlaying().artist;
  m += "\n    Track: " + stDevice.getNowPlaying().track;
  m += "\n    Album: " + stDevice.getNowPlaying().album;
  m += "\n    Volume: " + String(stDevice.getVolume());
  m += "\n    Shuffle: " + String(SoundTouch::shuffleStatusToString(stDevice.getNowPlaying().shuffleStatus));
  m += "\nDebug:" + stDevice.debugMessage;
  m += "\nDebugCmd:" + stDevice.debugMessageCmd;
  m += "\nFree Heap: " + String(ESP.getFreeHeap());
  m += "\nUptime:" + String(millis());
  server.send(200, "text/plain; charset=UTF-8", m);
}
//---------------------------------------------------------------------
/// Send button that was pressed on the remote to the device
static void sendButton(SoundTouchButton b) {
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
      stDevice.sendButtonClick(b);
      return;
    case SoundTouchButton::BluetoothAux:
      // We'd need to figure out the current source and depending on that
      // switch to Bluetooth or AUX.
      return;
    case SoundTouchButton::VolumeUp:
      stDevice.changeVolumeDelta(+3);
      return;
    case SoundTouchButton::VolumeDown:
      stDevice.changeVolumeDelta(-3);
      return;
    case SoundTouchButton::Mute: // unavailable on remote
    case SoundTouchButton::INVALID:
      return;
  }
}
//---------------------------------------------------------------------
/// Send button that was pressed on the remote to the device
static void sendButton(WaveButton b) {
  switch (b) {
    case WaveButton::Preset1:
      stDevice.sendButtonClick(SoundTouchButton::Preset1);
      return;
    case WaveButton::Preset2:
      stDevice.sendButtonClick(SoundTouchButton::Preset2);
      return;
    case WaveButton::Preset3:
      stDevice.sendButtonClick(SoundTouchButton::Preset3);
      return;
    case WaveButton::Preset4:
      stDevice.sendButtonClick(SoundTouchButton::Preset4);
      return;
    case WaveButton::Preset5:
      stDevice.sendButtonClick(SoundTouchButton::Preset5);
      return;
    case WaveButton::Preset6:
      stDevice.sendButtonClick(SoundTouchButton::Preset6);
      return;
    case WaveButton::PlayPause:
      stDevice.sendButtonClick(SoundTouchButton::PlayPause);
      return;
    case WaveButton::StopEject:
      stDevice.sendButtonClick(SoundTouchButton::Pause);
    case WaveButton::Prev:
      stDevice.sendButtonClick(SoundTouchButton::Prev);
      return;
    case WaveButton::Next:
      stDevice.sendButtonClick(SoundTouchButton::Next);
      return;
    case WaveButton::Mute:
      stDevice.sendButtonClick(SoundTouchButton::Mute);
      return;
    case WaveButton::BluetoothAux:
      // We'd need to figure out the current source and depending on that
      // switch to Bluetooth or AUX.
      return;
    case WaveButton::VolumeUp:
      stDevice.changeVolumeDelta(+3);
      return;
    case WaveButton::VolumeDown:
      stDevice.changeVolumeDelta(-3);
      return;
    case WaveButton::PlayMode:
      switch (stDevice.getNowPlaying().shuffleStatus) {
        case SoundTouch::ShuffleStatus::SHUFFLE_ON: 
          stDevice.sendButtonClick(SoundTouchButton::ShuffleOff);
          break;
        case SoundTouch::ShuffleStatus::SHUFFLE_OFF:
          stDevice.sendButtonClick(SoundTouchButton::ShuffleOn);
          break;
      }
      return;
    case WaveButton::INVALID:
      return;
    default:
      return;
  }
}
//---------------------------------------------------------------------
void setup() {
  // Connect wifi
  WiFi.begin(ssid, password);
  // Web server
  MDNS.begin(mDNSName);
  server.on("/", handleRoot);
  server.begin();
  MDNS.addService("http", "tcp", 80);
  // Start up the IR receiver.
  irrecv.enableIRIn();
  // Startup websocket connection
  stDevice.begin();
  // Startup oled screen
  screenManager.begin();
}
//---------------------------------------------------------------------
void loop() {
  MDNS.update();
  if (irrecv.decode(&results_buf)) {
    // We have captured something
    static unsigned long lastIRMessage = 0; // make sure we don't register a single button press multiple times
    auto button = decodeWaveFromIR(irrecv, results_buf);
    if (button != WaveButton::INVALID && (millis() - lastIRMessage > 300)) {
      lastButton = button;
      sendButton(button);
      lastIRMessage = millis();
    }
    irrecv.resume();
  }
  // Handle clients from web interface
  server.handleClient();
  // Dispatches WebSocket notifications
  stDevice.run();
  // Update the screen if required
  screenManager.tick();
}
//---------------------------------------------------------------------
