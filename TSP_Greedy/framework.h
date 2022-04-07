// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <d3d9.h>

using namespace std;

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#pragma comment(lib, "d3d9.lib")

#include "Structs.h"
#include "TSP_Greedy.h"
#include "Menu.h"
#include "Graph.h"
#include "Math.h"
#include "Greedy.h"