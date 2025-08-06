//
//  Parser.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef PARSER_HPP
#define PARSER_HPP

#include <stack>

#include "Tokenizer.hpp"
#include "AST.hpp"



class ShuntingYard {
public:
    ShuntingYard() = default;
    
    void setTokens(const std::vector<std::pair<std::string, Tokenizer::TokenType>>& tokens);
    
    std::unique_ptr<ASTNode> parse();
    
private:
    int precedence(Tokenizer::TokenType type) const;
    
    bool isLeftAssociative(char op) const;
    void isStackEmpty(const std::stack<std::pair<std::string, Tokenizer::TokenType>>& stack) const;
    void isStackEmpty(const std::stack<std::unique_ptr<ASTNode>>& stack) const;
    
    std::unique_ptr<ASTNode> setupFuncHeader(const std::pair<std::string, Tokenizer::TokenType>& token);
    
    void pushToOpStack(std::stack<std::pair<std::string, Tokenizer::TokenType>>& opStack,
                       std::stack<std::unique_ptr<ASTNode>>& outputStack,
                       const std::pair<std::string, Tokenizer::TokenType>& token,
                       std::pair<std::string, Tokenizer::TokenType>& lastToken);
    
    void handleOperator(std::stack<std::pair<std::string, Tokenizer::TokenType>>& opStack,
                        std::stack<std::unique_ptr<ASTNode>>& outputStack);
    
private:
    std::vector<std::pair<std::string, Tokenizer::TokenType>> m_tokens;
    
    const std::vector<std::string> m_functions = {"sin", "cos", "tan", "exp", "log", "sqrt", "abs", "pow", "min", "max"};
    const std::unordered_map<std::string, double> m_constants = {
        {"pi", M_PI},
        {"e", M_E}
    };
};



class Parser {
public:
    Parser();

    void setExpression(const std::string& expression);

    std::unique_ptr<ASTNode> parse();

private:
    std::string m_expression;

    std::vector<std::pair<std::string, Tokenizer::TokenType>> m_tokens;

    Tokenizer m_tokenizer;
    ShuntingYard m_shuntingYard;
};


#endif
