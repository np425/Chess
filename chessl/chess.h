// TODO: Rewrite in separate cpp file

#ifndef CHESSL_GAME_H
#define CHESSL_GAME_H

#include <map>
#include <string>
#include <utility>
#include <list>

#include "position.h"

namespace chess {

struct NotatedMove : public MoveInfo {
    std::string notation;
};

typedef std::map<std::string, std::string> TagsMap;
typedef std::pair<std::string, std::string> Tag;
typedef std::list<NotatedMove> MovesList;

class ChessGame : public Position {
protected:
    TagsMap m_tags;
    MovesList m_moves;
    Position m_initialPos;

public:
    ChessGame() noexcept = default;

    explicit ChessGame(const Position &initialPos) noexcept : Position(initialPos) {
    }

    ChessGame(TagsMap tags, MovesList moves, const Position &initialPos) noexcept : Position(initialPos),
        m_tags(std::move(tags)), m_moves(std::move(moves)), m_initialPos(initialPos) {
    }

    bool makeMove(NotatedMove &notatedMove) noexcept {
        if (!Position::makeMove(notatedMove)) {
            return false;
        }

        m_moves.push_back(notatedMove);
        return true;
    }

    bool makeMove(MoveInfo &moveInfo) noexcept override {
        if (!Position::makeMove(moveInfo)) {
            return false;
        }

        m_moves.push_back({moveInfo});
        return true;
    }

    bool buildPos() noexcept {
        for (auto &move: m_moves) {
            if (!Position::makeMove(move)) {
                return false;
            }
        }
        return true;
    }

    /* Getters */
    [[nodiscard]] constexpr const TagsMap &getTags() const noexcept {
        return m_tags;
    }

    [[nodiscard]] constexpr const MovesList &getMovesList() const noexcept {
        return m_moves;
    }
};

}

#endif //CHESSL_GAME_H
