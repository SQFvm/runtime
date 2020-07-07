#pragma once
#include <string>
#include <memory>
#include "type.h"
#include "string_op.h"
#include "value.h"
namespace sqf
{
	class virtualmachine;
	class value;

	typedef value(*nularcb)(virtualmachine*);
	typedef value(*unarycb)(virtualmachine*, value::cref);
	typedef value(*binarycb)(virtualmachine*, value::cref, value::cref);
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
		virtual value execute(virtualmachine*, const value& left, const value& right) const = 0;
		bool matches(type ltype, type rtype) { return (mltype == ltype || mltype == type::ANY) && (mrtype == rtype || mrtype == type::ANY); }
		bool matches(std::string name, type ltype, type rtype) { return matches(ltype, rtype) && str_cmpi(mname.c_str(), -1, name.c_str(), -1); }
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
		value execute(virtualmachine* vm, const value& left, const value& right) const override { return mfnc(vm); }
	};
	class unarycmd : public cmd
	{
	private:
		unarycb mfnc;
	public:
		unarycmd(std::string name, type rtype, std::string description, unarycb fnc) : cmd(4, name, type::NA, rtype, description) { mfnc = fnc; }
		value execute(virtualmachine* vm, const value& left, const value& right) const override { return mfnc(vm, right); }
	};
	class binarycmd : public cmd
	{
	private:
		binarycb mfnc;
	public:
		binarycmd(short precedence, std::string name, type ltype, type rtype, std::string description, binarycb fnc) : cmd(precedence, name, ltype, rtype, description) { mfnc = fnc; }
		value execute(virtualmachine* vm, const value& left, const value& right) const override { return mfnc(vm, left, right); }
	};

	template<typename TData>
	class nularcmddata : public nularcmd
	{
	public:
		typedef value(*callback)(virtualmachine*, TData);
	private:
		callback mfnc;
		TData mdata;
	public:
		nularcmddata(TData data, std::string name, std::string description, callback fnc) : nularcmd(name, description, nullptr)
		{ mfnc = fnc; mdata = data; }
		value execute(virtualmachine* vm, const value& left, const value& right) const override { return mfnc(vm, mdata); }
	};
	template<typename TData>
	class unarycmddata : public unarycmd
	{
	public:
		typedef value(*callback)(virtualmachine*, TData, value::cref);
	private:
		callback mfnc;
		TData mdata;
	public:
		unarycmddata(TData data, std::string name, type rtype, std::string description, callback fnc) : unarycmd(name, rtype, description, nullptr)
		{ mfnc = fnc; mdata = data; }
		value execute(virtualmachine* vm, const value& left, const value& right) const override { return mfnc(vm, mdata, right); }
	};
	template<typename TData>
	class binarycmddata : public binarycmd
	{
	public:
		typedef value(*callback)(virtualmachine*, TData, value::cref, value::cref);
	private:
		callback mfnc;
		TData mdata;
	public:
		binarycmddata(TData data, short precedence, std::string name, type ltype, type rtype, std::string description, callback fnc) : binarycmd(precedence, name, ltype, rtype, description, nullptr)
		{ mfnc = fnc; mdata = data; }
		value execute(virtualmachine* vm, const value& left, const value& right) const override { return mfnc(vm, mdata, left, right); }
	};

	static std::shared_ptr<nularcmd> nular(std::string name, std::string description, nularcb fnc) { return std::make_shared<nularcmd>(name, description, fnc); }
	static std::shared_ptr<unarycmd> unary(std::string name, type rtype, std::string description, unarycb fnc) { return std::make_shared<unarycmd>(name, rtype, description, fnc); }
	static std::shared_ptr<binarycmd> binary(short precedence, std::string name, type ltype, type rtype, std::string description, binarycb fnc) { return std::make_shared<binarycmd>(precedence, name, ltype, rtype, description, fnc); }

	template<typename TData>
	static typename std::shared_ptr<nularcmddata<TData>> nulardata(TData data, std::string name, std::string description, typename nularcmddata<TData>::callback fnc)
	{
		return std::make_shared<nularcmddata<TData>>(data, name, description, fnc);
	}
	template<typename TData>
	static typename std::shared_ptr<unarycmddata<TData>> unarydata(TData data, std::string name, type rtype, std::string description, typename unarycmddata<TData>::callback fnc)
	{
		return std::make_shared<unarycmddata<TData>>(data, name, rtype, description, fnc);
	}
	template<typename TData>
	static typename std::shared_ptr<binarycmddata<TData>> binarydata(TData data, short precedence, std::string name, type ltype, type rtype, std::string description, typename  binarycmddata<TData>::callback fnc)
	{
		return std::make_shared<binarycmddata<TData>>(data, precedence, name, ltype, rtype, description, fnc);
	}
}
