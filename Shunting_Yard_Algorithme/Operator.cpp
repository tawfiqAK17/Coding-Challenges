#include "Operator.h"

Operator::Operator(types operator_type) {
    switch (operator_type) {
        case types::subtruction:
            priority = 1;
            break;
        case types::addition: 
            priority = 1.5;
            break;
        case types::multiplication:
            priority = 2;
            break;
        case types::division:
            priority = 2.5;
            break;
    }
}

double Operator::operator()(double lhs, double rhs) {
        if (priority == 1)
            return lhs - rhs;
        if (priority == 1.5)
            return lhs + rhs;
        if (priority == 2)
            return lhs * rhs;
        if (priority == 2.5)
            return lhs / rhs;
    return 0;
}

