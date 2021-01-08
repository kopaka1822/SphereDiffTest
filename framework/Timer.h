#pragma once
#include <chrono>
#include "error.h"

class Timer
{
	using clock = std::chrono::steady_clock;
public:
	/// \brief starts the timer
	void start()
	{
		m_start = clock::now();
		m_running = true;
	}

	/// \brief stops the timer
	/// \return the measured time in milliseconds
	float stop()
	{
		// was the timer running
		dassert(m_running);

		m_end = clock::now();
		m_running = false;
		return std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start).count() * 0.000001f;
	}

	/// \brief resets the time (stop and start)
	/// \return measured time in milliseconds
	float lap()
	{
		// was the timer started?
		dassert(m_running);

		auto tmp = clock::now();
		auto res = std::chrono::duration_cast<std::chrono::nanoseconds>(tmp - m_start).count() * 0.000001f;
		m_start = tmp;
		
		return res;
	}

	/// \brief returns the current time delta if the timer is running or the measured time if the timer was stopped
	/// \returns time in miliseconds
	float current() const
	{
		if (m_running)
		{
			auto tmp = clock::now();
			return std::chrono::duration_cast<std::chrono::nanoseconds>(tmp - m_start).count() * 0.000001f;
		}
		return std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start).count() * 0.000001f;
	}

private:
	clock::time_point m_start;
	clock::time_point m_end;
	bool m_running = false;
};