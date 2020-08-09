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
		class config;
		static const size_t invalid_config = ~((size_t)0);
		using config_iterator = std::vector<config>::iterator;
		class config
		{
			friend class confighost;
		public:
			class iterator_factory
			{
			private:
				std::vector<std::reference_wrapper<config>> m_gathered;
			public:
				using iterator = std::vector<std::reference_wrapper<config>>::iterator;
				iterator_factory(const config& current, confighost& r_confighost)
				{
					m_gathered.reserve(current.m_children.size());
					for (auto& id : current.m_children)
					{
						m_gathered.emplace_back(r_confighost.as_reference(id));
					}
				}
				iterator begin() { return m_gathered.begin(); }
				iterator end() { return m_gathered.end(); }
			};
			class iterator_recursive_factory
			{
			private:
				std::vector<std::reference_wrapper<config>> m_gathered;
			public:
				using iterator = std::vector<std::reference_wrapper<config>>::iterator;
				iterator_recursive_factory(config current, confighost& r_confighost)
				{
					std::vector<size_t> deleted(current.m_deleted);
					while (true)
					{
						auto factory = current.iterate(r_confighost);
						for (auto it : factory)
						{
							if (std::find(deleted.begin(), deleted.end(), it.get().m_self_index) == deleted.end())
							{
								m_gathered.emplace_back(it.get());
							}
						}
						if (!current.has_parent_logical())
						{
							break;
						}
						current = current.parent_logical(r_confighost);
						if (current.is_null())
						{
							break;
						}
					}
				}
				iterator begin() { return m_gathered.begin(); }
				iterator end() { return m_gathered.end(); }
			};

		private:
			std::vector<size_t> m_children;
			std::vector<size_t> m_deleted;
			sqf::runtime::value m_value;
			size_t m_parent_inherited_index;
			size_t m_parent_logical_index;
			size_t m_self_index;
			std::string m_name;
		public:

			bool operator==(const config& other) { return m_self_index == other.m_self_index; }
			bool operator!=(const config& other) { return !(*this == other); }

			config_iterator iterator(confighost& host)
			{
				return m_self_index == invalid_config ? host.end() : host.begin() + m_self_index;
			}
			config() : m_parent_inherited_index(invalid_config), m_parent_logical_index(invalid_config), m_self_index(invalid_config) {};

			bool has_parent_inherited() const { return m_parent_inherited_index != invalid_config; }
			config parent_inherited(confighost& host) const { return host.at(m_parent_inherited_index); }
			void parent_inherited(config conf) { m_parent_inherited_index = conf.m_self_index; }
			bool has_parent_logical() const { return m_parent_logical_index != invalid_config; }
			config parent_logical(confighost& host) const { return host.at(m_parent_logical_index); }
			void parent_logical(config conf) { m_parent_logical_index = conf.m_self_index; }

			std::string_view name() const { return m_name; }
			void name(std::string value) { m_name = value; }

			bool is_null() const { return m_self_index == invalid_config; }

			void push_child(confighost& host, config& conf)
			{
				if (conf.m_self_index == invalid_config)
				{
					host.push_back(conf);
				}
				m_children.push_back(conf.m_self_index);
			}
			void push_deleted(config& conf)
			{
				if (conf.m_self_index == invalid_config)
				{
					return;
				}
				m_deleted.push_back(conf.m_self_index);
			}

			config at(confighost& host, size_t index) const
			{
				if (m_children.size() <= index) { return { }; }
				return host.at(m_children[index]);
			}
			size_t children_size() const { return m_children.size(); }

			void value(sqf::runtime::value value) { m_value = value; }

			bool inherits_or_equal(confighost& host, config other) const
			{
				if (m_self_index == other.m_self_index)
				{
					return true;
				}
				config node = *this;
				while (node.has_parent_inherited())
				{
					node = node.parent_inherited(host);
					if (node.is_null())
					{
						return false;
					}
					if (m_self_index == node.m_self_index)
					{
						return true;
					}
				}
				return false;
			}

			sqf::runtime::value::cref value() const { return m_value; }

			/// <summary>
			/// Creates a new iterator factory that allows iterating over
			/// the childrens of this config node.
			/// </summary>
			/// <param name="host"></param>
			/// <returns></returns>
			iterator_factory iterate(confighost& host) { return iterator_factory(*this, host); }

			/// <summary>
			/// Creates a new iterator factory that allows iterating over
			/// the childrens of this config node.
			/// Also will automagically iterate over inherited nodes,
			/// honoring delete.
			/// </summary>
			/// <param name="host"></param>
			/// <returns></returns>
			iterator_recursive_factory iterate_recursive(confighost& host) { return iterator_recursive_factory(*this, host); }

			std::optional<config> navigate(confighost& host, std::string_view target_name)
			{
				for (auto& it : iterate_recursive(host))
				{
					if (it.get().name().compare(target_name))
					{
						return it;
					}
				}
				return {};
			}

			std::optional<config> navigate_local(confighost& host, std::string_view target_name)
			{
				for (auto& it : iterate(host))
				{
					if (it.get().name().compare(target_name))
					{
						return it;
					}
				}
				return {};
			}
		};
	private:
		std::vector<config> m_configs;
		bool m_is_final;
		config& as_reference(size_t index)
		{
			return m_configs.at(index);
		}
		const config& as_reference(size_t index) const
		{
			return m_configs.at(index);
		}

	public:
		confighost() : m_is_final(false)
		{
			m_configs.push_back(config());
		}
		void push_back(config conf)
		{
			if (conf.m_self_index != 0) { return; }
			conf.m_self_index = m_configs.size();
			m_configs.push_back(conf);
		}
		config& create_new() { return m_configs.emplace_back(); }
		config& root() { return as_reference(0); }
		const config& root() const { return as_reference(0); }
		config at(size_t index)
		{
			if (m_configs.size() <= index) { return {}; }
			return {};
		}

		config_iterator begin() { return m_configs.begin(); }
		config_iterator end() { return m_configs.end(); }
		config_iterator find_traverse_upwards_logical(config_iterator start, std::string_view view)
		{
			while (start->has_parent_logical())
			{
				auto parent = start->parent_logical(*this);
				if (parent.is_null())
				{
					return end();
				}
				start = parent.iterator(*this);
				auto factory = start->iterate(*this);
				auto res = std::find_if(factory.begin(), factory.end(), [view](config conf) { return conf.name() == view; });
				if (res != factory.end())
				{
					return res->get().iterator(*this);
				}
			}
			return end();
		}
		config_iterator find_traverse_upwards_inherited(config_iterator start, std::string_view view)
		{
			while (start->has_parent_inherited())
			{
				auto parent = start->parent_inherited(*this);
				if (parent.is_null())
				{
					return end();
				}
				start = parent.iterator(*this);
				auto factory = start->iterate(*this);
				auto res = std::find_if(factory.begin(), factory.end(), [view](config& conf) { return conf.name() == view; });
				if (res != factory.end())
				{
					return res->get().iterator(*this);
				}
			}
			return end();
		}
		

	};
}