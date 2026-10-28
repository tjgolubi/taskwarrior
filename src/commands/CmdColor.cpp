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

#include <CmdColor.h>
#include <Color.h>
#include <Context.h>
#include <Table.h>
#include <format.h>
#include <shared.h>

#include <fmt/color-ostream.h>

#include <sstream>

////////////////////////////////////////////////////////////////////////////////
CmdColor::CmdColor() {
  _keyword = "colors";
  _usage = "task          colors [sample | legend]";
  _description = "All colors, a sample, or a legend";
  _read_only = true;
  _displays_id = false;
  _needs_gc = false;
  _needs_recur_update = false;
  _uses_context = false;
  _accepts_filter = false;
  _accepts_modifications = false;
  _accepts_miscellaneous = true;
  _category = Command::Category::misc;
}

////////////////////////////////////////////////////////////////////////////////
int CmdColor::execute(std::string& output) {
  int rc = 0;

  // Get the non-attribute, non-fancy command line arguments.
  auto legend = false;
  auto words = Context::getContext().cli2.getWords();
  for (auto& word : words)
    if (closeEnough("legend", word)) legend = true;

  std::stringstream out;
  if (Context::getContext().color()) {
    // If the description contains 'legend', show all the colors currently in
    // use.
    if (legend) {
      out << "\nHere are the colors currently in use:\n";

      Table view;
      view.width(Context::getContext().getWidth());
      view.withColor(Context::getContext().config.getBoolean("color"));
      view.add("Color");
      view.add("Definition");

      for (auto& item : Context::getContext().config) {
        // Skip items with 'color' in their name, that are not referring to
        // actual colors.
        if (item.first != "_forcecolor" && item.first != "color" && item.first.find("color") == 0) {
          auto color = Style::Parse(Context::getContext().config.get(item.first));
          int row = view.addRow();
          view.set(row, 0, item.first, color);
          view.set(row, 1, item.second, color);
        }
      }

      out << view.render() << '\n';
    }

    // If there is something in the description, then assume that is a color,
    // and display it as a sample.
    else if (words.size()) {
      const auto spec1 = std::string{"black on bright yellow"};
      const auto spec2 = std::string{"underline cyan on bright blue"};
      const auto spec3 = std::string{"color214 on color202"};
      const auto spec4 = std::string{"rgb150 on rgb020"};
      const auto spec5 = std::string{"underline grey10 on grey3"};
      const auto spec6 = std::string{"red on color173"};
      const auto style1 = Style::Parse(spec1);
      const auto style2 = Style::Parse(spec2);
      const auto style3 = Style::Parse(spec3);
      const auto style4 = Style::Parse(spec4);
      const auto style5 = Style::Parse(spec5);
      const auto style6 = Style::Parse(spec6);

      std::string swatch;
      for (auto word = words.begin(); word != words.end(); ++word) {
        if (word != words.begin()) swatch += ' ';

        swatch += *word;
      }

      fmt::print(out,
"\nUse this command to see how colors are displayed by your terminal.\n"
"\n\n"
"16-color usage (supports underline, bold text, bright foreground/background):\n"
"  {}\n"
"  {}\n"
"\n"
"256-color usage (supports underline):\n"
"  {}\n"
"  {}\n"
"  {}\n"
"  {}\n"
"\n"
"Your sample:\n\n"
"  {}\n\n", fmt::styled("task color " + spec1 , style1),
            fmt::styled("task color " + spec2 , style2),
            fmt::styled("task color " + spec3 , style3),
            fmt::styled("task color " + spec4 , style4),
            fmt::styled("task color " + spec5 , style5),
            fmt::styled("task color " + spec6 , style6),
            fmt::styled("task color " + swatch, Style::Parse(swatch)));
    }

    // Show all supported colors.  Possibly show some unsupported ones too.
    else {
      using Term     = fmt::term_color;

      constexpr auto bold_red   = fmt::fg(Term::red)  .bold();
      constexpr auto bold_green = fmt::fg(Term::green).bold();
      constexpr auto bold_blue  = fmt::fg(Term::blue) .bold();

      constexpr std::string_view color_names[8] = {
        "black", "red", "blue", "green", "magenta", "cyan", "yellow", "white"
      };

      constexpr Term colors[8] = {
        Term::black,   Term::red,  Term::blue,   Term::green,
        Term::magenta, Term::cyan, Term::yellow, Term::white
      };

      out << "\nBasic colors\n";
      for (int i = 0; i < 8; ++i) {
        out << ' ';
        fmt::print(out, Color{colors[i]}, " {} ", color_names[i]);
      }
      out << '\n';

      auto base = Color{Term::white};
      for (int i = 0; i < 8; ++i) {
        // Use white text before green, then black text for the remaining backgrounds.
        if (i == 3) base.fg(Term::black);
        out << ' ';
        fmt::print(out, overlay(base, bg(colors[i])), " {} ", color_names[i]);
      }
      out << "\n\n";

      const std::string color_specs[6] = {
        "red", "bold red", "underline on blue", "on green", "on bright green",
        "inverse"
      };

      out << "Effects\n";
      for (int i = 0; i < 6; ++i) {
        out << ' ';
        base = (i == 3 || i == 4) ? Color{Term::black} : Color{};
        fmt::print(out, overlay(base, Style::Parse(color_specs[i])),
                   " {} ", color_specs[i]);
      }
      out << "\n\n";

      // 16 system colors.
      out << "color0 - color15" << '\n' << "  0 1 2 . . .\n";
      for (int r = 0; r < 2; ++r) {
        out << "  ";
        for (int i = 0; i < 8; ++i) {
          auto background = static_cast<Term>(r * 8 + i);
          fmt::print(out, fmt::bg(background), "  ");
        }

        out << '\n';
      }

      out << "          . . . 15\n\n";

      // Color cube.
      out << "Color cube rgb" << fmt::styled("0", bold_red)
          << fmt::styled("0", bold_green) << fmt::styled("0", bold_blue) << " - rgb"
          << fmt::styled("5", bold_red)   << fmt::styled("5", bold_green)
          << fmt::styled("5", bold_blue)  << " (also color16 - color231)" << '\n'
          << "  "
          << fmt::styled("0            "
                         "1            "
                         "2            "
                         "3            "
                         "4            "
                         "5", bold_red)
          << '\n'
          << "  "
          << fmt::styled("0 1 2 3 4 5  "
                         "0 1 2 3 4 5  "
                         "0 1 2 3 4 5  "
                         "0 1 2 3 4 5  "
                         "0 1 2 3 4 5  "
                         "0 1 2 3 4 5", bold_blue)
          << '\n';

      for (int g = 0; g < 6; ++g) {
        fmt::print(out, bold_green, " {}", g);
        for (int r = 0; r < 6; ++r) {
          for (int b = 0; b < 6; ++b)
            out << fmt::styled("  ", bg(fmt::cube_color{r, g, b}));
          out << ' ';
        }

        out << '\n';
      }

      out << '\n';

      // Grey ramp.
      out << "Gray ramp gray0 - gray23 (also color232 - color255)\n"
          << "  0 1 2 . . .                             . . . 23\n"
          << "  ";
      for (int g = 0; g < 24; ++g)
        out << fmt::styled("  ", bg(fmt::gray_color{g}));

      out << "\n\n"
          << "Try running 'task color white on red'.\n"
          << '\n';
    }
  } else {
    out << "Color is currently turned off in your .taskrc file.  To enable color, remove the line "
           "'color=off', or change the 'off' to 'on'.\n";
    rc = 1;
  }

  output = out.str();
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
