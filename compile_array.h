//
// Created by zero on 14.06.2024.
//

#include <iostream>
#include <array>
#include <string>
#include <vector>

template<class Head, class... Arrs>
consteval auto ConcatArr(Head head, Arrs... arrs) {
    if constexpr (sizeof...(arrs)) {
        std::array<typename Head::value_type, (... + arrs.size()) + head.size()> result {};
        auto it { std::copy(head.cbegin(), head.cend(), result.begin()) };
        ((it = std::copy(arrs.cbegin(), arrs.cend(), it)), ...);
        return result;
    } else {
        return head;
    }
}

template<class Head, class... Arrs>
consteval auto ToSig(Head head, Arrs... arrs) {
    if constexpr (sizeof ...(arrs)) {
        size_t const len { (... + arrs.size()) + head.size() };
        constexpr std::array<typename Head::value_type, 2> arr { (len <= 255) ? (len >> 8) : len, (len & 0xFF) };
        return ConcatArr(arr, head, arrs...);
    } else {
        size_t const len { head.size() };
        constexpr std::array<typename Head::value_type, 2> arr {
                static_cast<unsigned char>((len <= 255) ? (len >> 8) : len), (len & 0xFF) };
        return ConcatArr(arr, head);
    }
}

template<class Head, class... Tail>
auto ToVector(Head head, Tail... tail) {
    using type_t = std::vector<typename Head::value_type>;
    if constexpr (sizeof...(tail)) {
        std::vector<type_t> result { type_t { head.cbegin(), head.cend() }};
        (result.push_back(type_t { tail.cbegin(), tail.cend() }), ...);
        return result;
    } else {
        return std::vector<type_t> { head.cbegin(), head.cend() };
    }
}

constexpr std::array<uint8_t, 4> kArr1 { 1, 2, 3, 4 };
constexpr std::array<uint8_t, 4> kArr2 { 5, 6, 7, 8 };
constexpr std::array<uint8_t, 4> kArr3 { 9, 10, 11, 12 };
constexpr std::array<uint8_t, 0xFFFF> kArrBig {};

constexpr auto kSig1 { ToSig(kArr1) };
constexpr auto kSig2 { ToSig(kArrBig) };
constexpr auto kSig3 { ToSig(kArr1, kArr2, kArr3) };

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

template<class... Arrs>
consteval auto Len(Arrs&& ... arrs) {
    static_assert(sizeof...(Arrs), "error");
    return (... + arrs.size());
}

static_assert(Len(kArr1) == 4, "error");
static_assert(Len(kArr1, kArr2, kArr3) == 12, "error");

template<class Elm, auto... Arrs>
struct Buffer : std::array<Elm, (... + Arrs.size())> {
    consteval explicit Buffer() : std::array<Elm, (... + Arrs.size())> { ConcatArr(Arrs...) } {
    }

    using buffer_t = int;
};

constexpr Buffer<uint8_t, kArr1> kBuffer1 {};
constexpr Buffer<uint8_t, kArr1, kArr2, kArr3> kBuffer2 {};
constexpr Buffer<uint8_t, kArrBig> kBufferBig {};
static_assert(kBuffer1.size() == 4, "error");
static_assert(kBuffer2.size() == 12, "error");
static_assert(kBuffer1[1] == 2, "error");
static_assert(kBuffer2[11] == 12, "error");

