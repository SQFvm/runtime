#pragma once
#include <string>
#include <memory>
#include "type.h"
#include "wstring_op.h"
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
		std::wstring mdesc;
		std::wstring mname;
		short mprecedence;

	public:
		cmd(short precedence, std::wstring name, type ltype, type rtype, std::wstring description) { mprecedence = precedence; mname = name; mltype = ltype; mrtype = rtype; mdesc = description; }
		virtual std::shared_ptr<value> execute(virtualmachine*, std::shared_ptr<value> left, std::shared_ptr<value> right) const = 0;
		inline bool matches(type ltype, type rtype) { return (mltype == ltype || mltype == type::ANY) && (mrtype == rtype || mrtype == type::ANY); }
		inline bool matches(std::wstring name, type ltype, type rtype) { return matches(ltype, rtype) && wstr_cmpi(mname.c_str(), -1, name.c_str(), -1); }
		std::wstring desc(void) { return mdesc; }
		std::wstring name(void) { return mname; }
		type ltype(void) { return mltype; }
		type rtype(void) { return mrtype; }
		short precedence(void) { return mprecedence; }

	};


	class nularcmd : public cmd
	{
	private:
		nularcb mfnc;
	public:
		nularcmd(std::wstring name, std::wstring description, nularcb fnc) : cmd(4, name, type::NA, type::NA, description) { mfnc = fnc; }
		virtual std::shared_ptr<value> execute(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right) const { return mfnc(vm); }
	};
	class unarycmd : public cmd
	{
	private:
		unarycb mfnc;
	public:
		unarycmd(std::wstring name, type rtype, std::wstring description, unarycb fnc) : cmd(4, name, type::NA, rtype, description) { mfnc = fnc; }
		virtual std::shared_ptr<value> execute(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right) const { return mfnc(vm, right); }
	};
	class binarycmd : public cmd
	{
	private:
		binarycb mfnc;
	public:
		binarycmd(short precedence, std::wstring name, type ltype, type rtype, std::wstring description, binarycb fnc) : cmd(precedence, name, ltype, rtype, description) { mfnc = fnc; }
		virtual std::shared_ptr<value> execute(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right) const { return mfnc(vm, left, right); }
	};

	static std::shared_ptr<nularcmd> nular(std::wstring name, std::wstring description, nularcb fnc) { return std::make_shared<nularcmd>(name, description, fnc); }
	static std::shared_ptr<unarycmd> unary(std::wstring name, type rtype, std::wstring description, unarycb fnc) { return std::make_shared<unarycmd>(name, rtype, description, fnc); }
	static std::shared_ptr<binarycmd> binary(short precedence, std::wstring name, type ltype, type rtype, std::wstring description, binarycb fnc) { return std::make_shared<binarycmd>(precedence, name, ltype, rtype, description, fnc); }
}
