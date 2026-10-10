#include <iostream>
#include <string>
#include <thread>

int main()
{
    std::thread make_cat_eat(
        [](const std::string& name, int food_count)
        {
            std::cout << name << " eats " << food_count << " snacks\n";
        },
        "Buzzi",
        10
    );

    make_cat_eat.join();
}