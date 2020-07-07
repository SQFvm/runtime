#pragma once
#include <vector>
#include <chrono>

#include "frame.h"

namespace sqf::runtime
{
	class context
	{
	private:
		std::vector<sqf::runtime::frame> m_frames;
		bool m_suspended;
		std::chrono::system_clock::time_point m_wakeup_timestamp;

	public:
		context() = default;


		bool suspended() const { return m_suspended; }
		std::chrono::system_clock::time_point wakeup_timestamp() const { return m_wakeup_timestamp; }
		template<class T>
		void suspend(std::chrono::duration<T> duration)
		{
			m_wakeup_timestamp = std::chrono::system_clock::now() + duration;
			m_suspended = true;
		}
		void unsuspend() { m_suspended = false; }
		bool empty() const { return m_frames.empty(); }
		size_t size() const { return m_frames.size(); }
		void clear() { m_frames.clear(); }
		void push_back(sqf::runtime::frame frame) { m_frames.push_back(frame); }
		void pop_back() { m_frames.pop_back(); }


		frame& current_frame() { return m_frames.back(); }
	};
}