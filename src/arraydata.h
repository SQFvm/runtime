#pragma once
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <cmath>

#include "data.h"
#include "type.h"


namespace sqf
{
	class value;
	class virtualmachine;
	class arraydata : public data
	{
	private:
		std::vector<std::shared_ptr<value>> mvalue;
		bool check_type(virtualmachine*, const sqf::type*, size_t) const;
	protected:
		inline std::vector<std::shared_ptr<value>>& innervector() { return mvalue; }
	public:
		arraydata() : mvalue(std::vector<std::shared_ptr<value>>()) {}
		arraydata(size_t size) : mvalue(std::vector<std::shared_ptr<value>>(size)) {}
		arraydata(std::vector<std::shared_ptr<value>> v) : mvalue(std::vector<std::shared_ptr<value>>(v)) {}
		virtual std::string tosqf() const;
		std::shared_ptr<value>& operator[](int index) { return mvalue.at(index); }
		std::shared_ptr<value> operator[](int index) const { return index < 0 || index >= (int)mvalue.size() ? std::make_shared<value>() : mvalue[index]; }
		std::shared_ptr<value>& at(int index) { return mvalue.at(index); }
		const std::shared_ptr<value> at(int index) const { return mvalue.at(index); }
		inline size_t size() const { return mvalue.size(); }
		operator std::vector<std::shared_ptr<value>>() const { return mvalue; }
		virtual bool equals(std::shared_ptr<data> d) const;

		inline std::vector<std::shared_ptr<value>>::iterator begin() { return mvalue.begin(); }
		inline std::vector<std::shared_ptr<value>>::iterator end() { return mvalue.end(); }

		inline void push_back(std::shared_ptr<value> val) { mvalue.push_back(val); }
		inline std::shared_ptr<value> pop_back() { auto back = mvalue.back(); mvalue.pop_back(); return back; }
		void resize(int newsize);
		void reverse();
		void extend(std::vector<std::shared_ptr<value>> other);
		void delete_at(int position);
		std::array<double, 3> as_vec3() const;
		std::array<double, 2> as_vec2() const;
		operator std::array<double, 3>() const { return as_vec3(); }

		inline bool check_type(virtualmachine* vm, type t, size_t len) const { return check_type(vm, t, len, len); }
		bool check_type(virtualmachine*, type, size_t min, size_t max) const;
		template<size_t size>
		inline bool check_type(virtualmachine* vm, std::array<sqf::type, size> arr) const { return check_type(vm, arr.data(), size); }

		static inline double distance3dsqr(const std::shared_ptr<arraydata> l, const std::shared_ptr<arraydata> r) { return distance3d(l->as_vec3(), r->as_vec3()); }
		static inline double distance3dsqr(const arraydata* l, const arraydata* r) { return distance3d(l->as_vec3(), r->as_vec3()); }
		static inline double distance3dsqr(std::array<double, 3> l, std::array<double, 3> r)
		{
			return std::pow(l[0] - r[0], 2) + std::pow(l[1] - r[1], 2) + std::pow(l[2] - r[2], 2);
		}
		static inline double distance3d(const std::shared_ptr<arraydata> l, const std::shared_ptr<arraydata> r) { return distance3d(l->as_vec3(), r->as_vec3()); }
		static inline double distance3d(const arraydata* l, const arraydata* r) { return  distance3d(l->as_vec3(), r->as_vec3()); }
		static inline double distance3d(std::array<double, 3> l, std::array<double, 3> r)
		{
			return std::sqrt(distance3dsqr(l, r));
		}
		static inline double distance2dsqr(const std::shared_ptr<arraydata> l, const std::shared_ptr<arraydata> r) { return  distance2d(l->as_vec2(), r->as_vec2()); }
		static inline double distance2dsqr(const arraydata* l, const arraydata* r) { return distance2d(l->as_vec2(), r->as_vec2()); }
		static inline double distance2dsqr(std::array<double, 2> l, std::array<double, 2> r)
		{
			return std::pow(l[0] - r[0], 2) + std::pow(l[1] - r[1], 2) + std::pow(l[2] - r[2], 2);
		}
		static inline double distance2d(const std::shared_ptr<arraydata> l, const std::shared_ptr<arraydata> r) { return distance2d(l->as_vec2(), r->as_vec2()); }
		static inline double distance2d(const arraydata* l, const arraydata* r) { return distance2d(l->as_vec2(), r->as_vec2()); }
		static inline double distance2d(std::array<double, 2> l, std::array<double, 2> r)
		{
			return std::sqrt(distance2dsqr(l, r));
		}
	};
}
