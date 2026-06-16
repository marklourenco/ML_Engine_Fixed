#include "GameState.h"
#include "CandPApp.h"

using namespace ML_Engine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Hello CandP";

	CandPApp myApp;
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);

	return 0;
}