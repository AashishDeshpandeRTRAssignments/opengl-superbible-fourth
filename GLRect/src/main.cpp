#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <stdio.h>

bool g_vsync = false;
bool g_run   = true;

void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(-25.0f, 25.0f, 25.0f, -25.0f);
	glFlush(); //Flush b/c no vsync
}

void win_resized(GLsizei w, GLsizei h)
{
	GLfloat ar;

	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ar = (GLfloat)w / (GLfloat)h;
	if (w <= h)
		glOrtho(-100.0, 100.0, -100.0 / ar, 100.0 / ar, 1.0, -1.0);
	else
		glOrtho(-100.0 * ar, 100.0 * ar, -100.0, 100.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void setup_render_state(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	win_resized(w, h);
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
	std::string   win_title;

	win_title       = "GLRect";
	vm.height       = 600;
	vm.width        = 800;
	vm.bitsPerPixel = 32;
	win.create(vm, win_title);
	win.setVerticalSyncEnabled(g_vsync);
	win.setActive();
	setup_render_state(vm.width, vm.height);
	while(g_run) {
		while (win.pollEvent(e))
			handle_event(e);
		render_scene();
		win.display();
	}
	win.close();

	return 0;
}
