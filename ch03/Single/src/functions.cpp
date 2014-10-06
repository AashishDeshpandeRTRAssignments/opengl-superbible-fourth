#include "global.h"

//GLOBALS
GLboolean g_vsync = GL_FALSE;
GLboolean g_run   = GL_TRUE;

//WIN VARS (Start square)
static GLuint win_w = WIDTH;
static GLuint win_h = HEIGHT;

//LIMITS
static const GLfloat limit   = 100.0f;
static const GLfloat rot_inc = GL_PI / 36.0f;

//SETTINGS
static GLfloat x_rot = 0.0f;
static GLfloat y_rot = 0.0f;

//Converts degrees to radians
GLfloat deg_to_rad(GLfloat degrees)
{
	return degrees * GL_PI / 180.0f;
}

//Converts radians to degrees
GLfloat rad_to_deg(GLfloat radians)
{
	return radians / GL_PI * 180.0f;
}

//Draw the 3 axes
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

//Draw our objects
void render_scene()
{
	//radius starts at 5 since the 0.1 the book gives is very small
	static GLdouble radius = 5.0;
	static GLdouble angle  = 0.0;
	//Incrementers to simulate single buffering
	GLdouble i = 0.1;
	GLdouble j = 0.0;

	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotatef(rad_to_deg(x_rot), 1.0f, 0.0f, 0.0f);
	glRotatef(rad_to_deg(y_rot), 0.0f, 1.0f, 0.0f);
	draw_axes();
	glColor3f(0.0f, 1.0f, 0.0f);
	//Spiral
	glBegin(GL_POINTS);
		for (i, j; i <= radius || j <= angle; i *= 1.01, j += 0.1)
			glVertex2d(i * cos(j), i * sin(j));
	glEnd();
	radius *= 1.01;
	angle += 0.1;
	if (angle > 30.0) {
		radius = 5.0;
		angle = 0.0;
	}
	glPopMatrix();
}

//Fix the viewport and projection matrix when the screen resizes
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

//Set any necessary GL settings
void setup_render_state()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	win_resized(win_w, win_h);
}

//Handle key presses
void process_key(sf::Event::KeyEvent e)
{
	switch (e.code) {
	case sf::Keyboard::Right:
		y_rot -= rot_inc;
		if (y_rot <= 0.0f)
			y_rot += 2.0f * GL_PI;
		break;
	case sf::Keyboard::Left:
		y_rot += rot_inc;
		if (y_rot >= 2.0f * GL_PI)
			y_rot -= 2.0f * GL_PI;
		break;
	case sf::Keyboard::Up:
		x_rot += rot_inc;
		if (x_rot >= 2.0f * GL_PI)
			x_rot -= 2.0f * GL_PI;
		break;
	case sf::Keyboard::Down:
		x_rot -= rot_inc;
		if (x_rot <= 0.0f)
			x_rot += 2.0f * GL_PI;
		break;
	case sf::Keyboard::Escape:
		sf::Event e;

		e.type = sf::Event::Closed;
		handle_event(e);
		break;
	case sf::Keyboard::Space:
		x_rot = 0.0f;
		y_rot = 0.0f;
		break;
	default:
		break;
	}
}

//Handle SFML events
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
