#pragma once

// Disable security function warnings (MSVC)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// If is not MSVC build
#ifndef _WIN32
#define _GLIBCXX_USE_CXX11_ABI		0
#define _stricmp					strcasecmp
#define _strnicmp					strncasecmp
#define _strdup						strdup
#define _unlink						unlink
#define _vsnprintf					vsnprintf
#define _write						write
#define _close						close
#define _acces						access
#define _vsnwprintf					vswprintf
#endif

// System Includes
#include <string>
#include <array>
#include <map>
#include <vector>

// System Includes
#ifndef _WIN32
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#else
#include <direct.h>
#endif

// CSSDK
#include <extdll.h>
#include <eiface.h>

// MetaMod SDK
#include <meta_api.h> 

// ReHLDS Api
#include <rehlds_api.h>
#include <rehlds_interfaces.h>

// ReGameDLL Api
#include <regamedll_api.h>
#include <regamedll_const.h>

// Game Events for Match Stats Round Events
#include <GameEvent.h>

// Plugin Includes
#include "MetaMod.h"
#include "MetaDLL.h"
#include "MetaEngine.h"

// ReAPI & ReGameDLL
#include "ReAPI.h"
#include "ReGameDLL.h"

// Time Format Class
#include "TimeFormat.h"

// JSON
#include "include/json.hpp"

// Match Bot
#include "MatchMenu.h"
#include "MatchAdmin.h"
#include "MatchAdminMenu.h"
#include "MatchBot.h"
#include "MatchBugFix.h"
#include "MatchCaptain.h"
#include "MatchChangeMap.h"
#include "MatchCommand.h"
#include "MatchData.h"
#include "MatchLanguage.h"
#include "MatchLO3.h"
#include "MatchPause.h"
#include "MatchReady.h"
#include "MatchRestrictItem.h"
#include "MatchStats.h"
#include "MatchTimer.h"
#include "MatchTask.h"
#include "MatchVoteMap.h"
#include "MatchVoteMenu.h"
#include "MatchVoteOvertime.h"
#include "MatchVoteTeam.h"
#include "MatchVoteSwapTeam.h"
#include "MatchUtil.h"
#include "MatchWarmup.h"
