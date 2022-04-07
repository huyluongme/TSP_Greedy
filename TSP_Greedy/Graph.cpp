#include "Graph.h"

string convertToString(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

void cGraph::Init() {
	Vertex.clear();
	string data_processed = convertToString(Graph_Data, strlen(Graph_Data));
	size_t found = data_processed.find("\n");
	int i = 0;
	while (found != string::npos) {
		string tmp = data_processed.substr(0, found);
		data_processed.erase(0, found + 1);
		if (tmp != "") {
			sVertex tmp_v;
			tmp_v.name = i;
			size_t found_tmp = tmp.find(" ");
			string pos_x, pos_y;
			pos_x = tmp.substr(0, found_tmp);
			pos_y = tmp.substr(found_tmp + 1, tmp.size() - 1);
			if (found_tmp != string::npos && found_tmp < tmp.size() - 1) {
				tmp_v.Pos.x = stof(pos_x);
				tmp_v.Pos.y = stof(pos_y);
			}
			Vertex.push_back(tmp_v);
		}
		found = data_processed.find("\n");
		i++;
	}

	found = data_processed.find(" ");
	if (found < data_processed.size() - 1) {
		sVertex tmp_v;
		tmp_v.name = i;
		string pos_x, pos_y;
		pos_x = data_processed.substr(0, found);
		pos_y = data_processed.substr(found, data_processed.size() - 1);
		tmp_v.Pos.x = stof(pos_x);
		if (pos_y.back() != '-')
			tmp_v.Pos.y = stof(pos_y);
		Vertex.push_back(tmp_v);
	}

	if (Vertex.size() > 1) {
		Edge.clear();
		for (int i = 0; i < Vertex.size() - 1; i++) {
			for (int j = i + 1; j < Vertex.size(); j++) {
				sEdge tmp;
				tmp.from = Vertex.at(i).name;
				tmp.to = Vertex.at(j).name;
				tmp.from_pos = Vertex.at(i).Pos;
				tmp.to_pos = Vertex.at(j).Pos;
				tmp.distance = Math->get_distance_between_tow_points(Vertex.at(i).Pos, Vertex.at(j).Pos);
				Edge.push_back(tmp);
			}
		}
	}
}

void cGraph::DrawNode(ImDrawList* draw_list, ImVec2 centerOxy) {
	for (int n = 0; n < Vertex.size(); n++) {
		ImU32 Col = IM_COL32(255, 255, 255, 255);
		draw_list->AddCircleFilled(ImVec2((Vertex.at(n).Pos.x * scale + centerOxy.x), (-Vertex.at(n).Pos.y * scale + centerOxy.y)), 4, Col);
		string label;
		label.push_back(alphabet.at(n));
		stringstream stream_x, stream_y;
		stream_x << std::fixed << std::setprecision(3) << Vertex.at(n).Pos.x;
		label += " (" + stream_x.str() + ", ";

		stream_y << std::fixed << std::setprecision(3) << Vertex.at(n).Pos.y;
		label += stream_y.str() + ")";
		draw_list->AddText(ImVec2(Vertex.at(n).Pos.x * scale + centerOxy.x - ImGui::CalcTextSize(label.c_str()).x / 2,
			(-Vertex.at(n).Pos.y * scale + centerOxy.y) - ImGui::CalcTextSize(label.c_str()).y / 2 + 15), Col, label.c_str());
	}
}

void cGraph::DrawEdge(ImDrawList* draw_list, ImVec2 centerOxy) {
	for (int n = 0; n < Edge.size(); n++) {
		draw_list->AddLine(ImVec2((Edge.at(n).from_pos.x * scale + centerOxy.x), (-Edge.at(n).from_pos.y * scale + centerOxy.y)),
			ImVec2((Edge.at(n).to_pos.x * scale + centerOxy.x), (-Edge.at(n).to_pos.y * scale + centerOxy.y)), IM_COL32(255, 255, 255, 255));
	}
}

void cGraph::DrawCycle(ImDrawList* draw_list, ImVec2 centerOxy) {
	for (int n = 0; n < Greedy->log_id; n++) {
		if (id_draw_cycle.at(n) != -1) {
			draw_list->AddLine(ImVec2((Edge.at(id_draw_cycle.at(n)).from_pos.x * scale + centerOxy.x), (-Edge.at(id_draw_cycle.at(n)).from_pos.y * scale + centerOxy.y)),
				ImVec2((Edge.at(id_draw_cycle.at(n)).to_pos.x * scale + centerOxy.x), (-Edge.at(id_draw_cycle.at(n)).to_pos.y * scale + centerOxy.y)), IM_COL32(255, 153, 0, 255), 1.5f);
		}
	}
}