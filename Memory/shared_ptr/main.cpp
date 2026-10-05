#include <iostream>
#include <string>
#include <memory>

class Feed 
{
    public:
        Feed() 
        {
            std::cout << "start eating...\n";
        }

        ~Feed()
        {
            std::cout << "end eating...\n";
        }
};

int main()
{   
    std::shared_ptr<Feed> f1 = std::make_shared<Feed>();
    std::cout << f1.use_count() << '\n';
    
    {
        std::shared_ptr<Feed> f2 = f1;
        std::cout << f1.use_count() << '\n';
    }

    std::cout << f1.use_count() << '\n';
    
    return 0;
}
