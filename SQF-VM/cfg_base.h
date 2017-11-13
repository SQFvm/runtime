#ifndef _CFG_BASE_H_
#define _CFG_BASE_H_

#ifndef _SQF_H_
#error "cfg_base.h" has to be included after "sqf.h" 
#endif // !_SQF_H_


typedef struct CONFIGNODE
{
	wchar_t* identifier;
	unsigned int identifier_length;
	struct CONFIGNODE** children;
	struct CONFIGNODE* parent;
	wchar_t* parentident;
	unsigned int parentident_length;
	unsigned int children_size;
	unsigned int children_top;
	PVALUE value;
}CONFIGNODE;
typedef CONFIGNODE* PCONFIGNODE;


PCONFIGNODE config_create_node(const wchar_t* ident);
PCONFIGNODE config_create_node_value(const wchar_t* ident, VALUE val);
PCONFIGNODE config_create_inheriting_node(const wchar_t* ident, const wchar_t* parent);
PCONFIGNODE config_create_inheriting_node_value(const wchar_t* ident, VALUE val, const wchar_t* parent);
inline PCONFIGNODE config_create_inheriting_node2(const wchar_t* ident, PCONFIGNODE parent) { config_create_node_with_parent(ident, parent->identifier); }
inline PCONFIGNODE config_create_inheriting_node_value2(const wchar_t* ident, VALUE val, PCONFIGNODE parent) { config_create_node_value_with_parent(ident, val, parent->identifier); }
void config_destroy_node(PCONFIGNODE config);
void config_clear_node(PCONFIGNODE config);

void config_set_value(PCONFIGNODE config, VALUE val);
VALUE config_get_value(PCONFIGNODE config);

void config_set_value_path(PCONFIGNODE config, const wchar_t* path, VALUE val);
VALUE config_get_value_path(PCONFIGNODE config, const wchar_t* path);

void config_push_node(PCONFIGNODE config, PCONFIGNODE node);
PCONFIGNODE config_pop_node(PCONFIGNODE config);

CONFIGNODE* config_find_parent(PCONFIGNODE config);

#endif // !_CFG_BASE_H_
