#pragma once
//---------------------------------------------------------------------
#include <type_traits>
//---------------------------------------------------------------------
namespace hox {
//---------------------------------------------------------------------
namespace __detail {
//---------------------------------------------------------------------
template<typename T1, typename ...TArg> struct MaxSize;
//---------------------------------------------------------------------
template<typename T1>
struct MaxSize<T1> {
  static constexpr size_t value = sizeof(T1);
};
//---------------------------------------------------------------------
template<typename T1, typename ...TArg>
struct MaxSize {
  static constexpr size_t value = sizeof(T1) >= MaxSize<TArg...>::value ? sizeof(T1) : MaxSize<TArg...>::value;
};
//---------------------------------------------------------------------
template<typename T1, typename T2, typename ...TArg> struct Contains;
//---------------------------------------------------------------------
template<typename T1, typename T2>
struct Contains<T1, T2> {
  using t1_t = typename std::remove_reference<std::remove_cv<T1>>::type;
  using t2_t = typename std::remove_reference<std::remove_cv<T2>>::type;
  static constexpr bool value = std::is_same<t1_t, t2_t>::value;
};
//---------------------------------------------------------------------
template<typename T1, typename T2, typename ...TArg>
struct Contains {
  static constexpr bool value = Contains<T1, T2>::value || Contains<T1, TArg...>::value;
};
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------
/// Uninitialized memory
struct Byte {
  /// The data
  char data;

  /// Constructor
  Byte() {}
};
//---------------------------------------------------------------------
/// Variant type
template<typename ...TArg>
class Variant {
  /// The raw data
  Byte data[__detail::MaxSize<TArg...>::value];
  
  public:
  /// Constructor
  Variant() {}

  /// Emplace a value
  template<typename T, typename ...TArgs>
  T& emplace(TArgs&& ...args) { 
    static_assert(__detail::Contains<T, TArg...>::value, "Type must be contained in the variant declaration");
    new (reinterpret_cast<T*>(data)) T(args...); return get<T>(); 
  }

  /// Get a value
  template<typename T>
  T& get() { return *reinterpret_cast<T*>(data); }
};
//---------------------------------------------------------------------
}
//---------------------------------------------------------------------