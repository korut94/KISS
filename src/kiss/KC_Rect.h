#pragma once

#include <cstdint>

#include <SFML/System/Vector2.hpp>

template <typename T>
class KC_Rect final
{
public:
    KC_Rect() = default;
    KC_Rect(const sf::Vector2<T>& aPosition, const sf::Vector2<T>& aSize);

    sf::Vector2<T> GetBottomLeft() const;
    sf::Vector2<T> GetBottomRight() const;
    sf::Vector2<T> GetCenter() const;
    sf::Vector2<T> GetPosition() const { return myPosition; }
    sf::Vector2<T> GetSize() const { return mySize; }
    sf::Vector2<T> GetTopLeft() const { return myPosition; }
    sf::Vector2<T> GetTopRight() const;
    bool IsPointInside(const sf::Vector2<T>& aPoint) const;


private:
    sf::Vector2<T> myPosition; // Rectangle's top-left corner position
    sf::Vector2<T> mySize; // Rectangle's size
};

using KC_IntRect = KC_Rect<std::int32_t>;
using KC_FloatRect = KC_Rect<float>;

template <typename T>
KC_Rect<T>::KC_Rect(const sf::Vector2<T>& aPosition, const sf::Vector2<T>& aSize)
    : myPosition(aPosition)
    , mySize(aSize)
{
}

template <typename T>
sf::Vector2<T> KC_Rect<T>::GetBottomLeft() const
{
    return { myPosition.x, myPosition.y + mySize.y };
}

template <typename T>
sf::Vector2<T> KC_Rect<T>::GetBottomRight() const
{
    return { myPosition.x + mySize.x, myPosition.y + mySize.y };
}

template <typename T>
sf::Vector2<T> KC_Rect<T>::GetCenter() const
{
    return { myPosition.x + mySize.x / T(2), myPosition.y + mySize.y / T(2) };
}

template <typename T>
inline sf::Vector2<T> KC_Rect<T>::GetTopRight() const
{
    return { myPosition.x + mySize.x, myPosition.y };
}

template <typename T>
bool KC_Rect<T>::IsPointInside(const sf::Vector2<T> &aPoint) const
{
    return 
        (aPoint.x >= myPosition.x && aPoint.y >= myPosition.y) &&
        (aPoint.x <= myPosition.x + mySize.x && aPoint.y <= myPosition.y + mySize.y);
}
