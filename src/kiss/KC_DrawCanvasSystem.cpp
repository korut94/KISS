#include "KC_DrawCanvasSystem.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace KC_DrawCanvasSystem_Private
{
    static std::vector<sf::Vertex> locVertexes;
}

void KC_DrawCanvasSystem::Run(sf::RenderWindow& aRenderWindow) const
{
    namespace Private = KC_DrawCanvasSystem_Private;

    Private::locVertexes.clear();

    for (KC_Entity entity : myEntitySet)
    {
        const KC_Canvas& canvas = GetComponent<KC_Canvas>(entity);
        Private::locVertexes.insert(Private::locVertexes.end(), canvas.myVertexes.cbegin(), canvas.myVertexes.cend());
    }

    aRenderWindow.draw(Private::locVertexes.data(), Private::locVertexes.size(), sf::PrimitiveType::Triangles);
}
