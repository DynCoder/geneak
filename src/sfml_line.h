#ifndef SFML_LINE_H
#define SFML_LINE_H

#include <SFML/Graphics.hpp>

class sfml_line
{
  public:
  
    sfml_line(double x = 0.0, double y = 0.0, double to_x = 1.0, double to_y = 1.0, double width = 3.0);
    
    sf::RectangleShape get_shape();
    
    void set_from(double x, double y);
    
    void set_to(double x, double y);
    
    void set_width(double width) { m_width = width; }
    
    void set_color(sf::Color color) { m_color = color; }
    
  private:
  
    double m_x;
    double m_y;
    double m_to_x;
    double m_to_y;
    double m_width;
    sf::Color m_color;
    
};

#endif // SFML_LINE_H