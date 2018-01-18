#include "full.h"

sqf::callstack::callstack() : mwith(commands::namespaces::missionNamespace()) {}

sqf::callstack::callstack(sqfnamespace& ns) : mwith(ns) {}
