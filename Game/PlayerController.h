#pragma once
namespace Engine {
	class TextureManager;
	class EntityManager;
}

namespace Nitro
{
	struct CarPhysicsComponent;
	class PlayerController
	{
	public:
		bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt_, Engine::EntityManager* entityManager_, Engine::AudioManager* audioManager_, bool* chaseWon, bool* runnerWon, Engine::TextureManager* textureManager_);
		static std::unique_ptr<PlayerController> Create()
		{
			return std::make_unique<PlayerController>();
		}
	private:
		void MoveWheel(float dt_, bool moveLeft, bool moveRight, CarPhysicsComponent* physics);
		void HandleGasAndBreaking(float dt_, bool moveUp, bool moveDown, CarPhysicsComponent* physics);
		void SteerTheCar(float dt_, Engine::Entity* player);
		void CollideWithOtherEntities(float dt_, Engine::Entity* player,bool* chaseWon);
		void HandleJump(float dt_, bool jump, Engine::Entity* player, Engine::AudioManager* audioManager_);
		void RunnerThrows(float dt_, bool thows, Engine::Entity* player, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
	};
}
