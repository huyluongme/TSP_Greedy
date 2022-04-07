#pragma once

#define SORT   0
#define SELECT 1
#define VERTEX_3 0
#define CYCLE 1
#define BOTH 2

typedef struct {
	int name;
	ImVec2 Pos;
}sVertex;

typedef struct {
	int from, to;
	ImVec2 from_pos, to_pos;
	float distance;
}sEdge;

typedef struct {
	int mode;
	bool selected;
	int id_edge;
	int parent;
	int reason;
	bool start;
	string from, to, descri;
}sLog;

typedef struct {
	int vertex_num;
	int id_edge;
}sResult;

extern vector<sVertex> Vertex;
extern vector<sEdge> Edge;
extern vector<sLog> Log;
extern vector<sResult>Result;
extern vector<int> id_draw_cycle;