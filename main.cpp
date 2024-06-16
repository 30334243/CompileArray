#include <iostream>
#include <gtest/gtest.h>
#include "compile_array.h"

TEST(Concat, Vector) {
    std::vector<uint8_t> const vec0{1, 2, 3, 4};
    std::vector<uint8_t> const vec1{1, 2, 3, 4, 1, 2, 3, 4};
    std::vector<uint8_t> const vec2{1, 2, 3, 4, 0, 4, 1, 2, 3, 4};
    std::vector<uint8_t> const vec3{1, 2, 3, 4, 0, 8, 1, 2, 3, 4, 5, 6, 7, 8};
    ASSERT_EQ(Concat::Vector(kVec1), vec0);
    ASSERT_EQ(Concat::Vector(kVec1,kArr1), vec1);
    ASSERT_EQ(Concat::Vector(kVec1,ToSig::Vector(kArr1)), vec2);
    ASSERT_EQ(Concat::Vector(kVec1,ToSig::Vector(kArr1, kVec2)), vec3);
}

TEST(Vectors, Vector) {
    std::vector<uint8_t> const vec1{0, 4, 1, 2, 3, 4};
    std::vector<uint8_t> const vec2{0, 8, 1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<uint8_t> const vec3{0, 12, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8};
    auto const vec{
        ToVectors(
            ToSig::Vector(kVec1)
            , ToSig::Vector(kArr1, kVec2)
            , ToSig::Vector(kVec3, kArr1, kVec2)
        )
    };
    ASSERT_EQ(vec[0], vec1);
    ASSERT_EQ(vec[1], vec2);
    ASSERT_EQ(vec[2], vec3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
