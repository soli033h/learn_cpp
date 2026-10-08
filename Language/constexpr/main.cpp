#include <iostream>
#include <string>

constexpr int square(int value)
{
    return value * value;
}

int main()
{
    constexpr int result = square(5);
    static_assert(result == 25);

    std::cout << result << '\n';
    
    return 0;
}