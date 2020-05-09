#include "precomp.h"
#include "GameComponents.h"
#include "TextureController.h"
#include "TheChase.h"
#include "PlayerController.h"

bool Nitro::TheChaseController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_) {

	return true;
}

void Nitro::TheChaseController::Update(float dt_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, bool chaseWon, bool runnerWon) {
	
	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	auto players = entityManager_->GetAllEntitiesWithComponent<Engine::PlayerComponent>();
	auto player1 = players[0];
	auto player2 = players[1];
	if (player1->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(player1, player2);
	}

	auto transform1 = players[0]->GetComponent<Engine::TransformComponent>();
	auto transform2 = players[1]->GetComponent<Engine::TransformComponent>();

	if (runnerWon) {

		auto winner = Engine::Entity::Create();
		winner->AddComponent<Engine::TransformComponent>(transform1->m_Position.x, transform1->m_Position.y);
		winner->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("runnerwon");
		winner->AddComponent<Engine::DrawableEntity>();
		entityManager_->AddEntity(std::move(winner));

		/*auto player = Engine::Entity::Create();
		player->AddComponent<Engine::TransformComponent>(transform1->m_Position.x, transform1->m_Position.y + 300);
		player->AddComponent<Engine::TextComponent>("Runner");
		player->AddComponent<TextInfoComponent>(PlayerTag::Two, TextInfoType::Runner, player2);
		entityManager_->AddEntity(std::move(player));*/

	}
	if (chaseWon) {

		auto winner = Engine::Entity::Create();
		winner->AddComponent<Engine::TransformComponent>(transform1->m_Position.x, transform1->m_Position.y);
		winner->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("chaserwon");
		winner->AddComponent<Engine::DrawableEntity>();
		entityManager_->AddEntity(std::move(winner));

		/*auto player = Engine::Entity::Create();
		player->AddComponent<Engine::TransformComponent>(transform1->m_Position.x, transform1->m_Position.y + 300);
		player->AddComponent<Engine::TextComponent>("Chaser");
		player->AddComponent<TextInfoComponent>(PlayerTag::One, TextInfoType::Runner, player1);
		entityManager_->AddEntity(std::move(player));*/
	}

}
