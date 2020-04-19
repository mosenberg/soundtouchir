#pragma once
//---------------------------------------------------------------------
#include <limits>
#include <type_traits>
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
/// A stringbuilder class that writes to a pre-allocated buffer that cannot grow
class LeanStringStreamBase {
  /// The length of the buffer, last character will be used for trailing zero
  unsigned len;
  /// The position
  unsigned pos;
  /// Was the output truncated?
  bool truncated;
  /// The buffer
  char* buffer;

  public:  
  /// Constructor
  LeanStringStreamBase(char* buffer, unsigned bufferSize, unsigned offset = 0) : len(bufferSize), pos(offset), buffer(buffer), truncated(false) { }

  /// Get the resulting text
  char* data() {
    buffer[pos] = '\0';
    return buffer;
  }
  /// Get the data as raw uint8_t pointer
  uint8_t* dataRaw() { return reinterpret_cast<uint8_t*>(data()); }
  /// Get the current position
  unsigned length() const { return pos; }
  /// Get the available space
  unsigned available() const { return (len - 1) - pos; }
  /// Was input truncated because buffer length was exceeded?
  bool wasTruncated() const { return truncated; }

  /// Append a c-style string
  LeanStringStreamBase& operator<<(const char* c) {
    while (*c) {
      write(*c);
      ++c;
    }
    return *this;
  }
  /// Append a character
  LeanStringStreamBase& operator<<(char c) {
    write(c);
    return *this;
  }
  /// Append a number
  template<class T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
  LeanStringStreamBase& operator<<(T v) {
    char value[] = "+00000000000000000";
    char* vp = value + sizeof(value) - 2;
    bool neg = std::is_signed<T>::value && (v < 0);
    if (neg) v *= -1;
    while ((v > 0) && (vp > value)) {
      *vp = '0' + (v % 10);
      v /= 10;
      --vp;
    }
    if (neg) { 
      *vp = '-';
      --vp; 
    }
    return (*this) << vp;
  }
  
  private:
  /// Write a character to the buffer.
  /// \return true if writing was successful, false if buffer-size is exceeded
  bool write(char c) {
    if (pos < (len - 1)) {
      buffer[pos] = c;
      ++pos;
      return true;
    } else {
      return false;
    }
  }
};
//---------------------------------------------------------------------
/// A stringstream helper that only uses stack-allocated memory of a static
/// size to build strings to avoid heap-fragmentation.
template<unsigned N = 128>
class LeanStringStream : public LeanStringStreamBase {
  /// The buffer, reserved an additional character for trailing zero
  char buffer[N + 1];

  public:
  /// Constructor
  LeanStringStream() : LeanStringStreamBase(buffer, N + 1) { }

  /// Get the buffer size
  static const unsigned BUFFER_SIZE = N;
};
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------
