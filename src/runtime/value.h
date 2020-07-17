#pragma once
#include "data.h"

#include <string>
#include <memory>
#include <functional>



namespace sqf::runtime
{
	class value
	{
	public:
		using cwref = std::reference_wrapper<const value>;
		using cref = const value&;
	private:
		std::shared_ptr<sqf::runtime::data> m_data;
	public:
		value();
		value(std::shared_ptr<sqf::runtime::data> d) : m_data(std::move(d)) { }
		template<class T>
		value(T t) { m_data << t; }

		explicit operator type() const { return m_data ? m_data->type() : sqf::types::t_nothing(); };

		template<class T>
		bool is() const
		{
			static_assert(std::is_base_of<sqf::runtime::type, T>::value, "value::is<T>() can only be used with sqf::runtime::type types.");
			return this->operator sqf::runtime::type() == T();
		}

		bool operator==(cref other) const { return (m_data.get() && other.m_data.get()) || (m_data != nullptr && m_data->equals(other.data())); }


		/// <summary>
		/// Returns the m_data member.
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<sqf::runtime::data> data() const { return m_data; }

		template<class TData, typename TValue>
		TValue data_as() { return (TValue)(data<TData>()->value()); }

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