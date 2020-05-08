#pragma once
namespace Engine {
	class TextureManager;
	class EntityManager;
}

namespace Nitro
{
	class TheChaseController
	{
	public:
		bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_,bool chaseWon, bool runnerWon);
		static std::unique_ptr<TheChaseController> Create()
		{
			return std::make_unique<TheChaseController>();
		}

	};
}

