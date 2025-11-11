#include <Inc/ML_Engine.h>
#include "ShapeState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	ML_Engine::AppConfig config;
	config.appName = L"HelloShapes";

	ML_Engine::App& myApp = ML_Engine::MainApp();
    myApp.AddState<ShapeState>("ShapeState");
    myApp.AddState<TriangleShapeState>("TriangleShapeState");
	myApp.AddState<TriforceShapeState>("TriforceShapeState");
	myApp.AddState<DiamondShapeState>("DiamondShapeState");
	myApp.AddState<HeartShapeState>("HeartShapeState");
	myApp.Run(config);

	return 0;
}