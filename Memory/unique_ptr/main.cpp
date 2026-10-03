#include <iostream>
#include <memory>

class Feed
{   
    private:
        std::string name;

    public:
        Feed(std::string n) : name(std::move(n))
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

int main()
{
    auto feeding_by_john = std::make_unique<Feed>("Beatle");
    feeding_by_john->yummy();

    auto feeding_by_paul = std::move(feeding_by_john);
    feeding_by_paul->yummy();

    return 0;
}