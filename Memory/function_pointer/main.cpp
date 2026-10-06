#include <iostream>

void meow()
{
    std::cout << "meow~! \n"; 
}

void purr()
{
    std::cout << "purr... \n";
}

int main() 
{

    void (*catSound)();

    catSound = meow;
    catSound();

    catSound = purr;
    catSound();
    
    return 0;
}
