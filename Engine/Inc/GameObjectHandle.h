#pragma once

namespace ML_Engine
{
	class GameObjectHandle
	{
	public:
		GameObjectHandle() = default;
	private:
		friend class GameWorld;
		int mIndex = -1; // index of the slot in the game world
		int mGeneration = -1; // to verify if the item is already tagged for deletion/removed
	};
}