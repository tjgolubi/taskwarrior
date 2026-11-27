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

#include <cmake.h>
// cmake.h include header must come first

#include <Context.h>
#include <rust/cxx.h>
#include <signal.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <new>
#include <print>
#include <regex>

////////////////////////////////////////////////////////////////////////////////
int main(int argc, const char** argv) {
  int status{0};

  // Ignore SIGPIPE from writes to network sockets after the remote end has hung
  // up. Rust code expects this, and the Rust runtime ignores this signal at startup.
  signal(SIGPIPE, SIG_IGN);

  Context globalContext;
  Context::setContext(&globalContext);

  // Reuse the existing help command instead of maintaining separate output.
  if (argc == 2 && !strcmp(argv[1], "--help")) {
    argv[1] = "help";
  }

  // Lightweight version checking that doesn't require initialization or any I/O.
  if (argc == 2 && !strcmp(argv[1], "--version")) {
    std::println(std::cout, "{}", VERSION);
  } else {
    try {
      status = Context::getContext().initialize(argc, argv);
      if (status == 0) status = Context::getContext().run();
    }

    catch (const std::string& error) {
      std::println(std::cerr, "{}", error);
      status = -1;
    }

    catch (rust::Error& err) {
      std::println(std::cerr, "{}", err.what());
      status = -1;
    }

    catch (std::bad_alloc& error) {
      // Report allocation failure without allocating a formatted string.
      std::fflush(stdout);
      std::fputs("Error: Memory allocation failed: ", stderr);
      std::fputs(error.what(), stderr);
      std::fputc('\n', stderr);
      std::fflush(stderr);
      status = -3;
    }

    catch (const std::regex_error& e) {
      std::println(std::cout, "regex_error caught: {}", e.what());
    }

    catch (...) {
      std::println(std::cerr, "Unknown error. Please report.");
      status = -2;
    }
  }

  return status;
}

////////////////////////////////////////////////////////////////////////////////
