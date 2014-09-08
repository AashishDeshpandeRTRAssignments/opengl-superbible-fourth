#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <stdio.h>
#include <math.h>

//CONSTANTS
#define GL_PI (3.141592653f)

//GLOBALS
GLboolean g_vsync = false;
GLboolean g_run   = true;

//WIN VARS (Start square)
GLuint  win_w = 600;
GLuint  win_h = 600;

//LIMITS
GLfloat limit   = 100.0f;
GLfloat rot_inc = GL_PI / 36.0f;
GLfloat max_ang = 6.0f * GL_PI;
GLfloat ang_inc = 0.1f;


void draw_axes()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
	glEnd();
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
}

void render_scene()
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat theta;

	glClear(GL_COLOR_BUFFER_BIT);
	draw_axes();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
		z = -limit / 2.0f;
		for (theta = 0.0f; theta <= max_ang; theta += ang_inc) {
			x = limit / 2.0f * cos(theta);
			y = limit / 2.0f * sin(theta);
			glVertex3f(x, y, z);
			z += 5.0f * ang_inc;
		}
	glEnd();
}

void win_resized(GLsizei w, GLsizei h)
{
	GLfloat ar;
	GLfloat max_x;
	GLfloat max_y;
	GLfloat max_z;

	win_w = w;
	win_h = h;
	if (win_h == 0)
		win_h = 1;
	glViewport(0, 0, win_w, win_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ar = (GLfloat)w / (GLfloat)h;
	if (w <= h) {
		max_x = limit;
		max_y = limit / ar;
	} else {
		max_x = limit * ar;
		max_y = limit;
	}
	max_z = limit;
	glOrtho(-max_x, max_x, -max_y, max_y, -max_z, max_z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void setup_render_state()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	win_resized(win_w, win_h);
}

void process_key(sf::Event::KeyEvent e)
{
	switch (e.code) {
	case (sf::Keyboard::Right):
		glRotatef(-rot_inc * 180.0f / GL_PI, 0.0f, 1.0f, 0.0f);
		break;
	case (sf::Keyboard::Left):
		glRotatef(rot_inc * 180.0f / GL_PI, 0.0f, 1.0f, 0.0f);
		break;
	case (sf::Keyboard::Up):
		glRotatef(rot_inc * 180.0f / GL_PI, 1.0f, 0.0f, 0.0f);
		break;
	case (sf::Keyboard::Down):
		glRotatef(-rot_inc * 180.0f / GL_PI, 1.0f, 0.0f, 0.0f);
		break;
	case (sf::Keyboard::Escape):
		g_run = false;
		break;
	case (sf::Keyboard::Space):
		glLoadIdentity();
		break;
	default:
		break;
	}
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
	case sf::Event::KeyPressed:
		process_key(e.key);
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

	win_title       = "Points";
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
		render_scene();
		win.display();
	}
	win.close();

	return 0;
}
