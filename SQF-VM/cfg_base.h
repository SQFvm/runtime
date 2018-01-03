#ifndef _CFG_BASE_H_
#define _CFG_BASE_H_

#ifndef _SQF_H_
#error "cfg_base.h" has to be included after "sqf.h" 
#endif // !_SQF_H_
struct CONFIGNODE;
typedef union CFGVAL
{
	PVALUE value;
	struct CONFIGNODE **cfgnodes;
} CFGVAL;
typedef struct CONFIGNODE
{
	struct CONFIGNODE* parent;
	wchar_t* identifier;
	unsigned int identifier_length;
	wchar_t* inheritingident;
	unsigned int inheritingident_length;
	unsigned int children_size;
	unsigned int children_top;
	unsigned int refcount;
	CFGVAL value;
}CONFIGNODE;
typedef CONFIGNODE* PCONFIGNODE;

PCMD CONFIG_TYPE(void);
PCONFIGNODE config_create_node(const wchar_t* ident, int identlen);
PCONFIGNODE config_create_node_value(const wchar_t* ident, int identlen, VALUE val);
PCONFIGNODE config_create_inheriting_node(const wchar_t* ident, const wchar_t* parent);
PCONFIGNODE config_create_inheriting_node_value(const wchar_t* ident, VALUE val, const wchar_t* parent);
void config_destroy_node(PCONFIGNODE config);
void config_clear_node(PCONFIGNODE config);

void config_set_value(PCONFIGNODE config, VALUE val);
const PVALUE config_get_value(PCONFIGNODE config);

unsigned int config_count_parents(PCONFIGNODE config);

void config_push_node(PCONFIGNODE config, PCONFIGNODE node);

CONFIGNODE* config_find_inheriting_node(PCONFIGNODE config);

PCONFIGNODE sqf_configFile(void);

#endif // !_CFG_BASE_H_
