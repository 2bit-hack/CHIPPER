#include <chrono>
#include <thread>
#include <unordered_map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../includes/chip.hpp"

const int pixelScale = 10;
const int width = 64;
const int height = 32;

sf::SoundBuffer buffer;
sf::Sound beep;
std::unordered_map<Byte, sf::Keyboard::Key> mapKeys;

void mapKeysToKeyboard() {
    mapKeys[0x1] = sf::Keyboard::Num1;
    mapKeys[0x2] = sf::Keyboard::Num2;
    mapKeys[0x3] = sf::Keyboard::Num3;
    mapKeys[0xC] = sf::Keyboard::Num4;
    mapKeys[0x4] = sf::Keyboard::Q;
    mapKeys[0x5] = sf::Keyboard::W;
    mapKeys[0x6] = sf::Keyboard::E;
    mapKeys[0xD] = sf::Keyboard::R;
    mapKeys[0x7] = sf::Keyboard::A;
    mapKeys[0x8] = sf::Keyboard::S;
    mapKeys[0x9] = sf::Keyboard::D;
    mapKeys[0xE] = sf::Keyboard::F;
    mapKeys[0xA] = sf::Keyboard::Z;
    mapKeys[0x0] = sf::Keyboard::X;
    mapKeys[0xB] = sf::Keyboard::C;
    mapKeys[0xF] = sf::Keyboard::V;
}

void drawToScreen(sf::RenderWindow& target, Chip& chip) {
    sf::Vector2f pixelSize(pixelScale, pixelScale);
    sf::RectangleShape pixel(pixelSize);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (chip.m_frameBuffer[(i * width) + j])
                pixel.setFillColor(sf::Color::White);
            else
                pixel.setFillColor(sf::Color::Black);
            pixel.setPosition(j * pixelScale, i * pixelScale);
            target.draw(pixel);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "A ROM name is required to run CHIPPER\n";
        exit(ROM_LOAD_ERR);
    }

    if (!buffer.loadFromFile("./sounds/beep.wav"))
        std::cout << "Failed to load sound\n";
    else
        beep.setBuffer(buffer);

    mapKeysToKeyboard();

    std::string filepath = "./roms/" + std::string(argv[1]);

    Chip chip;
    bool loaded = chip.loadROM(filepath);
    if (!loaded)
        exit(ROM_LOAD_ERR);

    // chip.debug_dumpMem();

    sf::RenderWindow window(
        sf::VideoMode(width * pixelScale, height * pixelScale),
        "CHIPPER - " + std::string(argv[1]));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        chip.play();

        if (chip.m_drawFlag) {
            window.clear();
            drawToScreen(window, chip);
            chip.m_drawFlag = false;
            window.display();
        }

        if (chip.m_soundTimer)
            beep.play();

        chip.m_keys[0x0] = sf::Keyboard::isKeyPressed(mapKeys[0x0]);
        chip.m_keys[0x1] = sf::Keyboard::isKeyPressed(mapKeys[0x1]);
        chip.m_keys[0x2] = sf::Keyboard::isKeyPressed(mapKeys[0x2]);
        chip.m_keys[0x3] = sf::Keyboard::isKeyPressed(mapKeys[0x3]);
        chip.m_keys[0x4] = sf::Keyboard::isKeyPressed(mapKeys[0x4]);
        chip.m_keys[0x5] = sf::Keyboard::isKeyPressed(mapKeys[0x5]);
        chip.m_keys[0x6] = sf::Keyboard::isKeyPressed(mapKeys[0x6]);
        chip.m_keys[0x7] = sf::Keyboard::isKeyPressed(mapKeys[0x7]);
        chip.m_keys[0x8] = sf::Keyboard::isKeyPressed(mapKeys[0x8]);
        chip.m_keys[0x9] = sf::Keyboard::isKeyPressed(mapKeys[0x9]);
        chip.m_keys[0xA] = sf::Keyboard::isKeyPressed(mapKeys[0xA]);
        chip.m_keys[0xB] = sf::Keyboard::isKeyPressed(mapKeys[0xB]);
        chip.m_keys[0xC] = sf::Keyboard::isKeyPressed(mapKeys[0xC]);
        chip.m_keys[0xD] = sf::Keyboard::isKeyPressed(mapKeys[0xD]);
        chip.m_keys[0xE] = sf::Keyboard::isKeyPressed(mapKeys[0xE]);
        chip.m_keys[0xF] = sf::Keyboard::isKeyPressed(mapKeys[0xF]);

        // TODO: change this to actually push a frame every 16.67ms
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}