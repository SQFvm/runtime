#pragma once
#include <vector>
#include <chrono>

#include "frame.h"
#include "value.h"

namespace sqf::runtime
{
	class context final
	{
	private:
		std::vector<sqf::runtime::frame> m_frames;
		std::vector<sqf::runtime::value> m_values;
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
		void push_frame(sqf::runtime::frame frame) { m_frames.push_back(frame); }
		void push_value(sqf::runtime::value value) { m_values.push_back(value); }
		sqf::runtime::frame pop_frame() { auto frame = m_frames.back(); m_frames.pop_back(); return frame; }
		sqf::runtime::value pop_value() { if (m_values.empty()) { return {}; } else { auto value = m_values.back(); m_values.pop_back(); return value; } }
		sqf::runtime::value::cref peek_value() { if (m_values.empty()) { return {}; } else { return m_values.back(); } }

		std::vector<sqf::runtime::frame>::reverse_iterator frames_rbegin() { return m_frames.rbegin(); }
		std::vector<sqf::runtime::frame>::reverse_iterator frames_rend() { return m_frames.rend(); }


		frame& current_frame() { return m_frames.back(); }
	};
}