#pragma once
#include "Scene.h"

namespace ast
{
	class SceneMainMenu : public Scene
	{
	public:
		virtual void Initialize() override;

		virtual void PollEvents(const sf::Event& event) override;

		virtual void Update(float dt) override;

		virtual void Render(sf::RenderWindow& window) override;

	private:
		sf::RectangleShape back;
	};
}
