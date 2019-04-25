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
  m_text.setString("Hello world!\n\nsample text");
}

void sfml_drawing_screen::exec() {
  std::clog << "Start exec loop\n";
  while (active(game_state::drawing) && close_at != 0) {
    std::clog << "Create event\n";
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      std::clog << "process event\n";
      process_event(event);
    }
    std::clog << "Set pos and draw\n";
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
      }
      break;
      
    case sf::Event::KeyPressed:
      switch (event.key.code)
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
      std::clog << "Process window manager\n";
      sfml_window_manager::get().process();
      break;
  }
}

void sfml_drawing_screen::set_positions() {
  m_text.setPosition(m_window.mapPixelToCoords(sf::Vector2i(100, 100)));
}

void sfml_drawing_screen::draw_objects() {
  m_window.clear(sf::Color(120, 120, 120));
  m_window.draw(m_text);
  m_window.display();
}

void sfml_drawing_screen::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_drawing_screen::close() {
  m_window.close();
}

////////////////
