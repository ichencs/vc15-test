#pragma once

#ifdef _DEBUG
#pragma comment(lib,"ntestboostd.lib") 
#pragma message("Automatically linking with ntestboostd debug lib")
#else
#pragma comment(lib,"ntestboost.lib") 
#pragma message("Automatically linking with ntestboost release lib")
#endif


#include "ntestDef.h"
#include "testboost.h"