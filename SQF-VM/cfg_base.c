#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <stdbool.h>
#include <stdint.h>

#include "sqffull.h"


void TYPE_CONFIG_CALLBACK(void* input, CPCMD self)
{
	PVALUE val = input;
	PCONFIGNODE node = val->val.ptr;
	if (node == 0)
		return;
	if (val->type == 0)
	{
		node->refcount--;
		if (node->refcount <= 0)
		{
			config_destroy_node(node);
		}
	}
	else
	{
		node->refcount++;
	}
}

void uprefcount(PCONFIGNODE node)
{
	node->refcount++;
}

void downrefcount(PCONFIGNODE node)
{
	node->refcount--;
	if (node->refcount <= 0)
	{
		config_destroy_node(node);
	}
}

PCMD CONFIG_TYPE(void)
{
	static PCMD cmd = 0;
	if (cmd == 0)
	{
		cmd = create_command(L"CONFIG", 't', TYPE_CONFIG_CALLBACK, 0, 0, 0, 0);
	}
	return cmd;
}

PCONFIGNODE sqf_configFile(void)
{
	static PCONFIGNODE node = 0;
	if (node == 0)
	{
		node = config_create_node(L"bin\\config.bin", -1);
		node->refcount++;
	}
	return node;
}



PCONFIGNODE config_create_node(const wchar_t* ident, int identlen)
{
	PCONFIGNODE node = malloc(sizeof(CONFIGNODE));
	node->value.cfgnodes = malloc(sizeof(PCONFIGNODE) * 10);
	node->children_size = 10;
	node->children_top = 0;
	node->refcount = 0;
	node->identifier_length = identlen == -1 ? wcslen(ident) : identlen;
	if (identlen == 0)
	{
		node->identifier = 0;
	}
	else
	{
		node->identifier = malloc(sizeof(wchar_t) * (node->identifier_length + 1));
		wcsncpy(node->identifier, ident, node->identifier_length);
		node->identifier[node->identifier_length] = L'\0';
	}
	node->parent = 0;
	node->inheritingident = 0;
	node->inheritingident_length = 0;
	return node;
}
PCONFIGNODE config_create_node_value(const wchar_t* ident, int identlen, VALUE val)
{
	PCONFIGNODE node = malloc(sizeof(CONFIGNODE));
	node->children_size = 0;
	node->children_top = 0;
	node->identifier_length = identlen == -1 ? wcslen(ident) : identlen;
	if (identlen == 0)
	{
		node->identifier = 0;
	}
	else
	{
		node->identifier = malloc(sizeof(wchar_t) * (node->identifier_length + 1));
		wcsncpy(node->identifier, ident, node->identifier_length);
		node->identifier[node->identifier_length] = L'\0';
	}
	node->parent = 0;
	node->inheritingident = 0;
	node->refcount = 0;
	node->inheritingident_length = 0;
	node->value.value = value_create_noref(val.type, val.val);
	return node;
}
PCONFIGNODE config_create_inheriting_node(const wchar_t* ident, const wchar_t* parent)
{
	PCONFIGNODE node = config_create_node(ident, -1);
	node->inheritingident_length = wcslen(parent);
	node->inheritingident = malloc(sizeof(wchar_t) * (node->inheritingident_length + 1));
	wcscpy(node->inheritingident, ident);
	return node;
}
PCONFIGNODE config_create_inheriting_node_value(const wchar_t* ident, VALUE val, const wchar_t* parent)
{
	PCONFIGNODE node = config_create_node_value(ident, -1, val);
	node->inheritingident_length = wcslen(parent);
	node->inheritingident = malloc(sizeof(wchar_t) * (node->inheritingident_length + 1));
	wcscpy(node->inheritingident, ident);
	return node;
}
void config_destroy_node(PCONFIGNODE config)
{
	config_clear_node(config);
	free(config->identifier);
	free(config->inheritingident);
	free(config);
}
void config_clear_node(PCONFIGNODE config)
{
	int i;
	if (config->children_size != 0)
	{
		for (i = 0; i < config->children_top; i++)
		{
			if (config->value.cfgnodes[i]->parent == config)
			{
				config->value.cfgnodes[i]->parent = 0;
			}
			downrefcount(config->value.cfgnodes[i]);
		}
		free(config->value.cfgnodes);
		config->children_size = 0;
		config->children_top = 0;
		config->value.cfgnodes = 0;
	}
	else if (config->value.value != 0)
	{
		inst_destroy_value(config->value.value);
		config->value.value = 0;
	}
}

void config_set_value(PCONFIGNODE config, VALUE val)
{
	config_clear_node(config);
	config->value.value = value_create_noref(val.type, val.val);
}
const PVALUE config_get_value(PCONFIGNODE config)
{
	if (config->children_size != 0)
	{
		return 0;
	}
	else
	{
		return config->value.value;
	}
}

unsigned int config_count_parents(PCONFIGNODE config)
{
	unsigned int i = 0;
	while ((config = config->parent) != 0)
	{
		i++;
	}
	return i;
}

void config_push_node(PCONFIGNODE config, PCONFIGNODE node)
{
	if (config->children_size == 0)
	{
		config_clear_node(config);
		node->value.cfgnodes = malloc(sizeof(PCONFIGNODE) * 10);
		node->children_size = 10;
		node->children_top = 0;
	}
	if (node->children_top >= config->children_size)
	{
		config->children_size += 10;
		config->value.cfgnodes = realloc(config->value.cfgnodes, sizeof(PCONFIGNODE) * config->children_size);
	}
	config->value.cfgnodes[config->children_top] = node;
	config->children_top++;
	node->parent = config;
	uprefcount(node);
}

CONFIGNODE* config_find_inheriting_node(PCONFIGNODE config)
{
	PCONFIGNODE node = config;
	unsigned int i;
	while ((node = node->parent) != 0)
	{
		if (node->identifier != 0 && !wstr_cmpi(config->inheritingident, -1, node->identifier, -1))
		{
			return node;
		}
		for (i = 0; i < node->children_top; i++)
		{
			if (!wstr_cmpi(config->inheritingident, -1, node->value.cfgnodes[i]->identifier, -1))
			{
				return node->value.cfgnodes[i];
			}
		}
	}
	return 0;
}