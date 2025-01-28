#include "KC_Intersection.h"

#include <array>

namespace KC_Intersection
{
namespace Private
{
    static constexpr const size_t locAxisCount = 2;
} // namespace Private

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

bool MovingRectVsMovingRect(
    const KC_FloatRect& aRectA, const KC_FloatRect& aRectB,
    sf::Vector2f aVelocityA, sf::Vector2f aVelocityB,
    float& outContactTime
)
{
    if (RectVsRect(aRectA, aRectB))
    {
        outContactTime = 0.f;
        return true;
    }

    float firstContactTime = 0.f;
    float lastContactTime = 1.f;

    // Relative velocity respect A; effectively treating A as stationary.
    const sf::Vector2f relativeVelocity = aVelocityB - aVelocityA;
    const std::array<float, Private::locAxisCount> axisVelocity = { relativeVelocity.x, relativeVelocity.y };
    const std::array<float, Private::locAxisCount> minA = { aRectA.GetTopLeft().x, aRectA.GetTopLeft().y };
    const std::array<float, Private::locAxisCount> minB = { aRectB.GetTopLeft().x, aRectB.GetTopLeft().y };
    const std::array<float, Private::locAxisCount> maxA = { aRectA.GetBottomRight().x, aRectA.GetBottomRight().y };
    const std::array<float, Private::locAxisCount> maxB = { aRectB.GetBottomRight().x, aRectB.GetBottomRight().y };

    // For each axis
    for (size_t i = 0; i < Private::locAxisCount; ++i)
    {
        if (axisVelocity[i] < 0.f)
        {
            if (maxB[i] < minA[i])
                return 0;

            if (maxA[i] < minB[i])
            {
                firstContactTime = std::max((maxA[i] - minB[i]) / axisVelocity[i], firstContactTime);
            }
            if (minA[i] < maxB[i])
            {
                lastContactTime = std::min((minA[i] - maxB[i]) / axisVelocity[i], lastContactTime);
            }
        }
        else if (axisVelocity[i] > 0.f)
        {
            if (minB[i] > maxA[i])
                return 0;

            if (minA[i] > maxB[i])
            {
                firstContactTime = std::max((minA[i] - maxB[i]) / axisVelocity[i], firstContactTime);
            }
            if (maxA[i] > minB[i])
            {
                lastContactTime = std::min((maxA[i] - minB[i]) / axisVelocity[i], lastContactTime);
            }
        }

        if (firstContactTime > lastContactTime)
            return false;
    }

    outContactTime = firstContactTime;
    return true;
}
} // namespace KC_Intersection
