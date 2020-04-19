#pragma once
//---------------------------------------------------------------------
#include "../util/Defines.hpp"
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
namespace images {
//---------------------------------------------------------------------
namespace sources {
//---------------------------------------------------------------------
constexpr uint8_t width = 21;
constexpr uint8_t height = 21;
//---------------------------------------------------------------------
static uint8_t airplay[] = {
  0x00, 0x1F, 0x00, 0xC0, 0x60, 0x00, 0x20, 0x80, 0x00, 0x18, 0x0E, 0x03, 
  0x8C, 0x31, 0x06, 0x64, 0xC4, 0x04, 0x22, 0x9B, 0x08, 0x92, 0x20, 0x09, 
  0x49, 0x40, 0x12, 0x29, 0x80, 0x12, 0x29, 0x80, 0x12, 0x29, 0x80, 0x12, 
  0x49, 0x40, 0x12, 0x89, 0x24, 0x12, 0x12, 0x0E, 0x09, 0x22, 0x9F, 0x08, 
  0x84, 0x3F, 0x04, 0xC8, 0x7F, 0x02, 0xE0, 0xFF, 0x00, 0xF0, 0xFF, 0x01, 
  0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
static uint8_t aux[] = {
  0x00, 0x00, 0x18, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x06, 0x00, 0x40, 0x03, 
  0x00, 0xE0, 0x01, 0x00, 0xF0, 0x01, 0x00, 0xF8, 0x03, 0x00, 0xFC, 0x01, 
  0x00, 0xFE, 0x00, 0x00, 0x7F, 0x00, 0x80, 0x3F, 0x00, 0xC0, 0x1F, 0x00, 
  0x40, 0x0E, 0x00, 0x40, 0x06, 0x00, 0xE0, 0x03, 0x00, 0x70, 0x00, 0x00, 
  0x38, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x06, 0x00, 0x00, 
  0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
static uint8_t bluetooth[] = {
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x3F, 0x80, 0x01, 
  0xE3, 0x80, 0x01, 0x83, 0x85, 0x01, 0x83, 0x0D, 0x03, 0x83, 0x19, 0x03, 
  0xC3, 0x18, 0x03, 0x3F, 0x18, 0x03, 0xC3, 0x18, 0x03, 0x83, 0x19, 0x03, 
  0x83, 0x0D, 0x03, 0x83, 0x85, 0x01, 0x83, 0x81, 0x01, 0xE3, 0x80, 0x01, 
  0x3F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
static uint8_t hdmi[] = {
  0xF8, 0x3F, 0x00, 0xE8, 0x2F, 0x00, 0x28, 0x28, 0x00, 0xE8, 0x2E, 0x00, 
  0x28, 0x28, 0x00, 0x28, 0x28, 0x00, 0xFC, 0x7F, 0x00, 0x04, 0x40, 0x00, 
  0x04, 0x40, 0x00, 0x04, 0x40, 0x00, 0xC4, 0x47, 0x00, 0x04, 0x40, 0x00, 
  0xC4, 0x47, 0x00, 0x0C, 0x60, 0x00, 0x08, 0x20, 0x00, 0xF8, 0x3F, 0x00, 
  0x10, 0x10, 0x00, 0xE0, 0x0F, 0x00, 0x40, 0x04, 0x00, 0x40, 0x04, 0x00, 
  0x40, 0x04, 0x00, };
//---------------------------------------------------------------------
static uint8_t genericmusic[] = {
  0x00, 0xC0, 0x0F, 0x00, 0xF0, 0x0F, 0x00, 0xFE, 0x0F, 0x80, 0x3F, 0x0C, 
  0x80, 0x07, 0x0C, 0x80, 0x01, 0x0C, 0x80, 0x01, 0x0C, 0x80, 0x01, 0x0C, 
  0x80, 0x01, 0x0C, 0x80, 0x01, 0x0C, 0x80, 0x01, 0x0C, 0x80, 0xC1, 0x0F, 
  0x80, 0xE1, 0x0F, 0xF8, 0xF1, 0x0F, 0xFC, 0xF1, 0x0F, 0xFE, 0xF1, 0x0F, 
  0xFE, 0xE1, 0x07, 0xFE, 0x81, 0x03, 0xFC, 0x00, 0x00, 0x78, 0x00, 0x00, 
  0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
static uint8_t radio[] = {
  0x60, 0x00, 0x00, 0x20, 0x00, 0x00, 0x30, 0x00, 0x00, 0x18, 0x00, 0x00, 
  0x08, 0x00, 0x00, 0x1C, 0xB6, 0x01, 0xFE, 0xFF, 0x07, 0x03, 0x00, 0x0C, 
  0xF1, 0x01, 0x08, 0x59, 0xE3, 0x0B, 0xAD, 0x06, 0x08, 0x55, 0xE5, 0x0B, 
  0xAD, 0x06, 0x08, 0x55, 0xE5, 0x0B, 0xAD, 0x06, 0x08, 0x59, 0x03, 0x08, 
  0xF1, 0x01, 0x08, 0x03, 0x00, 0x0C, 0xFE, 0xFF, 0x07, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
static uint8_t spotify[] = {
  0x00, 0x1F, 0x00, 0xC0, 0x7F, 0x00, 0xF0, 0xFF, 0x01, 0xF8, 0xFF, 0x03, 
  0xFC, 0xFF, 0x07, 0xFC, 0xFF, 0x07, 0x1E, 0x80, 0x0F, 0xCE, 0x1F, 0x0E, 
  0xFF, 0xFF, 0x1C, 0x3F, 0xE0, 0x1F, 0x9F, 0x07, 0x1F, 0xFF, 0x7F, 0x1E, 
  0x3F, 0xF0, 0x1F, 0x9E, 0x87, 0x0F, 0xFE, 0xBF, 0x0F, 0xFC, 0xFF, 0x07, 
  0xFC, 0xFF, 0x07, 0xF8, 0xFF, 0x03, 0xF0, 0xFF, 0x01, 0xC0, 0x7F, 0x00, 
  0x00, 0x1F, 0x00, };
//---------------------------------------------------------------------
static uint8_t storedmusic[] = {
  0x00, 0x00, 0x00, 0xE0, 0x3F, 0x00, 0xF0, 0x7F, 0x00, 0x18, 0xC0, 0x00, 
  0x18, 0xCC, 0x00, 0x18, 0xD6, 0x00, 0x18, 0xC2, 0x00, 0x18, 0xC2, 0x00, 
  0x98, 0xC3, 0x00, 0x58, 0xC2, 0x00, 0x58, 0xC2, 0x00, 0x98, 0xC1, 0x00, 
  0x18, 0xC0, 0x00, 0xD8, 0xDF, 0x00, 0xF8, 0xFF, 0x00, 0x38, 0xE0, 0x00, 
  0x18, 0xC8, 0x00, 0x38, 0xE0, 0x00, 0xF0, 0x7F, 0x00, 0xE0, 0x3F, 0x00, 
  0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------
namespace playmode {
//---------------------------------------------------------------------
constexpr uint8_t width = 19;
constexpr uint8_t height = 11;
//---------------------------------------------------------------------
static uint8_t repeatall[] = {
  0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x60, 0x00, 0xF0, 0xFF, 0x00, 
  0x08, 0x60, 0x01, 0x04, 0x20, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 
  0x02, 0x80, 0x00, 0x02, 0x40, 0x00, 0xFC, 0x3F, 0x00, };
//---------------------------------------------------------------------
static uint8_t repeat_one[] = {
  0x00, 0x01, 0x00, 0x80, 0x21, 0x00, 0x00, 0x61, 0x00, 0x70, 0xFD, 0x00, 
  0x08, 0x61, 0x01, 0x84, 0x23, 0x01, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 
  0x02, 0x80, 0x00, 0x02, 0x40, 0x00, 0xFC, 0x3F, 0x00, };
//---------------------------------------------------------------------
constexpr uint8_t shuffle_width = 21;
constexpr uint8_t shuffle_height = 11;
static uint8_t shuffle[] = {
  0x00, 0x20, 0x00, 0x00, 0x60, 0x00, 0x1F, 0xFE, 0x00, 0x20, 0x61, 0x00, 
  0xC0, 0x20, 0x00, 0xC0, 0x00, 0x00, 0x20, 0x21, 0x00, 0x10, 0x62, 0x00, 
  0x0F, 0xFC, 0x00, 0x00, 0x60, 0x00, 0x00, 0x20, 0x00, };
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------
namespace transportcontrol {
//---------------------------------------------------------------------
constexpr uint8_t width = 19;
constexpr uint8_t height = 21;
//---------------------------------------------------------------------
static uint8_t pause[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x70, 0x00, 
  0xE0, 0x70, 0x00, 0xE0, 0x70, 0x00, 0xE0, 0x70, 0x00, 0xE0, 0x70, 0x00, 
  0xE0, 0x70, 0x00, 0xE0, 0x70, 0x00, 0xE0, 0x70, 0x00, 0xE0, 0x70, 0x00, 
  0xE0, 0x70, 0x00, 0xE0, 0x70, 0x00, 0xE0, 0x70, 0x00, 0xE0, 0x70, 0x00, 
  0xE0, 0x70, 0x00, 0xE0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
static uint8_t play[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 
  0x60, 0x00, 0x00, 0xE0, 0x00, 0x00, 0xE0, 0x01, 0x00, 0xE0, 0x03, 0x00, 
  0xE0, 0x07, 0x00, 0xE0, 0x0F, 0x00, 0xE0, 0x1F, 0x00, 0xE0, 0x0F, 0x00, 
  0xE0, 0x07, 0x00, 0xE0, 0x03, 0x00, 0xE0, 0x01, 0x00, 0xE0, 0x00, 0x00, 
  0x60, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
static uint8_t skipback[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x40, 0x00, 
  0x60, 0x60, 0x00, 0x60, 0x70, 0x00, 0x60, 0x78, 0x00, 0x60, 0x7C, 0x00, 
  0x60, 0x7E, 0x00, 0x60, 0x7F, 0x00, 0xE0, 0x7F, 0x00, 0x60, 0x7F, 0x00, 
  0x60, 0x7E, 0x00, 0x60, 0x7C, 0x00, 0x60, 0x78, 0x00, 0x60, 0x70, 0x00, 
  0x60, 0x60, 0x00, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
static uint8_t skipnext[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x60, 0x00, 
  0x60, 0x60, 0x00, 0xE0, 0x60, 0x00, 0xE0, 0x61, 0x00, 0xE0, 0x63, 0x00, 
  0xE0, 0x67, 0x00, 0xE0, 0x6F, 0x00, 0xE0, 0x7F, 0x00, 0xE0, 0x6F, 0x00, 
  0xE0, 0x67, 0x00, 0xE0, 0x63, 0x00, 0xE0, 0x61, 0x00, 0xE0, 0x60, 0x00, 
  0x60, 0x60, 0x00, 0x20, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------
constexpr uint8_t hibernate_width = 31;
constexpr uint8_t hibernate_height = 30;
static uint8_t hibernate[] = {
  0x00, 0xF8, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x80, 0x67, 0x00, 0x00, 
  0xC0, 0x63, 0xC0, 0x07, 0xE0, 0x60, 0x00, 0x02, 0x70, 0x60, 0x00, 0x01, 
  0x38, 0x60, 0x00, 0x01, 0x18, 0xC0, 0x80, 0x00, 0x1C, 0xC0, 0xC0, 0x07, 
  0x0C, 0xC0, 0x01, 0x00, 0x06, 0xC0, 0x01, 0x00, 0x06, 0x80, 0x03, 0x00, 
  0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x0F, 0x00, 0x07, 0x00, 0x1E, 0x00, 
  0x07, 0x00, 0x7C, 0x60, 0x07, 0x00, 0xF0, 0x7F, 0x07, 0x00, 0xC0, 0x7F, 
  0x06, 0x00, 0x00, 0x60, 0x0C, 0x00, 0x00, 0x60, 0x0C, 0x00, 0x00, 0x60, 
  0x1C, 0x00, 0x00, 0x70, 0x18, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x1C, 
  0x70, 0x00, 0x00, 0x0E, 0xE0, 0x00, 0x00, 0x07, 0xC0, 0x03, 0x80, 0x03, 
  0x80, 0x0F, 0xF0, 0x01, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0xF8, 0x1F, 0x00, 
  };
//---------------------------------------------------------------------
constexpr uint8_t loudspeaker_width = 32;
constexpr uint8_t loudspeaker_height = 32;
static uint8_t loudspeaker[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xC0, 0x0F, 0x08, 
  0x00, 0xE0, 0x0C, 0x18, 0x00, 0x78, 0x0C, 0x38, 0x00, 0x1C, 0x0C, 0x30, 
  0x40, 0x0E, 0x8C, 0x60, 0xF8, 0x07, 0x8C, 0x61, 0xDC, 0x01, 0x8C, 0xC3, 
  0x9F, 0x01, 0x0C, 0xC3, 0x9B, 0x01, 0x0C, 0xC3, 0x9B, 0x01, 0x0C, 0xC2, 
  0x9B, 0x01, 0x0C, 0xC6, 0x1B, 0x01, 0x0C, 0xC6, 0x9B, 0x01, 0x0C, 0xC3, 
  0x9F, 0x01, 0x0C, 0xC3, 0x9E, 0x01, 0x0C, 0x43, 0xF8, 0x03, 0x8C, 0x61, 
  0xE0, 0x07, 0x8C, 0x60, 0x00, 0x1E, 0x0C, 0x70, 0x00, 0x38, 0x0C, 0x30, 
  0x00, 0xF0, 0x0C, 0x18, 0x00, 0xC0, 0x0D, 0x0C, 0x00, 0x80, 0x0F, 0x00, 
  0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------
}