#include <iostream>
#include <thread>

void feed_cat(const char* cat_name)
{
    std::cout << cat_name << " is eating..!" << "\n";
}

int main() 
{
    std::thread feeding_buzzi(feed_cat, "Buzzi");
    std::thread feeding_pimpy(feed_cat, "Pimpy");

    feeding_buzzi.join();
    feeding_pimpy.join();

    return 0;
}