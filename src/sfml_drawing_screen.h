#ifndef SFML_DRAWING_SCREEN_H
#define SFML_DRAWING_SCREEN_H

#include "game_state.h"
#include "sfml_resources.h"
#include "sfml_text_input.h"
#include "sfml_button.h"
#include <SFML/Graphics.hpp>

class sfml_drawing_screen
{
public:

    sfml_drawing_screen(std::string newick = "");

    void exec();

    void process_event(sf::Event event);

    ///@param game state to change to
    void close(game_state s);

    ///Close the game
    void close();

    void set_positions();
    void set_sizes();
    
    void draw_objects();
    
    void set_path(std::string p) { m_path = p; }

private:
    
    sf::RenderWindow& m_window;

    sf::RectangleShape m_tool_bar;
    sf::RectangleShape m_drawing_area;
    
    sfml_text_input m_input;
    
	  sfml_button m_confirm;
    sf::Sprite m_save_sprite;
	
    sf::View m_drawing_view;
    
    bool m_move_left;
    bool m_move_right;
    bool m_move_up;
    bool m_move_down;
    
    std::vector<sfml_line> m_tree_lines;
    std::vector<sf::Text> m_tree_text;
    std::vector<sf::Vector3f> m_add_nodes;
    std::vector<sf::Sprite> m_edit_buttons;
    std::vector<sf::Vector2<sf::Vector2f>> m_long_nodes;
    std::vector<sf::Sprite> m_long_buttons;
    std::vector<sf::Sprite> m_short_buttons;
    std::vector<sf::Sprite> m_end_buttons;
    int m_end_y;
    
    bool m_clicked;
    
    int update_tree(std::string in);
    
    int get_string_pos(sf::Sprite &button);
    sf::Vector2i get_par_pos(sf::Sprite &button);
    
    bool hover(float x, float y, float range);

    std::string m_path;
    
    sf::Texture& m_add_image;
  
    sf::Texture& m_remove_image;
  
    sf::Texture& m_split_image;
    
};

std::string get_time();

#endif // SFML_DRAWING_SCREEN_H
