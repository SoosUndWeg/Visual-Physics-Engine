//
//  AST.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef AST_HPP
#define AST_HPP

#include <unordered_map>
#include <string>
#include <numeric>
#include <vector>

using Environment = std::unordered_map<std::string, double>;

struct ASTNode {

    virtual ~ASTNode() = default;

    virtual double evaluate(const Environment& env) const = 0;

    virtual std::string toString() const = 0;

};

struct BinaryOperationNode : public ASTNode {
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    char operation; // e.g., '+', '-', '*', '/'

    BinaryOperationNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right, char operation)
        : left(std::move(left)), right(std::move(right)), operation(operation) {}

    double evaluate(const Environment& env) const override;

    std::string toString() const override {
        return "(" + left->toString() + " " + operation + " " + right->toString() + ")";
    }
};

struct VariableNode : public ASTNode {
    std::string m_name;
    bool m_negative = false;

    VariableNode(const std::string& name, bool negative = false) : m_name(name), m_negative(negative) {}

    double evaluate(const Environment& env) const override;

    std::string toString() const override {
        return m_name;
    }
};

struct ConstantNode : public ASTNode {
    double value;

    ConstantNode(double value) : value(value) {}

    double evaluate(const Environment& env) const override;

    std::string toString() const override {
        return std::to_string(value);
    }
};

struct FunctionNode : public ASTNode {
    std::string function; // e.g., "sin", "cos", "tan"
    std::unique_ptr<ASTNode> argument;

    FunctionNode(const std::string& function, std::unique_ptr<ASTNode> argument)
        : function(function), argument(std::move(argument)) {}

    double evaluate(const Environment& env) const override;

    std::string toString() const override {
        return function + "(" + argument->toString() + ")";
    }
};

struct FunctionHeaderNode : public ASTNode {
    std::string name;
    std::vector<std::string> parameters;

    std::unique_ptr<ASTNode> body;

    FunctionHeaderNode(const std::string& name, const std::vector<std::string>& parameters, std::unique_ptr<ASTNode> body = nullptr)
        : name(name), parameters(parameters), body(std::move(body)) {}

    double evaluate(const Environment& env) const override;

    std::string toString() const override;

    void setBody(std::unique_ptr<ASTNode> newBody);
    std::size_t getParameterCount() const;

    double evaluateWithParameters(const std::vector<double>& args) const;
};

#endif // AST_HPP
