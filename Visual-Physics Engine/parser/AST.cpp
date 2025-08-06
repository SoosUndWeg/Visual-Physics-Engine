//
//  AST.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "AST.hpp"

#include <unordered_map>
#include <cmath>



double BinaryOperationNode::evaluate(const Environment& env) const {
    double leftValue = left->evaluate(env);
    double rightValue = right->evaluate(env);

    switch (operation) {
        case '+':
            return leftValue + rightValue;
        case '-':
            return leftValue - rightValue;
        case '*':
            return leftValue * rightValue;
        case '/':
            if (rightValue == 0) {
                return 0.0;
            }
            return leftValue / rightValue;
        case '^':
            return std::pow(leftValue, rightValue);
        default:
            throw std::runtime_error("Unknown operation");
    }
}

double VariableNode::evaluate(const Environment& env) const {
    auto it = env.find(m_name);
    if (it != env.end()) {
        
        if (m_negative)
            return -it->second;
        else
            return it->second;
        
    } else {
        throw std::runtime_error("Variable not found: " + m_name);
    }
}

double ConstantNode::evaluate(const Environment& env) const {
    return value;
}

FunctionNode::FunctionNode(const std::string& func, std::unique_ptr<ASTNode> arg)
    : argument(std::move(arg)), functionName(func) {}

double FunctionNode::evaluate(const Environment& env) const {
    double argValue = argument->evaluate(env);

    if (functionMap.find(functionName) == functionMap.end()) {
        
        throw std::runtime_error("Function not found: " + functionName);
        
    } else {
        
        return functionMap.at(functionName)(argValue);
    }
}

double FunctionHeaderNode::evaluate(const Environment& env) const {
    return body ? body->evaluate(env) : 0.0;
}

std::string FunctionHeaderNode::toString() const {
    std::string params;
    for (const auto& param : parameters) {
        params += (params.empty() ? "" : ", ") + param;
    }
    return "function " + name + "(" + params + ") = " + (body ? body->toString() : "void");
}

void FunctionHeaderNode::setBody(std::unique_ptr<ASTNode> newBody) {
    body = std::move(newBody);
}

std::size_t FunctionHeaderNode::getParameterCount() const {
    return parameters.size();
}

double FunctionHeaderNode::evaluateWithParameters(const std::vector<double>& args) const {
    if (args.size() != parameters.size()) {
        throw std::runtime_error("Invalid number of arguments");
    }

    Environment env;
    for (std::size_t i = 0; i < parameters.size(); ++i) {
        env[parameters[i]] = args[i];
    }

    return body ? body->evaluate(env) : 0.0;
}


NegationNode::NegationNode(std::unique_ptr<ASTNode> node)
    : m_node(std::move(node)) {}

double NegationNode::evaluate(const Environment& env) const {
    return -m_node->evaluate(env);
}

std::string NegationNode::toString() const {
    return "-" + m_node->toString();
}
