#pragma once
#include "pch.h"
#include "scenes/SceneManager.h"
#include "window/WindowHandler.h"

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

	private:
		SceneManager m_SceneManager;
		WindowHandler m_WindowHandler;
	};

}
