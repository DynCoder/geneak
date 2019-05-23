HEADERS += game_state.h \
           sfml_button.h \
           sfml_drawing_screen.h \
           sfml_resources.h \
           sfml_text_input.h \
           sfml_window_manager.h \
           sfml_line.h
SOURCES += game_state.cpp \
           main.cpp \
           sfml_button.cpp \
           sfml_drawing_screen.cpp \
           sfml_resources.cpp \
           sfml_text_input.cpp \
           sfml_window_manager.cpp \
           sfml_line.cpp

# SFML
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
