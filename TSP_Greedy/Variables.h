#pragma once

vector<sVertex> Vertex;
vector<sEdge> Edge;
vector<sLog> Log;
vector<sResult>Result;
vector<int> id_draw_cycle;

cTSP_Greedy* TSP_Greedy = new cTSP_Greedy();
cMenu* Menu = new cMenu();
cGraph* Graph = new cGraph();
cMath* Math = new cMath();
cGreedy* Greedy = new cGreedy();