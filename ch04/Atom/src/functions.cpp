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
	static GLfloat fElect1 = 0.0f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(rad_to_deg(x_rot), 1.0f, 0.0f, 0.0f);
	glRotatef(rad_to_deg(y_rot), 0.0f, 1.0f, 0.0f);
	draw_axes();

	//Red Nucleus
	glColor3ub(255, 0, 0);
	glutSolidSphere(10.0, 15, 15);

	//Yellow Electrons
	glColor3ub(255,255,0);

	//First Electron Orbit
	//Save viewing transformation
	glPushMatrix();

	//Rotate by angle of revolution
	glRotatef(fElect1, 0.0f, 1.0f, 0.0f);

	//Translate out from origin to orbit distance
	glTranslatef(90.0f, 0.0f, 0.0f);

	//Draw the electron
	glutSolidSphere(6.0f, 15, 15);

	//Restore the viewing transformation
	glPopMatrix();

	//Second Electron Orbit
	glPushMatrix();
	glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
	glTranslatef(-70.0f, 0.0f, 0.0f);
	glutSolidSphere(6.0f, 15, 15);
	glPopMatrix();

	//Third Electron Orbit
	glPushMatrix();
	glRotatef(360.0f-45.0f,0.0f, 0.0f, 1.0f);
	glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 60.0f);
	glutSolidSphere(6.0f, 15, 15);
	glPopMatrix();

	//Increment the angle of revolution, lower because I render at 60 fps
	fElect1 += 1.6667f;
	if(fElect1 > 360.0f)
		fElect1 = 0.0f;

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
	//Using glut because I didn't feel like writing a sphere drawing func
	int *argc;
	char **argv;
	glutInit(argc, argv);
	glEnable(GL_DEPTH_TEST);
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
