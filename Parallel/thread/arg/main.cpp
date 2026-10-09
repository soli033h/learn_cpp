#include <iostream>
#include <thread>

void add_score(int score)
{
    score += 10;
    std::cout << "worker score: " << score << '\n';
}

int main()
{
    int score = 100;

    std::thread worker(add_score, score);
    worker.join();

    std::cout << "main score: " << score << '\n';
}