#pragma once
#include "Scene.h"
#include "components/Components.h"
#include "systems/Systems.h"
#include "prefabs/Asteroid.h"
#include "prefabs/Ship.h"

namespace ast
{
	
	class SceneSinglePlayer : public Scene
	{
	public:
		virtual void Initialize() override;

		virtual void PollEvents(const sf::Event& event) override;

		virtual void Update(float dt) override;

		virtual void Render(sf::RenderWindow& window) override;

	};

}

