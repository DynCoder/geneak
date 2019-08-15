#include "sfml_resources.h"
#include <cassert>

sfml_resources *sfml_resources::m_instance = nullptr; //!OCLINT static accepted singleton

sfml_resources::sfml_resources() {
  
}

void sfml_resources::load(std::string path) {
  {
    std::string filename = path + "sourcesans.otf";
    if (!m_default_font.loadFromFile(filename))
      throw std::runtime_error("Couldn't find resource " + filename);
  }
  {
    std::string filename = path + "draw.png";
    if (!m_draw_image.loadFromFile(filename))
      throw std::runtime_error("Couldn't find resource " + filename);
  }
  {
    std::string filename = path + "add.png";
    if (!m_add_image.loadFromFile(filename))
      throw std::runtime_error("Couldn't find resource " + filename);
  }
  {
    std::string filename = path + "remove.png";
    if (!m_remove_image.loadFromFile(filename))
      throw std::runtime_error("Couldn't find resource " + filename);
  }
  {
    std::string filename = path + "split.png";
    if (!m_split_image.loadFromFile(filename))
      throw std::runtime_error("Couldn't find resource " + filename);
  }
}

sfml_resources &sfml_resources::get() {
  if (!m_instance)
    m_instance = new sfml_resources();
  assert(m_instance);
  return *m_instance;
}
