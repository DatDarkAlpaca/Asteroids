#pragma once
#include "pch.h"
#include "scenes/SceneManager.h"
#include "window/WindowHandler.h"

namespace ast {

	class Application
	{
	public:
		Application()
			: m_SceneManager(this) { }

	public:
		void Run();

	private:
		void PollEvents();

		void Update(float dt);

		void Render();

	private:
		void InitializeSeed();

		void CreateScenes();

	public:
		inline void RequestClose() { m_WindowHandler.GetWindow().close(); }

	private:
		SceneManager m_SceneManager;
		WindowHandler m_WindowHandler;
	};

}
