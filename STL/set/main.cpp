#include <iostream>
#include <set>

int main()
{
    std::set<std::string> beatles;

    beatles.insert("john lennon");
    beatles.insert("paul mcCartney");
    beatles.insert("george harrison");
    beatles.insert("ringo starr");

    beatles.insert("john lennon"); // 중복 삽입 시도

    for (std::string member : beatles)
    {
        std::cout << member << "\n"; // 자동 정렬되어 출력
    }

    beatles.erase("john lennon");

    return 0;
}