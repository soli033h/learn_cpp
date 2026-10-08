# Type Traits

Type traits는 컴파일 시간에 타입의 특성을 검사하거나 타입을 변환하기 위한 C++ 표준 라이브러리 기능이다.

주로 템플릿 코드에서 다음과 같은 작업에 사용한다.

- 타입이 정수, 포인터, 배열 또는 클래스인지 확인
- 두 타입이 같은 타입인지 확인
- 타입의 `const`, 참조, 포인터 등의 속성 제거
- 타입의 특성에 따라 다른 코드를 선택
- 템플릿 인자의 조건을 컴파일 시간에 검증

관련 기능은 `<type_traits>` 헤더에서 제공된다.

```cpp
#include <type_traits>
```

## 기본 형태

대부분의 type trait는 타입을 템플릿 인자로 받는 구조체 형태로 제공된다.

```cpp
std::is_integral<int>::value
```

위 표현은 `int`가 정수 타입인지 검사하며, 결과는 `bool` 값이다.

C++17부터는 `_v` 접미사가 붙은 변수 템플릿을 사용할 수 있다.

```cpp
std::is_integral_v<int>
```

두 표현은 같은 의미이다.

```cpp
std::is_integral<int>::value
std::is_integral_v<int>
```

일반적으로 현대 C++ 코드에서는 더 간결한 `_v` 형태를 사용한다.

## 타입 검사

### 정수 타입 검사

`std::is_integral`은 타입이 정수 타입인지 검사한다.

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
void print_category(const T& value)
{
    if constexpr (std::is_integral_v<T>)
    {
        std::cout << value << " is an integral type\n";
    }
    else
    {
        std::cout << value << " is not an integral type\n";
    }
}

int main()
{
    print_category(10);
    print_category(3.14);
}
```

`int`, `short`, `long`, `long long`, `char`, `bool` 등은 정수 타입으로 분류된다. 부동 소수점 타입인 `float`, `double`은 정수 타입이 아니다.

### 부동 소수점 타입 검사

```cpp
std::is_floating_point_v<float>  // true
std::is_floating_point_v<double> // true
std::is_floating_point_v<int>    // false
```

### 포인터와 배열 검사

```cpp
std::is_pointer_v<int*>       // true
std::is_pointer_v<int>        // false
std::is_array_v<int[3]>       // true
std::is_array_v<int*>         // false
```

### 클래스와 열거형 검사

```cpp
struct User
{
};

enum class Color
{
    Red,
    Blue
};

static_assert(std::is_class_v<User>);
static_assert(std::is_enum_v<Color>);
```

### 함수 타입과 호출 가능 타입 검사

```cpp
int add(int left, int right)
{
    return left + right;
}

static_assert(std::is_function_v<decltype(add)>);
static_assert(std::is_invocable_v<decltype(add), int, int>);
```

`std::is_invocable_v`는 주어진 타입과 인자를 사용하여 호출 가능한지 검사한다. 함수, 함수 객체, 람다를 템플릿에서 다룰 때 유용하다.

## 타입 비교

`std::is_same`은 두 타입이 동일한지 검사한다.

```cpp
#include <type_traits>

static_assert(std::is_same_v<int, int>);
static_assert(!std::is_same_v<int, double>);
```

`const`와 참조도 타입의 일부이므로 다음 결과에 주의해야 한다.

```cpp
static_assert(!std::is_same_v<int, const int>);
static_assert(!std::is_same_v<int, int&>);
```

템플릿 타입 추론 결과를 확인할 때는 `std::remove_cvref_t` 또는 `std::remove_reference_t`를 함께 사용할 수 있다.

```cpp
using Value = const int&;

static_assert(std::is_same_v<std::remove_cvref_t<Value>, int>);
```

`std::remove_cvref_t`는 C++20에서 제공되며, `const`, `volatile`, 참조를 제거한다.

<!-- TODO: 2026.10.08 -->

## 타입 변환

Type traits는 타입의 속성을 검사할 뿐 아니라 새로운 타입을 만들 수도 있다. C++14부터는 대부분의 변환 trait에 `_t` 접미사가 붙은 별칭을 사용할 수 있다.

### 참조 제거

```cpp
using Reference = int&;
using Value = std::remove_reference_t<Reference>;

static_assert(std::is_same_v<Value, int>);
```

`std::remove_reference_t<T>`는 `T&`와 `T&&`에서 참조를 제거한다.

```cpp
using LValue = std::remove_reference_t<int&>;  // int
using RValue = std::remove_reference_t<int&&>; // int
```

### `const`와 `volatile` 제거

```cpp
using Qualified = const volatile int;
using Unqualified = std::remove_cv_t<Qualified>;

static_assert(std::is_same_v<Unqualified, int>);
```

`cv`는 `const`와 `volatile`을 함께 가리키는 표현이다.

### 포인터와 배열의 변환

```cpp
using Pointer = std::add_pointer_t<int>;
using Array = std::add_const_t<int[3]>;

static_assert(std::is_same_v<Pointer, int*>);
static_assert(std::is_same_v<Array, const int[3]>);
```

### `std::decay`

`std::decay_t`는 값을 전달하는 것과 유사한 형태로 타입을 변환한다. 일반적으로 다음 변환이 수행된다.

- 배열을 포인터로 변환
- 함수 타입을 함수 포인터로 변환
- 참조 제거
- 최상위 `const`와 `volatile` 제거

```cpp
using Array = int[3];
using Decayed = std::decay_t<Array>;

static_assert(std::is_same_v<Decayed, int*>);
```

`std::decay_t`는 함수 템플릿에서 타입을 값으로 저장하거나, 타입 추론 결과를 일반적인 값 타입으로 정규화할 때 사용할 수 있다. 다만 참조와 배열의 특성을 보존해야 하는 경우에는 무조건 사용하지 않아야 한다.

## 복사와 이동 특성 검사

객체의 생성 및 대입 가능 여부도 검사할 수 있다.

```cpp
#include <memory>
#include <type_traits>

static_assert(std::is_copy_constructible_v<int>);
static_assert(std::is_move_constructible_v<std::unique_ptr<int>>);
static_assert(!std::is_copy_constructible_v<std::unique_ptr<int>>);
```

주요 trait는 다음과 같다.

```cpp
std::is_default_constructible_v<T>
std::is_copy_constructible_v<T>
std::is_move_constructible_v<T>
std::is_copy_assignable_v<T>
std::is_move_assignable_v<T>
std::is_destructible_v<T>
```

이러한 trait는 템플릿이 요구하는 타입의 조건을 미리 확인할 때 유용하다.

## 타입의 관계 검사

### 기본 클래스 관계

```cpp
struct Animal
{
};

struct Cat : Animal
{
};

static_assert(std::is_base_of_v<Animal, Cat>);
```

`std::is_base_of_v<Base, Derived>`는 첫 번째 타입이 두 번째 타입의 기본 클래스인지 검사한다.

### 변환 가능 여부

```cpp
static_assert(std::is_convertible_v<int, double>);
static_assert(!std::is_convertible_v<double*, int*>);
```

`std::is_convertible_v<From, To>`는 `From` 타입의 값이 `To` 타입으로 변환 가능한지 검사한다.

### 대입 가능 여부

```cpp
static_assert(std::is_assignable_v<int&, int>);
static_assert(!std::is_assignable_v<const int&, int>);
```

## `if constexpr`와 함께 사용하기

Type traits는 `if constexpr`와 함께 사용할 때 템플릿 인자의 특성에 따라 서로 다른 코드를 선택하는 데 사용할 수 있다.

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
void print_value(const T& value)
{
    if constexpr (std::is_integral_v<T>)
    {
        std::cout << "integer: " << value << '\n';
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
        std::cout << "floating point: " << value << '\n';
    }
    else
    {
        std::cout << "other type\n";
    }
}
```

`if constexpr`에서 선택되지 않은 분기는 해당 템플릿 인스턴스에 대해 컴파일되지 않는다. 따라서 타입에 따라 유효한 연산이 다를 때 조건부 코드를 안전하게 작성할 수 있다.

## `std::enable_if`

`std::enable_if`는 조건이 참일 때만 특정 템플릿을 활성화하는 기능이다.

```cpp
#include <type_traits>

template <
    typename T,
    typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
void process(T value)
{
    // 정수 타입에 대해서만 활성화된다.
}
```

`std::enable_if`는 C++11부터 사용할 수 있지만, 문법이 복잡하고 오류 메시지가 이해하기 어려울 수 있다. C++17 이후에는 `if constexpr`, C++20 이후에는 `requires`와 Concepts를 우선적으로 사용하는 것이 일반적이다.

## Concepts와의 관계

Type traits는 개별 타입의 특성을 검사하는 저수준 도구이고, Concepts는 템플릿이 요구하는 조건을 이름 있는 제약으로 표현하는 기능이다.

```cpp
#include <concepts>
#include <type_traits>

template <typename T>
concept Integral = std::is_integral_v<T>;

Integral auto increment(Integral auto value)
{
    return value + 1;
}
```

C++20에서는 표준 Concepts가 제공되는 경우 이를 우선 사용할 수 있다.

```cpp
#include <concepts>

std::integral auto increment(std::integral auto value)
{
    return value + 1;
}
```

Type traits가 제공하는 세부적인 조건을 조합해야 할 때는 직접 Concept를 정의하여 사용할 수 있다.

```cpp
#include <type_traits>

template <typename T>
concept SmallIntegral =
    std::is_integral_v<T> && (sizeof(T) <= sizeof(int));
```

## `static_assert`와 함께 사용하기

`static_assert`를 사용하면 타입 조건을 컴파일 시간에 검증할 수 있다.

```cpp
#include <type_traits>

template <typename T>
void store_value(T value)
{
    static_assert(
        std::is_copy_constructible_v<T>,
        "T must be copy constructible");

    // 값 저장 로직
}
```

조건을 만족하지 않는 타입으로 함수를 인스턴스화하면 컴파일 오류가 발생한다. 런타임 검사보다 이른 시점에 오류를 발견할 수 있다는 장점이 있다.

## 주의 사항

### 타입의 세부 속성을 확인해야 한다

`std::is_integral_v<T>`와 같은 trait는 `T`를 그대로 검사한다. 참조나 `const`가 포함된 타입을 검사할 때는 의도한 결과인지 확인해야 한다.

```cpp
using Type = const int&;

static_assert(!std::is_integral_v<Type>);
static_assert(std::is_integral_v<std::remove_cvref_t<Type>>);
```

### trait가 타입의 모든 사용 가능성을 보장하지는 않는다

어떤 trait가 `true`라고 해서 타입을 특정 방식으로 사용하는 모든 표현식이 유효하다는 의미는 아니다. 실제 연산 가능 여부, 접근 권한, 오버로드 해결 결과 등은 별도로 확인해야 한다.

### 과도한 사용을 피해야 한다

단순한 템플릿 제약을 type traits로 복잡하게 표현하면 코드 가독성과 오류 메시지가 나빠질 수 있다. C++20을 사용할 수 있다면 의미 있는 이름의 Concept를 정의하는 편이 더 명확하다.

## 요약

Type traits는 타입을 실행 시간에 저장하거나 검사하는 기능이 아니라, **컴파일 시간에 타입 정보를 다루는 도구**이다.

주요 사용 패턴은 다음과 같다.

```cpp
#include <type_traits>

template <typename T>
void inspect(T&& value)
{
    using Type = std::remove_cvref_t<T>;

    if constexpr (std::is_integral_v<Type>)
    {
        // 정수 타입 처리
    }
    else if constexpr (std::is_floating_point_v<Type>)
    {
        // 부동 소수점 타입 처리
    }
    else
    {
        // 그 밖의 타입 처리
    }
}
```

핵심 내용은 다음과 같다.

1. `<type_traits>` 헤더에서 타입 검사와 타입 변환 기능을 제공한다.
2. `_v` 형태는 trait의 `value`를 간결하게 사용하는 방법이다.
3. `_t` 형태는 trait의 `type`을 간결하게 사용하는 방법이다.
4. `std::is_same`, `std::is_integral`, `std::is_pointer` 등으로 타입의 특성을 검사할 수 있다.
5. `std::remove_reference`, `std::remove_cvref`, `std::decay` 등으로 타입을 변환할 수 있다.
6. `if constexpr`와 `static_assert`를 사용하면 타입 조건에 따른 컴파일 시간 처리가 가능하다.
7. C++20에서는 복잡한 제약을 Concepts와 `requires`로 표현하는 것이 더 명확할 수 있다.

