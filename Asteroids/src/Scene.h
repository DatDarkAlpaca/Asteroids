#pragma once
#include "pch.h"

namespace ast
{

	enum class SceneType
	{
		None = 0,
		MainMenu = 1
	};

	class Scene
	{
	protected:
		Scene() = default;

	public:
		virtual ~Scene() = default;

	public:
		virtual void Initialize() {};

		virtual void PollEvents(const sf::Event& event) = 0;

		virtual void Update(float dt) = 0;

		virtual void Render(sf::RenderWindow& window) = 0;

		virtual void Cleanup() {};

	public:
		entt::registry registry;

		friend class Entity;
	};

}
