#pragma once

#include "framework.h"

class cMath
{
public:
	float get_distance_between_tow_points(ImVec2 A_pos, ImVec2 B_pos);
};

extern class cMath* Math;