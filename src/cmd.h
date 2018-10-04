#pragma once
#include <string>
#include <memory>
#include "type.h"
#include "string_op.h"
namespace sqf
{
	class virtualmachine;
	class value;

	typedef std::shared_ptr<value>(*nularcb)(virtualmachine*);
	typedef std::shared_ptr<value>(*unarycb)(virtualmachine*, std::shared_ptr<value>);
	typedef std::shared_ptr<value>(*binarycb)(virtualmachine*, std::shared_ptr<value>, std::shared_ptr<value>);
	class cmd
	{
	private:
		type mltype;
		type mrtype;
		std::string mdesc;
		std::string mname;
		short mprecedence;

	public:
		cmd(short precedence, std::string name, type ltype, type rtype, std::string description) { mprecedence = precedence; mname = name; mltype = ltype; mrtype = rtype; mdesc = description; }
		virtual std::shared_ptr<value> execute(virtualmachine*, std::shared_ptr<value> left, std::shared_ptr<value> right) const = 0;
		inline bool matches(type ltype, type rtype) { return (mltype == ltype || mltype == type::ANY) && (mrtype == rtype || mrtype == type::ANY); }
		inline bool matches(std::string name, type ltype, type rtype) { return matches(ltype, rtype) && str_cmpi(mname.c_str(), -1, name.c_str(), -1); }
		std::string desc() { return mdesc; }
		std::string name() { return mname; }
		type ltype() { return mltype; }
		type rtype() { return mrtype; }
		short precedence() { return mprecedence; }

	};


	class nularcmd : public cmd
	{
	private:
		nularcb mfnc;
	public:
		nularcmd(std::string name, std::string description, nularcb fnc) : cmd(4, name, type::NA, type::NA, description) { mfnc = fnc; }
		virtual std::shared_ptr<value> execute(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right) const { return mfnc(vm); }
	};
	class unarycmd : public cmd
	{
	private:
		unarycb mfnc;
	public:
		unarycmd(std::string name, type rtype, std::string description, unarycb fnc) : cmd(4, name, type::NA, rtype, description) { mfnc = fnc; }
		virtual std::shared_ptr<value> execute(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right) const { return mfnc(vm, right); }
	};
	class binarycmd : public cmd
	{
	private:
		binarycb mfnc;
	public:
		binarycmd(short precedence, std::string name, type ltype, type rtype, std::string description, binarycb fnc) : cmd(precedence, name, ltype, rtype, description) { mfnc = fnc; }
		virtual std::shared_ptr<value> execute(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right) const { return mfnc(vm, left, right); }
	};

	inline static std::shared_ptr<nularcmd> nular(std::string name, std::string description, nularcb fnc) { return std::make_shared<nularcmd>(name, description, fnc); }
	inline static std::shared_ptr<unarycmd> unary(std::string name, type rtype, std::string description, unarycb fnc) { return std::make_shared<unarycmd>(name, rtype, description, fnc); }
	inline static std::shared_ptr<binarycmd> binary(short precedence, std::string name, type ltype, type rtype, std::string description, binarycb fnc) { return std::make_shared<binarycmd>(precedence, name, ltype, rtype, description, fnc); }
}
