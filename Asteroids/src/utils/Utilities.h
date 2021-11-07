#pragma once
#include "pch.h"

namespace ast
{
	inline void CenterTextOrigin(sf::Text& text)
	{
		sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

	class Timer
	{
	public:
		Timer(float time) : m_MaxTime(time) { }

		Timer() = default;

	public:
		void SetTime(float time) { m_MaxTime = time; }

	public:
		inline void Update(float dt)
		{
			if(!m_Done)
				m_CurrentTime += dt;

			if (m_CurrentTime >= m_MaxTime)
				m_Done = true;
		}

		inline bool IsDone() const { return m_Done; }

	public:
		inline void Restart() { m_CurrentTime = 0.f; m_Done = false; }

	private:
		float m_CurrentTime = 0.f, m_MaxTime = 0.f;
		bool m_Done;
	};
}