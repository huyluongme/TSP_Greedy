#pragma once

#include "framework.h"

class cGreedy
{
public:
	cGreedy() {
		log_id = 0;
	}
	int log_id;
	void Greedy_TSP();
private:
	void init_forest(vector <int>& parent, int n);
	int find_root(vector<int> parent, int u);
	bool is_vertex_deg_3(sEdge edge_new);
	bool is_cycle(int from_root, int to_root);
	void update_forest(vector<int>& parent, int r1, int r2);
};

extern class cGreedy* Greedy;

