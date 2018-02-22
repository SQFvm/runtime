#include "virtualmachine.h"
#include "astnode.h"
#include "helper.h"
#include "parsesqf.h"
#include "parseconfig.h"
#include "instruction.h"
#include "instassignto.h"
#include "instassigntolocal.h"
#include "instcallbinary.h"
#include "instcallnular.h"
#include "instcallunary.h"
#include "instendstatement.h"
#include "instgetvariable.h"
#include "instmakearray.h"
#include "instpush.h"
#include "vmstack.h"
#include "commandmap.h"
#include "cmd.h"
#include "configdata.h"
#include "value.h"
#include "sidedata.h"
#include "groupdata.h"
#include "scriptdata.h"

#include <iostream>
#include <cwctype>
#include <sstream>


sqf::virtualmachine::virtualmachine(unsigned long long maxinst)
{
	mout = &std::wcout;
	mwrn = &std::wcerr;
	merr = &std::wcerr;
	minstcount = 0;
	mmaxinst = maxinst;
	mmainstack = std::make_shared<vmstack>();
	mactivestack = mmainstack;
	merrflag = false;
	mwrnflag = false;
}
void sqf::virtualmachine::execute()
{
	while (mspawns.size() != 0 || !mmainstack->isempty())
	{
		mactivestack = mmainstack;
		performexecute();
		
		for (auto it : mspawns)
		{
			mactivestack = it->stack();
			performexecute(150);
		}
		mspawns.remove_if([](std::shared_ptr<scriptdata> it) { return it->hasfinished(); });
	}
}
void sqf::virtualmachine::performexecute(size_t exitAfter)
{
	std::shared_ptr<sqf::instruction> inst;
	while (exitAfter != 0 && (inst = mactivestack->popinst(this)).get())
	{
		minstcount++;
		exitAfter--;
		if (mmaxinst != 0 && mmaxinst == minstcount)
		{
			err() << L"MAX INST COUNT REACHED (" << mmaxinst << L")" << std::endl;
			break;
		}
		inst->execute(this);
		if (merrflag)
		{
			err() << inst->dbginf(L"RNT") << std::endl;
			//Only for non-scheduled (and thus the mainstack)
			if (mactivestack->isscheduled())
			{
				break;
			}
		}
		if (mwrnflag)
		{
			wrn() << inst->dbginf(L"WRN") << std::endl;
			mwrnflag = false;
		}
	}
}
std::wstring sqf::virtualmachine::dbgsegment(const wchar_t* full, size_t off, size_t length)
{
	auto sstream = std::wstringstream();
	size_t i = off < 15 ? 0 : off - 15;
	size_t len = 30 + length;
	if (i < 0)
	{
		len += i;
		i = 0;
	}
	for (size_t j = i; j < i + len; j++)
	{
		wchar_t wc = full[j];
		if (wc == L'\0' || wc == L'\n')
		{
			if (j < off)
			{
				i = j + 1;
			}
			else
			{
				len = j - i;
				break;
			}
		}
	}
	sstream << std::wstring(full + i, full + i + len) << std::endl
		<< std::wstring(off - i, L' ') << std::wstring(length, L'^') << std::endl;
	return sstream.str();
}
bool contains_nular(std::wstring ident)
{
	return sqf::commandmap::get().contains_n(ident);
}
bool contains_unary(std::wstring ident)
{
	return sqf::commandmap::get().contains_u(ident);
}
bool contains_binary(std::wstring ident)
{
	return sqf::commandmap::get().contains_b(ident);
}
short precedence(std::wstring s)
{
	auto srange = sqf::commandmap::get().getrange_b(s);
	if (!srange.get() || srange->empty())
	{
		return 0;
	}
	return srange->begin()->get()->precedence();
}

void navigate_sqf(const wchar_t* full, sqf::virtualmachine* vm, std::shared_ptr<sqf::callstack> stack, astnode node)
{
	switch (node.kind)
	{
	case sqf::parse::sqf::sqfasttypes::BINARYEXPRESSION:
	{
		navigate_sqf(full, vm, stack, node.children[0]);
		navigate_sqf(full, vm, stack, node.children[2]);
		auto inst = std::make_shared<sqf::inst::callbinary>(sqf::commandmap::get().getrange_b(node.children[1].content));
		inst->setdbginf(node.children[1].line, node.children[1].col, node.file, vm->dbgsegment(full, node.children[1].offset, node.children[1].length));
		stack->pushinst(inst);
	}
	break;
	case sqf::parse::sqf::sqfasttypes::UNARYEXPRESSION:
	{
		navigate_sqf(full, vm, stack, node.children[1]);
		auto inst = std::make_shared<sqf::inst::callunary>(sqf::commandmap::get().getrange_u(node.children[0].content));
		inst->setdbginf(node.children[0].line, node.children[0].col, node.file, vm->dbgsegment(full, node.children[0].offset, node.children[0].length));
		stack->pushinst(inst);
	}
	break;
	case sqf::parse::sqf::sqfasttypes::NULAROP:
	{
		auto inst = std::make_shared<sqf::inst::callnular>(sqf::commandmap::get().get(node.content));
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqf::parse::sqf::sqfasttypes::HEXNUMBER:
	{
		auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(std::stol(node.content, 0, 16)));
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqf::parse::sqf::sqfasttypes::NUMBER:
	{
		auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(std::stod(node.content)));
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqf::parse::sqf::sqfasttypes::STRING:
	{
		auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(node.content));
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqf::parse::sqf::sqfasttypes::CODE:
	{
		auto cs = std::make_shared<sqf::callstack>();
		for (auto subnode : node.children)
		{
			navigate_sqf(full, vm, cs, subnode);
		}
		auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(cs));
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqf::parse::sqf::sqfasttypes::ARRAY:
	{
		for (auto subnode : node.children)
		{
			navigate_sqf(full, vm, stack, subnode);
		}
		auto inst = std::make_shared<sqf::inst::makearray>(node.children.size());
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqf::parse::sqf::sqfasttypes::ASSIGNMENT:
	{
		navigate_sqf(full, vm, stack, node.children[1]);
		auto inst = std::make_shared<sqf::inst::assignto>(node.children[0].content);
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqf::parse::sqf::sqfasttypes::ASSIGNMENTLOCAL:
	{
		navigate_sqf(full, vm, stack, node.children[1]);
		auto inst = std::make_shared<sqf::inst::assigntolocal>(node.children[0].content);
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqf::parse::sqf::sqfasttypes::VARIABLE:
	{
		auto inst = std::make_shared<sqf::inst::getvariable>(node.content);
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	default:
	{
		for (size_t i = 0; i < node.children.size(); i++)
		{
			if (i != 0)
			{
				auto inst = std::make_shared<sqf::inst::endstatement>();
				inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
				stack->pushinst(inst);
			}
			auto subnode = node.children[i];
			navigate_sqf(full, vm, stack, subnode);
		}
	}
	}
}

void sqf::virtualmachine::parse_sqf(std::wstring code, std::wstringstream* sstream)
{
	auto h = sqf::parse::helper(merr, dbgsegment, contains_nular, contains_unary, contains_binary, precedence);
	bool errflag = false;
	auto node = sqf::parse::sqf::parse_sqf(code.c_str(), h, errflag);
	print_navigate_ast(sstream, node, sqf::parse::sqf::astkindname);
}

void sqf::virtualmachine::parse_sqf(std::shared_ptr<sqf::vmstack> vmstck, std::wstring code, std::shared_ptr<sqf::callstack> cs)
{
	if (!cs.get())
	{
		cs = std::make_shared<sqf::callstack>();
		vmstck->pushcallstack(cs);
	}
	auto h = sqf::parse::helper(merr, dbgsegment, contains_nular, contains_unary, contains_binary, precedence);
	bool errflag = false;
	auto node = sqf::parse::sqf::parse_sqf(code.c_str(), h, errflag);
#if defined(_DEBUG)
	static bool isinitial = true;
	if (isinitial)
	{
		isinitial = false;
		out() << L"-------------------------------" << std::endl;
		print_navigate_ast(mout, node, sqf::parse::sqf::astkindname);
		out() << L"-------------------------------" << std::endl;
	}
#endif

	if (!errflag)
	{
		navigate_sqf(code.c_str(), this, cs, node);
	}
}

void navigate_config(const wchar_t* full, sqf::virtualmachine* vm, std::shared_ptr<sqf::configdata> parent, astnode node)
{
	auto kind = (sqf::parse::config::configasttypes::configasttypes)node.kind;
	switch (kind)
	{
	case sqf::parse::config::configasttypes::CONFIGNODE:
	{
		std::shared_ptr<sqf::configdata> curnode;
		if (node.children.size() > 0 && node.children.front().kind == sqf::parse::config::configasttypes::CONFIGNODE_PARENTIDENT)
		{
			curnode = std::make_shared<sqf::configdata>(parent, node.content, node.children.front().content);
			for (size_t i = 1; i < node.children.size(); i++)
			{
				auto subnode = node.children[i];
				navigate_config(full, vm, curnode, subnode);
			}
		}
		else
		{
			curnode = std::make_shared<sqf::configdata>(parent, node.content);
			for (auto subnode : node.children)
			{
				navigate_config(full, vm, curnode, subnode);
			}
		}
		parent->push_back(std::make_shared<sqf::value>(curnode, sqf::type::CONFIG));
	} break;
	case sqf::parse::config::configasttypes::VALUENODE:
	{
		std::shared_ptr<sqf::configdata> curnode = std::make_shared<sqf::configdata>(parent, node.content);
		for (auto subnode : node.children)
		{
			navigate_config(full, vm, curnode, subnode);
		}
		parent->push_back(std::make_shared<sqf::value>(curnode, sqf::type::CONFIG));
	} break;
	case sqf::parse::config::configasttypes::STRING:
		parent->cfgvalue(std::make_shared<sqf::value>(node.content));
		break;
	case sqf::parse::config::configasttypes::NUMBER:
		parent->cfgvalue(std::make_shared<sqf::value>(std::stod(node.content)));
		break;
	case sqf::parse::config::configasttypes::HEXNUMBER:
		parent->cfgvalue(std::make_shared<sqf::value>(std::stol(node.content, 0, 16)));
		break;
	case sqf::parse::config::configasttypes::LOCALIZATION:
		parent->cfgvalue(std::make_shared<sqf::value>(node.content));
		break;
	case sqf::parse::config::configasttypes::ARRAY:
	{
		std::vector<std::shared_ptr<sqf::value>> values;
		for (auto subnode : node.children)
		{
			navigate_config(full, vm, parent, subnode);
			values.push_back(parent->cfgvalue());
		}
		parent->cfgvalue(std::make_shared<sqf::value>(values));
	} break;
	case sqf::parse::config::configasttypes::VALUE:
		break;
	default:
	{
		for (auto subnode : node.children)
		{
			navigate_config(full, vm, parent, subnode);
		}
	}
	}
}
void sqf::virtualmachine::parse_config(std::wstring code, std::shared_ptr<configdata> parent)
{
	auto h = sqf::parse::helper(merr, dbgsegment, contains_nular, contains_unary, contains_binary, precedence);
	bool errflag = false;
	auto node = sqf::parse::config::parse_config(code.c_str(), h, errflag);
#if defined(_DEBUG)
	static bool isinitial = true;
	if (isinitial)
	{
		isinitial = false;
		out() << L"-------------------------------" << std::endl;
		print_navigate_ast(mout, node, sqf::parse::config::astkindname);
		out() << L"-------------------------------" << std::endl;
	}
#endif

	if (!errflag)
	{
		navigate_config(code.c_str(), this, parent, node);
	}
}

size_t sqf::virtualmachine::push_obj(std::shared_ptr<sqf::innerobj> obj)
{
	if (mfreeobjids.size() != 0)
	{
		auto id = mfreeobjids.back();
		mfreeobjids.pop_back();
		mobjlist[id] = obj;
		return id;
	}
	else
	{
		auto id = mobjlist.size();
		mobjlist.push_back(obj);
		return id;
	}
}

std::shared_ptr<sqf::innerobj> sqf::virtualmachine::get_obj_netid(size_t netid)
{
	if (mobjlist.size() <= netid)
	{
		return std::shared_ptr<innerobj>();
	}
	return mobjlist[netid];
}

std::wstring sqf::virtualmachine::get_group_id(std::shared_ptr<sqf::sidedata> side)
{
	int sidenum = side->side();
	int id = mgroupidcounter[sidenum]++;
	auto sstream = std::wstringstream();
	sstream << side->tosqf() << L" ALPHA " << id;
	return sstream.str();
}

void sqf::virtualmachine::push_group(std::shared_ptr<sqf::groupdata> d)
{
	mgroups[d->side()->side()].push_back(d);
}
