#include <iostream>
#include <utility>

void meow(int& number)
{
    std::cout << "lvalue reference value is..." << number << "\n";
}

void meow(int&& number)
{
    std::cout << "rvalue reference value is..." << number << "\n";
}

int main()
{
    int n = 25;

    meow(n); // lvalue 
    meow(26); // rvalue

    meow(std::move(n)); // lvalue를 rvalue로 casting, 더 정확히는 xvalue
}