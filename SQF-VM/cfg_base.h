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
typedef CONFIGNODE* PCONFIG;

PCMD CONFIG_TYPE(void);
PCONFIG config_create_node(const wchar_t* ident, int identlen, int initiallen);
PCONFIG config_create_node_value(const wchar_t* ident, int identlen, VALUE val);
PCONFIG config_create_inheriting_node(const wchar_t* ident, int identlen, const wchar_t* parent, int initiallen);
PCONFIG config_create_inheriting_node_value(const wchar_t* ident, int identlen, VALUE val, const wchar_t* parent);
void config_destroy_node(PCONFIG config);
//Clears the whole node and makes it ready for reuse either as valuenode or as classnode
//If classnode, will down the reference counter of all children by one and set their parent to 0 if it matches param 1
//param 1: node to clear 
void config_clear_node(PCONFIG config);
//Resizes the classnode to provided newsize. If valuenode provided, will call config_clear_node to make it a clean node first.
//If new size is smaller then children_top, overhead of nodes will be removed (refcount goes down and parent is set to 0 if matching)
//param 1: node to resize children of.
//param 2: new size of children (will use realloc without checks if enough space is available)
void config_resize_children(PCONFIG node, unsigned int newsize);

//Clones provided source node and returns a fresh node.
//Fresh node will start with empty reference counter!
//The value of a valuenode will not be copied.
//param 1: node to clone.
PCONFIG config_clone_node(const PCONFIG source);

//Merges the source classnode into the target classnode.
//If a child is already existing in target, it will be merged too.
//If source & target are valuenodes, method will call config_set_value(target, source->value.value);
//If either source or target is a valuenode, nothing will be done.
//param 1: classnode to merge into.
//param 2: classnode to merge from.
//
//Remarks: Each node will be a new clone in target using config_clone_node.
//         Source node & children are left untouched.
void config_merge(PCONFIG target, const PCONFIG source);

//Clears current node using config_clear_node and makes this node a valuenode.
//param 1: node to set the value onto.
//param 2: new value to set.
void config_set_value(PCONFIG config, VALUE val);
//Gets the value from a valuenode. If not a valuenode, 0 will be returned.
//param 1: the node to receive the value from.
const PVALUE config_get_value(PCONFIG config);

//Counts how many parent levels are on given config (not inheriting!)
//param 1: the node to count the parents from
unsigned int config_count_parents(PCONFIG config);
//Pushes a node to provided classnode. If param 1 is not a classnode, config_clear_node will be called.
//If the node to push already has a parent, new parent will be set but the node itself won't be removed from its old parent.
//param 1: classnode to push the sub-node to
//param 2: node to be pushed.
//
//Remarks: if param 2 comes from a different classnode, you have to manually remove it from there afterwards.
void config_push_node(PCONFIG config, PCONFIG node);

//Searches the config tree for given inheriting node.
//Returns 0 if no node was found.
//param 1: node to receive the inheriting node from.
CONFIGNODE* config_find_inheriting_node(PCONFIG config);
//Returns the SQF equivalent of configFile (static variable)
PCONFIG sqf_configFile(void);

#endif // !_CFG_BASE_H_
