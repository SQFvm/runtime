#include "callstack.h"
#include <utility>

sqf::callstack::callstack(std::shared_ptr<sqf::sqfnamespace> ns) : mwith(std::move(ns)) {}
