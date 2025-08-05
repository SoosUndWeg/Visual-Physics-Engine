//
//  AST.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "AST.hpp"

#include <print>



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
                throw std::runtime_error("Division by zero");
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

double FunctionNode::evaluate(const Environment& env) const {
    double argValue = argument->evaluate(env);

    if (function == "sin") {
        return std::sin(argValue);
    } else if (function == "cos") {
        return std::cos(argValue);
    } else if (function == "tan") {
        return std::tan(argValue);
    } else if (function == "sqrt") {
        if (argValue < 0) {
            throw std::runtime_error("Square root of negative number");
        }
        return std::sqrt(argValue);
    } else {
        throw std::runtime_error("Unknown function: " + function);
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
