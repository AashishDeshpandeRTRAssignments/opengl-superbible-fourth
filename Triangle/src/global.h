//INCLUDES
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//CONSTANTS
#define GL_PI  (3.14159f)
#define WIDTH  (600)
#define HEIGHT (600)

//GLOBALS
extern GLboolean g_vsync;
extern GLboolean g_run;

//FUNCTIONS
void setup_render_state();
void draw_axes();
void render_scene();
void win_resized(GLsizei w, GLsizei h);
void process_key(sf::Event::KeyEvent e);
void handle_event(sf::Event e);
