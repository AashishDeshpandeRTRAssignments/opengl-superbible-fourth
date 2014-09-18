#include "global.h"

//GLOBALS
GLboolean g_vsync = false;
GLboolean g_run   = true;

//WIN VARS (Start square)
static GLuint win_w = WIDTH;
static GLuint win_h = HEIGHT;

//LIMITS
static const GLfloat limit      = 100.0f;
static const GLfloat rot_inc    = GL_PI / 36.0f;

//SETTINGS
static GLfloat x_rot = 0.0f;
static GLfloat y_rot = 0.0f;

//Bitmap of camp fire
static const GLubyte fire[128] = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xc0,
	0x00, 0x00, 0x01, 0xf0,
	0x00, 0x00, 0x07, 0xf0,
	0x0f, 0x00, 0x1f, 0xe0,
	0x1f, 0x80, 0x1f, 0xc0,
	0x0f, 0xc0, 0x3f, 0x80,
	0x07, 0xe0, 0x7e, 0x00,
	0x03, 0xf0, 0xff, 0x80,
	0x03, 0xf5, 0xff, 0xe0,
	0x07, 0xfd, 0xff, 0xf8,
	0x1f, 0xfc, 0xff, 0xe8,
	0xff, 0xe3, 0xbf, 0x70,
	0xde, 0x80, 0xb7, 0x00,
	0x71, 0x10, 0x4a, 0x80,
	0x03, 0x10, 0x4e, 0x40,
	0x02, 0x88, 0x8c, 0x20,
	0x05, 0x05, 0x04, 0x40,
	0x02, 0x82, 0x14, 0x40,
	0x02, 0x40, 0x10, 0x80,
	0x02, 0x64, 0x1a, 0x80,
	0x00, 0x92, 0x29, 0x00,
	0x00, 0xb0, 0x48, 0x00,
	0x00, 0xc8, 0x90, 0x00,
	0x00, 0x85, 0x10, 0x00,
	0x00, 0x03, 0x00, 0x00,
	0x00, 0x00, 0x10, 0x00
};

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
	glEnd();	glEnd();

}

//Draw our objects
void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotatef(rad_to_deg(x_rot), 1.0f, 0.0f, 0.0f);
	glRotatef(rad_to_deg(y_rot), 0.0f, 1.0f, 0.0f);
	draw_axes();
	glColor3f(1.0f, 0.0f, 0.0f);
	//Octagon
	glBegin(GL_POLYGON);
		glVertex2f(-20.0f, 50.0f);
		glVertex2f(20.0f, 50.0f);
		glVertex2f(50.0f, 20.0f);
		glVertex2f(50.0f, -20.0f);
		glVertex2f(20.0f, -50.0f);
		glVertex2f(-20.0f, -50.0f);
		glVertex2f(-50.0f, -20.0f);
		glVertex2f(-50.0f, 20.0f);
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
	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple(fire);
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
