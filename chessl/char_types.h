#ifndef CHESS_CHAR_TYPES_H
#define CHESS_CHAR_TYPES_H

namespace chess {

struct CharUtils {
    [[nodiscard]] static constexpr bool isLower(char chr) noexcept {
        return chr >= 'a' && chr <= 'z';
    }

    [[nodiscard]] static constexpr char toUpper(char chr) noexcept {
        return (CharUtils::isLower(chr) ? (char) (chr - 32) : chr);
    }

    static constexpr void skipWhiteSpace(const char *&it) noexcept {
        while (*it == ' ' || *it == '\n') {
            ++it;
        }
    }
};

class UpperChar {
protected:
    char m_chr;

public:
    constexpr explicit UpperChar() = default;

    constexpr explicit UpperChar(char chr) : m_chr(CharUtils::toUpper(chr)) {
    }

    constexpr explicit operator char() const noexcept {
        return m_chr;
    }

    constexpr explicit operator bool() const noexcept {
        return m_chr;
    }

    constexpr bool operator==(const UpperChar &other) const {
        return m_chr == other.m_chr;
    }

    static constexpr UpperChar assume(char chr) {
        UpperChar upperChr{};
        upperChr.m_chr = chr;
        return upperChr;
    }
};

}

#endif //CHESS_CHAR_TYPES_H
