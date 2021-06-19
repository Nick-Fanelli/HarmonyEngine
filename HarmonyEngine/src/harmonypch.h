#pragma once

#ifndef NDEBUG
    #define HARMONY_DEBUG 1
#else
    #define HARMONY_DEBUG 0
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <regex>
#include <memory>
#include <thread>
#include <utility>

#include <string>
#include <stack>
#include <array>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include "Debug/Instrumentor.h"
#include "Core/Log.h"
#include "Core/Utils.h"