#include <iostream>
#include <queue>
#include <string>

int main() 
{
    std::queue<std::string> cats;
    
    //cats.push("Buzzi");
    //cats.push("Pimpy");
    //cats.push("Luna");
    
    cats.emplace("Buzzi");
    cats.emplace("Pimpy");
    cats.emplace("Luna");
    
    std::cout << cats.size() << '\n';
    std::cout << cats.front() << '\n';
    std::cout << cats.back() << '\n';
    
    while (!cats.empty())
    {
        std::cout << cats.front() << '\n';
        cats.pop();
    }
    
    return 0;
}
