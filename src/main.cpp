#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <imconfig-SFML.h>
#include <imgui-SFML.h>

#include <imgui.h>

#include "editor.h"

int main (int argc, char* argv[]) {
	sf::RenderWindow window(sf::VideoMode(800, 600), "ImGui + SFML = <3");
	window.setFramerateLimit(144);
	if (!ImGui::SFML::Init(window)) return -1;

	editor_init();

	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event{};
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed) {
				window.close();
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