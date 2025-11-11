#include "Precompiled.h"
#include "ML_Engine.h"

ML_Engine::App& ML_Engine::MainApp()
{
	static App sApp;
	return sApp;
}