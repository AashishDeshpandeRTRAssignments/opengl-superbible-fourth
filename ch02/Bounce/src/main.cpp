#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <stdio.h>
#include <float.h>

//GLOBALS
GLboolean g_vsync = false;
GLboolean g_run   = true;

//RECTANGLE VARS
GLfloat x1     = 0.0f;
GLfloat y1     = 0.0f;
GLfloat r_size = 25.0f;
GLfloat x_step = 1.0f;
GLfloat y_step = 1.0f;
GLfloat timer  = 0.033f;

//WIN VARS (Start square)
GLuint  win_w = 600;
GLuint  win_h = 600;

//LIMITS
GLfloat limit = 100.0f;
GLfloat max_y = limit;
GLfloat min_y = -limit;
GLfloat max_x = limit;
GLfloat min_x = -limit;

void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(x1, y1, x1 + r_size, y1 - r_size);
}

void xform_rect()
{
	//Change direction if leaving the building
	if (x1 < min_x || x1 + r_size > max_x)
		x_step *= -1.0f;
	if (y1 - r_size < min_y || y1 > max_y)
		y_step *= -1.0f;

	x1 += x_step;
	y1 += y_step;

	//Check that the window hasn't resized and lost the square
	if (x1 + r_size > max_x) {
		if (x_step > 0.0f)
			x_step *= -1.0f;
		x1 = max_x - r_size;
	} else if (x1 < min_x) {
		if (x_step < 0.0f)
			x_step *= -1.0f;
		x1 = min_x;
	}

	if (y1 > max_y) {
		if (y_step > 0.0f)
			y_step *= -1.0f;
		y1 = max_y;
	} else if (y1 - r_size < min_y) {
		if (y_step < 0.0f)
			y_step *= -1.0f;
		y1 = min_y + r_size;
	}
}

void win_resized(GLsizei w, GLsizei h)
{
	GLfloat ar;

	win_w = w;
	win_h = h;
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ar = (GLfloat)w / (GLfloat)h;
	if (w <= h) {
		min_x = -limit;
		max_x = limit;
		min_y = -limit / ar;
		max_y = limit / ar;
	} else {
		min_x = -limit * ar;
		max_x = limit * ar;
		min_y = -limit;
		max_y = limit;
	}
	glOrtho(min_x, max_x, min_y, max_y, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void setup_render_state()
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	win_resized(win_w, win_h);
}

void handle_event(sf::Event e)
{
	switch (e.type) {
	case sf::Event::Closed:
		fprintf(stderr,
		        "OpenGL Version String: %s\n",
		        glGetString(GL_VERSION));
		g_run = false;
		break;
	case sf::Event::Resized:
		win_resized(e.size.width, e.size.height);
		break;
	default:
		break;
	}
}

int main(int argc, const char * const argv[])
{
	sf::Event     e;
	sf::Window    win;
	sf::VideoMode vm;
	sf::Clock     c;
	std::string   win_title;

	win_title       = "Bounce";
	vm.width        = win_w;
	vm.height       = win_h;
	vm.bitsPerPixel = 32;
	win.create(vm, win_title);
	win.setFramerateLimit(60);
	win.setVerticalSyncEnabled(g_vsync);
	win.setActive();
	setup_render_state();
	c.restart();
	while(g_run) {
		while (win.pollEvent(e))
			handle_event(e);
		if (c.getElapsedTime().asSeconds() >= timer) {
			xform_rect();
			c.restart();
		}
		render_scene();
		win.display();
	}
	win.close();

	return 0;
}
