#include "sfml_line.h"
#include <math.h>
#include <cassert>

#define PI 3.14159265358979323846

sfml_line::sfml_line(double x, double y, double to_x, double to_y, double width)
  :m_x{x}, m_y{y}, m_to_x{to_x}, m_to_y{to_y}, m_width{width}
{
  m_color = sf::Color(0, 0, 0);
}

sf::RectangleShape sfml_line::get_shape() {
  sf::RectangleShape shape;
  shape.setFillColor(m_color);
  if (m_x > m_to_x) {
    double tmp = m_x;
    m_x = m_to_x;
    m_to_x = tmp;
  }
  if (m_y > m_to_y) {
    double tmp = m_y;
    m_y = m_to_y;
    m_to_y = tmp;
  }
  double dx = m_to_x - m_x;
  double dy = m_to_y - m_y;
  assert(dx >= 0);
  assert(dy >= 0);
  double length = sqrt(pow(dx, 2) + pow(dy, 2));
  if (length <= 0) length = m_width;
  // NOTE inverted width and height here for good rotation
  shape.setSize(sf::Vector2f(length, m_width));
  shape.setOrigin(m_width/2.0,m_width/2.0);
  shape.setPosition(m_x, m_y);
  double rot;
  if (dx == 0) {
    rot = 90;
  } else {
    rot = atan(dy/dx) * 180 / PI;
  }
  shape.setRotation(rot);
  return shape;
}

void sfml_line::set_from(double x, double y) {
  m_x = x;
  m_y = y;
}

void sfml_line::set_to(double x, double y) {
  m_to_x = x;
  m_to_y = y;
}