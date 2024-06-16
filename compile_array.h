//
// Created by zero on 14.06.2024.
//

#include <iostream>
#include <array>
#include <string>
#include <vector>

constexpr std::array<uint8_t, 4> kArr1{1, 2, 3, 4};
constexpr std::array<uint8_t, 4> kArr2{5, 6, 7, 8};
constexpr std::array<uint8_t, 4> kArr3{9, 10, 11, 12};
constexpr std::array<uint8_t, 0xFFFF> kArrBig{};

inline std::vector<uint8_t> const kVec1{1, 2, 3, 4};
inline std::vector<uint8_t> const kVec2{5, 6, 7, 8};
inline std::vector<uint8_t> const kVec3{9, 10, 11, 12};

namespace Concat {
    template<class Head, class... Tail>
    constexpr auto Array(Head head, Tail... tail) {
        if constexpr (sizeof...(tail)) {
            using type_t = typename Head::value_type;
            std::array<type_t, (... + tail.size()) + head.size()> result{};
            auto it{std::copy(head.cbegin(), head.cend(), result.begin())};
            ((it = std::copy(tail.cbegin(), tail.cend(), it)), ...);
            return result;
        } else {
            return head;
        }
    }

    template<class Head, class... Tail>
    constexpr auto Vector(Head head, Tail... tail) {
        if constexpr (sizeof...(tail)) {
            using type_t = typename Head::value_type;
            std::vector<type_t> result{head.cbegin(), head.cend()};
            (result.insert(result.end(), tail.cbegin(), tail.cend()), ...);
            return result;
        } else {
            return head;
        }
    }
}

namespace ToSig {
    template<class Head, class... Arrs>
    constexpr auto Array(Head head, Arrs... arrs) {
        if constexpr (sizeof ...(arrs)) {
            size_t const len{(... + arrs.size()) + head.size()};
            constexpr std::array<typename Head::value_type, 2> arr{(len <= 255) ? (len >> 8) : len, (len & 0xFF)};
            return Concat::Array(arr, head, arrs...);
        } else {
            size_t const len{head.size()};
            constexpr std::array<typename Head::value_type, 2> arr{
                static_cast<unsigned char>((len <= 255) ? (len >> 8) : len), (len & 0xFF)
            };
            return Concat::Array(arr, head);
        }
    }

    template<class Head, class... Tail>
    constexpr auto Vector(Head head, Tail... tail) {
        using type_t = typename Head::value_type;
        if constexpr (sizeof ...(Tail)) {
            size_t const len{(... + tail.size()) + head.size()};
            std::vector<type_t> const vec{static_cast<type_t>((len <= 255) ? (len >> 8) : len), (len & 0xFF)};
            return Concat::Vector(vec, head, tail...);
        } else {
            size_t const len{head.size()};
            std::vector<type_t> const vec{
                static_cast<type_t>((len <= 255) ? (len >> 8) : len), (len & 0xFF)
            };
            return Concat::Vector(vec, head);
        }
    }
}

template<class Head, class... Tail>
auto ToVectors(Head head, Tail... tail) {
    using type_t = std::vector<typename Head::value_type>;
    if constexpr (sizeof...(tail)) {
        std::vector<type_t> result{type_t{head.cbegin(), head.cend()}};
        (result.push_back(type_t{tail.cbegin(), tail.cend()}), ...);
        return result;
    } else {
        return std::vector<type_t>{head.cbegin(), head.cend()};
    }
}

constexpr auto kSig1{ToSig::Array(kArr1)};
constexpr auto kSig2{ToSig::Array(kArrBig)};
constexpr auto kSig3{ToSig::Array(kArr1, kArr2, kArr3)};

static_assert(kSig1[0] == 0, "error");
static_assert(kSig1[1] == 4, "error");
static_assert(kSig1.size() == 6, "error");

static_assert(kSig2[0] == 0xFF, "error");
static_assert(kSig2[1] == 0xFF, "error");
static_assert(kSig2.size() == 0x10001, "error");

static_assert(kSig3[0] == 0, "error");
static_assert(kSig3[1] == 12, "error");
static_assert(kSig3[13] == 12, "error");
static_assert(kSig3.size() == 14, "error");
