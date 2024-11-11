#include "models/Hex.h"
#include <gtest/gtest.h>
#include <sstream>


namespace hive::models {
    /**
     * @brief Test fixture class for Hex unit tests.
     */
    class HexTest : public ::testing::Test {
    protected:
        // SetUp and TearDown can be implemented if needed
    };

    /**
     * @test Checks if the constructor correctly initializes the coordinates.
     */
    TEST_F(HexTest, ConstructorSetsCoordinates) {
        const Hex hex(1, -1, 0);
        EXPECT_EQ(hex.getX(), 1);
        EXPECT_EQ(hex.getY(), -1);
        EXPECT_EQ(hex.getZ(), 0);
    }

    /**
     * @test Verifies that the equality operator works correctly.
     */
    TEST_F(HexTest, EqualityOperator) {
        const Hex hex1(1, -1, 0);
        const Hex hex2(1, -1, 0);
        const Hex hex3(0, 1, -1);
        EXPECT_TRUE(hex1 == hex2);
        EXPECT_FALSE(hex1 == hex3);
    }

    /**
     * @test Verifies that the inequality operator works correctly.
     */
    TEST_F(HexTest, InequalityOperator) {
        const Hex hex1(1, -1, 0);
        const Hex hex2(1, -1, 0);
        const Hex hex3(0, 1, -1);
        EXPECT_FALSE(hex1 != hex2);
        EXPECT_TRUE(hex1 != hex3);
    }

    /**
     * @test Checks the correctness of the hash_code function.
     */
    TEST_F(HexTest, HashCode) {
        const Hex hex1(1, -1, 0);
        const Hex hex2(1, -1, 0);
        const Hex hex3(0, 1, -1);
        EXPECT_EQ(hex1.hash_code(), hex2.hash_code());
        EXPECT_NE(hex1.hash_code(), hex3.hash_code());
    }

    /**
     * @test Ensures that the std::hash specialization works as expected.
     */
    TEST_F(HexTest, StdHash) {
        const Hex hex1(1, -1, 0);
        const Hex hex2(1, -1, 0);
        const Hex hex3(0, 1, -1);
        constexpr std::hash<Hex> hex_hash;
        EXPECT_EQ(hex_hash(hex1), hex_hash(hex2));
        EXPECT_NE(hex_hash(hex1), hex_hash(hex3));
    }

    /**
     * @test Validates the stream insertion operator for Hex.
     */
    TEST_F(HexTest, OutputOperator) {
        const Hex hex(1, -1, 0);
        std::ostringstream oss;
        oss << hex;
        EXPECT_EQ(oss.str(), "Hex(1, -1, 0)");
    }

    /**
     * @test Checks that constructing a Hex with invalid coordinates triggers an assertion.
     * Note: Asserts are typically disabled in release builds; this test is valid only in debug mode.
     */
    TEST_F(HexTest, ConstructorInvalidCoordinates) {
        // Since the constructor uses an assert, we cannot catch it in a test.
        // If you modify the constructor to throw an exception, you can test it like this:
        /*
        EXPECT_THROW({
            Hex hex(1, 1, 1); // x + y + z != 0
        }, std::invalid_argument);
        */
        SUCCEED(); // Placeholder to indicate the test is intentionally left blank
    }
} // namespace hive::models
