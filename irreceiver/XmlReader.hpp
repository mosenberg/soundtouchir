#pragma once
//---------------------------------------------------------------------
#include "Arduino.h"
#include <memory>
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
/// A string span
class Span {
  /// The begin of the span
  const char* begin;
  /// The length of the span
  size_t len;

  public:
  /// Constructor
  Span() : begin(nullptr), len(0) { }
  /// Constructor from a C-Style string
  Span(const char* begin);
  /// Constructor
  Span(const char* begin, size_t len) : begin(begin), len(len) { }
  /// Constructor
  Span(const char* begin, const char* end) : begin(begin), len(end - begin) { }
  
  /// Get the begin
  const char* getBegin() const { return begin; }
  /// Get the end
  const char* getEnd() const { return begin + len; }
  /// Get the length
  size_t getLength() const { return len; }

  /// Compare span against c-style string
  bool operator==(const char* rhs);
  /// Compare span against another span
  bool operator==(const Span& rhs);
  /// Does this span start with the specified span
  bool startsWith(const Span& pattern);
  /// Returns the index of the specified pattern within this span
  /// \return npos when the pattern is not contained in this span
  size_t indexOf(const Span& pattern, size_t startIndex = 0);

  /// Does the span have a value?
  operator bool() const { return begin != nullptr; }

  /// Convert to a string
  String toString() const;

  /// Return invalid length
  static inline size_t npos() { return ~0; }
};
//---------------------------------------------------------------------
/// A XML Document
class XmlDocument {
  /// The data
  Span data;

  /// Tag when XML document does not have a header
  enum NoHeaderTag { NoXmlHeader };
  /// Constructor for XML document that is guaranteed to not have a header
  XmlDocument(Span data, NoHeaderTag) : data(std::move(data)) { }

  public:
  /// Constructor
  explicit XmlDocument(const Span& data);
  /// Constructor
  XmlDocument() : data() { }

  /// Return the body of this XML document as text, resolving predefined entities
  String getBodyAsText() const;
  /// Return the body of this XML document as text, NOT considering predefined entities, e.g. &gt;
  /// \remarks Returns nullptr if the element is empty, e.g. <elem />
  Span getBodyAsTextRaw() const;
  /// Returns the body of this XML document and interprets it as XML Document
  /// Returns an empty XML document 
  XmlDocument getBody() const;

  /// Get an attribute with the specified name
  Span getAttribute(const char* name) const;
  /// Get a sibling with the specified name
  XmlDocument getSibling(const char* name) const;

  /// Get the name of the current element
  Span getName() const;

  /// Does the document have a value?
  operator bool() const { return !!data; }

  private:
  /// Skip XML header
  Span skipHeader(const Span& v);
  
};
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------
