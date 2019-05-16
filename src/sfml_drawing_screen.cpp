#include "sfml_drawing_screen.h"
#include "sfml_window_manager.h"
#include "sfml_resources.h"
#include <iostream>
#include <cassert>

// Base class //

sfml_drawing_screen::sfml_drawing_screen(int ca)
    : close_at{ ca }, m_window{ sfml_window_manager::get().get_window() },
      m_default_font{ sfml_resources::get().get_default_font() }
{
  m_text.setFont(m_default_font);
  m_text.setString("Hello world!");
  
  m_tool_bar.setFillColor(sf::Color(100, 100, 100));
  m_drawing_area.setFillColor(sf::Color(220, 220, 220));
}

void sfml_drawing_screen::exec() {
  set_sizes();
  while (active(game_state::drawing) && close_at != 0) {
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      process_event(event);
    }
    if (!m_window.isOpen()) return;
    set_positions();
    draw_objects();
    if (close_at > 0) --close_at;
  }
  if (close_at == 0) close();
}

void sfml_drawing_screen::process_event(sf::Event event) {
  switch (event.type) {
    case sf::Event::Closed:
        close();
        break;
        
    case sf::Event::Resized:
      {
        sf::View view = m_window.getDefaultView();
        sfml_window_manager::get().update();
        view.setSize(static_cast<float>(m_window.getSize().x),
                     static_cast<float>(m_window.getSize().y));
        m_window.setView(view);
        set_sizes();
      }
      break;
      
    case sf::Event::KeyPressed:
      switch (event.key.code) //!OCLINT too few branches
      {
        case sf::Keyboard::Escape:
          close();
          break;
          
        default:
          break;
      }
      break;
    case sf::Event::MouseButtonPressed:
      break;
    default:
      sfml_window_manager::get().process();
      break;
  }
}

void sfml_drawing_screen::set_positions() {
  m_text.setPosition(m_window.mapPixelToCoords(sf::Vector2i(100, 50 - m_text.getGlobalBounds().height)));
  m_tool_bar.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0, 0)));
  m_drawing_area.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0, 100)));
}

void sfml_drawing_screen::set_sizes() {
  m_tool_bar.setSize(sf::Vector2f(m_window.getSize().x, 100));
  m_drawing_area.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y - 100));
}

void sfml_drawing_screen::draw_objects() {
  m_window.clear();
  m_window.draw(m_tool_bar);
  m_window.draw(m_text);
  m_window.draw(m_drawing_area);
  // sf::View o_view = getView
  // setView(m_draw_view)
  // Draw tree
  // setView(o_view)
  m_window.display();
}

void sfml_drawing_screen::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_drawing_screen::close() {
  m_window.close();
}

////////////////
