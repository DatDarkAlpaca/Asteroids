#pragma once
#include "pch.h"

namespace ast
{

	enum class SceneType
	{
		None = 0,
		MainMenu = 1,
		SinglePlay = 2
	};

	class SceneManager;

	class Scene
	{
	protected:
		Scene() = default;

	public:
		virtual ~Scene() = default;

	public:
		inline void SetManager(SceneManager* manager) { this->manager = manager; }

	public:
		virtual void Initialize() {};

		virtual void PollEvents(const sf::Event& event) = 0;

		virtual void Update(float dt) = 0;

		virtual void Render(sf::RenderWindow& window) = 0;

		virtual void Cleanup() {};

	protected:
		SceneManager* manager;

	public:
		entt::registry registry;
	};

}
