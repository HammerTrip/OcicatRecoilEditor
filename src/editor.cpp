#include "editor.h"
#include "pattern.h"
#include "platform.h"

#include <fstream>
#include <json.hpp>

static sf::Font FONT;

static Pattern kPattern;

static bool bMoveSelectedPoint = false;

static bool bRequestedOpenPopup = false;
static sf::Vector2f popupMousePosition { 0, 0 };

static bool bRequestPatternClean = false;
static bool bRequestPatternLoad = false;

static size_t selectedPointIndex = 0;
static size_t pointIdCounter = 0;

static sf::RectangleShape bgRect;
static sf::RectangleShape bgLineHorizontal;
static sf::RectangleShape bgLineVertical;

static sf::Vector2f mousePosition;

const sf::Vector2f crossCenter { 200, 300 };
const sf::Vector2f crossSize { 400, 600 };
const float crossThickness = 2;

const float COORD_TO_SCR_MLP = 10.f;

const char* CONTEXT_MENU_ID = "ContextMenu";
const char* CLEAR_MENU_ID = "ClearPattern";

const char* IMGUI_TITLE = "The great Editor selfness.";

static float defaultTimeBonus = 0.3f;



sf::Vector2f get_cross_center () {
	return crossCenter;
}

sf::Vector2f vec_to_canvas (sf::Vector2f vec) {
	sf::Vector2f v = vec - crossCenter;
	v /= COORD_TO_SCR_MLP;

	return v;
}

sf::Vector2f mouse_to_canvas () {
	return vec_to_canvas(mousePosition);
}

void editor_clear_pattern () {
	kPattern = Pattern();
	selectedPointIndex = 0;
}

void editor_sort_points () {
	size_t saved_selected_index = 0;

	if (PatternPoint* p = get_selected_point(selectedPointIndex))
		saved_selected_index = p->id;
	
	std::sort(kPattern.points.begin(), kPattern.points.end(), [] (const PatternPoint& a, const PatternPoint& b) {
		if (a.time != 0 || b.time != 0)
			return a.time < b.time;
		else
			return a.id < b.id;
	});

	for (size_t i = 0; i < kPattern.points.size(); i++) {
		PatternPoint& p = kPattern.points[i];
		
		if (saved_selected_index == p.id) {
			selectedPointIndex = i;
			break;
		}
	}
}



static void select_point (size_t index) {
	if (index >= kPattern.points.size())
		index = 0;

	if (PatternPoint* p = get_selected_point(selectedPointIndex))
		p->set_selected(false);
	
	selectedPointIndex = index;

	if (PatternPoint* p = get_selected_point(selectedPointIndex))
		p->set_selected(true);
}

static void select_point_prev () {
	if (kPattern.points.size() <= 1)
		return;

	size_t index = selectedPointIndex;

	if (index == 0)
		index = kPattern.points.size() - 1;
	else
		index--;
	
	select_point(index);
}

static void select_point_next () {
	if (kPattern.points.size() <= 1)
		return;

	size_t index = selectedPointIndex;

	if (index == kPattern.points.size() - 1)
		index = 0;
	else
		index++;

	printf("selected %zu index %zu size %zu\n", selectedPointIndex, index, kPattern.points.size());
	
	select_point(index);
}

PatternPoint* get_selected_point (size_t index) {
	if (index >= kPattern.points.size())
		return nullptr;

	return &kPattern.points[index];
}

static PatternPoint* add_point () {
	PatternPoint newpoint;
	newpoint.id = pointIdCounter;

	if (kPattern.points.size() > 0)
		newpoint.time = kPattern.points[kPattern.points.size() - 1].time + defaultTimeBonus;

	pointIdCounter++;
	kPattern.points.push_back(newpoint);
	
	select_point(kPattern.points.size() - 1);

	editor_sort_points();

	return &kPattern.points[selectedPointIndex];
}

static void add_point_at_position (sf::Vector2f pos) {
	add_point();

	if (PatternPoint* p = get_selected_point(selectedPointIndex)) {
		auto v = pos;

		p->x = v.x;
		p->y = v.y;

		p->set_position(v * COORD_TO_SCR_MLP);
	}
}

static void remove_point () {
	if (kPattern.points.size() == 0)
		return;

	if (selectedPointIndex >= kPattern.points.size())
		return;

	kPattern.points.erase(kPattern.points.begin() + selectedPointIndex);
}

static void draw_imgui () {
	ImGui::SetNextWindowPos({ crossSize.x, 0 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ crossSize.x, crossSize.y }, ImGuiCond_FirstUseEver);

	uint32_t flags = 0;

	if (ImGui::Begin(IMGUI_TITLE, nullptr, flags)) {
		if (ImGui::Button("Clear pattern"))
			ImGui::OpenPopup(CLEAR_MENU_ID);
		
		if (ImGui::BeginPopup(CLEAR_MENU_ID)) {
			ImGui::Text("Are you sure?");

			ImGui::NewLine();

			if (ImGui::Selectable("Yes"))
				bRequestPatternClean = true;

			ImGui::NewLine();
			
			ImGui::Selectable("No");

			ImGui::EndPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Save")) {
			editor_save();
		}

		ImGui::SameLine();

		if (ImGui::Button("Load")) {
			bRequestPatternClean = true;
			bRequestPatternLoad = true;
		}
		
		ImGui::NewLine();

		ImGui::InputText("Pattern name", kPattern.name, Pattern::MAX_NAME);

		ImGui::NewLine();

		if (ImGui::Button("Add point"))
			add_point();
		
		ImGui::SameLine();

		if (ImGui::Button("<-##SELECT_PREV"))
			select_point_prev();
		
		ImGui::SameLine();

		if (ImGui::Button("->##SELECT_NEXT"))
			select_point_next();

		ImGui::NewLine();

		ImGui::Text("Selected point: %zu", selectedPointIndex);

		if (selectedPointIndex < kPattern.points.size()) {
			PatternPoint& p = kPattern.points[selectedPointIndex];

			float x = p.x;
			float y = p.y;

			float old_time = p.time;

			ImGui::SliderFloat("x", &x, -40.f, 40.f, "%.2f", 0);
			ImGui::SliderFloat("y", &y, -60.f, 60.f, "%.2f", 0);
			
			ImGui::SliderFloat("time", &p.time, 0, 15.f, "%.3f");
			ImGui::SameLine();
			ImGui::InputFloat("##time", &p.time, 0.05f, 0.15f, "%.3f");

			p.x = x;
			p.y = y;

			p.set_position({ x * COORD_TO_SCR_MLP, y * COORD_TO_SCR_MLP });

			if (p.time != old_time)
				editor_sort_points();
		}

		ImGui::NewLine();

		ImGui::SliderFloat("Default delta time", &defaultTimeBonus, 0.f, 2.f, "%.3f");

		if (bRequestedOpenPopup) {
			ImGui::OpenPopup(CONTEXT_MENU_ID);
			bRequestedOpenPopup = false;
		}

		if (ImGui::BeginPopup(CONTEXT_MENU_ID)) {
			if (ImGui::Selectable("Make point here"))
				add_point_at_position(vec_to_canvas(popupMousePosition));
			
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}

static void draw_arrow (PatternPoint& a, PatternPoint& b, sf::RenderWindow& wnd) {
	const auto va = a.circle.getPosition();
	const auto vb = b.circle.getPosition();

	const size_t VTX_COUNT = 2;

	static sf::VertexArray lines = sf::VertexArray(sf::LinesStrip, VTX_COUNT);
	lines[0].position = va;
	lines[1].position = vb;

	for (size_t i = 0; i < VTX_COUNT; i++)
		lines[i].color = sf::Color::Black;

	wnd.draw(lines);
}

static void draw_point_number (PatternPoint& p, size_t n, sf::RenderWindow& wnd) {
	sf::Text text;
	text.setFont(FONT);
	text.setString(std::to_string(n));

	auto pos = p.circle.getPosition();

	text.setPosition(pos + sf::Vector2f(8.f, 8.f));
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(16);

	wnd.draw(text);
}

static void editor_draw_pattern (sf::RenderWindow& wnd) {
	wnd.draw(bgRect);
	wnd.draw(bgLineHorizontal);
	wnd.draw(bgLineVertical);

	for (int i = 0; i < kPattern.points.size(); i++) {
		PatternPoint& p = kPattern.points[i];

		wnd.draw(p.circle);
	}

	for (int i = 0; i < kPattern.points.size(); i++) {
		PatternPoint& p = kPattern.points[i];

		if (i > 0) {
			draw_arrow(kPattern.points[i - 1], p, wnd);
		}

		draw_point_number(p, i, wnd);
	}
}

void ocicat::editor_init () {
	bgRect.setPosition({ 0, 0 });
	bgRect.setSize({ crossSize.x, crossSize.y });
	bgRect.setFillColor(sf::Color::White);

	bgLineHorizontal.setPosition({ 0, crossSize.y / 2.f });
	bgLineHorizontal.setSize({ crossSize.x, crossThickness });
	bgLineHorizontal.setFillColor(sf::Color::Black);

	bgLineVertical.setPosition({ crossSize.x / 2, 0 });
	bgLineVertical.setSize({ crossThickness, crossSize.y });
	bgLineVertical.setFillColor(sf::Color::Black);

	if (!FONT.loadFromFile("ProggyClean.ttf")) {
		printf("Can't load font\n");
	}
}

void ocicat::editor_draw (sf::RenderWindow& wnd) {
	draw_imgui();

	if (bRequestPatternClean) {
		bRequestPatternClean = false;
		editor_clear_pattern();
	}

	if (bRequestPatternLoad) {
		bRequestPatternLoad = false;
		editor_load();
	}

	if (bMoveSelectedPoint) {
		if (PatternPoint* p = get_selected_point(selectedPointIndex)) {
			sf::Vector2f pos = mouse_to_canvas();

			p->x = pos.x;
			p->y = pos.y;
			p->set_position(pos * COORD_TO_SCR_MLP);
		}
	}

	editor_draw_pattern(wnd);
}

void ocicat::evnt_left_pressed () {
	auto mouseVec = mouse_to_canvas();

	for (size_t i = 0; i < kPattern.points.size(); i++) {
		PatternPoint& p = kPattern.points[i];

		if (p.is_inside(mouseVec) && i != selectedPointIndex) {
			select_point(i);
			return;
		}
	}

	for (size_t i = 0; i < kPattern.points.size(); i++) {
		PatternPoint& p = kPattern.points[i];

		if (p.is_inside(mouseVec) && i == selectedPointIndex) {
			bMoveSelectedPoint = true;
			return;
		}
	}
}

void ocicat::evnt_left_released () {
	bMoveSelectedPoint = false;
}

void ocicat::evnt_mouse_position (sf::Vector2i pos) {
	mousePosition.x = pos.x;
	mousePosition.y = pos.y;
}

void evnt_right_pressed () {
	if (mousePosition.x >= crossSize.x || mousePosition.y >= crossSize.y)
		return;

	bRequestedOpenPopup = true;
	popupMousePosition = mousePosition;
}

void editor_save () {
	std::string filename = pf_get_save_file();

	printf("Save filename: %s\n", filename.c_str());

	if (filename.empty() || filename == "") {
		fprintf(stderr, "Empty filename for saving!\n");
		return;
	}

	using namespace nlohmann;

	json j;
	
	j["size"] = kPattern.points.size();
	j["name"] = kPattern.name;

	for (size_t i = 0; i < kPattern.points.size(); i++) {
		const std::string id = std::to_string(i);
		
		PatternPoint& p = kPattern.points[i];

		j[id]["x"] = p.x;
		j[id]["y"] = p.y;
		j[id]["time"] = p.time;
	}

	std::ofstream fs { filename };
	fs << std::setw(6) << j << std::endl;
}

void editor_load () {
	std::string filename = pf_get_open_file();

	printf("Load filename: %s\n", filename.c_str());

	if (filename.empty() || filename == "") {
		fprintf(stderr, "Empty filename to load!\n");
		return;
	}

	std::ifstream fs { filename };
	
	using namespace nlohmann;
	
	json j;
	fs >> j;

	size_t size = j["size"].template get<int>();
	auto name = j["name"].template get<std::string>();

	memcpy(kPattern.name, name.data(), name.size());

	for (size_t i = 0; i < size; i++) {
		std::string id = std::to_string(i);

		float x = j[id]["x"].template get<float>();
		float y = j[id]["y"].template get<float>();
		float time = j[id]["time"].template get<float>();

		PatternPoint pl;
		pl.x = x;
		pl.y = y;
		pl.time = time;

		PatternPoint* p = add_point();
		*p = pl;
	}

	editor_sort_points();

	for (auto& p : kPattern.points) {
		p.set_selected(false);
		p.set_position(sf::Vector2f(p.x, p.y) * COORD_TO_SCR_MLP);
	}
}