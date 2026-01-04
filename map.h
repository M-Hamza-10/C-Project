
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class map
{
private:
    sf::View mapView;
    // vertical & horizontal lines for perspective grid
    std::vector<sf::VertexArray> vLines;
    std::vector<sf::VertexArray> hLines;
    sf::VertexArray quads{sf::Quads};

    sf::Texture texture;
    sf::Texture background;
    sf::Sprite back_G;




public:
    // ONLY ONE constructor
    map(int winWidth, int winHeight);
    void generateGrid();

    // ONLY ONE update function
    void update(float dt);

      // transforms world -> screen using perspective
    sf::Vector2f transformPerspective(float x, float y);
    void addPerspectiveQuad(sf::VertexArray &arr,
                            float x1, float y1,
                            float x2, float y2,
                            const sf::Vector2f texture[4]);
        
    // perspective parameters
    float perspectiveX;
    float perspectiveY;

    float offsetX = 0.f;
    float offsetY = 0.f;
    float speedY = 100.0f;

    int winWidth;
    int winHeight;

    // number of lines and spacing
    const int V_NB_LINES = 16;       // number of vertical lines  16x 0.4 = 6.4f
    const float V_SPACING = 0.4f; // horizontal spacing between lines

    const int H_NB_LINES = 30;   // number of horizontal lines
    const float H_SPACING = 0.3f; // vertical spacing between lines
    const sf::Vector2f texture1[4] =
        {
            {0, 0},
            {256, 0},
            {256, 256},
            {0, 256}};

    // tile system (like Kivy Galaxy)
    int NB_TILES = 16;
    std::vector<std::pair<int, int>> tileCoords; // tileX, tileY
    std::vector<sf::ConvexShape> tiles;

    int currentYLoop = 0;
    float currentOffsetY = 0;

    // draw the entire map
    void draw(sf::RenderWindow &window);
};
