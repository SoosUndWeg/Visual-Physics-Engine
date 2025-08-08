//
//  Tokenizer.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "Tokenizer.hpp"

#include <print>


Tokenizer::Tokenizer(const std::string& text)
    : m_text(text) {
}

void Tokenizer::setNewText(const std::string& text) {
    m_text = text;
}

void Tokenizer::tokenize() {
    eraseWhitespace();

    for(const char& c : m_text) {
        
        if (std::isalpha(c)) {
            
            m_tokens.push_back(std::make_pair(std::string(1, c), TokenType::Identifier));
            
        } else if (m_number.find(c) != std::string::npos) {
            
            m_tokens.push_back(std::make_pair(std::string(1, c), TokenType::Number));
            
        } else if (m_binaryOperators.find(c) != std::string::npos) {
            
            if (c == '+' || c == '-') {
                m_tokens.push_back(std::make_pair(std::string(1, c), TokenType::BinaryPMOperator));
            } else if (c == '*' || c == '/') {
                m_tokens.push_back(std::make_pair(std::string(1, c), TokenType::BinaryMDOperator));
            } else if (c == '^') {
                m_tokens.push_back(std::make_pair(std::string(1, c), TokenType::BinaryPowerOperator));
            }
            
        } else if (m_punctuation.find(c) != std::string::npos) {
            
            m_tokens.push_back(std::make_pair(std::string(1, c), TokenType::Punctuation));
            
        } else if (m_delimiters.find(c) != std::string::npos) {
            
            m_tokens.push_back(std::make_pair(std::string(1, c), TokenType::Delimiter));
            
        } else if (m_bracketOpen.find(c) != std::string::npos) {
            
            m_tokens.push_back(std::make_pair(std::string(1, c), TokenType::BracketOpen));
            
        } else if (m_bracketClose.find(c) != std::string::npos) {
            
            m_tokens.push_back(std::make_pair(std::string(1, c), TokenType::BracketClose));
            
        } else {
            
            m_tokens.push_back(std::make_pair(std::string(1, c), TokenType::Unknown));
            
        }
    }
}

const std::vector<std::pair<std::string, Tokenizer::TokenType>>& Tokenizer::getTokens() const {
    return m_tokens;
}

void Tokenizer::eraseWhitespace() {
    m_text.erase(std::remove_if(m_text.begin(), m_text.end(),
        [](unsigned char c) {
            return std::isspace(c);
        }), m_text.end());
}
