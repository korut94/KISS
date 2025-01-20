#include "KC_Intersection.h"

namespace KC_Intersection
{
bool RectVsRect(const KC_FloatRect& a, const KC_FloatRect& b)
{
    const sf::Vector2f bottomRightA = a.GetBottomRight();
    const sf::Vector2f bottomRightB = b.GetBottomRight();
    const sf::Vector2f topLeftA = a.GetTopLeft();
    const sf::Vector2f topLeftB = b.GetTopLeft();

    if (bottomRightA.x < topLeftB.x || bottomRightB.x < topLeftA.x)
        return false;
    
    if (bottomRightA.y < topLeftB.y || bottomRightB.y < topLeftA.y)
        return false;

    return true;
}
} // namespace KC_Intersection
