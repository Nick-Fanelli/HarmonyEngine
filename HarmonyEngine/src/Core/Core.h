#pragma once

// Enable or Disable Debug
#ifndef NDEBUG
    #define HARMONY_DEBUG 1
#else
    #define HARMONY_DEBUG 0
#endif

// Define what platform we're on!
#include "Platform.h"

// Profiling Output
#define HARMONY_PROFILE HARMONY_DEBUG