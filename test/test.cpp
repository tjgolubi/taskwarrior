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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test.h>
#include <unistd.h>

#include <iomanip>
#include <print>

///////////////////////////////////////////////////////////////////////////////
UnitTest::UnitTest() : _planned(0), _counter(0), _passed(0), _failed(0), _skipped(0) {}

///////////////////////////////////////////////////////////////////////////////
UnitTest::UnitTest(int planned)
    : _planned(planned), _counter(0), _passed(0), _failed(0), _skipped(0) {
  std::println("1..{}", _planned);
}

///////////////////////////////////////////////////////////////////////////////
UnitTest::~UnitTest() {
  float percentPassed = 0.0;
  if (_planned > 0)
    percentPassed = (100.0 * _passed) / std::max(_planned, _passed + _failed + _skipped);

  if (_counter < _planned) {
    std::println("# Only {} tests, out of a planned {} were run.", _counter, _planned);
    _skipped += _planned - _counter;
  }

  else if (_counter > _planned)
    std::println("# {} tests were run, but only {} were planned.", _counter, _planned);

  std::println("# {} passed, {} failed, {} skipped. {:.3g}% passed.", _passed, _failed, _skipped,
               percentPassed);
  exit(_failed > 0);
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::plan(int planned) {
  _planned = planned;
  _counter = 0;
  _passed = 0;
  _failed = 0;
  _skipped = 0;

  std::println("1..{}", _planned);
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::planMore(int extra) {
  _planned += extra;
  std::println("1..{}", _planned);
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::ok(bool expression, const std::string& name, bool expfail /* = false */) {
  ++_counter;

  bool success = expression;

  if (success and !expfail) {
    ++_passed;
    std::println("{} {} - {}", green("ok"), _counter, name);
  } else {
    if (success == expfail) ++_failed;
    std::println("{} {} - {}{}", red("not ok"), _counter, name,
                 (expfail ? (success ? " # FIXED" : " # TODO") : ""));
  }
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::notok(bool expression, const std::string& name, bool expfail /* = false */) {
  ++_counter;

  bool success = not expression;

  if (success and !expfail) {
    ++_passed;
    std::println("{} {} - {}", green("ok"), _counter, name);
  } else {
    if (success == expfail) ++_failed;
    std::println("{} {} - {}{}", red("not ok"), _counter, name,
                 (expfail ? (success ? " # FIXED" : " # TODO") : ""));
  }
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::is(bool actual, bool expected, const std::string& name, bool expfail /* = false */) {
  ++_counter;
  bool success = (actual == expected);

  if (success and !expfail) {
    ++_passed;
    std::println("{} {} - {}", green("ok"), _counter, name);
  } else {
    if (success == expfail) ++_failed;
    std::println("{} {} - {}{}\n# expected: {:d}\n#      got: {:d}", red("not ok"), _counter, name,
                 (expfail ? (success ? " # FIXED" : " # TODO") : ""), expected, actual);
  }
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::is(size_t actual, size_t expected, const std::string& name,
                  bool expfail /* = false */) {
  ++_counter;
  bool success = (actual == expected);

  if (success and !expfail) {
    ++_passed;
    std::println("{} {} - {}", green("ok"), _counter, name);
  } else {
    if (success == expfail) ++_failed;
    std::println("{} {} - {}{}\n# expected: {}\n#      got: {}", red("not ok"), _counter, name,
                 (expfail ? (success ? " # FIXED" : " # TODO") : ""), expected, actual);
  }
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::is(int actual, int expected, const std::string& name, bool expfail /* = false */) {
  ++_counter;
  bool success = (actual == expected);

  if (success and !expfail) {
    ++_passed;
    std::println("{} {} - {}", green("ok"), _counter, name);
  } else {
    if (success == expfail) ++_failed;
    std::println("{} {} - {}{}\n# expected: {}\n#      got: {}", red("not ok"), _counter, name,
                 (expfail ? (success ? " # FIXED" : " # TODO") : ""), expected, actual);
  }
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::is(double actual, double expected, const std::string& name,
                  bool expfail /* = false */) {
  ++_counter;
  bool success = (actual == expected);

  if (success and !expfail) {
    ++_passed;
    std::println("{} {} - {}", green("ok"), _counter, name);
  } else {
    if (success == expfail) ++_failed;
    std::println("{} {} - {}{}\n# expected: {:.6g}\n#      got: {:.6g}", red("not ok"), _counter,
                 name, (expfail ? (success ? " # FIXED" : " # TODO") : ""), expected, actual);
  }
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::is(double actual, double expected, double tolerance, const std::string& name,
                  bool expfail /* = false */) {
  ++_counter;
  bool success = (fabs(actual - expected) <= tolerance);

  if (success and !expfail) {
    ++_passed;
    std::println("{} {} - {}", green("ok"), _counter, name);
  } else {
    if (success == expfail) ++_failed;
    std::println("{} {} - {}{}\n# expected: {:.6g}\n#      got: {:.6g}", red("not ok"), _counter,
                 name, (expfail ? (success ? " # FIXED" : " # TODO") : ""), expected, actual);
  }
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::is(unsigned char actual, unsigned char expected, const std::string& name,
                  bool expfail /* = false */) {
  ++_counter;
  bool success = (actual == expected);

  if (success and !expfail) {
    ++_passed;
    std::println("{} {} - {}", green("ok"), _counter, name);
  } else {
    if (success == expfail) ++_failed;
    std::println("{} {} - {}{}\n# expected: {}\n#      got: {}", red("not ok"), _counter, name,
                 (expfail ? (success ? " # FIXED" : " # TODO") : ""), static_cast<char>(expected),
                 static_cast<char>(actual));
  }
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::is(const std::string& actual, const std::string& expected, const std::string& name,
                  bool expfail /* = false */) {
  ++_counter;
  bool success = (actual == expected);

  if (success and !expfail) {
    ++_passed;
    std::println("{} {} - {}", green("ok"), _counter, name);
  } else {
    if (success == expfail) ++_failed;
    std::println("{} {} - {}{}\n# expected: '{}'\n#      got: '{}'", red("not ok"), _counter, name,
                 (expfail ? (success ? " # FIXED" : " # TODO") : ""), expected, actual);
  }
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::is(const char* actual, const char* expected, const std::string& name,
                  bool expfail /* = false */) {
  ++_counter;
  bool success = (!strcmp(actual, expected));

  if (success and !expfail) {
    ++_passed;
    std::println("{} {} - {}", green("ok"), _counter, name);
  } else {
    if (success == expfail) ++_failed;
    std::println("{} {} - {}{}\n# expected: '{}'\n#      got: '{}'", red("not ok"), _counter, name,
                 (expfail ? (success ? " # FIXED" : " # TODO") : ""), expected, actual);
  }
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::diag(const std::string& text) {
  auto start = text.find_first_not_of(" \t\n\r\f");
  auto end = text.find_last_not_of(" \t\n\r\f");
  if (start != std::string::npos && end != std::string::npos)
    std::println("# {}", text.substr(start, end - start + 1));
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::pass(const std::string& text) {
  ++_counter;
  ++_passed;
  std::println("{} {} - {}", green("ok"), _counter, text);
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::fail(const std::string& text) {
  ++_counter;
  ++_failed;
  std::println("{} {} - {}", red("not ok"), _counter, text);
}

///////////////////////////////////////////////////////////////////////////////
void UnitTest::skip(const std::string& text) {
  ++_counter;
  ++_skipped;
  std::println("{} {} - {} # skip", yellow("ok"), _counter, text);
}

///////////////////////////////////////////////////////////////////////////////
std::string UnitTest::red(const std::string& input) {
  if (isatty(fileno(stdout))) return std::string("\033[31m" + input + "\033[0m");

  return input;
}

///////////////////////////////////////////////////////////////////////////////
std::string UnitTest::green(const std::string& input) {
  if (isatty(fileno(stdout))) return std::string("\033[32m" + input + "\033[0m");

  return input;
}

///////////////////////////////////////////////////////////////////////////////
std::string UnitTest::yellow(const std::string& input) {
  if (isatty(fileno(stdout))) return std::string("\033[33m" + input + "\033[0m");

  return input;
}

///////////////////////////////////////////////////////////////////////////////
