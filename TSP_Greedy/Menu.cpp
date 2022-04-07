#include "Menu.h"

void cMenu::DrawMenu(LPDIRECT3DDEVICE9 pDevice) {
	InitializeImGuiStartFrame(pDevice);
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	ImGui::Begin(u8"Lý thuyết đồ thị", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	canvas_p0.x = canvas_p0.x + 150.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x - 600.0f, canvas_p0.y + canvas_sz.y - 50.0f);

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
	const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

	ImVec2 centerOxy((canvas_p1.x - canvas_p0.x) / 2 + origin.x, canvas_p1.y / 2 + (origin.y - canvas_p0.y));

	draw_list->AddCircleFilled(centerOxy, 2, IM_COL32(255, 255, 255, 255));

	// Pan (we use a zero mouse threshold when there's no context menu)
	// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);

	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
	}
	Graph->DrawEdge(draw_list, centerOxy);
	
	Graph->DrawNode(draw_list, centerOxy);
	Graph->DrawCycle(draw_list, centerOxy);
	draw_list->PopClipRect();

	ImGui::SetCursorPos(ImVec2(canvas_p0.x - 150.0f, canvas_p0.y));
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Data:");
	ImGui::BeginChild("Data_input", ImVec2(canvas_p0.x - 14, canvas_sz.y - 23), true);
	if (ImGui::InputTextMultiline("##source", Graph->Graph_Data, IM_ARRAYSIZE(Graph->Graph_Data), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 35.3), ImGuiInputTextFlags_AllowTabInput))
		Graph->Init();
	ImGui::EndChild();

	ImGui::SetCursorPos(ImVec2(canvas_p1.x + 7, canvas_p0.y));
	ImGui::BeginChild("Function", ImVec2(canvas_sz.x - canvas_p1.x, canvas_sz.y), true);
	ImGui::PushFont(io.Fonts->Fonts[1]);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
	if (ImGui::Button("Greedy", ImVec2(canvas_sz.x - canvas_p1.x - 18, 40))) {
		Greedy->Greedy_TSP();
	}
	ImGui::PopFont();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Log:");
	ImGui::BeginChild("Log", ImVec2(canvas_sz.x - canvas_p1.x - 18, canvas_sz.y - 300), true);
	for (int i = 0; i < Log.size(); i++) {
		ImVec4 Col = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		if (i == Greedy->log_id - 1)
			Col = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		if (Log.at(i).mode == SORT) {
			ImGui::TextColored(Col,u8"[Bước 1]\n	- Sắp xếp các cạnh theo thứ tự độ dài tăng dần:");
			ImGui::TextColored(Col,"%s", Log.at(i).descri.c_str());
		}
		else {
			ImGui::TextColored(Col,u8"[Bước %d:]", i + 1);
			if (Log.at(i).selected)
				ImGui::TextColored(Col,u8"	- Chọn %s đưa vào phương án", Log.at(i).descri.c_str());
			else {
				ImGui::TextColored(Col,u8"	- Không chọn %s", Log.at(i).descri.c_str());
				if (Log.at(i).start)
					ImGui::TextColored(Col,u8"	- Vì chưa tạo thành chu trình Hamilton");
				else {
					if (Log.at(i).reason == BOTH)
						ImGui::TextColored(Col,u8"	- Vì tạo thành đỉnh cấp 3 và chu trình thiếu");
					else if (Log.at(i).reason == VERTEX_3)
						ImGui::TextColored(Col,u8"	- Vì tạo thành đỉnh cấp 3");
					else
						ImGui::TextColored(Col,u8"	- Vì tạo thành chu trình thiếu");
				}
			}
		}
		if (i == Log.size() - 1)
			ImGui::TextColored(Col,u8"	- Hoàn tất chu trình Hamilton");
		ImGui::Separator();
	}
	;
	ImGui::EndChild();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"Số bước: %d/%d", Greedy->log_id, Log.size());
	if (ImGui::Button(u8"Previous", ImVec2(209, 40)))
		if (Greedy->log_id > 0) Greedy->log_id--;
	ImGui::SameLine();
	if (ImGui::Button(u8"Next", ImVec2(209, 40)))
		if (Greedy->log_id < Log.size()) Greedy->log_id++;
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"Result:");
	ImGui::BeginChild("result:", ImVec2(canvas_sz.x - canvas_p1.x - 18, canvas_sz.y - canvas_p1.y + 74), true);
	float distance = 0;
	for (int i = 0; i < Result.size(); i++) {
		ImGui::Text("- %c%c: %.3f", Graph->alphabet[Edge.at(Result.at(i).id_edge).from], Graph->alphabet[Edge.at(Result.at(i).id_edge).to]
			, Edge.at(Result.at(i).id_edge).distance);
		distance += Edge.at(Result.at(i).id_edge).distance;
	}
	ImGui::Text(u8"Tổng độ dài: %.3f", distance);
	ImGui::EndChild();
	ImGui::PopStyleVar();
	
	ImGui::EndChild();

	ImGui::SetCursorPos(ImVec2(canvas_p0.x, canvas_p1.y + 5));
	ImGui::BeginChild("Scale", ImVec2(canvas_p1.x - canvas_p0.x, canvas_sz.y - canvas_p1.y + 4), true);
	//ImGui::PushFont(io.Fonts->Fonts[1]);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Scale:");
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 4.0f);
	ImGui::PushItemWidth(canvas_p1.x - canvas_p0.x - 70);
	ImGui::SliderInt("##scale", &Graph->scale, 0, 100);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::End();
	InitializeImGuiEndFrame(pDevice);
}

void cMenu::InitializeImGui(LPDIRECT3DDEVICE9 pDevice, HWND hwnd)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(pDevice);

	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 26.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 14.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesVietnamese());

	io.IniFilename = NULL;
}

void cMenu::InitializeImGuiStartFrame(LPDIRECT3DDEVICE9 pDevice)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void cMenu::InitializeImGuiEndFrame(LPDIRECT3DDEVICE9 pDevice)
{
	// Rendering
	ImGui::EndFrame();
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
	if (pDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		pDevice->EndScene();
	}
}