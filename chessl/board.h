#ifndef CHESSL_BOARD_H
#define CHESSL_BOARD_H

#include <array>
#include <cmath>

namespace chess {

namespace literals {
static constexpr int BOARD_SIZE_X = 8;
static constexpr int BOARD_SIZE_Y = 8;
}

enum Player {
    WHITE = 0,
    BLACK,
    PL_NONE
};

enum PieceType {
    VOID = 0,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    PAWN
};

/* + for White, - for Black, 0 for neutral */
typedef int Piece;

/* Conversions */
[[nodiscard]] constexpr Player pieceToPlayer(Piece piece) noexcept {
    if (piece > 0) {
        return WHITE;
    }
    if (piece < 0) {
        return BLACK;
    }
    return PL_NONE;
}

[[nodiscard]] constexpr PieceType pieceToType(Piece piece) noexcept {
    return static_cast<PieceType>(std::abs(piece));
}

[[nodiscard]] constexpr int getPlayerSign(Player pl) noexcept {
    return (pl == WHITE ? 1 : -1);
}

// Gets the sign of a number, returns 0 if number is 0
[[nodiscard]] constexpr int numToSign(int num) noexcept {
    if (num > 0) {
        return 1;
    }
    if (num < 0) {
        return -1;
    }
    return 0;
}

[[nodiscard]] constexpr bool xInBounds(int x) noexcept {
    return x < literals::BOARD_SIZE_X && x >= 0;
}

[[nodiscard]] constexpr bool yInBounds(int y) noexcept {
    return y < literals::BOARD_SIZE_Y && y >= 0;
}

struct Coord {
    int x;
    int y;

    [[nodiscard]] constexpr int flat() const noexcept {
        return y * literals::BOARD_SIZE_X + x;
    };

    [[nodiscard]] constexpr bool operator==(const Coord &other) const noexcept {
        return x == other.x && y == other.y;
    };

    [[nodiscard]] constexpr bool operator!=(const Coord &other) const noexcept {
        return !operator==(other);
    };

    [[nodiscard]] constexpr bool withinBounds() const noexcept {
        return xInBounds(x) && yInBounds(y);
    }

    [[nodiscard]] constexpr Coord operator+(const Coord &other) const noexcept {
        return {x + other.x, y + other.y};
    };

    [[nodiscard]] constexpr Coord operator-(const Coord &other) const noexcept {
        return {x - other.x, y - other.y};
    }

    constexpr Coord &operator+=(const Coord &other) noexcept {
        x += other.x;
        y += other.y;
        return *this;
    };

    constexpr Coord &operator-=(const Coord &other) noexcept {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Coord &operator++() noexcept {
        if (x != literals::BOARD_SIZE_X - 1) {
            ++x;
        } else {
            x = 0;
            ++y;
        }
        return *this;
    }

    constexpr Coord &operator--() noexcept {
        if (x != 0) {
            --x;
        } else {
            x = 0;
            --y;
        }
        return *this;
    }

    [[nodiscard]] constexpr bool operator<(const Coord &other) const noexcept {
        return x < other.x && y < other.y;
    }

    [[nodiscard]] constexpr bool operator>(const Coord &other) const noexcept {
        return other.operator<(*this);
    }

    [[nodiscard]] constexpr bool operator<=(const Coord &other) const noexcept {
        return x <= other.x && y <= other.y;
    }

    [[nodiscard]] constexpr bool operator>=(const Coord &other) const noexcept {
        return other.operator<=(*this);
    }

    [[nodiscard]] constexpr int operator%(const Coord &other) const noexcept {
        return this->flat() % other.flat();
    }

    [[nodiscard]] constexpr Coord toSign() const noexcept {
        return {numToSign(x), numToSign(y)};
    };
};

struct Path {
    Coord origin;
    Coord target;

    [[nodiscard]] constexpr Coord distance() const noexcept {
        return target - origin;
    }
};

class Board {
public:
    typedef std::array<std::array<Piece, literals::BOARD_SIZE_X>, literals::BOARD_SIZE_Y> PieceArray;

protected:
    PieceArray pieces_;

public:
    constexpr explicit Board() noexcept = default;

    constexpr explicit Board(PieceArray pieces) noexcept : pieces_(pieces) {
    }

    [[nodiscard]] constexpr Piece operator[](const Coord &coord) const noexcept {
        return pieces_[coord.y][coord.x];
    };

    [[nodiscard]] constexpr Piece &operator[](const Coord &coord) noexcept {
        return pieces_[coord.y][coord.x];
    };
};

namespace literals {
constexpr Coord END_COORD{0, BOARD_SIZE_Y};
constexpr Coord INVALID_COORD = {BOARD_SIZE_X, BOARD_SIZE_Y};

constexpr Board DEFAULT_BOARD(Board::PieceArray{{
    {+ROOK, +KNIGHT, +BISHOP, +QUEEN, +KING, +BISHOP, +KNIGHT, +ROOK},
    {+PAWN, +PAWN, +PAWN, +PAWN, +PAWN, +PAWN, +PAWN, +PAWN},
    {VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID},
    {VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID},
    {VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID},
    {VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID},
    {-PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN},
    {-ROOK, -KNIGHT, -BISHOP, -QUEEN, -KING, -BISHOP, -KNIGHT, -ROOK}
}});
}

}

#endif//CHESSL_BOARD_H
