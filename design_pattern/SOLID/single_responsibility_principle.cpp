
#include <iostream>
#include <string>

// SRP: 고양이의 "데이터"만 담당 (변경 이유: 고양이 정보 구조가 바뀔 때)
class CatProfile {
public:
    std::string name;
    int age;
    std::string favorite_snack;

    CatProfile(std::string n, int a, std::string snack)
        : name(std::move(n)), age(a), favorite_snack(std::move(snack)) {}
};

// SRP: 고양이 정보를 "출력/스토리텔링"만 담당 (변경 이유: 출력 형식이 바뀔 때)
class CatStoryPrinter {
public:
    static void print_meow_story(const CatProfile& cat) {
        std::cout << cat.name << " (" << cat.age << " years old): Meow!\n";
        std::cout << "  Favorite snack: " << cat.favorite_snack << "\n";
    }
};

// SRP: 고양이 정보를 "저장 포맷 생성"만 담당 (변경 이유: 저장 형식이 바뀔 때)
class CatReportSaver {
public:
    static std::string to_csv_row(const CatProfile& cat) {
        return cat.name + "," + std::to_string(cat.age) + "," + cat.favorite_snack;
    }
};

int main() {
    CatProfile navi{"Navi", 3, "Tuna"};

    std::cout << "=== Single Responsibility Principle (Cat Example) ===\n";
    CatStoryPrinter::print_meow_story(navi);

    std::cout << "\n[Saved row]\n";
    std::cout << CatReportSaver::to_csv_row(navi) << "\n";

    return 0;
}
