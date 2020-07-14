#pragma once
#include "value.h"

#include <vector>
#include <string>
#include <string_view>
#include <optional>


namespace sqf::runtime
{
	class confighost
	{
	public:
		static const size_t invalid_config = ~((size_t)0);

		class confignav;
		class config
		{
			friend class confighost;
		public:
			class iterator
			{
			public:
				using difference_type = long;
				using value_type = config;
				using pointer = const config*;
				using reference = const config&;
				using iterator_category = std::forward_iterator_tag;

			private:
				config& mr_config;
				confighost& mr_confighost;
				size_t m_config_location;
			public:
				iterator(config& r_config, confighost& r_confighost, size_t config_location) :
					mr_config(r_config),
					mr_confighost(r_confighost),
					m_config_location(config_location)
				{}

				iterator& operator++() { if (m_config_location == mr_config.m_children.size()) { return *this; } m_config_location++; return *this; }
				iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }
				bool operator==(iterator other) const { return m_config_location == m_config_location && mr_config.m_self_index == other.mr_config.m_self_index; }
				bool operator!=(iterator other) const { return !(*this == other); }
				value_type& operator*() { return mr_confighost.as_reference(m_config_location); }
				// iterator traits
			};

			class iterator_factory
			{
			private:
				config& mr_config;
				confighost& mr_confighost;
			public:
				iterator_factory(config& r_config, confighost& r_confighost) :
					mr_config(r_config),
					mr_confighost(r_confighost)
				{}
				iterator begin() { return iterator(mr_config, mr_confighost, mr_config.m_children.front()); }
				iterator end() { return iterator(mr_config, mr_confighost, mr_config.m_children.back()); }
			};

		private:
			std::vector<size_t> m_children;
			sqf::runtime::value m_value;
			size_t m_parent_inherited_index;
			size_t m_parent_logical_index;
			size_t m_self_index;
			std::string name;

		public:
			config() : m_parent_inherited_index(0), m_parent_logical_index(0), m_self_index(0) {};

			confignav parent_inherited(confighost& host) const { return host.at(m_parent_inherited_index); }
			void parent_inherited(config conf) { m_parent_inherited_index = conf.m_self_index; }
			confignav parent_logical(confighost& host) const { return host.at(m_parent_logical_index); }
			void parent_logical(config conf) { m_parent_logical_index = conf.m_self_index; }

			void push_back(confighost& host, config conf)
			{
				if (conf.m_self_index == 0)
				{
					host.push_back(conf);
				}
				m_children.push_back(conf.m_self_index);
			}

			confignav at(confighost& host, size_t index) const
			{
				if (m_children.size() <= index) { return { host }; }
				return host.at(m_children[index]);
			}
			size_t children_size() const { return m_children.size(); }

			void value(sqf::runtime::value value) { m_value = value; }

			sqf::runtime::value::cref value() const { return m_value; }

			iterator_factory iterate(confighost& host) { return iterator_factory(*this, host); }

			std::optional<config> navigate(confighost& host, std::string_view target_name)
			{
				for (auto& it : iterate(host))
				{
					if (it.name.compare(target_name))
					{
						return it;
					}
				}
				return {};
			}
		};
		class confignav
		{
		private:
			confighost& mr_confighost;
			size_t m_entry_index;
		public:
			bool is_null() const { return m_entry_index == invalid_config || m_entry_index >= mr_confighost.m_configs.size(); }
			config& get() const { return mr_confighost.as_reference(m_entry_index); }

			confignav(confighost& confighost) : mr_confighost(confighost), m_entry_index(invalid_config) {}
			confignav(confighost& confighost, size_t index) : mr_confighost(confighost), m_entry_index(index) {}

			config& operator*() { return mr_confighost.as_reference(m_entry_index); }
		};
	private:
		std::vector<config> m_configs;
		bool m_is_final;
		config& as_reference(size_t index)
		{
			return m_configs.at(index);
		}

	public:
		confighost() : m_is_final(false) {}
		void push_back(config conf)
		{
			if (conf.m_self_index != 0) { return; }
			conf.m_self_index = m_configs.size();
			m_configs.push_back(conf);
		}
		confignav at(size_t index)
		{
			if (m_configs.size() <= index) { return { *this }; }
			return { *this, index };
		}
	};
}