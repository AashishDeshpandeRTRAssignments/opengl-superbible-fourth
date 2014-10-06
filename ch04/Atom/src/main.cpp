#include "config.h"
#include "global.h"

int main(int argc, const char * const argv[])
{
	sf::Event           e;
	sf::Window          win;
	sf::VideoMode       vm;
	sf::ContextSettings cs;

	vm.width        = WIDTH;
	vm.height       = HEIGHT;
	vm.bitsPerPixel = 32;
	cs.depthBits    = 24;
	cs.stencilBits  = 8;
	win.create(vm, APP_TITLE" "APP_VERSION, sf::Style::Default, cs);
	if (!g_vsync)
		win.setFramerateLimit(60);
	win.setVerticalSyncEnabled(g_vsync);
	win.setActive();
	setup_render_state();
	while (g_run) {
		while (win.pollEvent(e))
			handle_event(e);
		render_scene();
		win.display();
	}
	win.close();

	return 0;
}
