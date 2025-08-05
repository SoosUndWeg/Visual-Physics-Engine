//
//  Tokenizer.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>

class Tokenizer {
public:
    enum class TokenType {
        Identifier,
        Number,
        BinaryOperator,
        UnaryOperator,
        Punctuation,
        Delimiter,
        BracketOpen,
        BracketClose,
        Function,
        FunctionHeader,
        Unknown
    };

public:
    Tokenizer() = default;

    Tokenizer(const std::string& text);

    void setNewText(const std::string& text);

    void tokenize();
    
    void old();

    const std::vector<std::pair<std::string, TokenType>>& getTokens() const;

private:
    void eraseWhitespace();

private:
    std::vector<std::pair<std::string, TokenType>> m_tokens;
    std::string m_text;

    const std::string m_delimiters = "=<>!&|";
    const std::string m_number = "0123456789";
    const std::string m_binaryOperators = "+-*/^";
    const std::string m_punctuation = ".,";
    const std::string m_bracketOpen = "(";
    const std::string m_bracketClose = ")";

    TokenType m_lastToken = TokenType::Unknown;
    TokenType m_lastLastToken = TokenType::Unknown;
};

#endif
