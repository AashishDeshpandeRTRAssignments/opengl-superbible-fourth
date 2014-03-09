#include "config.h"
#include "global.h"

int main(int argc, const char * const argv[])
{
	sf::Event         e;
	sf::Window        win;
	sf::VideoMode     vm;
	std::string       win_title;
	std::stringstream version;

	version<<"v"<<Triangle_VERSION_MAJOR<<"."<<Triangle_VERSION_MINOR;
	win_title       = "Triangle " + version.str();
	vm.width        = WIDTH;
	vm.height       = HEIGHT;
	vm.bitsPerPixel = 32;
	win.create(vm, win_title);
	win.setFramerateLimit(60);
	win.setVerticalSyncEnabled(g_vsync);
	win.setActive();
	setup_render_state();
	while(g_run) {
		while (win.pollEvent(e))
			handle_event(e);
		render_scene();
		win.display();
	}
	win.close();

	return 0;
}
