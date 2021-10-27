#pragma once

#include "harmonypch.h"

#ifdef DEBUG

// Profiling Function
#include "../../../debug/Profile.h"


#define HARMONY_PROFILE_SCOPE(name) ::ScopeTimer timer##__LINE__(name);
#define HARMONY_PROFILE_FUNCTION() HARMONY_PROFILE_SCOPE();

#else

#define HARMONY_PROFILE_SCOPE(name);

#endif