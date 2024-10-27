#include <iostream>
class Operator {
public:
    enum class types{multiplication, division, addition, subtruction};
    Operator(types operator_type);
    inline bool operator<(Operator& rhs) {return priority < rhs.GetPriority();}
    // to apply the operator on two numbers
    double operator()(double lhs, double rhs);
    inline int GetPriority() const {return priority;}
    inline char GetSymbol() const {
        if (priority == 1)
            return '-';
        if (priority == 1.5)
            return '+';
        if (priority == 2)
            return '*';
        if (priority == 2.5)
            return '/';
        return ' ';
    }
    friend std::ostream& operator<<(std::ostream& os, const Operator& op); 
private:
    float priority;
};
