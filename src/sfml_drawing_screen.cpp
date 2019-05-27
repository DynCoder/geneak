#include "sfml_drawing_screen.h"
#include "sfml_window_manager.h"
#include "sfml_resources.h"
#include "sfml_line.h"
#include <iostream>
#include <cassert>
#include <functional>

sfml_drawing_screen::sfml_drawing_screen(int ca)
    : close_at{ ca }, m_window{ sfml_window_manager::get().get_window() },
      m_input(20, 20, 50, 50), m_tree_lines{}, m_tree_text{}
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

        case sf::Keyboard::Return:
          if (m_input.is_selected()) {
            std::clog << "Updated!" << std::endl;
            update_tree(m_input.get_string());
          }

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
        std::clog << "Updated!" << std::endl;
        update_tree(m_input.get_string());
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
  m_confirm.set_size(100, 50, m_window);

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

void sfml_drawing_screen::update_tree(std::string in) { //!OCLINT ofc way too complicated
  m_tree_lines.clear();
  m_tree_text.clear();
  if (in.size() == 0) return;

  {
    int parentheses = 0;
    int y = 0;
    std::vector<int> intercoords;
    std::string chars = "";
    char prevchar;
    for (char& c : in) {
      if ((c >= 'A' && c <= 'Z') ||
          (c >= 'a' && c <= 'z') ||
          (c == ' ')) {
        chars += c;
        if (chars.at(0) == ' ') {
          chars = "";
        }
      } else {
        if (chars != "") {
          m_tree_lines.push_back(sfml_line(parentheses * 40, y, (parentheses + 1) * 40, y));
          sf::Text txt;
          txt.setFillColor(sf::Color(25, 25, 25));
          txt.setFont(sfml_resources::get().get_default_font());
          txt.setString(chars);
          sf::FloatRect bounds = txt.getLocalBounds();
          txt.setOrigin(0, bounds.top  + bounds.height/2.0f);
          txt.setPosition((parentheses * 40) + 50, y);
          m_tree_text.push_back(txt);
          y += 40;
          chars = "";
        }
        if (c == '(') {
          parentheses++;
          intercoords.push_back(y);
          if (prevchar == '(') {
            m_tree_lines.back().set_from((parentheses - 1) * 40, y + 20);
          }
        }
        if (c == ')') {
          int y_mid = (intercoords.back() + y - 40) / 2;
          int tmp_y = 40;
          if (prevchar == ')') {
            m_tree_lines.back().set_to((parentheses + 1) * 40, y - 60);
          }
          m_tree_lines.push_back(sfml_line((parentheses -1) * 40, y_mid,
                                            parentheses * 40, y_mid));
          m_tree_lines.push_back(sfml_line(parentheses * 40, intercoords.back(),
                                           parentheses * 40, y - 40));
          intercoords.pop_back();
          parentheses--;
        }
      }
      prevchar = c;
    }
    if (chars != "") {
      m_tree_lines.push_back(sfml_line(parentheses * 40, y, (parentheses + 1) * 40, y));
      sf::Text txt;
      txt.setFillColor(sf::Color(25, 25, 25));
      txt.setFont(sfml_resources::get().get_default_font());
      txt.setString(chars);
      sf::FloatRect bounds = txt.getLocalBounds();
      txt.setOrigin(0, bounds.top  + bounds.height/2.0f);
      txt.setPosition((parentheses * 40) + 50, y);
      m_tree_text.push_back(txt);
      y += 40;
      chars = "";
    }
  }

  std::clog << in << std::endl;
}
