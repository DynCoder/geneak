#ifndef SFML_RESOURCES_H
#define SFML_RESOURCES_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

/// Singleton class to access all resources
class sfml_resources { //!OCLINT ignore too many fields for now

public:

  /// Access the resources
  static sfml_resources &get();
  
  void load(std::string path);
  
  // Resource getters //
  
  sf::Font& get_default_font() noexcept { return m_default_font; }
  
  sf::Texture& get_draw_image() noexcept { return m_draw_image; }
  
  sf::Texture& get_add_image() noexcept { return m_add_image; }
  
  sf::Texture& get_remove_image() noexcept { return m_remove_image; }
  
  sf::Texture& get_split_image() noexcept { return m_split_image; }
  
  //////////////////////
  
private:

  /// Do not construct a sfml_resources,
  /// use sfml_resources::get() instead
  sfml_resources();
  
  static sfml_resources *m_instance; //!OCLINT accepted for now
  
  // Resources //
  
  sf::Font m_default_font;
  
  sf::Texture m_draw_image;

  sf::Texture m_add_image;
  
  sf::Texture m_remove_image;
  
  sf::Texture m_split_image;
  
  ///////////////

};

#endif // SFML_RESOURCES_H
