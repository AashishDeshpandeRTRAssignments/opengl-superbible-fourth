#include "global.h"

//GLOBALS
GLboolean g_vsync = false;
GLboolean g_run   = true;

//WIN VARS (Start square)
GLuint win_w = WIDTH;
GLuint win_h = HEIGHT;

//LIMITS
GLfloat limit   = 100.0f;
GLfloat rot_inc = (GL_PI / 36.0f);
GLfloat max_ang = GL_PI;
GLfloat ang_inc = (GL_PI / 20.0f);

//SETTINGS
GLfloat   x_rot = 0.0f;
GLfloat   y_rot = 0.0f;

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
	GLfloat   x;
	GLfloat   y;
	GLfloat   z;
	GLfloat   theta;

	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotatef(rad_to_deg(x_rot), 1.0f, 0.0f, 0.0f);
	glRotatef(rad_to_deg(y_rot), 0.0f, 1.0f, 0.0f);
	draw_axes();
	z = 0.0f;
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
		for (theta = 0.0f; theta <= max_ang; theta += ang_inc) {
			//Top half
			x = limit / 2.0f * cos(theta);
			y = limit / 2.0f * sin(theta);
			glVertex3f(x, y, z);
			x = limit / 2.0f * cos(theta + GL_PI);
			y = limit / 2.0f * sin(theta + GL_PI);
			glVertex3f(x, y, z);
		}
	glEnd();
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
	glShadeModel(GL_FLAT);
	glFrontFace(GL_CW);
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
