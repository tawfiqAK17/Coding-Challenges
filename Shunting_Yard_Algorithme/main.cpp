#include "Operator.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
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
template<class T>
void printStackInOrder(std::stack<T>stack) {
    // Use a vector to store the elements temporarily
    std::vector<T> temp;

    // Move elements from the stack to the vector
    while (!stack.empty()) {
        temp.push_back(stack.top());
        stack.pop();
    }

    std::reverse(temp.begin(), temp.end());
    // Now print the vector in reverse order (which represents the original stack order)
    for (auto i : temp) {
            std::cout << i << std::endl;
    }
    std::cout << std::endl;
}
void printQueue(std::queue<std::variant<Operator, double>> q) {
    // Print elements by removing from the copy of the queue
    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << std::endl;
}

int main() {
    std::cout << ShuntingYard("((5 + 3) * (8 - 2)) / (6 + (4 * (3 - 1)))") << std::endl;
    std::cout << (((float)5 + 3) * (8 - 2)) / (6 + (4 * (3 - 1))) << std::endl;
/* -((45.8/2.3)+(6.7*9.8))+(34.5/(7.2-3.6))*(8.9+1.1) */
}

double ShuntingYard(std::string expression) {
    std::vector<std::variant<Operator, double>> tokens;
    std::stack<double> result_stack;
    ParsExpression(expression, tokens);
    auto revers_polish_notation = Revers_polish_notation(tokens);
    for (auto& i : tokens) 
        std::cout << i << " ";
    std::cout <<std::endl;
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
    std::cout << sub_expression << std::endl;
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
        std::cout << lhs <<std::get<Operator>(revers_polish_notation.front()) << rhs << std::endl;
        result_stack.push(std::get<Operator>(revers_polish_notation.front())(lhs, rhs));
        revers_polish_notation.pop();
        /* printStackInOrder(result_stack); */
    }
}
