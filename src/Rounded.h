////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2006 - 2021, Tomas Babej, Paul Beckingham, Federico Hernandez.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// https://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_ROUNDED
#define INCLUDED_ROUNDED

#include <cmath>
#include <format>

template <typename T>
struct Rounded {
  T value;
};

template <typename T>
Rounded<T> rounded(T value, int width) {
  if (0 < value && value < 1) {
    double factor = 1;
    for (int i = 2; i < width; ++i) factor *= 10;

    // Round in the original floating-point type, as libshared does.
    value = static_cast<T>(std::round(static_cast<T>(value * factor)) / factor);
  }
  return {value};
}

template <typename T>
struct std::formatter<Rounded<T>> : std::formatter<T> {
  template <typename FormatContext>
  auto format(const Rounded<T>& arg, FormatContext& ctx) const {
    return std::formatter<T>::format(arg.value, ctx);
  }
};

#endif
