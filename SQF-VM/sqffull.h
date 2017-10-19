
#include <math.h>

#include "basetype.h"
#include "vector.h"
#include "wstring_op.h"
#include "wstring_map.h"
#include "textrange.h"


#include "sqf_base.h"
#include "sqf_inst.h"
#include "sqf.h"
#include "sqf_types.h"
#include "sqf_scalar_type.h"
#include "sqf_array_type.h"
#include "sqf_string_type.h"
#include "sqf_object_type.h"
#include "sqf_script_type.h"
#include "sqf_side_type.h"
#include "sqf_parse.h"
#include "sqf_commands.h"


#ifndef __GNUC__
#define snwprintf swprintf
#define vsnwprintf vswprintf
#endif // __GNUC__
