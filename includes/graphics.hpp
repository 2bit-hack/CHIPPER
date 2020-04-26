#ifndef GFX_H
#define GFX_H

#include <SFML/Graphics.hpp>

class Graphics {
  public:
    const int pixelScale = 10;
    sf::RenderWindow window;

    // just for testing
    sf::CircleShape shape;

    Graphics();

    void drawFrameBuffer();
};

#endif