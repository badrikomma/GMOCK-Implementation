
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ChessPiece {
public:
    virtual ~ChessPiece() = default;
    virtual bool CanMoveTo(int x, int y) const = 0;
};

class Pawn : public ChessPiece {
public:
    bool CanMoveTo(int x, int y) const override {
        return x == 1 && y == 0;
    }
};

class Board {
public:
    virtual ~Board() = default;
    virtual bool IsEmpty(int x, int y) const = 0;
};

class ChessGame {
public:
    ChessGame(const ChessPiece& piece, const Board& board)
        : piece_(piece), board_(board) {}

    bool MovePiece(int x, int y) {
        if (!piece_.CanMoveTo(x, y))
            return false;
        return board_.IsEmpty(x, y);
    }

private:
    const ChessPiece& piece_;
    const Board& board_;
};

class MockChessPiece : public ChessPiece {
public:
    MOCK_CONST_METHOD2(CanMoveTo, bool(int x, int y));
};

class MockBoard : public Board {
public:
    MOCK_CONST_METHOD2(IsEmpty, bool(int x, int y));
};

TEST(ChessGameTest, MovePieceSuccess) {
    MockChessPiece piece;
    EXPECT_CALL(piece, CanMoveTo(1, 0)).WillOnce(testing::Return(true));

    MockBoard board;
    EXPECT_CALL(board, IsEmpty(1, 0)).WillOnce(testing::Return(true));

    ChessGame game(piece, board);
    EXPECT_TRUE(game.MovePiece(1, 0));
}

TEST(ChessGameTest, MovePieceFailure) {
    MockChessPiece piece;
    EXPECT_CALL(piece, CanMoveTo(2, 2)).WillOnce(testing::Return(false));

    MockBoard board;
    EXPECT_CALL(board, IsEmpty(testing::_, testing::_)).Times(0);

    ChessGame game(piece, board);
    EXPECT_FALSE(game.MovePiece(2, 2));
}
