#include <iostream>

class Cat
{
    protected:
        virtual void meow() = 0;
};

class Calico : protected Cat 
{
    protected: 
        void meow() override 
        {
            std::cout << "meow~" << "\n";
        }
};

int main()
{
    return 0;
}