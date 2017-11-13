#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdbool.h>
#include <stdint.h>

#include "sqffull.h"

PCONFIGNODE config_create_node(const wchar_t* ident)
{
	PCONFIGNODE node = malloc(sizeof(CONFIGNODE));
	node->children = malloc(sizeof(PCONFIGNODE) * 10);
	node->children_size = 10;
	node->children_top = 0;
	node->identifier_length = wcslen(ident);
	node->identifier = malloc(sizeof(wchar_t) * (node->identifier_length + 1));
	wcscpy(node->identifier, ident);
	node->parent = 0;
	node->parentident = 0;
	node->parentident_length = 0;
	node->value = 0;
	return node;
}
PCONFIGNODE config_create_node_value(const wchar_t* ident, VALUE val)
{
	PCONFIGNODE node = malloc(sizeof(CONFIGNODE));
	node->children = 0;
	node->children_size = 0;
	node->children_top = 0;
	node->identifier_length = wcslen(ident);
	node->identifier = malloc(sizeof(wchar_t) * (node->identifier_length + 1));
	wcscpy(node->identifier, ident);
	node->parent = 0;
	node->parentident = 0;
	node->parentident_length = 0;
	node->value = value_create_noref(val.type, val.val);
	return node;
}
PCONFIGNODE config_create_inheriting_node(const wchar_t* ident, const wchar_t* parent)
{
	PCONFIGNODE node = config_create_node(ident);
	node->parentident_length = wcslen(parent);
	node->parentident = malloc(sizeof(wchar_t) * (node->parentident_length + 1));
	wcscpy(node->parentident, ident);
	return node;
}
PCONFIGNODE config_create_inheriting_node_value(const wchar_t* ident, VALUE val, const wchar_t* parent)
{
	PCONFIGNODE node = config_create_node_value(ident, val);
	node->parentident_length = wcslen(parent);
	node->parentident = malloc(sizeof(wchar_t) * (node->parentident_length + 1));
	wcscpy(node->parentident, ident);
	return node;
}
extern inline PCONFIGNODE config_create_inheriting_node2(const wchar_t* ident, PCONFIGNODE parent);
extern inline PCONFIGNODE config_create_inheriting_node_value2(const wchar_t* ident, VALUE val, PCONFIGNODE parent);

void config_destroy_node(PCONFIGNODE config)
{
	config_clear_node(config);
	free(config->identifier);
	free(config->parentident);
	free(config);
}
void config_clear_node(PCONFIGNODE config)
{
	int i;
	if (config->children != 0)
	{
		for (i = 0; i < config->children_top; i++)
		{
			config_destroy_node(config->children[i]);
		}
		free(config->children);
		config->children_size = 0;
		config->children_top = 0;
	}
	if (config->value != 0)
	{
		inst_destroy_value(config->value);
	}
}

void config_set_value(PCONFIGNODE config, VALUE val)
{

}
VALUE config_get_value(PCONFIGNODE config)
{

}

void config_set_value_path(PCONFIGNODE config, const wchar_t* path, VALUE val)
{

}
VALUE config_get_value_path(PCONFIGNODE config, const wchar_t* path)
{

}

void config_push_node(PCONFIGNODE config, PCONFIGNODE node)
{

}
PCONFIGNODE config_pop_node(PCONFIGNODE config)
{

}

CONFIGNODE* config_find_parent(PCONFIGNODE config)
{

}