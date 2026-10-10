#include <iostream>
#include <string>
#include <thread>

void meow(const std::string& name)
{
    std::cout << name << " meow~" << '\n';
}

int main()
{
    std::string cat = "Buzzi";

    std::thread worker(meow, std::cref(cat));
    worker.join();

}