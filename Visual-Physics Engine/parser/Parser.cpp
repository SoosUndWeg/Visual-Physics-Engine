//
//  Parser.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "Parser.hpp"

#include <print>

Parser::Parser()
    : m_tokenizer("") {
}

void Parser::setExpression(const std::string& expression) {
    m_expression = expression;
    m_tokenizer.setNewText(expression);
}

std::unique_ptr<ASTNode> Parser::parse() {
    m_tokenizer.tokenize();

    m_tokens = m_tokenizer.getTokens();

    m_shuntingYard.setTokens(m_tokens);

    return m_shuntingYard.parse();
}

void ShuntingYard::setTokens(const std::vector<std::pair<std::string, Tokenizer::TokenType>>& tokens) {
    m_tokens = tokens;
}

std::unique_ptr<ASTNode> ShuntingYard::parse() {
    
    std::stack<std::pair<std::string, Tokenizer::TokenType>> opStack;
    std::stack<std::unique_ptr<ASTNode>> outputStack;

    std::unique_ptr<FunctionHeaderNode> funcHeaderNode;

    //Function definitions
    if (m_tokens[0].second == Tokenizer::TokenType::Identifier &&
        m_tokens[1].second == Tokenizer::TokenType::BracketOpen) {
    
        std::string functionName = m_tokens[0].first;
        std::vector<std::string> parameters;

        int i = 2;
        while (m_tokens[i].second == Tokenizer::TokenType::Identifier) {
            
            parameters.push_back(m_tokens[i].first);

            if (m_tokens[i + 2].second == Tokenizer::TokenType::Identifier) {
                i += 2;
            } else {
                i++;
            }
        }

        funcHeaderNode = std::make_unique<FunctionHeaderNode>(functionName, parameters);

        for (int j = 0; j < i + 2; ++j) {
            
            // Remove tokens used for function header
            m_tokens.erase(m_tokens.begin());
        }
    }
    
    std::pair<std::string, Tokenizer::TokenType> lastToken = {"", Tokenizer::TokenType::Unknown};
    
    for (int i = 0; i < m_tokens.size(); ++i) {
        
        std::print("Token: {}\n", m_tokens[i].first);
        
        auto& token = m_tokens[i];
        
        if (token.second == Tokenizer::TokenType::Number) {
            
            for (int j = i + 1; j < m_tokens.size(); ++j) {
                
                // Check if the next token is a number or a dot
                if (m_tokens[j].second == Tokenizer::TokenType::Number || m_tokens[j].second == Tokenizer::TokenType::Punctuation) {
                    
                    // Append the next token to the current number
                    token.first += m_tokens[j].first;
                    
                    // Remove the next token from the list
                    i++;
                    
                } else {
                    
                    break;
                }
            }
            
            std::print("Parsed number: {}\n", token.first);
            
            // Push the number to the output stack as a ConstantNode
            if (lastToken.second == Tokenizer::TokenType::UnaryOperator){
                std::print("L-> Got a p/m\n");
                outputStack.push(std::make_unique<ConstantNode>(std::stod(lastToken.first + token.first)));
                
            } else {
                std::print("L-> Got a number {}\n", token.first);
                outputStack.push(std::make_unique<ConstantNode>(std::stod(token.first)));
            }
            
            lastToken = token;
            
        } else if (token.second == Tokenizer::TokenType::BinaryPMOperator ||
                   token.second == Tokenizer::TokenType::BinaryMDOperator ||
                   token.second == Tokenizer::TokenType::BinaryPowerOperator) {
            
            if (token.second == Tokenizer::TokenType::BinaryPMOperator &&
                (lastToken.second == Tokenizer::TokenType::BinaryPMOperator ||
                lastToken.second == Tokenizer::TokenType::BinaryMDOperator ||
                lastToken.second == Tokenizer::TokenType::BinaryPowerOperator ||
                lastToken.second == Tokenizer::TokenType::Unknown ||
                lastToken.second == Tokenizer::TokenType::Function ||
                lastToken.second == Tokenizer::TokenType::BracketOpen)) {
                
                lastToken = token;
                lastToken.second = Tokenizer::TokenType::UnaryOperator;
                
                if (lastToken.first == "-") {
                    
                    opStack.push(lastToken);
                }
                
            } else {
                
                pushToOpStack(opStack, outputStack, token, lastToken);

            }
        } else if (token.second == Tokenizer::TokenType::Identifier) {
            
            bool isFunction = false;
            
            // Check if the identifier is a function
            for (const auto& func : m_functions) {
                
                isFunction = true;
                
                // Check if the identifier matches the function name
                for (int j = 0; j < func.size(); ++j) {
                    
                    // Prevent out of bounds access
                    if (j + i > m_tokens.size() - 1) {
                        
                        isFunction = false;
                        break;
                    }
                    std::print("Checking function {} against token {}\n", func, m_tokens[i + j].first);
                    
                    // Compare the tokens with the function name
                    if (m_tokens[i + j].first[0] != func[j]) {
                        
                        isFunction = false;
                        break;
                    }
                }
                
                if (isFunction) {
                    
                    // Set the token type to Function
                    lastToken = token;
                    token.first = func;
                    token.second = Tokenizer::TokenType::Function;
                    
                    while ((isLeftAssociative(token.first[0]) && precedence(token.second) <= precedence(lastToken.second)) ||
                           (!isLeftAssociative(token.first[0]) && precedence(token.second) < precedence(lastToken.second))) {
                        
                        handleOperator(opStack, outputStack);
                    }
                    
                    // Skip the tokens that were part of the function identifier
                    i += func.size() - 1;
                    
                    pushToOpStack(opStack, outputStack, token, lastToken);
                    
                    
                    
                    break;
                }
            }
            
            bool isConstant = false;
            
            // Check if the identifier is a constant
            for (const auto& pair : m_constants) {
                
                isConstant = true;
                
                // Compare the identifier with the constant name
                for (int j = 0; j < pair.first.size(); ++j) {
                    
                    // Prevent out of bounds access
                    if (j + i > m_tokens.size() - 1) {
                        break;
                    }
                    
                    // Compare the tokens with the constant name
                    if (m_tokens[i + j].first[0] != pair.first[j]) {
                        isConstant = false;
                        break;
                    }
                }
                
                if (isConstant) {
                    
                    outputStack.push(std::make_unique<ConstantNode>(pair.second));
                    
                    lastToken = token;
                    
                    // Skip the tokens that were part of the constant identifier
                    i += pair.first.size() - 1;
                    
                    break;
                }
            }

            if (!isFunction && !isConstant) {
                std::print("Identifier found: {} {}\n", lastToken.first, token.first);
                // If the identifier is not a function or constant, treat it as a variable
                if (lastToken.second == Tokenizer::TokenType::UnaryOperator && lastToken.first == "-") {
                    
                    outputStack.push(std::make_unique<VariableNode>(token.first, true));
                    
                } else {
                    
                    outputStack.push(std::make_unique<VariableNode>(token.first, false));
                    
                }
                
                lastToken = token;
                
            }

        } else if (token.second == Tokenizer::TokenType::BracketOpen) {
            
            // Push the opening bracket to the operator stack
            opStack.push(token);
            
            lastToken = token;
            
        } else if (token.second == Tokenizer::TokenType::BracketClose) {
            
            // Pop operators from the stack until an opening bracket is found
            while (!opStack.empty() && opStack.top().second != Tokenizer::TokenType::BracketOpen) {
                std::print("-->Handling operator: {}\n", opStack.top().first);
                handleOperator(opStack, outputStack);
                
            }
            std::print("Stack before popping: {}\n", opStack.top().first);
            opStack.pop();
            token = opStack.top();
            std::print("STack after popping: {}\n", token.first);
            
            if (!opStack.empty() && token.second == Tokenizer::TokenType::Function) {
                std::print("Function before bracket found {}\n", token.first);
                handleOperator(opStack, outputStack);
                
                lastToken = token;
                
            } else {
                
                lastToken = token;
            }

        }
    }
    
    while (!opStack.empty()) {
        
        handleOperator(opStack, outputStack);
    }
    
    isStackEmpty(outputStack);
    
    auto node = std::move(outputStack.top());
    outputStack.pop();
    
    if (funcHeaderNode) {
        
        funcHeaderNode->setBody(std::move(node));
        
        return std::move(funcHeaderNode);
        
    } else {
        
        return node;
    }

    return std::move(funcHeaderNode);
}

int ShuntingYard::precedence(Tokenizer::TokenType type) const {
    switch (type) {
        case Tokenizer::TokenType::BinaryPMOperator:
            
            return 1; // +, -
            
        case Tokenizer::TokenType::BinaryMDOperator:
            
            return 2; // *, /
            
        case Tokenizer::TokenType::BinaryPowerOperator:
            
            return 3; // ^
            
        case Tokenizer::TokenType::Function:
            
            return 4; // Function calls
            
        default:
            
            return 0; // Unknown or no precedence
    }
}

bool ShuntingYard::isLeftAssociative(char op) const {
    return op != '^';
}

void ShuntingYard::isStackEmpty(const std::stack<std::pair<std::string, Tokenizer::TokenType>>& stack) const {
    if(stack.empty()) {
        throw std::runtime_error("Stack is empty");
    }
}

void ShuntingYard::isStackEmpty(const std::stack<std::unique_ptr<ASTNode>>& stack) const {
    if(stack.empty()) {
        throw std::runtime_error("Stack is empty");
    }
}

std::unique_ptr<ASTNode> ShuntingYard::setupFuncHeader(const std::pair<std::string, Tokenizer::TokenType>& token) {
    //Irgendwie besser machen
    std::unique_ptr<ASTNode> header = std::make_unique<FunctionHeaderNode>(token.first, std::vector<std::string>());
    return header;
}

void ShuntingYard::pushToOpStack(std::stack<std::pair<std::string, Tokenizer::TokenType>>& opStack,
                                    std::stack<std::unique_ptr<ASTNode>>& outputStack,
                                    const std::pair<std::string, Tokenizer::TokenType>& token,
                                 std::pair<std::string, Tokenizer::TokenType>& lastToken) {
    
    while (!opStack.empty() &&
           ((isLeftAssociative(token.first[0]) && precedence(opStack.top().second) >= precedence(token.second)) ||
            (!isLeftAssociative(token.first[0]) && precedence(opStack.top().second) > precedence(token.second)))) {
        
        handleOperator(opStack, outputStack);
    }
    
    opStack.push(token);
    lastToken = token;
}

void ShuntingYard::handleOperator(std::stack<std::pair<std::string, Tokenizer::TokenType>>& opStack,
                                    std::stack<std::unique_ptr<ASTNode>>& outputStack) {
    
    isStackEmpty(opStack);
    
    auto op = opStack.top();
    opStack.pop();
    
    if (op.second == Tokenizer::TokenType::FunctionHeader) {
        
        isStackEmpty(outputStack);
        
        outputStack.push(setupFuncHeader(op));
        
    } else if (op.second == Tokenizer::TokenType::BinaryPMOperator ||
               op.second == Tokenizer::TokenType::BinaryMDOperator ||
               op.second == Tokenizer::TokenType::BinaryPowerOperator) {
        
        isStackEmpty(outputStack);
        
        auto right = std::move(outputStack.top());
        outputStack.pop();
        
        isStackEmpty(outputStack);
        
        auto left = std::move(outputStack.top());
        outputStack.pop();
        
        outputStack.push(std::make_unique<BinaryOperationNode>(std::move(left), std::move(right), op.first[0]));
        
    } else if (op.second == Tokenizer::TokenType::UnaryOperator) {
        
        isStackEmpty(outputStack);
        auto arg = std::move(outputStack.top());
        outputStack.pop();
        
        outputStack.push(std::make_unique<NegationNode>(std::move(arg)));
    
    } else if (op.second == Tokenizer::TokenType::Function) {
        
        isStackEmpty(outputStack);
        
        auto arg = std::move(outputStack.top());
        outputStack.pop();
        
        outputStack.push(std::make_unique<FunctionNode>(op.first, std::move(arg)));
        
    } else if (op.second == Tokenizer::TokenType::BracketClose) {
        
        while (!opStack.empty() && opStack.top().second != Tokenizer::TokenType::BracketOpen) {
            
            handleOperator(opStack, outputStack);
        }
        
    } else if (op.second == Tokenizer::TokenType::BracketOpen) {
        
        if (!opStack.empty() && opStack.top().second == Tokenizer::TokenType::Function) {
            
            handleOperator(opStack, outputStack);
            
        }
        
    } else {
        
        throw std::runtime_error("Unknown operator " + op.first + " type in stack");
    }
}
