#pragma once

#include "harmonypch.h"

#if HARMONY_DEBUG

    #define HARMONY_ASSERT(value) if(value) { Log::Error("Harmony Assertion!"); exit(-1); }
    #define HARMONY_ASSERT_MESSAGE(value, message) if(value) { Log::FormatError("Harmony Assertion!:\n\t%s", message); exit(-1); }

#else

    #define HARMONY_ASSERT(value)
    #define HARMONY_ASSERT_MESSAGE(value, message)

#endif