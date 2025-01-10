#pragma once

#include "KC_GameSystem.h"

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

class KC_Canvas;

template <typename T, typename... Args>
class KC_PaintSystem : public KC_GameSystem<KC_Canvas, T, Args...>
{
protected:
    static void DrawRectangle(KC_Canvas& aCanvas, sf::Vector2f aCenter, sf::Vector2f aSize, sf::Color aColor);

protected:
    using KC_GameSystem<KC_Canvas, T, Args...>::KC_System;
    using BaseClass = KC_PaintSystem; // Override default constructor
};

template <typename T, typename... Args>
void KC_PaintSystem<T, Args...>::DrawRectangle(KC_Canvas& aCanvas, sf::Vector2f aCenter, sf::Vector2f aSize, sf::Color aColor)
{
    std::vector<sf::Vertex>& vertexes = aCanvas.myVertexes;

    // TODO: Duplication of vertexes, is it necessary with SFML?
    const std::array<sf::Vertex, 6> rectangleVertexes =
    {{
        {{ aCenter.x - aSize.x / 2.f, aCenter.y - aSize.y / 2.f }, aColor}, // Top-left
        {{ aCenter.x - aSize.x / 2.f, aCenter.y + aSize.y / 2.f }, aColor}, // Bottom-left
        {{ aCenter.x + aSize.x / 2.f, aCenter.y + aSize.y / 2.f }, aColor}, // Bottom-right
        {{ aCenter.x - aSize.x / 2.f, aCenter.y - aSize.y / 2.f }, aColor}, // Top-left
        {{ aCenter.x + aSize.x / 2.f, aCenter.y - aSize.y / 2.f }, aColor}, // Top-right
        {{ aCenter.x + aSize.x / 2.f, aCenter.y + aSize.y / 2.f }, aColor}  // Bottom-right
    }};

    vertexes.insert(vertexes.end(), rectangleVertexes.begin(), rectangleVertexes.end());
}
