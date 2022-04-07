#pragma once

#include "framework.h"

class cGraph
{
public:
	
	cGraph() {
		scale = 30;
	}

	char Graph_Data[1024 * 16] = "";
	int scale;
	string alphabet = "abcdefghijklmnopqrstuvwxyz";

	void Init();
	void DrawNode(ImDrawList* draw_list, ImVec2 centerOxy);
	void DrawEdge(ImDrawList* draw_list, ImVec2 centerOxy);
	void DrawCycle(ImDrawList* draw_list, ImVec2 centerOxy);
};
extern class cGraph* Graph;

