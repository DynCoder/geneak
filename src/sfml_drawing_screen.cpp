#include "sfml_drawing_screen.h"
#include "sfml_window_manager.h"
#include "sfml_resources.h"
#include <iostream>
#include <cassert>
// Base class //

sfml_drawing_screen::sfml_drawing_screen(int ca)
    : close_at{ ca }, m_window{ sfml_window_manager::get().get_window() },
      m_input(20, 20, 50, 50)
{
  m_tool_bar.setFillColor(sf::Color(100, 100, 100));
  m_drawing_area.setFillColor(sf::Color(220, 220, 220));
  m_confirm.get_shape().setFillColor(sf::Color(45, 235, 205));
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
    
    m_input.update();
    
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
          
        case sf::Keyboard::Left:
          m_input.left();
          break;
          
        case sf::Keyboard::Right:
          m_input.right();
          break;
          
        default:
          break;
      }
      break;
    
    case sf::Event::MouseButtonPressed:
      m_input.select(m_window);
      break;
    
    case sf::Event::TextEntered:
      m_input.input(event, m_window);
      break;
    
    default:
      sfml_window_manager::get().process();
      break;
  }
}

void sfml_drawing_screen::set_positions() {
  m_tool_bar.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0, 0)));
  m_drawing_area.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0, 100)));
  m_input.set_pos(20, 20, m_window);
  m_confirm.set_pos((((m_window.getSize().x - 40) / 10) * 8) + 50, 20, m_window);
}

void sfml_drawing_screen::set_sizes() {
  m_tool_bar.setSize(sf::Vector2f(m_window.getSize().x, 100));
  m_drawing_area.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y - 100));
  m_input.set_size(((m_window.getSize().x - 40) / 10) * 8, 50, m_window);
  m_confirm.set_size(120, 50, m_window);
}

void sfml_drawing_screen::draw_objects() {
  m_window.clear();
  m_window.draw(m_tool_bar);
  m_window.draw(m_drawing_area);
  
  m_window.draw(m_input.get_shape());
  m_window.draw(m_input.get_text());
  
  m_window.draw(m_confirm.get_shape());
  m_window.draw(m_confirm.get_text());
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
