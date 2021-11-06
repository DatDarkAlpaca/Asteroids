#pragma once
#include "Scene.h"
#include "gui/GUI.h"

namespace ast
{
	class SceneMainMenu : public Scene
	{
	public:
		virtual void Initialize() override;

		virtual void PollEvents(const sf::Event& event) override;

		virtual void Update(float dt) override;

		virtual void Render(sf::RenderWindow& window) override;

		virtual void Cleanup() override;

	private:
		gui::Container m_MainContainer;
		sf::RectangleShape m_Background;

		sf::Font* m_Font;
	};
}
