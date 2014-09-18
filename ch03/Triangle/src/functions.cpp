#include "global.h"

//GLOBALS
GLboolean g_vsync = false;
GLboolean g_run   = true;

//WIN VARS (Start square)
GLuint win_w = WIDTH;
GLuint win_h = HEIGHT;

//LIMITS
GLfloat limit   = 100.0f;
GLfloat rot_inc = GL_PI / 36.0f;
GLfloat max_ang = 2.0f * GL_PI;
GLfloat ang_inc = GL_PI / 8.0f;

//SETTINGS
GLboolean cull  = false;
GLboolean depth = false;
GLboolean wire  = false;

void draw_axes()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(-limit, 0.0f, 0.0f);
		glVertex3f(limit, 0.0f, 0.0f);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f, -limit, 0.0f);
		glVertex3f(0.0f, limit, 0.0f);
	glEnd();
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, -limit);
		glVertex3f(0.0f, 0.0f, limit);
	glEnd();
}

void render_scene()
{
	GLfloat   x;
	GLfloat   y;
	GLfloat   z;
	GLfloat   theta;
	GLboolean red = false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_axes();
	if (cull)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	if (depth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	if (wire)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);
	z = 0.0f;
	//Draw cone
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0f, 0.0f, 0.75f * limit);
		for (theta = 0.0f; theta <= max_ang; theta += ang_inc) {
			if (red)
				glColor3f(1.0f, 0.0f, 0.0f);
			else
				glColor3f(0.0f, 1.0f, 0.0f);
			x = limit / 2.0f * cos(theta);
			y = limit / 2.0f * sin(theta);
			glVertex3f(x, y, z);
			red = !red;
		}
	glEnd();
	//Draw base
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0f, 0.0f, 0.0f);
		for (theta = 0.0f; theta <= max_ang; theta += ang_inc) {
			if (red)
				glColor3f(1.0f, 0.0f, 0.0f);
			else
				glColor3f(0.0f, 1.0f, 0.0f);
			x = limit / 2.0f * cos(theta);
			y = limit / 2.0f * sin(theta);
			glVertex3f(x, y, z);
			red = !red;
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
	glShadeModel(GL_FLAT);
	win_resized(win_w, win_h);
}

void process_key(sf::Event::KeyEvent e)
{
	switch (e.code) {
	case sf::Keyboard::Right:
		glRotatef(-rot_inc * 180.0f / GL_PI, 0.0f, 1.0f, 0.0f);
		break;
	case sf::Keyboard::Left:
		glRotatef(rot_inc * 180.0f / GL_PI, 0.0f, 1.0f, 0.0f);
		break;
	case sf::Keyboard::Up:
		glRotatef(rot_inc * 180.0f / GL_PI, 1.0f, 0.0f, 0.0f);
		break;
	case sf::Keyboard::Down:
		glRotatef(-rot_inc * 180.0f / GL_PI, 1.0f, 0.0f, 0.0f);
		break;
	case sf::Keyboard::Escape:
		sf::Event e;

		e.type = sf::Event::Closed;
		handle_event(e);
		break;
	case sf::Keyboard::Space:
		glLoadIdentity();
		cull  = false;
		depth = false;
		wire  = false;
		break;
	case sf::Keyboard::C:
		cull = !cull;
		break;
	case sf::Keyboard::D:
		depth = !depth;
		break;
	case sf::Keyboard::W:
		wire = !wire;
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
