#include "Operator.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <variant>
#include <vector>

std::ostream &operator<<(std::ostream &os, const Operator &op) {
    std::cout << op.GetSymbol();
    return os;
}
std::ostream &operator<<(std::ostream &os, const std::variant<Operator, double> &op) {
        if (std::holds_alternative<Operator>(op))
            std::cout << std::get<Operator>(op);
        else
            std::cout << std::get<double>(op);
    return os;
}
double ShuntingYard(std::string expression);
void ParsExpression(const std::string& expression, std::vector<std::variant<Operator, double>>& tokens);
std::string sub_expression(const std::string& expression, int& idx);
std::queue<std::variant<Operator, double>> Revers_polish_notation(const std::vector<std::variant<Operator, double>>& tokens);
void Solve(std::queue<std::variant<Operator, double>>& revers_polish_notation, std::stack<double>& result_stack);
int main() {
    std::cout << ShuntingYard("((5 + 3) * (8 - 2)) / (6 + (4 * (3 - 1)))") << std::endl;
    std::cout << (((float)5 + 3) * (8 - 2)) / (6 + (4 * (3 - 1))) << std::endl;
}

double ShuntingYard(std::string expression) {
    std::vector<std::variant<Operator, double>> tokens;
    std::stack<double> result_stack;
    ParsExpression(expression, tokens);
    auto revers_polish_notation = Revers_polish_notation(tokens);
    Solve(revers_polish_notation, result_stack);
    return result_stack.top();
}

void ParsExpression(const std::string& expression, std::vector<std::variant<Operator, double>>& tokens) {
    std::string number = "";
    for (int i = 0; i < expression.size(); i++) {
        switch (expression[i]) {
            case '+': 
                if(!number.empty())
                    tokens.push_back(std::stod(number));
                tokens.emplace_back(Operator::types::addition);
                number = "";
                break;
            case '-': 
                if(!number.empty())
                    tokens.push_back(std::stod(number));
                tokens.emplace_back(Operator::types::subtruction);
                number = "";
                break;
            case '*': 
                if(!number.empty())
                    tokens.push_back(std::stod(number));
                tokens.emplace_back(Operator::types::multiplication);
                number = "";
                break;
            case '/': 
                if(!number.empty())
                    tokens.push_back(std::stod(number));
                tokens.emplace_back(Operator::types::division);
                number = "";
                break;
            case '(':
                tokens.push_back(ShuntingYard(sub_expression(expression, i)));
                break;
            case ' ':
                break;
            case ')':
                break;
            default:
                number += expression[i];
                break;
        }
    }
    if(!number.empty()) {
        tokens.push_back(std::stod(number));
    }
}
std::string sub_expression(const std::string& expression, int& idx) {
    int i = idx + 1;
    int opned = 0;
    while (true) {
        if (expression[i] == '(')
            opned++;
        else if (expression[i] == ')') {
            opned--;
            if (opned <= 0)
                break;
        }
        i++;
    }
    std::string sub_expression = expression.substr(idx + 1, i - idx);
    idx = i + 1;
    return sub_expression;
}
std::queue<std::variant<Operator, double>> Revers_polish_notation(const std::vector<std::variant<Operator, double>>& tokens) {
    std::queue<std::variant<Operator, double>> revers_polish_notation;
    std::stack<Operator> operators_stack;
    for (auto token : tokens){
        if (std::holds_alternative<double>(token)) {
            revers_polish_notation.push(std::get<double>(token));
            continue;
        }
        if (operators_stack.empty() || operators_stack.top() < std::get<Operator>(token))
            operators_stack.push(std::get<Operator>(token));
        else {
            while (!operators_stack.empty()) {
                if (operators_stack.top() < std::get<Operator>(token))
                    break;
                revers_polish_notation.push(operators_stack.top());
                operators_stack.pop();
            }
            operators_stack.push(std::get<Operator>(token));
        }
    }
    while (!operators_stack.empty()) {
        revers_polish_notation.push(operators_stack.top()); 
        operators_stack.pop();
    }
    return revers_polish_notation;
}
void Solve(std::queue<std::variant<Operator, double>>& revers_polish_notation, std::stack<double>& result_stack) {
    while (!revers_polish_notation.empty()) {
        if (std::holds_alternative<double>(revers_polish_notation.front())) {
            result_stack.push(std::get<double>(revers_polish_notation.front()));
            revers_polish_notation.pop();
            continue;
        }
        double rhs = result_stack.top();
        result_stack.pop();
        double lhs = result_stack.top();;
        result_stack.pop();
        result_stack.push(std::get<Operator>(revers_polish_notation.front())(lhs, rhs));
        revers_polish_notation.pop();
    }
}
