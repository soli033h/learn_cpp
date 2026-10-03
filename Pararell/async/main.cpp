#include <iostream>
#include <future>
#include <thread>

int something_complicated()
{   
    int num = 0;
    while(num < 10000)
    {
        num++;
    }
    
    return num;
}

int main() 
{
    std::future<int> result = std::async(std::launch::async, something_complicated);
    
    for(int i = 0; i <= 9; i++)
    {
        std::cout << "something is running..." << "\n";
    }
    
    std::cout << result.get() << std::endl;
}
