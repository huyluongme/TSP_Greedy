#include "Math.h"

float cMath::get_distance_between_tow_points(ImVec2 A_pos, ImVec2 B_pos) {
	return sqrt(pow(B_pos.x - A_pos.x, 2) + pow(B_pos.y - A_pos.y, 2) * 1.0);
}