#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Cat
{     
    std::string name;
    int age;
    double weight_kg;
};

int main() {
    // Write C++ code here
    std::vector<Cat> cats = {
        {"Milo", 2, 4.2},
        {"Luna", 5, 3.8},
        {"Oliver", 7, 5.5},
        {"Bella", 1, 3.1}
    };
    
    std::sort(cats.begin(), cats.end(), [](const Cat& a, const Cat& b) 
    {
        return a.weight_kg < b.weight_kg;
    });

    for (const auto& cat : cats)
        {
            std::cout << cat.name << ": " << cat.weight_kg << " kg" << '\n';
        }
    
    return 0;
}
