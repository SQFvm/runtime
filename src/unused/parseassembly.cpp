#include <cwctype>
#include <string_view>
#include "virtualmachine.h"
#include "compiletime.h"
#include "string_op.h"
#include "type.h"
#include "vmstack.h"
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
#include "convert.h"
#include "commandmap.h"
#include "stringdata.h"
#include "value.h"
#include "parseassembly.h"
#include "stringmanip.h"
#ifndef DISABLE_DEBUG_SEGMENT
#include "debugsegment.h"
#include <scalardata.h>
#endif // !DISABLE_DEBUG_SEGMENT




namespace err = logmessage::assembly;

void sqf::parse::assembly::skip(position_info& info)
{
	while (true)
	{
		switch (m_contents[info.offset])
		{
		case ' ': info.offset++; info.column++; continue;
		case '\t': info.offset++; info.column++; continue;
		case '\r': info.offset++; info.column++; continue;
		case '\n': info.offset++; info.line++; info.column = 0; continue;
		case '#':
			if ((m_contents[info.offset + 1] == 'l' || m_contents[info.offset + 1] == 'L') &&
				(m_contents[info.offset + 2] == 'i' || m_contents[info.offset + 1] == 'I') &&
				(m_contents[info.offset + 3] == 'n' || m_contents[info.offset + 1] == 'N') &&
				(m_contents[info.offset + 4] == 'e' || m_contents[info.offset + 1] == 'E'))
			{
				info.offset += 6;
				size_t start = info.offset;
				for (; m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n' && m_contents[info.offset] != ' '; info.offset++);
				auto str = std::string(m_contents_actual.substr(start, info.offset - start));
				info.line = static_cast<size_t>(std::stoul(str));

				for (; m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n' && m_contents[info.offset] == ' '; info.offset++);
				if (m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n')
				{
					start = info.offset;
					for (; m_contents[info.offset] != '\0' && m_contents[info.offset] != '\n'; info.offset++);
					auto str = std::string(m_contents_actual.substr(start, info.offset - start));
					info.file = strip_quotes(str);
				}
				break;
			}
		default: return;
		}
	}
}
//ident = [a-zA-Z]+;
//size_t ident(size_t off) { size_t i; for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z'); i++); return i - off; }
//command = [-+*/%a-zA-Z|&_><=\[\]]+;
size_t sqf::parse::assembly::command(size_t off) { size_t i; for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z') || m_contents[i] == '-' || m_contents[i] == '+' || m_contents[i] == '*' || m_contents[i] == '/' || m_contents[i] == '%' || m_contents[i] == '|' || m_contents[i] == '&' || m_contents[i] == '_' || m_contents[i] == '>' || m_contents[i] == '<' || m_contents[i] == '=' || m_contents[i] == '[' || m_contents[i] == ']'; i++) {}; return i - off; }
//anytext = (?![ \t\r\n;])+;
size_t sqf::parse::assembly::anytext(size_t off) { size_t i; for (i = off; m_contents[i] != ' ' && m_contents[i] != '\t' && m_contents[i] != '\r' && m_contents[i] != '\n' && m_contents[i] != ';'; i++) {}; return i - off; }
//type = [a-zA-Z]+;
size_t sqf::parse::assembly::type(size_t off) { size_t i; for (i = off; (m_contents[i] >= 'a' && m_contents[i] <= 'z') || (m_contents[i] >= 'A' && m_contents[i] <= 'Z'); i++) {}; return i - off; }
//integer = [0-9]+;
size_t sqf::parse::assembly::integer(size_t off) { size_t i; for (i = off; (m_contents[i] >= '0' && m_contents[i] <= '9'); i++) { /*EMPTY*/ } return i - off; }
//semicolon = ';';
size_t sqf::parse::assembly::semicolon(size_t off) { return m_contents[off] == ';' ? 1 : 0; }
//ASSEMBLY = { INSTRUCTIONS ';' { ';' } };
void sqf::parse::assembly::assembly_root(position_info& info)
{
	skip(info);
	//Iterate over instructions as long as it is an instruction start.
	while (instructions_start(info.offset))
	{
		instructions(info);
		skip(info);
		//Make sure at least one semicolon is available
		if (!semicolon(info.offset))
		{
			log(err::ExpectedSemicolon(info));
		}
		else
		{
			size_t semicolonlen;
			//Add semicolons up until no semicolon is left
			while ((semicolonlen = semicolon(info.offset)) > 0)
			{
				info.offset += semicolonlen;
				info.column += semicolonlen;
				skip(info);
			}
		}
	}
}
//INSTRUCTIONS = ENDSTATEMENT | ARG | PUSH;
bool sqf::parse::assembly::instructions_start(size_t off) { return endstatement_start(off) || arg_start(off) || push_start(off); }
void sqf::parse::assembly::instructions(position_info& info)
{
	if (endstatement_start(info.offset))
	{
		endstatement(info);
	}
	else if (arg_start(info.offset))
	{
		arg(info);
	}
	else if (push_start(info.offset))
	{
		push(info);
	}
	else
	{
		log(err::NoViableAlternativeInstructions(info));
	}
}
//ARG = CALLUNARY | CALLBINARY | ASSIGNTOLOCAL | ASSIGNTO | CALLNULAR | GETVARIABLE | MAKEARRAY
bool sqf::parse::assembly::arg_start(size_t off) { return callunary_start(off) || callbinary_start(off) || assignto_start(off) || assigntolocal_start(off) || callnular_start(off) || getvariable_start(off) || makearray_start(off); }
void sqf::parse::assembly::arg(position_info& info)
{
	if (callunary_start(info.offset))
	{
		callunary(info);
	}
	else if (callbinary_start(info.offset))
	{
		callbinary(info);
	}
	else if (assigntolocal_start(info.offset))
	{
		assigntolocal(info);
	}
	else if (assignto_start(info.offset))
	{
		assignto(info);
	}
	else if (callnular_start(info.offset))
	{
		callnular(info);
	}
	else if (getvariable_start(info.offset))
	{
		getvariable(info);
	}
	else if (makearray_start(info.offset))
	{
		makearray(info);
	}
	else
	{
		log(err::NoViableAlternativeArg(info));
	}
}
//ENDSTATEMENT = "endStatement"
bool sqf::parse::assembly::endstatement_start(size_t off) { return str_cmpi(m_contents + off, compiletime::strlen("endStatement"), "endStatement", compiletime::strlen("endStatement")) == 0; }
void sqf::parse::assembly::endstatement(position_info& info)
{
	if (endstatement_start(info.offset))
	{
		skip(info);
		auto inst = std::make_shared<sqf::inst::endstatement>();
#ifndef DISABLE_DEBUG_SEGMENT
		inst->setdbginf(info.line, info.column, std::string(), sqf::parse::dbgsegment(m_contents, info.offset, compiletime::strlen("endStatement")));
#endif // !DISABLE_DEBUG_SEGMENT

		m_vm->active_vmstack()->push_back(m_vm, inst);
		info.offset += compiletime::strlen("endStatement");
		info.column += compiletime::strlen("endStatement");
	}
	else
	{
		log(err::ExpectedEndStatement(info));
	}
}
//CALLUNARY = "callUnary" command
bool sqf::parse::assembly::callunary_start(size_t off) { return str_cmpi(m_contents + off, compiletime::strlen("callUnary"), "callUnary", compiletime::strlen("callUnary")) == 0; }
void sqf::parse::assembly::callunary(position_info& info)
{
	size_t identstart = info.offset;
	size_t identcol = info.column;
	size_t identline = info.line;
	if (callunary_start(info.offset))
	{
		info.offset += compiletime::strlen("callUnary");
		info.column += compiletime::strlen("callUnary");
		skip(info);
	}
	else
	{
		log(err::ExpectedCallUnary(info));
	}

	size_t cmdlen;
	if (!(cmdlen = command(info.offset)))
	{
		log(err::ExpectedUnaryOperator(info));
	}
	else
	{
		std::string cmdname = std::string(m_contents_actual.substr(info.offset, cmdlen));
		auto cmdrange = sqf::commandmap::get().getrange_u(cmdname);
		if (cmdrange->empty())
		{
			log(err::UnknownUnaryOperator(info, cmdname));
		}
		else
		{
			auto inst = std::make_shared<sqf::inst::callunary>(cmdrange);
#ifndef DISABLE_DEBUG_SEGMENT
			inst->setdbginf(identline, identcol, std::string(), sqf::parse::dbgsegment(m_contents, identstart, compiletime::strlen("callUnary")));
#endif // !DISABLE_DEBUG_SEGMENT
			m_vm->active_vmstack()->push_back(m_vm, inst);
		}
		info.offset += cmdlen;
		info.column += cmdlen;
		skip(info);
	}
}
//CALLBINARY = "callBinary" command
bool sqf::parse::assembly::callbinary_start(size_t off) { return str_cmpi(m_contents + off, compiletime::strlen("callBinary"), "callBinary", compiletime::strlen("callBinary")) == 0; }
void sqf::parse::assembly::callbinary(position_info& info)
{
	size_t identstart = info.offset;
	size_t identcol = info.column;
	size_t identline = info.line;
	if (callbinary_start(info.offset))
	{
		info.offset += compiletime::strlen("callBinary");
		info.column += compiletime::strlen("callBinary");
		skip(info);
	}
	else
	{
		log(err::ExpectedCallBinary(info));
	}

	size_t cmdlen;
	if (!(cmdlen = command(info.offset)))
	{
		log(err::ExpectedBinaryOperator(info));
	}
	else
	{
		std::string cmdname = std::string(m_contents_actual.substr(info.offset, cmdlen));
		auto cmdrange = sqf::commandmap::get().getrange_b(cmdname);
		if (cmdrange->empty())
		{
			log(err::UnknownBinaryOperator(info, cmdname));
		}
		else
		{
			auto inst = std::make_shared<sqf::inst::callbinary>(cmdrange);
#ifndef DISABLE_DEBUG_SEGMENT
			inst->setdbginf(identline, identcol, std::string(), sqf::parse::dbgsegment(m_contents, identstart, compiletime::strlen("callBinary")));
#endif // !DISABLE_DEBUG_SEGMENT
			m_vm->active_vmstack()->push_back(m_vm, inst);
		}
		info.offset += cmdlen;
		info.column += cmdlen;
		skip(info);
	}
}
//ASSIGNTO = "assignTo" anytext
bool sqf::parse::assembly::assignto_start(size_t off) { return str_cmpi(m_contents + off, compiletime::strlen("assignTo"), "assignTo", compiletime::strlen("assignTo")) == 0; }
void sqf::parse::assembly::assignto(position_info& info)
{
	size_t identstart = info.offset;
	size_t identcol = info.column;
	size_t identline = info.line;
	if (assignto_start(info.offset))
	{
		info.offset += compiletime::strlen("assignTo");
		info.column += compiletime::strlen("assignTo");
		skip(info);
	}
	else
	{
		log(err::ExpectedAssignTo(info));
	}

	size_t textlen;
	if (!(textlen = anytext(info.offset)))
	{
		log(err::ExpectedVariableName(info));
	}
	else
	{
		std::string variable_name = std::string(m_contents_actual.substr(info.offset, textlen));
		auto inst = std::make_shared<sqf::inst::assignto>(variable_name);
#ifndef DISABLE_DEBUG_SEGMENT
		inst->setdbginf(identline, identcol, std::string(),  sqf::parse::dbgsegment(m_contents, identstart, compiletime::strlen("assignTo")));
#endif // !DISABLE_DEBUG_SEGMENT
		m_vm->active_vmstack()->push_back(m_vm, inst);
		info.offset += textlen;
		info.column += textlen;
		skip(info);
	}
}
//ASSIGNTOLOCAL = "assignToLocal" anytext
bool sqf::parse::assembly::assigntolocal_start(size_t off) { return str_cmpi(m_contents + off, compiletime::strlen("assignToLoca"), "assignToLoca", compiletime::strlen("assignToLoca")) == 0; }
void sqf::parse::assembly::assigntolocal(position_info& info)
{
	size_t identstart = info.offset;
	size_t identcol = info.column;
	size_t identline = info.line;
	if (assigntolocal_start(info.offset))
	{
		info.offset += compiletime::strlen("assignToLocal");
		info.column += compiletime::strlen("assignToLocal");
		skip(info);
	}
	else
	{
		log(err::ExpectedAssignToLocal(info));
	}

	size_t textlen;
	if (!(textlen = anytext(info.offset)))
	{
		log(err::ExpectedVariableName(info));
	}
	else
	{
		std::string variable_name = std::string(m_contents_actual.substr(info.offset, textlen));
		auto inst = std::make_shared<sqf::inst::assigntolocal>(variable_name);
#ifndef DISABLE_DEBUG_SEGMENT
		inst->setdbginf(identline, identcol, std::string(), sqf::parse::dbgsegment(m_contents, identstart, compiletime::strlen("assignToLoca")));
#endif // !DISABLE_DEBUG_SEGMENT
		m_vm->active_vmstack()->push_back(m_vm, inst);

		info.offset += textlen;
		info.column += textlen;
		skip(info);
	}
}
//CALLNULAR = "callNular" command
bool sqf::parse::assembly::callnular_start(size_t off) { return str_cmpi(m_contents + off, compiletime::strlen("callNular"), "callNular", compiletime::strlen("callNular")) == 0; }
void sqf::parse::assembly::callnular(position_info& info)
{
	size_t identstart = info.offset;
	size_t identcol = info.column;
	size_t identline = info.line;
	if (callnular_start(info.offset))
	{
		info.offset += compiletime::strlen("callNular");
		info.column += compiletime::strlen("callNular");
		skip(info);
	}
	else
	{
		log(err::ExpectedCallNular(info));
	}

	size_t cmdlen;
	if (!(cmdlen = command(info.offset)))
	{
		log(err::ExpectedNularOperator(info));
	}
	else
	{
		std::string cmdname = std::string(m_contents_actual.substr(info.offset, cmdlen));
		auto cmd = sqf::commandmap::get().get(cmdname);
		if (!cmd.get())
		{
			log(err::UnknownNularOperator(info, cmdname));
		}
		else
		{
			auto inst = std::make_shared<sqf::inst::callnular>(cmd);
#ifndef DISABLE_DEBUG_SEGMENT
			inst->setdbginf(identline, identcol, std::string(),  sqf::parse::dbgsegment(m_contents, identstart, compiletime::strlen("callNular")));
#endif // !DISABLE_DEBUG_SEGMENT
			m_vm->active_vmstack()->push_back(m_vm, inst);
		}
		info.offset += cmdlen;
		info.column += cmdlen;
		skip(info);
	}
}
//GETVARIABLE = "getVariable" anytext
bool sqf::parse::assembly::getvariable_start(size_t off) { return str_cmpi(m_contents + off, compiletime::strlen("getVariable"), "getVariable", compiletime::strlen("getVariable")) == 0; }
void sqf::parse::assembly::getvariable(position_info& info)
{
	size_t identstart = info.offset;
	size_t identcol = info.column;
	size_t identline = info.line;
	if (getvariable_start(info.offset))
	{
		info.offset += compiletime::strlen("getVariable");
		info.column += compiletime::strlen("getVariable");
		skip(info);
	}
	else
	{
		log(err::ExpectedGetVariable(info));
	}

	size_t textlen;
	if (!(textlen = anytext(info.offset)))
	{
		log(err::ExpectedVariableName(info));
	}
	else
	{
		std::string variable_name = std::string(m_contents_actual.substr(info.offset, textlen));
		auto inst = std::make_shared<sqf::inst::getvariable>(variable_name);
#ifndef DISABLE_DEBUG_SEGMENT
		inst->setdbginf(identline, identcol, std::string(),  sqf::parse::dbgsegment(m_contents, identstart, compiletime::strlen("getVariable")));
#endif // !DISABLE_DEBUG_SEGMENT
		m_vm->active_vmstack()->push_back(m_vm, inst);
		info.offset += textlen;
		info.column += textlen;
		skip(info);
	}
}
//MAKEARRAY = "makeArray" anytext
bool sqf::parse::assembly::makearray_start(size_t off) { return str_cmpi(m_contents + off, compiletime::strlen("makeArray"), "makeArray", compiletime::strlen("makeArray")) == 0; }
void sqf::parse::assembly::makearray(position_info& info)
{
	size_t identstart = info.offset;
	size_t identcol = info.column;
	size_t identline = info.line;
	if (makearray_start(info.offset))
	{
		info.offset += compiletime::strlen("makeArray");
		info.column += compiletime::strlen("makeArray");
		skip(info);
	}
	else
	{
		log(err::ExpectedMakeArray(info));
	}

	size_t intlen;
	if (!(intlen = integer(info.offset)))
	{
		log(err::ExpectedInteger(info));
	}
	else
	{
		std::string integer_str = std::string(m_contents_actual.substr(info.offset, intlen));
		auto inst = std::make_shared<sqf::inst::makearray>(static_cast<size_t>(std::stoul(integer_str)));
#ifndef DISABLE_DEBUG_SEGMENT
		inst->setdbginf(identline, identcol, std::string(),  sqf::parse::dbgsegment(m_contents, identstart, compiletime::strlen("makeArray")));
#endif // !DISABLE_DEBUG_SEGMENT
		m_vm->active_vmstack()->push_back(m_vm, inst);
		info.offset += intlen;
		info.column += intlen;
		skip(info);
	}
}
//PUSH = "push" type { anytext }
bool sqf::parse::assembly::push_start(size_t off) { return str_cmpi(m_contents + off, compiletime::strlen("push"), "push", compiletime::strlen("push")) == 0; }
void sqf::parse::assembly::push(position_info& info)
{
	size_t identstart = info.offset;
	size_t identcol = info.column;
	size_t identline = info.line;
	auto pushtype = sqf::type::NA;
	if (push_start(info.offset))
	{
		info.offset += compiletime::strlen("push");
		info.column += compiletime::strlen("push");
		skip(info);
	}
	else
	{
		log(err::ExpectedPush(info));
	}

	size_t typelen;
	if (!(typelen = type(info.offset)))
	{
		log(err::ExpectedTypeName(info));
	}
	else
	{
		pushtype = sqf::parsetype(std::string(m_contents_actual.substr(info.offset, typelen)));
		info.offset += typelen;
		info.column += typelen;
		skip(info);
	}
	size_t textlen = 0;
	size_t curtextlen;
	while ((curtextlen = anytext(info.offset)) > 0)
	{
		textlen += curtextlen;
		info.offset += curtextlen;
		info.column += curtextlen;
		skip(info);
	}
	if (textlen > 0)
	{
		try
		{
			auto data = std::string(m_contents_actual.substr(info.offset - textlen, textlen));
			auto inst = std::make_shared<sqf::inst::push>(sqf::value(sqf::convert(std::make_shared<sqf::stringdata>(data, true), pushtype)));
#ifndef DISABLE_DEBUG_SEGMENT
			inst->setdbginf(identline, identcol, std::string(),  sqf::parse::dbgsegment(m_contents, identstart, compiletime::strlen("push")));
#endif // !DISABLE_DEBUG_SEGMENT
			m_vm->active_vmstack()->push_back(m_vm, inst);
		}
		catch (std::out_of_range&)
		{
			auto inst = std::make_shared<sqf::inst::push>(sqf::value(std::make_shared<sqf::scalardata>(std::nanf(""))));
#ifndef DISABLE_DEBUG_SEGMENT
			inst->setdbginf(identline, identcol, std::string(), sqf::parse::dbgsegment(m_contents, identstart, compiletime::strlen("push")));
#endif // !DISABLE_DEBUG_SEGMENT
			m_vm->active_vmstack()->push_back(m_vm, inst);
			log(err::NumberOutOfRange(info));
		}
	}
}	
void sqf::parse::assembly::parse()
{
	position_info info = { 1, 0, 0, m_file };
	assembly_root(info);
}