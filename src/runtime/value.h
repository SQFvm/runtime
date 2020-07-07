#pragma once
#include <string>
#include <memory>
#include "data.h"



namespace sqf::runtime
{
	class value
	{
	public:
		using cref = const value&;
	private:
		std::shared_ptr<sqf::runtime::data> m_data;
	public:
		value();
		value(std::shared_ptr<sqf::runtime::data> d) : m_data(std::move(d)) { }

		explicit operator type() const { return m_data ? m_data->type() : t_nothing(); };


		/// <summary>
		/// Returns the m_data member.
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<sqf::runtime::data> data() const { return m_data; }

		/// <summary>
		/// Attempts to convert the data-member to the provided data type.
		/// Will use std::dynamic_pointer_cast.
		/// </summary>
		/// <remark>
		/// If it can be ensured that this will be the correct type, use sqf::runtime::value::data<T>.
		/// </remarks>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template <class T>
		std::shared_ptr<T> data_try() const {
			static_assert(std::is_base_of<sqf::runtime::data, T>::value, "value::data_try_as<T>() can only convert to sqf::runtime::data types");
			return std::dynamic_pointer_cast<T>(m_data);
		}
		/// <summary>
		/// Converts the data-member to the provided data type.
		/// Will use std::static_pointer_cast.
		/// </summary>
		/// <remark>
		/// If in doubt of the correct type, use sqf::runtime::value::data_try<T>.
		/// </remarks>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<class T>
		std::shared_ptr<T> data() const {
			static_assert(std::is_base_of<sqf::runtime::data, T>::value, "value::data<T>() can only convert to sqf::runtime::data types");
			return std::static_pointer_cast<T>(m_data);
		}
	};
}