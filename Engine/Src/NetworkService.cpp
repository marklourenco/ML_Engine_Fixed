#include "Precompiled.h"
#include "NetworkService.h"

using namespace ML_Engine;
using namespace ML_Engine::Network;

void NetworkService::Update(float deltaTime)
{
	NetworkManager::Get()->Update(deltaTime);
}
void NetworkService::DebugUI()
{
	NetworkManager::Get()->DebugUI();
}