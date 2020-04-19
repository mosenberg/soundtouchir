#include "XmlReader.hpp"
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
static const char xmlHeaderStart[] = "<?xml";
//---------------------------------------------------------------------
Span::Span(const char* data)
  : begin(data), len(strlen(data))
// Constructor from C-Style string
{
}
//---------------------------------------------------------------------
bool Span::operator==(const char* rhs)
// Compare with C-Style string
{
  size_t i = 0;
  for (; ((i != len) && rhs[i]); ++i) {
    if (begin[i] != rhs[i])
      return false;
  }
  // Both must be at the end for equality
  return (i == len) && (rhs[i] == '\0');
}
//---------------------------------------------------------------------
bool Span::operator==(const Span& rhs) 
// Compare with another span
{
  if (rhs.len != len)
    return false;
  for (size_t i = 0; i != len; ++i) {
    if (begin[i] != rhs.begin[i])
      return false;
  }
  return true;
}
//---------------------------------------------------------------------
/// Does the specified character set start with the specified pattern
bool Span::startsWith(const Span& pattern) {
  if (len < pattern.len)
    return false;
  // Restrict this span to the length of pattern
  Span lhs(begin, pattern.len);
  return lhs == pattern;
}
//---------------------------------------------------------------------
size_t Span::indexOf(const Span& pattern, size_t startIndex)
// Get the index of the specified pattern within this span
{
  if (pattern.len > len)
    return npos();
  for (size_t offs = startIndex; offs != (len - pattern.len + 1); ++offs) {
    size_t remaining = len - offs;
    if (Span(begin + offs, remaining).startsWith(pattern)) {
      return offs;
    }
  }
  return npos();
}
//---------------------------------------------------------------------
String Span::toString() const
// Convert to a string
{
  String result;
  result.reserve(len);
  for (const char* iter = begin, *limit = begin + len; iter != limit; ++iter) {
    result.concat(*iter);
  }
  return result;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// XML Parser
//---------------------------------------------------------------------
static bool isWhiteSpaceAscii(char c) { return (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n'); }
//---------------------------------------------------------------------
/// Skip whitespaces
static const char* skipWhitespace(const char* iter, const char* limit) {
  while (iter != limit && isWhiteSpaceAscii(*iter)) {
    ++iter;
  }
  return iter;
}
//---------------------------------------------------------------------
/// Skip a string used in an XML attribute
/// \returns Pointer past the end of the string or nullptr, if the string is not terminated
/// \pre iter points to the opening "
static const char* skipString(const char* iter, const char* limit) {
  bool escape = false;
  // Skip the opening "
  ++iter;
  while (iter != limit) {
    switch (*iter) {
      case '\\':
        escape = true;
        ++iter;
        continue;
      case '"':
        if (escape) {
          escape = false;
          ++iter;
          continue;
        } else {
          return iter + 1;
        }
      default:
        escape = false;
        ++iter;
        continue;
    }
  }
  // We didn't see a trailing ", so the string is malformed
  return nullptr;
}
//---------------------------------------------------------------------
/// Skip an XML tag, i.e. everything between < and >
/// \pre iter points to the opening <
static const char* skipXmlTag(const char* iter, const char* limit) {
  iter = skipWhitespace(iter, limit);
  if (iter == limit) return nullptr;
  if (*iter != '<') return nullptr;
  // Skip the opening <
  ++iter;
  while (iter && (iter != limit)) {
    switch (*iter) {
      case '"':
        iter = skipString(iter, limit);
        break;
      case '<':
        // This hints that the tag is malformed
        return nullptr;
      case '>':
        ++iter;
        return iter;
      default:
        ++iter;
        break;
    }
  }
  // The tag is malformed since we didn't see the trailing >
  return nullptr;
}
//---------------------------------------------------------------------
/// Extract name of a XML tag
/// \pre iter and limit must either be equal or iter must point to an opening <
static Span extractTagName(const char* iter, const char* limit, bool* isClosing = nullptr) {
  iter = skipWhitespace(iter, limit);
  if (iter == limit) return Span();
  if (*iter != '<') return Span();
  ++iter;
  if (iter == limit) return Span();
  auto tagNameBegin = iter;
  if ((*tagNameBegin == '/') && isClosing) {
    *isClosing = true;
    ++tagNameBegin;
  }
  ++iter;
  while (iter != limit) {
    char c = *iter;
    if ((c == '/') || (c == '>') || isWhiteSpaceAscii(c)) {
      break;
    }
    ++iter;
  }
  return Span(tagNameBegin, iter);
}
//---------------------------------------------------------------------
enum class XmlTagMode : uint8_t { OpeningTag, ClosingTag, SelfClosingTag };
//---------------------------------------------------------------------
/// Skip an XML tag and return 
static const char* skipXmlTagIntrinsic(const char* iter, const char* limit, Span& tagName, XmlTagMode& mode) {
  bool isClosingTag = false;
  tagName = extractTagName(iter, limit, &isClosingTag);
  // Invalid XML tag, we cannot skip it
  if (tagName.getLength() == 0) return nullptr;
  iter = tagName.getEnd();
  if (iter == limit) return nullptr;
  // Otherwise, we're inside the tag now, pointing right after the tag name
  while (iter && (iter != limit)) {
    char c = *iter;
    switch (c) {
      case '/':
        ++iter;
        if (iter == limit) return nullptr;
        if (*iter == '>' && !isClosingTag) {
          mode = XmlTagMode::SelfClosingTag;
          ++iter;
          return iter;
        } else {
          return nullptr;
        }
        break;
      case '<':
        // malformed tag
        return nullptr;
      case '>':
        ++iter;
        mode = (isClosingTag ? XmlTagMode::ClosingTag : XmlTagMode::OpeningTag);
        return iter;
      case '"':
        iter = skipString(iter, limit);
        break;
      default:
        ++iter;
        break;
    }
  }
  // Tag did not close
  return nullptr;
}
//---------------------------------------------------------------------
XmlDocument::XmlDocument(const Span& data)
// Constructor
{ 
  this->data = skipHeader(data); 
}
//---------------------------------------------------------------------
Span XmlDocument::skipHeader(const Span& data)
// Skip XML header
{
  const char* iter = data.getBegin();
  const char* limit = data.getEnd();
  iter = skipWhitespace(iter, limit);
  if (Span(iter, limit).startsWith(xmlHeaderStart)) {
    iter = skipXmlTag(iter, limit);
    if (!iter) {
      return Span();
    }
  }
  iter = skipWhitespace(iter, limit);
  if (iter != limit) {
    // Ensure the document starts with a <
    if (*iter != '<') {
      return Span();
    }
  }
  return Span(iter, limit);
}
//---------------------------------------------------------------------
Span XmlDocument::getName() const
// Get the name of the current tag
{
  const char* iter = data.getBegin();
  const char* limit = data.getEnd();
  return extractTagName(iter, limit);
}
//---------------------------------------------------------------------
static Span skipBody(const char* iter, const char* limit, const char*& afterEndingTag)
// Skip over the body
// Returns the content of the XML body
// afterEndingTag points after the closing tag that delimits the body
{
  // Skip the opening tag, need to remember the name so we can do at
  // least very basic sanity checking here.
  Span openingTagName;
  XmlTagMode openingTagMode;
  iter = skipXmlTagIntrinsic(iter, limit, openingTagName, openingTagMode);
  if (!iter || (iter == limit)) return Span();
  switch (openingTagMode) {
    case XmlTagMode::OpeningTag: break;
    case XmlTagMode::ClosingTag: return Span(); // invalid XmlDocument
    case XmlTagMode::SelfClosingTag: {
      afterEndingTag = iter;
      return Span(iter,iter);
    }
  }
  // This is the begin of the body
  const char* bodyBegin = iter;
  int16_t level = 1; // start at 1 since we skipped the opening tag
  while (iter && (iter != limit)) {
    if (iter == limit) break;
    switch (*iter) {
      case '<': {
        Span tagName;
        XmlTagMode mode;
        const char* beforeClosingTagBegin = iter;
        iter = skipXmlTagIntrinsic(iter, limit, tagName, mode);
        switch (mode) {
          case XmlTagMode::OpeningTag:
            ++level;
            break;
          case XmlTagMode::ClosingTag:
            --level;
            if (level == 0) {
              if (tagName == openingTagName) {
                afterEndingTag = iter;
                return Span(bodyBegin, beforeClosingTagBegin);
              } else {
                return Span();
              }
            }
          case XmlTagMode::SelfClosingTag:
            break; // ignore
        }
        break;
      }
      case '>':
        // Malformed XML
        return Span();
      default:
        // Whitespaces or any other content, just ignore it
        ++iter;
        break;
    }
  }
  return Span();
}
//---------------------------------------------------------------------
Span XmlDocument::getBodyAsTextRaw() const
// Get body of this XML document as text
{
  const char* iter = data.getBegin();
  const char* limit = data.getEnd();
  const char* afterEndingTag; // unused
  return skipBody(iter, limit, afterEndingTag);
}
//---------------------------------------------------------------------
String XmlDocument::getBodyAsText() const
// Get body of this XML document as text, resolving predefined entities and converting utf-8 to extended ascii
{
  Span bodyRaw = getBodyAsTextRaw();
  const char* iter = bodyRaw.getBegin();
  const char* limit = bodyRaw.getEnd();

  String result;
  result.reserve(bodyRaw.getLength());
  while (iter != limit) {
    if (*iter == '&') {
      ++iter;
      auto remaining = limit - iter;
      if (remaining >= 5 && iter[0] == 'q' && iter[1] == 'u' && iter[2] == 'o' && iter[3] == 't' && iter[4] == ';') {
        iter += 5;
        result.concat('"');
      } else if (remaining >= 4 && iter[0] == 'a' && iter[1] == 'm' && iter[2] == 'p' && iter[3] == ';') {
        iter += 4;
        result.concat('&');
      } else if (remaining >= 5 && iter[0] == 'a' && iter[1] == 'p' && iter[2] == 'o' && iter[3] == 's' && iter[4] == ';') {
        iter += 5;
        result.concat('\'');
      } else if (remaining >= 3 && iter[0] == 'l' && iter[1] == 't' && iter[2] == ';') {
        iter += 3;
        result.concat('<');
      } else if (remaining >= 3 && iter[0] == 'g' && iter[1] == 't' && iter[2] == ';') {
        iter += 3;
        result.concat('>');
      } else {
        result.concat('&');
        ++iter;
      }
      continue;
    }
    if (*iter < 128) {
      result.concat(*iter);
    } else if (iter + 1 != limit) {
      // Handle extended ASCII
      char ch = *iter;
      ++iter;
      switch (ch) {
        case 0xC2: result.concat(*iter); break;
        case 0xC3: result.concat(static_cast<char>(*iter | 0xC0)); break;
        case 0x82:
          if (*iter == 0xAC) {
            // Only support euro symbol
            result.concat(0x80);
          }
          break;
      }
    }
    ++iter;
  }
  return result;
}
//---------------------------------------------------------------------
XmlDocument XmlDocument::getBody() const
// Get content as XML document
{
  // Let's reuse the body and just make sure it starts with a tag < character
  Span body = getBodyAsTextRaw();
  const char* iter = body.getBegin();
  const char* limit = body.getEnd();
  if (iter == limit) return XmlDocument(Span(), NoXmlHeader);
  iter = skipWhitespace(iter, limit);
  if (iter == limit) return XmlDocument(Span(), NoXmlHeader);
  if (*iter != '<') return XmlDocument(Span(), NoXmlHeader);
  return XmlDocument(Span(iter, limit));
}
//---------------------------------------------------------------------
Span XmlDocument::getAttribute(const char* name) const
// Get the attribute with the specified name
{
  // Get the name of the opening tag, the end of the name will point right to the character
  // after the tag name
  Span openingTagName = getName();
  const char* iter = openingTagName.getEnd();
  const char* limit = data.getEnd();
  const char* attrNameBegin = nullptr;
  while (iter != limit) {
    iter = skipWhitespace(iter, limit);
    if (iter == limit) break;
    switch (*iter) {
      case '/':
      case '>':
        // Indicates closing tag, we didn't find the attribute yet, so stop here
        return Span();
      case '<':
        // Invalid tag, but let's just say we didn't find it
        return Span();
      case '=': {
        // Marks the end of the attribute name
        if (attrNameBegin == nullptr) return Span();
        const char* attrNameEnd = iter;
        // Skip the corresponding value
        ++iter;
        iter = skipWhitespace(iter, limit);
        if (iter == limit) return Span();
        if (*iter != '"') return Span();
        ++iter;
        if (iter == limit) return Span();
        const char* valueBegin = iter;
        iter = skipString(iter, limit);
        if (!iter || (iter == limit)) return Span();
        const char* valueEnd = iter;
        if (Span(attrNameBegin, attrNameEnd) == name) {
          return Span(valueBegin, valueEnd-1);
        } else {
          attrNameBegin = nullptr;
        }
        continue;
      }
      default:
        if (!attrNameBegin) {
          attrNameBegin = iter;
        }
        ++iter;
        break;
    }
  }
  // We could not find the attribute
  return Span();
}
//---------------------------------------------------------------------
XmlDocument XmlDocument::getSibling(const char* name) const
// Get a sibling with the specified name
{
  if (getName() == name) {
    return *this;
  }
  // Find sibling
  const char* iter = data.getBegin();
  const char* limit = data.getEnd();
  while (iter!=limit) {
    Span body = skipBody(iter, limit, /*out*/ iter);
    if (!body) return XmlDocument();
    if (iter == limit) return XmlDocument();
    iter = skipWhitespace(iter, limit);
    if (extractTagName(iter, limit) == name) {
      return XmlDocument(Span(iter,limit));
    }
  }
  return XmlDocument();
}
//---------------------------------------------------------------------
}