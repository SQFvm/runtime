#include "callstack.h"
#include "namespaces.h"

sqf::callstack::callstack() : mwith(commands::namespaces::missionNamespace()) {}

sqf::callstack::callstack(std::shared_ptr<sqf::sqfnamespace> ns) : mwith(ns) {}
