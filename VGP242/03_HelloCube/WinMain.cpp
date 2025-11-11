#include <Inc/ML_Engine.h>
#include "ShapeState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	ML_Engine::AppConfig config;
	config.appName = L"Hello Cube";

	ML_Engine::App& myApp = ML_Engine::MainApp();
    myApp.AddState<ShapeState>("ShapeState");
	myApp.Run(config);

	return 0;
}