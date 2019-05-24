#include "sfml_drawing_screen.h"
#include "sfml_window_manager.h"
#include "sfml_resources.h"
#include "sfml_line.h"
#include <iostream>
#include <cassert>

sfml_drawing_screen::sfml_drawing_screen(int ca)
    : close_at{ ca }, m_window{ sfml_window_manager::get().get_window() },
      m_input(20, 20, 50, 50), tree_lines{}, tree_text{}
{
  m_tool_bar.setFillColor(sf::Color(100, 100, 100));
  m_drawing_area.setFillColor(sf::Color(220, 220, 220));
  m_confirm.get_shape().setFillColor(sf::Color(45, 235, 205));
  
  m_drawing_view = sf::View(sf::Vector2f(m_window.getSize().x / 2, 
                                         (m_window.getSize().y / 2) + 50),
                            sf::Vector2f(m_window.getSize().x, 
                                         m_window.getSize().y - 100));                                       
  
  m_move_left = false;
  m_move_right = false;
  m_move_up = false;
  m_move_down = false;
}

void sfml_drawing_screen::exec() { //!OCLINT can be complex
  set_sizes();
  while (active(game_state::drawing) && close_at != 0) {
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      process_event(event);
    }
    if (!m_window.isOpen()) return;
    
    if (m_move_left) m_drawing_view.move(sf::Vector2f(-0.5f, 0.0f));
    if (m_move_right) m_drawing_view.move(sf::Vector2f(0.5f, 0.0f));
    if (m_move_up) m_drawing_view.move(sf::Vector2f(0.0f, -0.5f));
    if (m_move_down) m_drawing_view.move(sf::Vector2f(0.0f, 0.5f));
    
    m_input.update();
    
    set_positions();
    draw_objects();
    
    if (close_at > 0) --close_at;
  }
  if (close_at == 0) close();
}

void sfml_drawing_screen::process_event(sf::Event event) { //!OCLINT can be complex
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
        update_tree(m_input.get_string());
      }
      break;
      
    case sf::Event::KeyPressed:
      switch (event.key.code)
      {
        case sf::Keyboard::Escape:
          close();
          break;
          
        case sf::Keyboard::Left:
          m_input.left();
          if (!m_input.is_selected()) m_move_left = true;
          break;
          
        case sf::Keyboard::Right:
          m_input.right();
          if (!m_input.is_selected()) m_move_right = true;
          break;
          
        case sf::Keyboard::Up:
          if (!m_input.is_selected()) m_move_up = true;
          break;
          
        case sf::Keyboard::Down:
          if (!m_input.is_selected()) m_move_down = true;
          break;
          
        default:
          break;
      }
      break;
      
    case sf::Event::KeyReleased:
      switch (event.key.code)
        {
          case sf::Keyboard::Left:
            m_move_left = false;
            break;
            
          case sf::Keyboard::Right:
            m_move_right = false;
            break;
            
          case sf::Keyboard::Up:
            m_move_up = false;
            break;
            
          case sf::Keyboard::Down:
            m_move_down = false;
            break;
            
          default:
            break;
        }
      break;
    
    case sf::Event::MouseButtonPressed:
      m_input.select(m_window);
      if (m_confirm.is_clicked(event, m_window)) {
        m_newick = m_input.get_string();
      }
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
  
  float tb_per = 100.0/m_window.getSize().y;
  m_drawing_view.setViewport(sf::FloatRect(0, tb_per, 1, 1 - tb_per));
  m_drawing_view.setCenter(0, 0);
}

void sfml_drawing_screen::draw_objects() {
  // Clear window
  m_window.clear();
  
  
  // Draw tool bar
  m_window.draw(m_tool_bar);
  
  m_window.draw(m_input.get_shape());
  m_window.draw(m_input.get_text());
  
  m_window.draw(m_confirm.get_shape());
  m_window.draw(m_confirm.get_text());
  
  
  // Draw tree viewer
  m_window.draw(m_drawing_area);
  
  sf::View o_view = m_window.getView();
  m_window.setView(m_drawing_view);
  
  // Draw tree //
  
  for (auto &line : m_tree_lines) {
    m_window.draw(line.get_shape());
  }
  
  for (auto &txt : m_tree_text) {
    m_window.draw(txt);
  }
  
  ///////////////
  
  m_window.setView(o_view);
  
  
  // Display all to window
  m_window.display();
}

void sfml_drawing_screen::close(game_state s) {
  sfml_window_manager::get().set_state(s);
}

void sfml_drawing_screen::close() {
  m_window.close();
}

void sfml_drawing_screen::update_tree(std::string in) {
  m_tree_lines.clear();
  m_tree_text.clear();
  // for char in string look if letter
  // if letter push whole word into sf text
  // push back sf text to m_text
  std::clog << in << std::endl;
  // Calculate branches
}
