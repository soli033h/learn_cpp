#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

std::atomic<int> feed_counter(1000);

void yummy()
{
    for (int i = 0; i < 1000; i++)
    {
        feed_counter.fetch_sub(1, std::memory_order_relaxed);
    }
}

int main()
{
    std::vector<std::thread> cats;
    
    for (int i = 0; i < 10; i++)
    {
        cats.emplace_back(yummy);    
    }
    
    for (auto& cat : cats)
    {
        cat.join();
    }
    
    std::cout << feed_counter.load() << "\n";
    return 0;
}
