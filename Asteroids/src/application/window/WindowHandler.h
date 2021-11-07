#pragma once
#include "pch.h"

namespace ast 
{

	// Todo: think of a better system to carry the window around:
	struct WindowHolder
	{
		sf::RenderWindow* window;
	} inline windowHolder;

	class WindowHandler
	{
	public:
		WindowHandler();

	public:
		inline sf::RenderWindow& GetWindow() { return m_Window; }

		inline void SetView() { m_Window.setView(m_MainView); }

		inline void ApplyResizedView(const sf::Event& event)
		{
			ApplyLetterboxView(m_MainView, event.size.width, event.size.height);
		}

		inline void ToggleFullscreen()
		{
			m_Fullscreen = !m_Fullscreen;
			CreateWindow();
		}

	private:
		sf::View& ApplyLetterboxView(sf::View view, int windowWidth, int windowHeight);

		void CreateWindow();

		void CreateView();

	private:
		sf::RenderWindow m_Window;
		sf::View m_MainView;

		bool m_Fullscreen = false;
	};

}
