#include "precomp.h"
#include "GameComponents.h"
#include "TextureController.h"
#include "TextController.h"


bool Nitro::TextController::Init(Engine::EntityManager* entityManager_) {

	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	auto players = entityManager_->GetAllEntitiesWithComponent<Engine::PlayerComponent>();
	auto player1 = players[0];
	auto player2 = players[1];
	if (player1->GetComponent<PlayerTagComponent>()->m_PlayerTag == PlayerTag::Two)
	{
		std::swap(player1, player2);
	}

	auto km1 = Engine::Entity::Create();
	km1->AddComponent<Engine::TextComponent>("km/h");
	km1->AddComponent<Engine::TransformComponent>(90.f, 40.f);
	km1->AddComponent<TextInfoComponent>(PlayerTag::One, TextInfoType::Kmh, player1);
	entityManager_->AddEntity(std::move(km1));

	auto km2 = Engine::Entity::Create();
	km2->AddComponent<Engine::TextComponent>("km/h");
	km2->AddComponent<Engine::TransformComponent>(1200.f, 40.f);
	km2->AddComponent<TextInfoComponent>(PlayerTag::Two, TextInfoType::Kmh, player2);
	entityManager_->AddEntity(std::move(km2));

	auto speed1 = Engine::Entity::Create();
	speed1->AddComponent<Engine::TextComponent>("0");
	speed1->AddComponent<Engine::TransformComponent>(40.f, 40.f);
	speed1->AddComponent<TextInfoComponent>(PlayerTag::One, TextInfoType::Speed, player1);
	entityManager_->AddEntity(std::move(speed1));

	auto speed2 = Engine::Entity::Create();
	speed2->AddComponent<Engine::TextComponent>("0");
	speed2->AddComponent<Engine::TransformComponent>(1150.f, 40.f);
	speed2->AddComponent<TextInfoComponent>(PlayerTag::Two, TextInfoType::Speed, player2);
	entityManager_->AddEntity(std::move(speed2));

	auto mile1 = Engine::Entity::Create();
	mile1->AddComponent<Engine::TextComponent>("0");
	mile1->AddComponent<Engine::TransformComponent>(40.f, 15.f);
	mile1->AddComponent<TextInfoComponent>(PlayerTag::One, TextInfoType::Distance, player1);
	entityManager_->AddEntity(std::move(mile1));

	auto mile2 = Engine::Entity::Create();
	mile2->AddComponent<Engine::TextComponent>("0");
	mile2->AddComponent<Engine::TransformComponent>(1150.f, 15.f);
	mile2->AddComponent<TextInfoComponent>(PlayerTag::Two, TextInfoType::Distance, player2);
	entityManager_->AddEntity(std::move(mile2));

	auto chase = player1->GetComponent<TheChaseComponent>()->m_TheChase;

	auto chaser = Engine::Entity::Create();
	chaser->AddComponent<Engine::TextComponent>("CHASER");
	if (chase == TheChase::Chaser) {
	chaser->AddComponent<TextInfoComponent>(PlayerTag::One, TextInfoType::Runner, player1);
	chaser->AddComponent<Engine::TransformComponent>(120.f, 15.f);
	}
	else {
	chaser->AddComponent<TextInfoComponent>(PlayerTag::Two, TextInfoType::Runner, player2);
	chaser->AddComponent<Engine::TransformComponent>(1000.f, 15.f);
	}
	entityManager_->AddEntity(std::move(chaser));

	auto runner = Engine::Entity::Create();
	runner->AddComponent<Engine::TextComponent>("RUNNER");
	if (chase == TheChase::Chaser) {
		runner->AddComponent<TextInfoComponent>(PlayerTag::Two, TextInfoType::Runner, player2);
		runner->AddComponent<Engine::TransformComponent>(1000.f, 15.f);
	}
	else {
		runner->AddComponent<TextInfoComponent>(PlayerTag::One, TextInfoType::Runner, player1);
		runner->AddComponent<Engine::TransformComponent>(120.f, 15.f);
	}
	entityManager_->AddEntity(std::move(runner));


	return true;
}

void Nitro::TextController::Update(float dt_, Engine::EntityManager* entityManager_) {

	ASSERT(entityManager_ != nullptr, "Must pass a valid entity manager");

	auto texts = entityManager_->GetAllEntitiesWithComponent<Engine::TextComponent>();

	for (auto text : texts) {
		auto tekst = text->GetComponent<Engine::TextComponent>();
		auto info = text->GetComponent<TextInfoComponent>();

		switch (info->m_Type) {
		case TextInfoType::Speed: 
		{
			auto speed = info->m_PlayerEntity->GetComponent<CarPhysicsComponent>();
			tekst->m_text = std::to_string((int)speed->m_CarSpeed/4);
		}
		break;
		case TextInfoType::Kmh: 
		{ 

		}
		break;
		case TextInfoType::Runner:
		{

		}
		break;
		case TextInfoType::Distance: {
			auto transform = info->m_PlayerEntity->GetComponent<Engine::TransformComponent>();
			auto newPosition = transform->m_Position.y;
			if (info->m_PlayerTag == PlayerTag::One) {
				if (oldPosition1 - 10 > (int)newPosition) {
					Distance1 += 10;
					std::string s = std::to_string(Distance1);
					tekst->m_text = s;
					oldPosition1 = (int)newPosition;
				}
			} else {
				if (oldPosition2 - 10 > (int)newPosition) {
					Distance2 += 10;
					std::string s = std::to_string(Distance2);
					tekst->m_text = s;
					oldPosition2 = (int)newPosition;
				}
			}
		} 
		break;
		default: {
			//LOG_CRITICAL("Enum for text type does not exist.");

		}
		}

	}

}
