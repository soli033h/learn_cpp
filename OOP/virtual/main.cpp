#include <iostream>

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
    Cat* Jasper = new Calico();
    Jasper -> meow();

    delete Jasper;
    
    return 0;
}