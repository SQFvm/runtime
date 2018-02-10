#include <iostream>
#include <cwctype>
#include <sstream>

#include "full.h"
#include "astnode.h"
#include "parsesqf.h"
#include "sqfasttypes.h"


sqf::virtualmachine::virtualmachine(unsigned long long maxinst)
{
	mout = &std::wcout;
	mwrn = &std::wcerr;
	merr = &std::wcerr;
	minstcount = 0;
	mmaxinst = maxinst;
	mstack = std::make_shared<vmstack>();
}

void sqf::virtualmachine::execute(void)
{
	instruction_s inst;
	while ((inst = mstack->popinst(this)).get())
	{
		minstcount++;
		if (mmaxinst != 0 && mmaxinst == minstcount)
		{
			err() << L"MAX INST COUNT REACHED (" << mmaxinst << L")" << std::endl;
			break;
		}
		inst->execute(this);
		if (merrflag)
		{
			err() << inst->dbginf(L"RNT") << std::endl;
			break;
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

void navigate(const wchar_t* full, const sqf::virtualmachine* vm, sqf::callstack_s stack, astnode node)
{
	switch (node.kind)
	{
	case sqfasttypes::BINARYEXPRESSION:
	{
		navigate(full, vm, stack, node.children[0]);
		navigate(full, vm, stack, node.children[2]);
		auto inst = std::make_shared<sqf::inst::callbinary>(sqf::commandmap::get().getrange_b(node.children[1].content));
		inst->setdbginf(node.children[1].line, node.children[1].col, node.file, vm->dbgsegment(full, node.children[1].offset, node.children[1].length));
		stack->pushinst(inst);
	}
	break;
	case sqfasttypes::UNARYEXPRESSION:
	{
		navigate(full, vm, stack, node.children[1]);
		auto inst = std::make_shared<sqf::inst::callunary>(sqf::commandmap::get().getrange_u(node.children[0].content));
		inst->setdbginf(node.children[0].line, node.children[0].col, node.file, vm->dbgsegment(full, node.children[0].offset, node.children[0].length));
		stack->pushinst(inst);
	}
	break;
	case sqfasttypes::NULAROP:
	{
		auto inst = std::make_shared<sqf::inst::callnular>(sqf::commandmap::get().get(node.content));
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqfasttypes::HEXNUMBER:
	{
		auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(std::stol(node.content, 0, 16)));
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqfasttypes::NUMBER:
	{
		auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(std::stod(node.content)));
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqfasttypes::STRING:
	{
		auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(node.content));
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqfasttypes::CODE:
	{
		auto cs = std::make_shared<sqf::callstack>();
		for each (auto subnode in node.children)
		{
			navigate(full, vm, cs, subnode);
		}
		auto inst = std::make_shared<sqf::inst::push>(std::make_shared<sqf::value>(cs));
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqfasttypes::ARRAY:
	{
		for each (auto subnode in node.children)
		{
			navigate(full, vm, stack, subnode);
		}
		auto inst = std::make_shared<sqf::inst::makearray>(node.children.size());
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqfasttypes::ASSIGNMENT:
	{
		navigate(full, vm, stack, node.children[1]);
		auto inst = std::make_shared<sqf::inst::assignto>(node.children[0].content);
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqfasttypes::ASSIGNMENTLOCAL:
	{
		navigate(full, vm, stack, node.children[1]);
		auto inst = std::make_shared<sqf::inst::assigntolocal>(node.children[0].content);
		inst->setdbginf(node.line, node.col, node.file, vm->dbgsegment(full, node.offset, node.length));
		stack->pushinst(inst);
	}
	break;
	case sqfasttypes::VARIABLE:
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
			navigate(full, vm, stack, subnode);
		}
	}
	}
}

void sqf::virtualmachine::parse_sqf(std::wstring code, callstack_s cs) const
{
	if (!cs.get())
	{
		cs = std::make_shared<sqf::callstack>();
		this->stack()->pushcallstack(cs);
	}
	auto h = sqf::parse::helper(merr, dbgsegment, contains_nular, contains_unary, contains_binary, precedence);
	bool errflag = false;
	auto node = sqf::parse::parse_sqf(code.c_str(), h, errflag);
#if defined(_DEBUG)
	out() << L"-------------------------------" << std::endl;
	print_navigate_ast(mout, node, sqf::parse::astkindname);
	out() << L"-------------------------------" << std::endl;
#endif

	if (!errflag)
	{
		navigate(code.c_str(), this, cs, node);
	}
}
