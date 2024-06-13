#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "chess.cpp"

// Класс для тестов, объявленный другом класса ChessGame
class ChessGameTest {
public:
    static void testInitialization() {
        ChessGame game;
        CHECK(game.getCurrentPlayer() == 'W');
        CHECK(game.getWhiteTimeLeft() == std::chrono::minutes(5));
        CHECK(game.getBlackTimeLeft() == std::chrono::minutes(5));
    }

    static void testValidMoves() {
        ChessGame game;
        CHECK(game.isValidMove("e2e4"));
        game.switchPlayer();
        CHECK(game.isValidMove("e7e5"));
    }
    static void testInvalidMoves() {
        ChessGame game;
        CHECK_FALSE(game.isValidMove("e2e5"));
        CHECK_FALSE(game.isValidMove("e9e5"));
        CHECK_FALSE(game.isValidMove("e2e9"));
    }

    static void testMovePieces() {
        ChessGame game;
        game.makeMove("e2e4");
        auto board = game.getBoard();
        CHECK(board[4][4] == 'P');
        CHECK(board[6][4] == ' ');
    }
};

TEST_CASE("Инициализация игры ChessGame") {
    ChessGameTest::testInitialization();
}

TEST_CASE("Корректные ходы") {
    ChessGameTest::testValidMoves();
}

TEST_CASE("Некорректные ходы") {
    ChessGameTest::testInvalidMoves();
}

TEST_CASE("Перемещение фигур") {
    ChessGameTest::testMovePieces();
}

    