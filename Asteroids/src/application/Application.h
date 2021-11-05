#pragma once
#include "pch.h"
#include "scenes/SceneManager.h"

namespace ast {

	class Application
	{
	public:
		void Run();

	private:
		void PollEvents();

		void Update(float dt);

		void Render();

	private:
		void InitializeSeed();

		void CreateScenes();

		void CreateWindow();

	private:
		sf::View& ApplyLetterboxView(sf::View view, int windowWidth, int windowHeight);

		void CreateView();

	private:
		sf::RenderWindow m_Window;
		sf::View m_MainView;

	private:
		SceneManager m_SceneManager;

	private:
		bool m_Fullscreen = false;
	};

}
