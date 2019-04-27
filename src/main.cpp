#include "game_state.h"
#include "sfml_window_manager.h"
#include "sfml_drawing_screen.h"
#include "sfml_text_input.h"
#include "sfml_button.h"
#include "sfml_resources.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <SFML/Graphics.hpp>
#include <unistd.h>

void test() {
  //test_sfml_window_manager();
  test_normal_char();
}

int show_sfml_drawing_screen(int ca) {
  sfml_drawing_screen ds(ca);
  ds.exec();
  return 0;
}

int main(int argc, char **argv) {
  #ifndef NDEBUG
  test();
  #else
  assert(1 == 2);
  #endif

  const std::vector<std::string> args(argv, argv + argc);
  int close_at = -1;
  
#ifdef CI
  std::clog << system("ls") << "\n";
#endif
  
  if (std::count(std::begin(args), std::end(args), "--version")) {
    // Travis: 2.1
    // RuG: 2.3.2
    std::cout
      << "SFML version: " << SFML_VERSION_MAJOR
      << "." << SFML_VERSION_MINOR
      #if(SFML_VERSION_MINOR > 1)
      << "." << SFML_VERSION_PATCH
      #endif
      << std::endl
    ;
    return 0;
  }
  
  if (std::count(std::begin(args), std::end(args), "--test")) {
    std::cout << "Hello world!" << std::endl;
    return 0;
  }
  
  if (std::count(std::begin(args), std::end(args), "--ci")) {
    close_at = 1000;
  }
  
  while (sfml_window_manager::get().get_window().isOpen()) {
    switch (sfml_window_manager::get().get_state()) {
      case game_state::drawing:
        show_sfml_drawing_screen(close_at);
        break;
    }
  }
  
  return 0;
}