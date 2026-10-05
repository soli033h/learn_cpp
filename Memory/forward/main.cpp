#include <iostream>
#include <string>
#include <utility>

void make_it_meow(int& num)
{
    std::cout << "Meow from Lvalue\n";
}

void make_it_meow(int&& num)
{
    std::cout << "Meow from Rvalue\n";
}

template <typename T>
void wrapper(T&& arg)
{
    // arg 자체는 Lvalue, 근데 std::forward<T>를 거치면 호출 당시, 원본의 값 범주로 복원됨
    make_it_meow(std::forward<T>(arg));
}

int main() {
    int number = 25;

    wrapper(number);
    wrapper(25);

    return 0;
}
