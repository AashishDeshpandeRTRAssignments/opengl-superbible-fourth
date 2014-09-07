#include "config.h"
#include "global.h"

int main(int argc, const char * const argv[])
{
	sf::Event           e;
	sf::Window          win;
	sf::VideoMode       vm;
	std::stringstream   win_title;
	sf::ContextSettings cs;

	win_title<<APP_TITLE<<" "<<APP_VERSION;
	vm.width        = WIDTH;
	vm.height       = HEIGHT;
	vm.bitsPerPixel = 32;
	cs.depthBits    = 24;
	cs.stencilBits  = 8;
	win.create(vm, win_title.str(), sf::Style::Default, cs);
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
