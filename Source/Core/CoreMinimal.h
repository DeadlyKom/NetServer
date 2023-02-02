#pragma once

#define INDEX_NONE (-1)

#include <set>
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <functional>

#include <cstdint>
#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <Utils.h>
#include <Timer.h>
#include <Socket.h>
#include <Delegates.h>
