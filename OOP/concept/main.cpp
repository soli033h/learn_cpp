#include <iostream>
#include <string>
#include <concepts>

template <typename T>
concept Meowable = requires(T cat)
{
    {cat.meow()} -> std::same_as<void>;
};

class Cat 
{
    private:
        std::string name;
    
    public:
        Cat(std::string name) : name(name) {}

        void meow() const 
        {
            std::cout << name << " said meow~" << '\n'; 
        }
};

void make_it_meow(Meowable auto const& cat)
{
    cat.meow();
}

int main()
{
    Cat buzzi("buzzi");

    make_it_meow(buzzi);
}