#include <iostream>
#include <string>
#include <stack>

int main() 
{
    std::stack<std::string> cats;
    
    //cats.push("Buzzi");
    //cats.push("Pimpy");
    //cats.push("Luna");
    
    cats.emplace("Buzzi");
    cats.emplace("Pimpy");
    cats.emplace("Luna");
    
    std::cout << cats.size() << "\n";
    std::cout << cats.top() << "\n";
    
    while(!cats.empty())
    {
        std::cout << cats.top() << "\n";
        cats.pop();
    }
    
    return 0;
}
