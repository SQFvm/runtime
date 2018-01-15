#ifndef _FULL
#define _FULL 1

#include <typeinfo>
#include <string>
#include <vector>
#include <memory>
#include <vector>
#include <array>
#include <stack>
#include <unordered_map>
#include <ostream>

extern "C" {
#include <wchar.h>
#include "wstring_op.h"
}

#include "varscope.h"
#include "type.h"
#include "data.h"
#include "convert.h"
#include "value.h"
#include "booldata.h"
#include "scalardata.h"
#include "stringdata.h"
#include "arraydata.h"
#include "namespace.h"
#include "instruction.h"
#include "callstack.h"
#include "vmstack.h"
#include "virtualmachine.h"
#include "cmd.h"
#include "instassignto.h"
#include "instassigntolocal.h"
#include "instcallbinary.h"
#include "instcallnullar.h"
#include "instcallunary.h"
#include "instendstatement.h"
#include "instgetvariable.h"
#include "instpush.h"

#endif // !_FULL