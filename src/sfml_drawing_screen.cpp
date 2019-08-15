#include "sfml_drawing_screen.h"
#include "sfml_window_manager.h"
#include "sfml_resources.h"
#include "sfml_line.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <functional>
#include <algorithm>
#include <vector>
#include <chrono>

/* TODO
- m_short_buttons and m_long_buttons for junctions
- Giving branches a different LETTER (names will mess up the edit buttons' functionality rn)
*/

sfml_drawing_screen::sfml_drawing_screen(std::string newick)
    : m_window{ sfml_window_manager::get().get_window() },
      m_input(20, 20, 50, 50), m_tree_lines{}, m_tree_text{}, m_end_y{0},
      m_add_image{ sfml_resources::get().get_add_image() },
      m_remove_image{ sfml_resources::get().get_remove_image() },
      m_split_image{ sfml_resources::get().get_split_image() }
{
  m_tool_bar.setFillColor(sf::Color(100, 100, 100));
  m_drawing_area.setFillColor(sf::Color(220, 220, 220));
  m_confirm.get_shape().setFillColor(sf::Color(45, 235, 205));

  m_save_sprite.setTexture(sfml_resources::get().get_draw_image());
  
  m_drawing_view = sf::View(sf::Vector2f(m_window.getSize().x / 2,
                                         (m_window.getSize().y / 2) + 50),
                            sf::Vector2f(m_window.getSize().x,
                                         m_window.getSize().y - 100));

  m_move_left = false;
  m_move_right = false;
  m_move_up = false;
  m_move_down = false;
  
  m_clicked = false;
  
  m_input.set_string(newick, m_window);
  update_tree(m_input.get_string());
  
  sf::Sprite spr3(m_add_image);
  spr3.setScale(2, 2);
  spr3.setPosition(0, m_end_y);
  m_end_buttons.push_back(spr3);
  
}

void sfml_drawing_screen::exec() { //!OCLINT can be complex
  set_sizes();
  while (active(game_state::drawing)) {
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


    m_edit_buttons.clear();
    m_long_buttons.clear();
    m_short_buttons.clear();
    m_end_buttons.clear();
    
    sf::View tmp_view = m_window.getView();
    m_window.setView(m_drawing_view);
    for (auto &v : m_add_nodes) {
      if (hover(v.x, v.y, 7.5)) {
        sf::Sprite spr(m_split_image);
        spr.setPosition(v.x - 7.5, v.y - 7.5);
        m_edit_buttons.push_back(spr);
      }
    }
    for (auto &v : m_long_nodes) {
      if ((hover(v.x.x, v.x.y, 7.5) || hover(v.x.x - 20, v.x.y, 7.5) ||
           hover(v.x.x - 10, v.x.y, 7.5))) {
        sf::Sprite spr1(m_add_image);
        spr1.setPosition(v.x.x - 7.5, v.x.y - 7.5);
        m_long_buttons.push_back(spr1);
        
        sf::Sprite spr2(m_remove_image);
        spr2.setPosition(v.x.x - 27.5, v.x.y - 7.5);
        m_short_buttons.push_back(spr2);
      }
    }
    sf::Sprite spr3(m_add_image);
    spr3.setScale(2, 2);
    spr3.setPosition(0, m_end_y);
    m_end_buttons.push_back(spr3);
    m_window.setView(tmp_view);
    
    {
      std::string str = m_input.get_string();
      if (str.size() < 3) {
        str = "";
        m_edit_buttons.clear();
        m_long_buttons.clear();
        m_short_buttons.clear();
      }
      m_input.set_string(str, m_window);
      update_tree(m_input.get_string());
    }
    
    set_positions();
    draw_objects();
#ifdef CI
    close();
#endif
  }
}

void sfml_drawing_screen::process_event(sf::Event event) { //!OCLINT can be complex
  int u = 0;
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
        case sf::Keyboard::A:
          m_input.left();
          if (!m_input.is_selected()) m_move_left = true;
          break;

        case sf::Keyboard::Right:
        case sf::Keyboard::D:
          m_input.right();
          if (!m_input.is_selected()) m_move_right = true;
          break;

        case sf::Keyboard::Up:
        case sf::Keyboard::W:
          if (!m_input.is_selected()) m_move_up = true;
          break;

        case sf::Keyboard::Down:
        case sf::Keyboard::S:
          if (!m_input.is_selected()) m_move_down = true;
          break;

        case sf::Keyboard::Return:
          if (m_input.is_selected()) {
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
          case sf::Keyboard::A:
            m_move_left = false;
            break;

          case sf::Keyboard::Right:
          case sf::Keyboard::D:
            m_move_right = false;
            break;

          case sf::Keyboard::Up:
          case sf::Keyboard::W:
            m_move_up = false;
            break;

          case sf::Keyboard::Down:
          case sf::Keyboard::S:
            m_move_down = false;
            break;

          default:
            break;
        }
      break;

    case sf::Event::MouseButtonPressed:
#ifndef NDEBUG
      m_input.select(m_window);
#endif
      if (m_confirm.is_clicked(event, m_window)) {
        update_tree(m_input.get_string());
        std::ofstream ofs;
        std::string path;
        if (m_path.back() == '\\') {
          path = m_path + "saves\\";
        } else if (m_path.back() == '/') {
          path = m_path + "saves/";
        } else {
#ifdef WIN32
          path = "saves\\";
#else
          path = "saves/";
#endif
        }
        path += "geneak-";
        path += get_time();
        path += ".gnk";
        std::clog << path << std::endl;
        ofs.open(path, std::fstream::out);
        if (ofs.is_open()) {
          ofs << m_input.get_string() << std::endl;
          ofs.close();
        } else {
          std::clog << "Couldn't create savefile!" << std::endl;
        }
      }
      for (auto &button : m_edit_buttons) {
        sf::Vector2f pos(button.getPosition() + sf::Vector2f(7.5, 7.5));
        if (hover(pos.x, pos.y, 7.5) && !m_clicked) {
          m_clicked = true;
          std::string str = m_input.get_string();
          str.insert(get_string_pos(button), ", X");
          m_input.set_string(str, m_window);
          update_tree(m_input.get_string());
        }
      }
      for (auto &button : m_long_buttons) {
        sf::Vector2f pos(button.getPosition() + sf::Vector2f(7.5, 7.5));
        if (hover(pos.x, pos.y, 7.5) && !m_clicked) {
          m_clicked = true;
          std::string str = m_input.get_string();
          sf::Vector2i p = get_par_pos(button);
          str.insert(p.x, "(");
          str.insert(p.y, ")");
          m_input.set_string(str, m_window);
          update_tree(m_input.get_string());
        }
      }
      for (auto &button : m_short_buttons) {
        sf::Vector2f pos(button.getPosition() + sf::Vector2f(7.5, 7.5));
        if (hover(pos.x, pos.y, 7.5) && !m_clicked) {
          m_clicked = true;
          std::string str = m_input.get_string();
          sf::Vector2i p = get_par_pos(m_long_buttons.at(u));
          if (str.at(p.x - 1) == '(' && str.at(p.x + 1) == ')') {
            str.erase(p.x - 1, 1);
            str.erase(p.x, 1);
            std::string newstr = "";
            int pa = 0;
            for (auto c : str) {
              if (c == '(') pa++;
              if (pa > 0) newstr += c;
              if (c == ')') pa--;
            }
            m_input.set_string(newstr, m_window);
            update_tree(m_input.get_string());
          } else if (str.at(p.x + 1) == ')') {
            str.erase(p.x - 2, 3);
            m_input.set_string(str, m_window);
            update_tree(m_input.get_string());
          } else if (str.at(p.x - 1) == ')' || str.at(p.x + 1) == '(') {
            str.erase(p.x, 1);
            m_input.set_string(str, m_window);
            update_tree(m_input.get_string());
          } else {
            str.erase(p.x, 3);
            m_input.set_string(str, m_window);
            update_tree(m_input.get_string());
          }
        }
        u++;
      }
      {
        sf::Sprite& button = m_end_buttons.back();
        sf::Vector2f pos(button.getPosition() + sf::Vector2f(15, 15));
        if (hover(pos.x, pos.y, 15) && !m_clicked) {
          m_clicked = true;
          std::string str = m_input.get_string();
          str += "(X)";
          m_input.set_string(str, m_window);
          update_tree(m_input.get_string());
        }
      }
      break;
      
    case sf::Event::MouseButtonReleased:
      m_clicked = false;
      break;

    case sf::Event::TextEntered:
#ifndef NDEBUG
      m_input.input(event, m_window);
#endif
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
  m_confirm.set_pos(m_window.getSize().x - 70, 20, m_window);
  m_save_sprite.setPosition(m_window.mapPixelToCoords(sf::Vector2i(m_window.getSize().x - 70, 20)));
}

void sfml_drawing_screen::set_sizes() {
  m_tool_bar.setSize(sf::Vector2f(m_window.getSize().x, 100));
  m_drawing_area.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y - 100));
  m_input.set_size(((m_window.getSize().x - 40) / 10) * 8, 50, m_window);
  m_confirm.set_size(50, 50, m_window);

  float tb_per = 100.0/m_window.getSize().y;
  m_drawing_view.setViewport(sf::FloatRect(0, tb_per, 1, 1 - tb_per));
  m_drawing_view.setCenter(0, 0);
}

void sfml_drawing_screen::draw_objects() {
  // Clear window
  m_window.clear();


  // Draw tool bar
  m_window.draw(m_tool_bar);

#ifndef NDEBUG
  m_window.draw(m_input.get_shape());
  m_window.draw(m_input.get_text());
#endif

  m_window.draw(m_confirm.get_shape());
  //m_window.draw(m_confirm.get_text());
  m_window.draw(m_save_sprite);


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
  
  for (auto &button : m_edit_buttons) {
    m_window.draw(button);
  }
  for (auto &button : m_long_buttons) {
    m_window.draw(button);
  }
  for (auto &button : m_short_buttons) {
    m_window.draw(button);
  }
  
  m_window.draw(m_end_buttons.back());

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

int sfml_drawing_screen::update_tree(std::string in) { //!OCLINT ofc way too complicated
  m_tree_lines.clear();
  m_tree_text.clear();
  
  bool error = false;
  
  if (in.size() < 3 && in != "") {
    error = true;
  }
  
  if (!error) {
    if (in.front() != '(') error = true;
    if (in.back() != ')') error = true;
    
    int par = 0;
    for (char& c : in) {
      if (c == '(') {
        par++;
      }
      if (c == ')') {
        par--;
      }
    }
    if (par != 0) error = true;
  }
  
  if (error) {
#ifndef CI
    m_input.get_text().setFillColor(sf::Color(255, 0, 0));
#endif
    return 2;
  }
  
  if (in == "") {
    return 1;
  }

  try {
    int parentheses = 0;
    int y = 0;
    std::string chars = "";
    std::vector<std::vector<int>> coords;
    int i = 0;
    int o = 0;
    m_add_nodes.clear();
    m_long_nodes.clear();
    for (char& c : in) {
      if (((c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c == ' ')) && parentheses != 0) {
        chars += c;
        o = i;
        if (chars.at(0) == ' ') {
          chars = "";
        }
      } else {
        if (chars != "") {
          m_tree_lines.push_back(sfml_line(parentheses * 40, y, (parentheses + 1) * 40, y));
          sf::Text txt;
#ifndef CI
          txt.setFillColor(sf::Color(25, 25, 25));
#endif
          txt.setFont(sfml_resources::get().get_default_font());
          txt.setString(chars);
          sf::FloatRect bounds = txt.getLocalBounds();
          txt.setOrigin(0, bounds.top  + bounds.height/2.0f);
          txt.setPosition((parentheses * 40) + 50, y);
          m_tree_text.push_back(txt);
          
          m_long_nodes.push_back(sf::Vector2<sf::Vector2f>(sf::Vector2f((parentheses + 1) * 40, y),
                                                           sf::Vector2f(o, i + 1)));
          
          coords.at(parentheses).push_back(y);
          y += 40;
          chars = "";
        }
        if (c == '(') {
          parentheses++;
          while (static_cast<unsigned>(parentheses + 1) > coords.size()) {
            coords.push_back({});
          }
        }
        if (c == ')') {
          std::vector<int>& x = coords.at(parentheses);
          std::sort(x.begin(), x.end());
          m_tree_lines.push_back(sfml_line(parentheses * 40, x.front(),
                                           parentheses * 40, x.back()));
          int mid = (x.front() + x.back()) / 2;
          m_tree_lines.push_back(sfml_line((parentheses - 1) * 40, mid,
                                           parentheses * 40, mid));
          m_add_nodes.push_back(sf::Vector3f(parentheses * 40, x.back(), i));
          x.clear();
          parentheses--;
          coords.at(parentheses).push_back(mid);
        }
      }
      i++;
    }
    if (chars != "") {
      m_tree_lines.push_back(sfml_line(parentheses * 40, y, (parentheses + 1) * 40, y));
      sf::Text txt;
#ifndef CI
      txt.setFillColor(sf::Color(25, 25, 25));
#endif
      txt.setFont(sfml_resources::get().get_default_font());
      txt.setString(chars);
      sf::FloatRect bounds = txt.getLocalBounds();
      txt.setOrigin(0, bounds.top  + bounds.height/2.0f);
      txt.setPosition((parentheses * 40) + 50, y);
      m_tree_text.push_back(txt);
      
      coords.at(parentheses).push_back(y);
      y += 40;
      chars = "";
    }
    m_end_y = y;
  } catch (int e) {
    std::clog << "Exception Nr. " << e << std::endl;
#ifndef CI
    m_input.get_text().setFillColor(sf::Color(255, 0, 0));
#endif
    return 3;
  }
  return 0;
}

int sfml_drawing_screen::get_string_pos(sf::Sprite &button) {
  for (auto &v : m_add_nodes) {
    if (sf::Vector2f(v.x - 7.5, v.y - 7.5) == button.getPosition()) {
      return v.z;
    }
  }
  return -1;
}

sf::Vector2i sfml_drawing_screen::get_par_pos(sf::Sprite &button) {
  for (auto &v : m_long_nodes) {
    if (sf::Vector2f(v.x.x - 7.5, v.x.y - 7.5) == button.getPosition()) {
      return sf::Vector2i(v.y);
    }
  }
  return sf::Vector2i(-1, -1);
}

bool sfml_drawing_screen::hover(float x, float y, float range) {
  sf::Vector2f mp = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window), m_drawing_view);
  return mp.x > x - range && mp.x < x + range &&
         mp.y > y - range && mp.y < y + range;
}

std::string get_time() {
  std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                            std::chrono::system_clock::now().time_since_epoch());
  std::string ts = std::to_string(ms.count());
  return ts;
}
