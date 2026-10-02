#include <iostream>
#include <vector>

int main()
{
    std::vector<std::string> cats = {"Buzzi", "Hazel", "Bella", "Lucy", "Oliver"};
    
    cats.push_back("Pearl");
    
    for (std::string cat : cats)
    {
        std::cout << cat << " ";
    }
    
    std::cout << "\n";
}