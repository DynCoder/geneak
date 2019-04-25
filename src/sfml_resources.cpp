#include "sfml_resources.h"
#include <cassert>

sfml_resources *sfml_resources::m_instance = nullptr; //!OCLINT static accepted singleton

sfml_resources::sfml_resources() {
#ifndef CI
  {
    std::string filename = "texgyrebonum.otf";
    if (!m_default_font.loadFromFile(filename))
      throw std::runtime_error("Couldn't find resource " + filename);
  }
#endif
}

sfml_resources &sfml_resources::get() {
  if (!m_instance)
    m_instance = new sfml_resources();
  assert(m_instance);
  return *m_instance;
}
