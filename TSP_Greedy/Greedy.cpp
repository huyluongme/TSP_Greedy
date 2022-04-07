#include "Greedy.h"

void cGreedy::init_forest(vector <int> &parent, int n) {
	for (int i = 0; i < n; i++)
		parent.at(i) = i;
}

int cGreedy::find_root(vector<int> parent, int u) {
	int tmp = u;
	while (tmp != parent[tmp]) tmp = parent[tmp];
	return tmp;
}

bool cGreedy::is_vertex_deg_3(sEdge edge_new) {
	int i = 0, count = 1;
	while (i < Result.size() && count < 3) {
		if (edge_new.from == Edge.at(Result.at(i).id_edge).from || edge_new.from == Edge.at(Result.at(i).id_edge).to)
			count++;
		i++;
	}
	if (count == 3) return true;
	i = 0; count = 1;
	while (i < Result.size()  && count < 3) {
		if (edge_new.to == Edge.at(Result.at(i).id_edge).from || edge_new.to == Edge.at(Result.at(i).id_edge).to)
			count++;
		i++;
	}
	return count == 3;
}

bool cGreedy::is_cycle(int from_root, int to_root) {
	return from_root == to_root;
}

void cGreedy::update_forest(vector<int> &parent, int r1, int r2) {
	parent.at(r2) = r1;
}

void cGreedy::Greedy_TSP() {
	Result.clear();
	Log.clear();
	id_draw_cycle.clear();

	sLog tmp;
	vector<int> parent;
	int n = Vertex.size();
	parent.resize(n);
	init_forest(parent, n);
	int from_root, to_root;
	int i, j;
	sResult tmp_res;

	sort(Edge.begin(), Edge.end(), [](sEdge a, sEdge b) {
		return a.distance < b.distance;
		});

	tmp.mode = SORT;
	tmp.descri.clear();
	for (int i = 0; i < Edge.size(); i++) {
		tmp.descri += "		+ ";
		tmp.descri.push_back(Graph->alphabet[Edge.at(i).from]);
		tmp.descri.push_back(Graph->alphabet[Edge.at(i).to]);
		stringstream stream;
		stream << fixed << setprecision(3) << Edge.at(i).distance;
		tmp.descri += ": "+ stream.str() + "\n";
	}
	tmp.start = false;
	id_draw_cycle.push_back(-1);
	Log.push_back(tmp);

	
	tmp_res.vertex_num = 0;
	for (i = 0; i < n * (n - 1) / 2; i++) {
		if (tmp_res.vertex_num == n -1) break;
		tmp.mode = SELECT;
		tmp.descri.clear();
		from_root = find_root(parent, Edge.at(i).from);
		to_root = find_root(parent, Edge.at(i).to);
		if (!is_vertex_deg_3(Edge.at(i)) && !is_cycle(from_root, to_root)) {
			tmp_res.vertex_num++;
			tmp_res.id_edge = i;
			Result.push_back(tmp_res);
			update_forest(parent, from_root, to_root);
			tmp.selected = true;
			tmp.descri.push_back(Graph->alphabet[Edge.at(i).from]);
			tmp.descri.push_back(Graph->alphabet[Edge.at(i).to]);
			id_draw_cycle.push_back(i);
		}
		else {
			tmp.selected = false;
			if (is_vertex_deg_3(Edge.at(i)) && is_cycle(from_root, to_root))
				tmp.reason = BOTH;
			else if (is_vertex_deg_3(Edge.at(i)) && !is_cycle(from_root, to_root))
				tmp.reason = VERTEX_3;
			else if (!is_vertex_deg_3(Edge.at(i)) && is_cycle(from_root, to_root))
				tmp.reason = CYCLE;
			tmp.descri.push_back(Graph->alphabet[Edge.at(i).from]);
			tmp.descri.push_back(Graph->alphabet[Edge.at(i).to]);
			id_draw_cycle.push_back(-1);
		}
		tmp.start = false;
		Log.push_back(tmp);
	}

	for (i = 0; i < n * (n - 1) / 2; i++) {
		tmp.mode = SELECT;
		tmp.descri.clear();
		from_root = find_root(parent, Edge.at(i).from);
		to_root = find_root(parent, Edge.at(i).to);
		if (!is_vertex_deg_3(Edge.at(i)) && is_cycle(from_root, to_root)) {
			tmp_res.vertex_num++;
			tmp_res.id_edge = i;
			Result.push_back(tmp_res);
			tmp.selected = true;
			tmp.descri.push_back(Graph->alphabet[Edge.at(i).from]);
			tmp.descri.push_back(Graph->alphabet[Edge.at(i).to]);
			tmp.start = true;
			id_draw_cycle.push_back(i);
			Log.push_back(tmp);
			break;
		}
		else {
			tmp.descri.push_back(Graph->alphabet[Edge.at(i).from]);
			tmp.descri.push_back(Graph->alphabet[Edge.at(i).to]);
			tmp.selected = false;
			tmp.reason = CYCLE;
			id_draw_cycle.push_back(-1);
		}
		tmp.start = true;
		Log.push_back(tmp);
	}
	log_id = Log.size();
}