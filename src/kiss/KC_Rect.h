#pragma once

#include "KC_Transform.h"

#include <SFML/System/Vector2.hpp>

#include <cstdint>

template <typename T>
class KC_Rect final
{
    template <typename G>
    friend KC_Rect<G> operator*(const KC_Transform&, const KC_Rect<G>&);

public:
    KC_Rect() = default;
    KC_Rect(sf::Vector2<T> aPosition, sf::Vector2<T> aSize);

    sf::Vector2<T> GetBottomLeft() const;
    sf::Vector2<T> GetBottomRight() const;
    sf::Vector2<T> GetCenter() const;
    sf::Vector2<T> GetPosition() const { return myPosition; }
    sf::Vector2<T> GetSize() const { return mySize; }
    sf::Vector2<T> GetTopLeft() const { return myPosition; }
    sf::Vector2<T> GetTopRight() const;
    bool IsPointInside(sf::Vector2<T> aPoint) const;

    void SetPosition(sf::Vector2<T> aPosition) { myPosition = aPosition; }
    void SetSize(sf::Vector2<T> aSize) { mySize = aSize; }

private:
    sf::Vector2<T> myPosition; // Rectangle's top-left corner position
    sf::Vector2<T> mySize; // Rectangle's size
};

template <typename T>
KC_Rect<T> operator*(const KC_Transform& aTransform);

using KC_IntRect = KC_Rect<int32_t>;
using KC_FloatRect = KC_Rect<float>;

template <typename T>
KC_Rect<T>::KC_Rect(sf::Vector2<T> aPosition, sf::Vector2<T> aSize)
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
bool KC_Rect<T>::IsPointInside(sf::Vector2<T> aPoint) const
{
    return (aPoint.x >= myPosition.x && aPoint.y >= myPosition.y)
        && (aPoint.x <= myPosition.x + mySize.x && aPoint.y <= myPosition.y + mySize.y);
}

template <typename T>
KC_Rect<T> operator*(const KC_Transform& aTransform, const KC_Rect<T>& aRect)
{
    return
    {
        aRect.myPosition + aTransform.myPosition, 
        { aRect.mySize.x * aTransform.myScale.x, aRect.mySize.y * aTransform.myScale.y }
    };
}
