#pragma once
#include <tuple>


typename char net_message_id;

class net_message
{
public:
	class payload
	{
	public:
		std::tuple<char*, size_t> serialize() = 0;
		void deserialize(char* data, size_t data_length) = 0;
	};
private:
	net_message_id m_id;
	char * m_data;
	size_t m_data_length;
public:
	net_message(char id, char * data, size_t data_length) : m_id(id), m_data(data), m_data_length(m_data_length) {}

	net_message_id id() const { return m_id; }
	std::tuple<char*, size_t> data() const { return std::make_tuple(m_data, m_data_length); }

	template<class T>
	T get_payload()
	{
		static_assert(std::is_base_of<payload, T>::value, "net_message::get_payload<T>() can only convert to net_message::payload types");
		T p;
		static_cast<payload>(p).deserialize(m_data, m_data_length);
		return p;
	}
};