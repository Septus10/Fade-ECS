#pragma once

// type aliases
#include <cstdint>
#include <wchar.h>

using u8    = unsigned char;
using u16   = unsigned short;
using u32   = unsigned int;
using u64   = unsigned long;

using i8    = signed char;
using i16   = signed short;
using i32   = signed int;
using i64   = signed long;

using usize = size_t;

using c8    = char;
using c16   = char16_t;
using c32   = char32_t;

// global variables
c8* g_ExecutablePath;
bool g_ShouldRun = true;