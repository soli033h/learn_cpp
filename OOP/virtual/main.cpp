#include <iostream>
#include <memory>

class Cat
{
    public:
        virtual void meow() = 0;

        virtual ~Cat() {}
};

class Calico : public Cat 
{
    public: 
        void meow() override 
        {
            std::cout << "meow~" << "\n";
        }
};

int main()
{   
    auto Jasper = std::make_unique<Calico>();
    Jasper -> meow();

    delete Jasper;
    
    return 0;
}
