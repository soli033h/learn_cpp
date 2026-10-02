#include <iostream>
#include <map>
#include <string>

int main() 
{
    std::map<std::string, int> beatles;

    beatles["john"] = 30;
    beatles["paul"] = 28;
    beatles["george"] = 27;
    beatles["ringo"] = 26;

    auto it = beatles.find("john");
    std::cout << "he's age: " << it -> second << "\n";
}