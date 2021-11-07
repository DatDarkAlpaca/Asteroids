#pragma once
#include "pch.h"
#include "Scene.h"

namespace ast
{

	class Application;

	class SceneManager
	{
	public:
		SceneManager(Application* application)
			: application(application) { }

	public:
		void PollEvents(const sf::Event& _event);

		void Update(float dt);

		void Render(sf::RenderWindow& window);

	public:
		inline void AddScene(SceneType sceneName, const std::shared_ptr<Scene>& scene)
		{
			m_Scenes.insert({ sceneName, scene });	
		}

		void SelectScene(SceneType sceneName);

	public:
		Application& GetApp() { return *application; }

	private:
		std::unordered_map<SceneType, std::shared_ptr<Scene>> m_Scenes;
		std::shared_ptr<Scene> m_CurrentScene;

	private:
		Application* application;
	};

}
