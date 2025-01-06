#pragma once

template <typename T>
class KC_Manager
{
public:
    static T& GetManager();
};

template <typename T>
T& KC_Manager<T>::GetManager()
{
    static T instance;
    return instance;
}
