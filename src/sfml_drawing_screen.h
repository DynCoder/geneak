#ifndef SFML_DRAWING_SCREEN_H
#define SFML_DRAWING_SCREEN_H

#include "game_state.h"
#include "sfml_resources.h"
#include <SFML/Graphics.hpp>

class sfml_drawing_screen
{
public:

    sfml_drawing_screen();

    void exec();

    void process_event(sf::Event event);

    ///@param game state to change to
    void close(game_state s);

    ///Close the game
    void close();

    void set_positions();
    
    void draw_objects();

private:

    sf::RenderWindow& m_window;
    
    sf::Text m_text;
    
    sf::Font m_default_font;

};

#endif // SFML_DRAWING_SCREEN_H
