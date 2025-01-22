#pragma once

#include <tuple>
#include <type_traits>

namespace KC_TemplateHelper
{
template <typename T, typename Tuple, size_t Index = 0>
struct GetTupleTypeIndex;

// Caso base: se il tipo nella tupla corrisponde a T, restituisci l'indice
template <typename T, typename... Types, size_t Index>
struct GetTupleTypeIndex<T, std::tuple<Types...>, Index> : std::conditional<
    std::is_same<T, typename std::tuple_element<Index, std::tuple<Types...>>::type>::value,
    std::integral_constant<size_t, Index>, GetTupleTypeIndex<T, std::tuple<Types...>, Index + 1>
>::type {};

template <typename T, size_t Index>
struct GetTupleTypeIndex<T, std::tuple<>, Index> : std::integral_constant<size_t, static_cast<size_t>(-1)> {};

template <typename T>
struct Unpack {};

template <typename... Args>
using UnpackedTuple = Unpack<std::tuple<Args...>>;

template <typename T, typename... Args>
struct IsInVariadic;

template <typename T, typename... Args>
struct IsInVariadic<T, T, Args...> : std::true_type {};

template <typename T, typename U, typename... Args>
struct IsInVariadic<T, U, Args...> : IsInVariadic<T, Args...> {};
}
