#include "full.h"

sqf::callstack::callstack() : mwith(commands::namespaces::missionNamespace()) {}

sqf::callstack::callstack(namespace_s ns) : mwith(ns) {}
