#include "sfml_line.h"
#include <math.h>

sfml_line::sfml_line(double x, double y, double to_x, double to_y, double width)
  :m_x{x}, m_x{y}, m_to_x{to_x}, m_to_y{to_y}, m_width{width}
{
  
}

sf::RectangleShape get_shape() {
  // TODO: return shape with calculated rotation
  sf::RectangleShape shape;
  double dx = m_to_x - m_x;
  double dy = m_to_y - m_y;
  double length = sqrt(pow(dx, 2) + pow(dy, 2));
  // NOTE inverted width and height here for good rotation
  shape.setSize(sf::Vector2f(length, m_width);
  //shape.setPosition(m_x, m_y);
  //shape.setRotation
}

void set_from(double x, double y) {
  m_x = x;
  m_y = y;
}

void set_to(double x, double y) {
  m_to_x = x;
  m_to_y = y;
}