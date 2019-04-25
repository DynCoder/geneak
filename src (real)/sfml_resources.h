#ifndef SFML_RESOURCES_H
#define SFML_RESOURCES_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

/// Singleton class to access all resources
//TODO: make this a member variable of sfml_game
class sfml_resources { //!OCLINT ignore too many fields for now

public:

  /// Access the resources
  static sfml_resources &get();
  
  // Resource getters //
  
  sf::Font& get_default_font() noexcept { return m_default_font; }
  
  //////////////////////
  
private:

  /// Do not construct a sfml_resources,
  /// use sfml_resources::get() instead
  sfml_resources();
  
  static sfml_resources *m_instance; //!OCLINT accepted for now
  
  // Resources //
  
  sf::Font m_default_font;

  ///////////////

};


#endif // SFML_RESOURCES_H
