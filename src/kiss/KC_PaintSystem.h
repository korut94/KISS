#pragma once

#include "KC_Canvas.h"
#include "KC_CanvasRectanglePalette.h"
#include "KC_GameSystem.h"

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

template <typename T, typename... Args>
class KC_PaintSystem : public KC_GameSystem<KC_Canvas, T, Args...>
{
    using Super = KC_GameSystem<KC_Canvas, T, Args...>;
    KC_DERIVED_SYSTEM(KC_System, KC_PaintSystem)

protected:
    KC_Canvas& GetCanvas(KC_Entity anEntity);
    
    static void DrawRectangle(KC_Canvas& aCanvas, sf::Vector2f aCenter, sf::Vector2f aSize, const KC_CanvasRectanglePalette& aPalette);

private:
    static void FillBufferForRectangle(sf::Vector2f aCenter, sf::Vector2f aSize, sf::Color aColor, std::array<sf::Vertex, 6>& outSomeVertexes);
};

template <typename T, typename... Args>
KC_Canvas& KC_PaintSystem<T, Args...>::GetCanvas(KC_Entity anEntity)
{
    return this->template GetComponent<KC_Canvas>(anEntity);
}

template <typename T, typename... Args>
void KC_PaintSystem<T, Args...>::DrawRectangle(KC_Canvas& aCanvas, sf::Vector2f aCenter, sf::Vector2f aSize, const KC_CanvasRectanglePalette& aPalette)
{
    std::vector<sf::Vertex>& vertexes = aCanvas.myVertexes;

    std::array<sf::Vertex, 6> rectangleVertexes;
    
    FillBufferForRectangle(aCenter, aSize, aPalette.myFillColor, rectangleVertexes);
    vertexes.insert(vertexes.end(), rectangleVertexes.begin(), rectangleVertexes.end());

    if (aPalette.myOutlineThickness == 0.f)
        return;

    const float outlineThickness = aPalette.myOutlineThickness;

    // Left border
    FillBufferForRectangle
    (
        aCenter - sf::Vector2f(aSize.x / 2.f, 0.f),
        { outlineThickness, aSize.y + outlineThickness },
        aPalette.myOutlineColor,
        rectangleVertexes
    );
    vertexes.insert(vertexes.end(), rectangleVertexes.begin(), rectangleVertexes.end());
    // Top border
    FillBufferForRectangle
    (
        aCenter + sf::Vector2f(0.f, aSize.y / 2.f),
        { aSize.x - outlineThickness, outlineThickness },
        aPalette.myOutlineColor,
        rectangleVertexes
    );
    vertexes.insert(vertexes.end(), rectangleVertexes.begin(), rectangleVertexes.end());
    // Right border
    FillBufferForRectangle
    (
        aCenter + sf::Vector2f(aSize.x / 2.f, 0.f),
        { outlineThickness, aSize.y + outlineThickness },
        aPalette.myOutlineColor,
        rectangleVertexes
    );
    vertexes.insert(vertexes.end(), rectangleVertexes.begin(), rectangleVertexes.end());
    // Bottom border
    FillBufferForRectangle
    (
        aCenter - sf::Vector2f(0.f, aSize.y / 2.f),
        { aSize.x - outlineThickness, outlineThickness },
        aPalette.myOutlineColor,
        rectangleVertexes
    );
    vertexes.insert(vertexes.end(), rectangleVertexes.begin(), rectangleVertexes.end());
}

template <typename T, typename... Args>
void KC_PaintSystem<T, Args...>::FillBufferForRectangle(
    sf::Vector2f aCenter,
    sf::Vector2f aSize,
    sf::Color aColor,
    std::array<sf::Vertex, 6>& outSomeVertexes)
{
    outSomeVertexes = {{
        {{ aCenter.x - aSize.x / 2.f, aCenter.y - aSize.y / 2.f }, aColor}, // Top-left
        {{ aCenter.x - aSize.x / 2.f, aCenter.y + aSize.y / 2.f }, aColor}, // Bottom-left
        {{ aCenter.x + aSize.x / 2.f, aCenter.y + aSize.y / 2.f }, aColor}, // Bottom-right
        {{ aCenter.x - aSize.x / 2.f, aCenter.y - aSize.y / 2.f }, aColor}, // Top-left
        {{ aCenter.x + aSize.x / 2.f, aCenter.y - aSize.y / 2.f }, aColor}, // Top-right
        {{ aCenter.x + aSize.x / 2.f, aCenter.y + aSize.y / 2.f }, aColor}  // Bottom-right
    }};
}
