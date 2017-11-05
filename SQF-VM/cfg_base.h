#ifndef _CFG_BASE_H_
#define _CFG_BASE_H_

#ifndef _SQF_H_
#error "cfg_base.h" has to be included after "sqf.h" 
#endif // !_SQF_H_


typedef struct CONFIGNODE
{
	wchar_t* identifier;
	unsigned int identifier_length;
	(struct CONFIGNODE*)* children;
	(struct CONFIGNODE*) parent;
	wchar_t* parentident;
	unsigned int parentident_length;
	unsigned int children_size;
	unsigned int children_top;
	PVALUE value;
}CONFIGNODE;
typedef CONFIGNODE* PCONFIGNODE;


void config_set_value(PCONFIGNODE config, VALUE val);
VALUE config_get_value(PCONFIGNODE config);

void config_set_value_path(PCONFIGNODE config, const wchar_t* path, VALUE val);
VALUE config_get_value_path(PCONFIGNODE config, const wchar_t* path);

#endif // !_CFG_BASE_H_
