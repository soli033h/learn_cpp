#include <iostream>
#include <string>
#include <thread>
#include <memory>

void let_people_know_my_age(std::unique_ptr<int> age)
{
    std::cout << *age << '\n';
}

int main()
{
    auto ptr_to_my_secret_age = std::make_unique<int>(25);    

    std::thread worker(let_people_know_my_age, std::move(ptr_to_my_secret_age));
    worker.join();

    //std::cout << "after func: "<< *ptr_to_my_secret_age << '\n';
}