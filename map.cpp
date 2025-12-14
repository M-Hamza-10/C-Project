
#include "map.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

map::map(int winWidth, int winHeight)
    : winWidth(winWidth), winHeight(winHeight),
      mapView(sf::FloatRect(0, 0, winWidth, winHeight))
{

    if (!texture.loadFromFile("Sprites/Map/Path_5.png"))
    {
        
        std::cout << "Failed to load texture!\n";
    }
    else
    {
        texture.setRepeated(true); // important if using many tiles
    }

    if (!background.loadFromFile("Sprites/Map/Background_3.png"))
    {
        std::cout << "Failed to load texture!\n";
    }
    else
    {
        back_G.setTexture(background);
    }

    // Make blank tiles
    for (int i = 0; i < NB_TILES; i++)
    {
        sf::ConvexShape tile;
        tile.setPointCount(4);
        tile.setFillColor(sf::Color(255, 255, 255, 80)); // semi-transparent
        tiles.push_back(tile);
    }

    float scaleX = static_cast<float>(winWidth) / back_G.getScale().x;
    float scaleY = static_cast<float>(winHeight) / back_G.getScale().y;

    back_G.setScale(0.5859375f, 0.5859375f);
    back_G.setPosition(0, -70);

    perspectiveX = (winWidth / 2.0f) - 110;
    perspectiveY = winHeight * 0.45f;

    std::cout << winWidth << " ";
    std::cout << winHeight;

    // Pre-build vertical lines
    for (int i = 0; i < V_NB_LINES; i++)
    {
        sf::VertexArray line(sf::Lines, 2);
        vLines.push_back(line); // push_back() adds a new item to the end of a container.
        // numbers.push_back(10); add 10 to numbers (must be a dynamic array)
    }

    // Pre-build horizontal lines
    for (int i = 0; i < H_NB_LINES; i++)
    { // sf::Lines gives two vertices of a line
        sf::VertexArray line(sf::Lines, 2);
        hLines.push_back(line);
    }


}

void map::generateGrid()
{

    quads.clear();

    for (int j = -1; j < H_NB_LINES - 1; j++)
    {
        float y1 = (j * H_SPACING * winHeight) - offsetY;
        float y2 = ((j + 1) * H_SPACING * winHeight) - offsetY;

        for (int i = 0; i < V_NB_LINES - 1; i++)
        {
            float rawX1 = (i - V_NB_LINES / 2) * (V_SPACING * winWidth) + offsetX;
            float rawX2 = ((i + 1) - V_NB_LINES / 2) * (V_SPACING * winWidth) + offsetX;

            float x1 = perspectiveX + rawX1;
            float x2 = perspectiveX + rawX2;

            addPerspectiveQuad(quads, x1, y1, x2, y2, nullptr);

            // Apply perspective to 4 corners (correct!)
            sf::Vector2f p1 = transformPerspective(x1, y1);
            sf::Vector2f p2 = transformPerspective(x2, y1);
            sf::Vector2f p3 = transformPerspective(x2, y2);
            sf::Vector2f p4 = transformPerspective(x1, y2);

            const sf::Vector2f texture[4] =
                {
                    {0, 0},
                    {256, 0},
                    {256, 256},
                    {0, 256}};

            quads.append(sf::Vertex(p1, texture[0]));
            quads.append(sf::Vertex(p2, texture[1]));
            quads.append(sf::Vertex(p3, texture[2]));
            quads.append(sf::Vertex(p4, texture[3]));

            // Kivy-style fade
            float midY = (p1.y + p3.y) * 0.5f;
            float diff_y = perspectiveY - midY;
            float factor_y = diff_y / perspectiveY;
            factor_y = powf(factor_y, 4);

            // float fade = 1.f - factor_y;

            // Colour the path
            //  sf::Color  colour = sf::Color(150,150,150);

            // int last = quads.getVertexCount();
            // quads[last-4].color = colour;
            // quads[last-3].color = colour;
            // quads[last-2].color = colour;
            // quads[last-1].color = colour;
        }
    }
}

// Add illusional Lines
sf::Vector2f map::transformPerspective(float x, float y)
{
    // direct conversion of Kivy formula
    float lin_y = y * (perspectiveY / winHeight);
    if (lin_y > perspectiveY)
        lin_y = perspectiveY;

    float diff_x = x - perspectiveX;
    float diff_y = perspectiveY - lin_y;

    float factor_y = diff_y / perspectiveY;
    factor_y = powf(factor_y, 4.f);

    float tr_x = perspectiveX + diff_x * factor_y;
    float tr_y = perspectiveY + factor_y * (winHeight - perspectiveY);

    return sf::Vector2f(tr_x, tr_y);
}


// ADD 3-D Illusional Rectangles

void map::addPerspectiveQuad(
    sf::VertexArray &arr,
    float x1, float y1,
    float x2, float y2,
    const sf::Vector2f texture[4] // texture coordinates or nullptr
)
{

    sf::Vector2f p1 = transformPerspective(x1, y1);
    sf::Vector2f p2 = transformPerspective(x2, y1);
    sf::Vector2f p3 = transformPerspective(x2, y2);
    sf::Vector2f p4 = transformPerspective(x1, y2);

    if (texture)
    {
        arr.append(sf::Vertex(p1, texture[0]));
        arr.append(sf::Vertex(p2, texture[1]));
        arr.append(sf::Vertex(p3, texture[2]));
        arr.append(sf::Vertex(p4, texture[3]));
    }
    else
    {
        arr.append(sf::Vertex(p1));
        arr.append(sf::Vertex(p2));
        arr.append(sf::Vertex(p3));
        arr.append(sf::Vertex(p4));
    }
}

void map::update(float dt)
{

    // tile-based scroll Vertical Scrool (Forward MOtion)
    offsetY += (speedY * dt);
    float spacingY = (H_SPACING * winHeight);

    while (offsetY >= spacingY)
    {
        offsetY -= spacingY;
    }

    // BUILD VERTICAL LINES
    int leftIndex = -V_NB_LINES / 2;

    for (int i = 0; i < V_NB_LINES; i++)
    {
        float rawX = (leftIndex + i) * (V_SPACING * winWidth) + offsetX;

        float worldX = perspectiveX + rawX;

        sf::Vector2f farPoint = transformPerspective(worldX, winHeight); // top of screen
        sf::Vector2f nearPoint = transformPerspective(worldX, -50);      // bottom

        vLines[i][0].position = farPoint;
        vLines[i][1].position = nearPoint;

        vLines[i][0].color = sf::Color(0,0,0,0);
        vLines[i][1].color = sf::Color(0,0,0,0);
    }

    // BUILD HORIZONTAL LINES
    for (int i = 0; i < H_NB_LINES; i++)
    {
        float lineY = (i * H_SPACING * winHeight) - offsetY;

        float leftX = perspectiveX - V_SPACING * winWidth * (V_NB_LINES / 2);
        float rightX = (perspectiveX - 400) + V_SPACING * winWidth * (V_NB_LINES / 2);

        auto leftPoint = transformPerspective(leftX, lineY);
        auto rightPoint = transformPerspective(rightX, lineY);

        hLines[i][0].position = leftPoint;
        hLines[i][1].position = rightPoint;

        hLines[i][0].color = sf::Color(0,0,0,0); // Transparent lines
        hLines[i][1].color = sf::Color(0,0,0,0);
    }
    // Calling the generate frid function
    generateGrid();

}

void map::draw(sf::RenderWindow &window)
{
    window.draw(back_G);
    window.draw(quads, &texture);
    // draw horizontal lines
    for (auto &l : hLines) // for(variable name : 10) Range from valur to 10
        window.draw(l);

    // draw vertical lines
    for (auto &l : vLines) // address fasten the process of calling bcz it directly goes to address
        window.draw(l);

    
}
