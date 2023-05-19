#include "App.h"

int main()
{
	App* app = new App(1600, 900, true);
	app->Run();

	delete app;
	return 0;
}
