#include "sfml_resources.h"
#include "game_state.h"
#include "sfml_window_manager.h"
#include "sfml_drawing_screen.h"
#include "sfml_text_input.h"
#include "sfml_button.h"
#include "sfml_line.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <SFML/Graphics.hpp>
#include <unistd.h>

void test() {
  //test_sfml_window_manager();
  test_normal_char();
}

int show_sfml_drawing_screen(std::string tree, std::string path) {
  sfml_drawing_screen ds(tree);
  ds.set_path(path);
  ds.exec();
  return 0;
}

int main(int argc, char **argv) { //!OCLINT
#ifndef NDEBUG
  test();
#else
  assert(1 == 2);
#endif

  std::cout << get_time() << std::endl;

  const std::vector<std::string> args(argv, argv + argc);
  
  std::string tree;
  std::string path = args.at(0);
  
  std::clog << path << std::endl;
  
  if ((!std::count(path.begin(), path.end(), '/')) &&
      (!std::count(path.begin(), path.end(), '\\'))) {
    path = "";
  } else {
    while (path.back() != '/' && path.back() != '\\') {
      path.pop_back();
    }
  }
  
  for (auto &arg : args) {
    if (arg.size() > 4) { //!OCLINT for exception safety non-collapsible
      if (arg.substr(arg.size() - 4) == ".gnk") {
        std::ifstream sfile;
        sfile.open(arg);
        if (sfile.is_open()) {
          std::getline(sfile, tree);
          sfile.close();
        } else {
          tree = "";
          std::clog << "Couldn't open .gnk file" << std::endl;
        }
      }
    }
  }
  sfml_resources::get().load(path);
  
  if (std::count(std::begin(args), std::end(args), "--version")) {
    // Travis: 2.3.2
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
  
  if (std::count(std::begin(args), std::end(args), "--ci")) {
    std::clog << "CI argument passed!\n";
#ifdef CI
    std::clog << "CI defined!\n";
#endif
  }
  
  while (sfml_window_manager::get().get_window().isOpen()) {
    if (sfml_window_manager::get().get_state() == game_state::drawing) {
      show_sfml_drawing_screen(tree, path);
    }
  }
  
  return 0;
}
