#include "inc.h"
#include "editor.h"

using namespace ocicat;

int main (int argc, char* argv[]) {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Ocicat Recoil Editor");
	window.setFramerateLimit(144);
	if (!ImGui::SFML::Init(window)) return -1;

	ImGuiIO& io = ImGui::GetIO();
    	
	editor_init();

	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event{};

		evnt_mouse_position(sf::Mouse::getPosition(window));

		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Button::Left)
					if (!io.WantCaptureMouse)
						evnt_left_pressed();

				if (event.mouseButton.button == sf::Mouse::Button::Right)
					evnt_right_pressed();
			}

			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Button::Left) {
					evnt_left_released();
				}
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		window.clear();
		editor_draw(window);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}