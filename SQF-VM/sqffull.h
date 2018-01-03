
#include <math.h>

#include "basetype.h"
#include "vector.h"
#include "wstring_op.h"
#include "wstring_map.h"
#include "textrange.h"
#include "tokenizer.h"


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

#include "cfg_base.h"
#include "cfg_parse.h"


#include "sqf_commands.h"


#if defined(__GNUC__) && defined(_WIN32)
#define swprintf snwprintf
#define vswprintf vsnwprintf
#endif // __GNUC__
