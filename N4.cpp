#include <iostream>
#include <cassert>
#include "less_than_comparable.hpp"
#include "counter.hpp"

class Number : public less_than_comparable<Number>, 
               public counter<Number> {
public:
    Number(int value) : m_value{value} {}

    int value() const { return m_value; }

    bool operator<(const Number& other) const {
        return m_value < other.m_value;
    }

private:
    int m_value;
};

int main() {
    Number one{1};
    Number two{2};
    Number three{3};
    Number four{4};

    assert(one >= one);
    assert(three <= four);
    assert(two == two);
    assert(three > two);
    assert(one < two);

    std::cout << "count: " << counter<Number>::count() << std::endl;

    assert(one != two); 
    assert(one == one);  
    assert(two >= one); 
    assert(four >= three); 
    assert(one <= two);      

    std::cout << "tests passed!" << std::endl;

    return 0;
}