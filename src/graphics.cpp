#include "../includes/graphics.hpp"

Graphics::Graphics() {
    window.create(sf::VideoMode(64 * pixelScale, 32 * pixelScale), "CHIPPER");
    shape.setRadius(100.0f);
    shape.setFillColor(sf::Color::Green);
}

void Graphics::drawFrameBuffer() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}