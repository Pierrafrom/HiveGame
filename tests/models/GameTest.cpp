/**
 * @file GameTest.cpp
 * @brief Unit tests for the Game class in the Hive game project.
 *
 * This file contains tests for validating the Singleton pattern,
 * game initialization, and other behaviors of the Game class.
 */

#include <gtest/gtest.h>
#include "models/Game.h"

namespace hive::models {
    /**
     * @class GameTest
     * @brief Test suite for the Game class.
     *
     * This class contains unit tests to validate the behavior of the Game class,
     * including Singleton implementation, game initialization, and deleted operations.
     */
    class GameTest : public testing::Test {
    protected:
        /**
         * @brief Optional setup before each test.
         */
        void SetUp() override {
            // Optional setup before each test
        }

        /**
         * @brief Optional cleanup after each test.
         */
        void TearDown() override {
            // Optional cleanup after each test
        }
    };

    /**
     * @test GameTest.SingletonInstanceIsSame
     * @brief Test that ensures the Singleton instance of the Game class is unique.
     *
     * This test validates that the `getInstance` method always returns the same instance.
     */
    TEST_F(GameTest, SingletonInstanceIsSame) {
        Game &instance1 = Game::getInstance();
        Game &instance2 = Game::getInstance();

        EXPECT_EQ(&instance1, &instance2) << "Singleton instances should be the same";
    }

    /**
     * @test GameTest.ResetGameInitializesCorrectly
     * @brief Test that ensures the game is correctly initialized after a reset.
     *
     * This test verifies that:
     * - The turn number is reset to 1.
     * - The first player is correctly set.
     * - The names of both players are initialized as expected.
     */
    TEST_F(GameTest, ResetGameInitializesCorrectly) {
        Game &game = Game::getInstance();

        // Reset the game with specific player names
        game.resetGame("Alice", "Bob");

        // Validate game state
        EXPECT_EQ(game.getTurnNumber(), 1) << "Turn number should be 1 after reset";
        EXPECT_EQ(game.getCurrentPlayer().getName(), "Alice") << "First player should be 'Alice'";
        EXPECT_EQ(game.getPlayer(0).getName(), "Alice") << "Player 0 should be 'Alice'";
        EXPECT_EQ(game.getPlayer(1).getName(), "Bob") << "Player 1 should be 'Bob'";
    }

    /**
     * @test GameTest.ResetGameClearsBoardAndStacks
     * @brief Test that ensures the board and undo/redo stacks are cleared after a reset.
     *
     * This test verifies that:
     * - The board is cleared and only contains the origin hex.
     * - Undo and redo stacks are empty, throwing exceptions when attempting to undo or redo.
     */
    TEST_F(GameTest, ResetGameClearsBoardAndStacks) {
        Game &game = Game::getInstance();

        // Perform some operations
        game.resetGame("Alice", "Bob");

        // Validate the board is cleared
        EXPECT_TRUE(game.getBoard().getBoard().size() == 1) << "Board should contain only the origin hex";

        // Ensure undo/redo stacks are empty
        EXPECT_THROW(game.undo(), std::runtime_error) << "Undo stack should be empty after reset";
        EXPECT_THROW(game.redo(), std::runtime_error) << "Redo stack should be empty after reset";
    }

    /**
     * @test GameTest.CopyConstructorDeleted
     * @brief Test that ensures the copy constructor is deleted.
     *
     * This test validates that the Game class cannot be copied.
     */
    TEST_F(GameTest, CopyConstructorDeleted) {
        static_assert(!std::is_copy_constructible_v<Game>,
                      "Game copy constructor should be deleted");
    }

    /**
     * @test GameTest.CopyAssignmentDeleted
     * @brief Test that ensures the copy assignment operator is deleted.
     *
     * This test validates that the Game class cannot be copy-assigned.
     */
    TEST_F(GameTest, CopyAssignmentDeleted) {
        static_assert(!std::is_copy_assignable_v<Game>,
                      "Game copy assignment operator should be deleted");
    }

    /**
     * @test GameTest.MoveConstructorDeleted
     * @brief Test that ensures the move constructor is deleted.
     *
     * This test validates that the Game class cannot be moved.
     */
    TEST_F(GameTest, MoveConstructorDeleted) {
        static_assert(!std::is_move_constructible_v<Game>,
                      "Game move constructor should be deleted");
    }

    /**
     * @test GameTest.MoveAssignmentDeleted
     * @brief Test that ensures the move assignment operator is deleted.
     *
     * This test validates that the Game class cannot be move-assigned.
     */
    TEST_F(GameTest, MoveAssignmentDeleted) {
        static_assert(!std::is_move_assignable_v<Game>,
                      "Game move assignment operator should be deleted");
    }
} // namespace hive::models
