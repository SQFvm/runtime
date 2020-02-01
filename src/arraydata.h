#pragma once
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <array>
#include <cmath>

#include "data.h"
#include "type.h"
#include "value.h"


namespace sqf
{
	class value;
	class virtualmachine;
	class arraydata : public data
	{
	private:
		std::vector<value> mvalue;
		bool check_type(virtualmachine*, const sqf::type*, size_t) const;
		bool check_type(virtualmachine*, const sqf::type*, size_t, size_t) const;
		bool recursion_test_helper(std::vector<std::shared_ptr<arraydata>>& visited);
	protected:
		auto& innervector() { return mvalue; }
		const auto& innervector() const { return mvalue; }
	public:
		// Returns true, if no recursion is present.
		// Returns false, if current array state contains a recursion.
		bool recursion_test() { std::vector<std::shared_ptr<arraydata>> vec; return recursion_test_helper(vec); }
        arraydata() = default;
		arraydata(size_t size) : mvalue(std::vector<value>(size)) {}
        arraydata(std::vector<value> v) : mvalue(std::move(v)) {}
		arraydata deep_copy() const;
		std::string tosqf() const override;
		value& operator[](size_t index) { return at(index); }
		value operator[](size_t index) const { return at(index); }
		value& at(size_t index) { return mvalue.at(index); }
		value at(size_t index) const { return mvalue.at(index); }
		size_t size() const { return mvalue.size(); }
		operator std::vector<value>() const { return mvalue; }
		bool equals(std::shared_ptr<data> d) const override;
		bool empty() const { return mvalue.empty(); }

		std::vector<value>::iterator begin() { return mvalue.begin(); }
		std::vector<value>::iterator end() { return mvalue.end(); }

		std::vector<value>::iterator erase(std::vector<value>::iterator begin, std::vector<value>::iterator end) { return mvalue.erase(begin, end); }

        //#TODO emplace back
		bool push_back(value val) { mvalue.push_back(std::move(val)); if (!recursion_test()) { mvalue.pop_back(); return false; } return true; }
		value pop_back() { auto back = mvalue.back(); mvalue.pop_back(); return back; }
		void resize(size_t newsize);
		void reverse();
		void extend(std::vector<value> other);
		void delete_at(int position);
		std::array<double, 3> as_vec3() const;
		std::array<double, 2> as_vec2() const;
		operator std::array<double, 3>() const { return as_vec3(); }
        //#TODO begin/end iterators
		bool get(size_t index, bool defval);
		float get(size_t index, float defval);
		std::string get(size_t index, std::string defval);
		int get(size_t index, int defval) { return static_cast<int>(get(index, static_cast<float>(defval))); }
		std::string get(size_t index, const char* defval) { return get(index, std::string(defval)); }

		bool check_type(virtualmachine* vm, sqf::type t, size_t len) const { return check_type(vm, t, len, len); }
		bool check_type(virtualmachine*, sqf::type, size_t min, size_t max) const;
		template<size_t size>
		bool check_type(virtualmachine* vm, const std::array<sqf::type, size>& arr) const { return check_type(vm, arr.data(), size); }
        bool check_type(virtualmachine* vm, const std::vector<sqf::type>& vec) const { return check_type(vm, vec.data(), vec.size()); }
		template<size_t size>
        bool check_type(virtualmachine* vm, const std::array<sqf::type, size>& arr, size_t optionalstart) const { return check_type(vm, arr.data(), size, optionalstart); }

		static double distance3dsqr(const std::shared_ptr<arraydata>& l, const std::shared_ptr<arraydata>& r) { return distance3d(l->as_vec3(), r->as_vec3()); }
		static double distance3dsqr(const arraydata* l, const arraydata* r) { return distance3d(l->as_vec3(), r->as_vec3()); }
		static double distance3dsqr(std::array<double, 3> l, std::array<double, 3> r)
		{
			return std::pow(l[0] - r[0], 2) + std::pow(l[1] - r[1], 2) + std::pow(l[2] - r[2], 2);
		}
		static double distance3d(const std::shared_ptr<arraydata>& l, const std::shared_ptr<arraydata>& r) { return distance3d(l->as_vec3(), r->as_vec3()); }
		static double distance3d(const arraydata* l, const arraydata* r) { return  distance3d(l->as_vec3(), r->as_vec3()); }
		static double distance3d(std::array<double, 3> l, std::array<double, 3> r)
		{
			return std::sqrt(distance3dsqr(l, r));
		}
		static double distance2dsqr(const std::shared_ptr<arraydata>& l, const std::shared_ptr<arraydata>& r) { return  distance2d(l->as_vec2(), r->as_vec2()); }
		static double distance2dsqr(const arraydata* l, const arraydata* r) { return distance2d(l->as_vec2(), r->as_vec2()); }
		static double distance2dsqr(std::array<double, 2> l, std::array<double, 2> r)
		{
			return std::pow(l[0] - r[0], 2) + std::pow(l[1] - r[1], 2);
		}
		static double distance2d(const std::shared_ptr<arraydata>& l, const std::shared_ptr<arraydata>& r) { return distance2d(l->as_vec2(), r->as_vec2()); }
		static double distance2d(const arraydata* l, const arraydata* r) { return distance2d(l->as_vec2(), r->as_vec2()); }
		static double distance2d(std::array<double, 2> l, std::array<double, 2> r)
		{
			return std::sqrt(distance2dsqr(l, r));
		}

        sqf::type dtype() const override { return sqf::type::ARRAY; }
	};
}
