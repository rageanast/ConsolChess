 static void testPawnMove() {
        ChessGame game;
        CHECK(game.isValidPawnMove('P', 6, 4, 4, 4));
        CHECK(game.isValidPawnMove('P', 6, 4, 5, 4));
        CHECK_FALSE(game.isValidPawnMove('P', 6, 4, 3, 4));
    }

    static void testKingMove() {
        ChessGame game;
        CHECK(game.isValidKingMove('K', 7, 4, 6, 4));
        CHECK_FALSE(game.isValidKingMove('K', 7, 4, 5, 4));
    }

    static void testRookMove() {
        ChessGame game;
        game.makeMove("e2e4");
        CHECK(game.isValidRookMove(7, 0, 7, 1));
        CHECK_FALSE(game.isValidRookMove(7, 0, 5, 1));
    }

    static void testKnightMove() {
        ChessGame game;
        CHECK(game.isValidKnightMove(7, 1, 5, 2));
        CHECK_FALSE(game.isValidKnightMove(7, 1, 7, 3));
    }

    static void testBishopMove() {
        ChessGame game;
        game.makeMove("e2e4");
        CHECK(game.isValidBishopMove(5, 0, 4, 1));
        CHECK_FALSE(game.isValidBishopMove(5, 0, 5, 1));
    }

    static void testQueenMove() {
        ChessGame game;
        game.makeMove("d2d4");
        game.makeMove("c2c4");
        CHECK(game.isValidQueenMove(4, 0, 4, 2));
        CHECK(game.isValidQueenMove(4, 0, 3, 1));
        CHECK_FALSE(game.isValidQueenMove(4, 0, 5, 2));
    }

    static void testKingInCheckAfterMove() {
        ChessGame game;
        CHECK_FALSE(game.isKingInCheckAfterMove(6, 4, 4, 4));
    }
};

TEST_CASE("Правильность хода пешки") {
    ChessGameTest::testPawnMove();
}

TEST_CASE("Правильность хода короля") {
    ChessGameTest::testKingMove();
}

TEST_CASE("Правильность хода ладьи") {
    ChessGameTest::testRookMove();
}

TEST_CASE("Правильность хода коня") {
    ChessGameTest::testKnightMove();
}

TEST_CASE("Правильность хода слона") {
    ChessGameTest::testBishopMove();
}

TEST_CASE("Правильность хода ферзя") {
    ChessGameTest::testQueenMove();
}

TEST_CASE("Шах после хода") {
    ChessGameTest::testKingInCheckAfterMove();
}
