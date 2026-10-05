#include <iostream>
#include <queue>
#include <string>

class Cat
{
    public: 
        std::string name;
        int treat_priority;

        bool operator < (const Cat& other) const 
        {
            return treat_priority < other.treat_priority;
        }
};

int main()
{
    std::priority_queue<Cat> cat_queue;

    cat_queue.push({"Buzzi", 7});
    cat_queue.push({"Pimpy", 10});
    cat_queue.push({"Milo", 3});
    cat_queue.push({"Luna", 8});

    while(!cat_queue.empty())
    {
        Cat current = cat_queue.top();
        cat_queue.pop();    

        std::cout << current.name << ": yummy~" << '\n';
    }

    return 0;
}
