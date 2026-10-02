#include <iostream>
#include <memory>

class Feed
{   
    private:
        std::string name;

    public:
        Feed()
        {
            std::cout << this -> name << " is hungry" << '\n';
        }
        ~Feed()
        {
            std::cout << this -> name << " is full" << '\n';
        }
        void yummy()
        {
            std::cout << this -> name << " is eating" << '\n';    
        }
};