#include <iostream>
#include <string>
#include <utility>
#include <type_traits>

// 호출식 자체의 value category를 보존해 출력하는 헬퍼
// 주의: forwarding-reference 매개변수(T&&)에 이름이 붙으면 함수 내부에서는 항상 lvalue 표현식이다.
// 따라서 value category 판별은 호출 지점에서 decltype((expr))로 타입을 넘겨 수행한다.
template <typename Expr>
void print_value_category(const char* expr_name) {
    std::cout << "Expression [" << expr_name << "] : ";
    if constexpr (std::is_lvalue_reference_v<Expr>) {
        std::cout << "lvalue (has name and addressable)\n";
    } else if constexpr (std::is_rvalue_reference_v<Expr>) {
        std::cout << "xvalue (expiring object, can be moved)\n";
    } else {
        std::cout << "prvalue (temporary object / literal)\n";
    }
}

#define PRINT_VALUE_CATEGORY(expr) print_value_category<decltype((expr))>(#expr)

class Cat {
public:
    std::string name;

    explicit Cat(std::string n) : name(std::move(n)) {
        std::cout << "  [Created] " << name << "\n";
    }

    // 복사 생성자 (lvalue를 전달받을 때)
    Cat(const Cat& other) : name(other.name) {
        std::cout << "  [Copy Created] " << name << " (copied from lvalue)\n";
    }

    // 이동 생성자 (rvalue를 전달받을 때)
    Cat(Cat&& other) noexcept : name(std::move(other.name)) {
        std::cout << "  [Move Created] " << name << " (resource moved from rvalue)\n";
    }

    // --- C++ Ref-qualifiers ---
    // lvalue 객체에서만 호출 가능한 멤버 함수
    void meow() & {
        std::cout << "  " << name << ": Meow! (called on lvalue object)\n";
    }

    // rvalue(임시/만료 예정) 객체에서만 호출 가능한 멤버 함수
    void meow() && {
        std::cout << "  " << name << ": Meow... (called on rvalue object)\n";
    }
};

// 함수 오버로딩을 통한 참조 구분
void adopt(Cat& cat) {
    std::cout << "-> [adopt(Cat&)] lvalue reference: " << cat.name << " adopted\n";
}

void adopt(Cat&& cat) {
    std::cout << "-> [adopt(Cat&&)] rvalue reference: " << cat.name << " adopted\n";
}

int main() {
    std::cout << "=== 1. Identifying Value Categories ===\n";
    Cat nabi{"Nabi"}; // nabi는 이름이 있는 변수 -> lvalue

    PRINT_VALUE_CATEGORY(nabi);              // lvalue
    PRINT_VALUE_CATEGORY(Cat{"Calico"});     // prvalue
    PRINT_VALUE_CATEGORY(std::move(nabi));   // xvalue

    std::cout << "\n=== 2. lvalue Reference vs rvalue Reference Overloading ===\n";
    Cat nero{"Nero"};

    adopt(nero);             // lvalue 전달 -> Cat& 호출
    adopt(Cat{"Cheese"});    // prvalue(임시 객체) 전달 -> Cat&& 호출
    adopt(std::move(nero));  // xvalue(std::move) 전달 -> Cat&& 호출

    std::cout << "\n=== 3. C++ Ref-qualifiers (Member Function Call) ===\n";
    Cat choco{"Choco"};

    choco.meow();            // choco는 lvalue -> meow() & 호출
    Cat{"Persian"}.meow();   // 임시 객체는 rvalue -> meow() && 호출
    std::move(choco).meow(); // xvalue 상태 -> meow() && 호출

    return 0;
}
