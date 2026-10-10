# `constexpr`

`constexpr`는 변수, 함수 또는 생성자를 **컴파일 시간에 계산할 수 있는 대상**으로 선언하는 키워드이다.

컴파일 시간에 계산 가능한 값은 프로그램이 실행되기 전에 계산되므로 런타임 계산을 줄일 수 있다. 그러나 `constexpr`가 지정된 함수라고 해서 항상 컴파일 시간에 실행되는 것은 아니다. 인수가 컴파일 시간에 결정된 값이면 컴파일 시간에 계산할 수 있고, 실행 중에 결정되는 값이면 런타임에 실행된다.

## `const`와 `constexpr`의 차이

`const`는 초기화 이후 값을 변경할 수 없음을 의미한다. `const` 변수의 값이 반드시 컴파일 시간에 결정되는 것은 아니다.

```cpp
int get_value()
{
    return 10;
}

const int runtime_value = get_value();
```

위 코드에서 `runtime_value`는 변경할 수 없지만 `get_value()`는 프로그램 실행 중에 호출될 수 있다.

반면 `constexpr` 변수는 컴파일 시간에 값을 계산할 수 있어야 한다.

```cpp
constexpr int compile_time_value = 10;
```

다음 코드는 `get_value()`가 `constexpr` 함수가 아니므로 컴파일 오류가 발생한다.

```cpp
constexpr int value = get_value();
```

두 키워드의 차이는 다음과 같다.

| 키워드 | 의미 |
| --- | --- |
| `const` | 초기화 이후 값을 변경할 수 없음 |
| `constexpr` | 컴파일 시간에 계산 가능한 상수여야 함 |

## `constexpr` 변수

`constexpr` 변수는 반드시 컴파일 시간에 결정되는 값으로 초기화해야 한다.

```cpp
constexpr int buffer_size = 1024;

static_assert(buffer_size > 0);
```

`constexpr` 변수는 상수이므로 이후에 값을 변경할 수 없다.

```cpp
constexpr int value = 10;

// value = 20; // 컴파일 오류
```

컴파일 시간 상수가 필요한 배열의 크기나 템플릿 인자로도 사용할 수 있다.

```cpp
constexpr int size = 5;

int values[size];
```

## `constexpr` 함수

함수에 `constexpr`를 지정하면 함수 호출 결과를 컴파일 시간에 계산할 수 있다.

```cpp
constexpr int square(int value)
{
    return value * value;
}

constexpr int result = square(5);

static_assert(result == 25);
```

이 예제에서 `square(5)`의 결과는 컴파일 시간에 계산된다. 따라서 `result`는 컴파일 시간 상수이며, `static_assert`를 사용하여 그 결과를 컴파일 시간에 검증할 수 있다.

## 컴파일 시간과 런타임 호출

`constexpr` 함수는 컴파일 시간과 런타임 모두에서 호출할 수 있다.

```cpp
constexpr int square(int value)
{
    return value * value;
}

constexpr int compile_time_result = square(5);

int input;
std::cin >> input;

int runtime_result = square(input);
```

`compile_time_result`는 인수인 `5`가 컴파일 시간에 알려져 있으므로 컴파일 시간에 계산된다. 반면 `runtime_result`는 사용자의 입력에 따라 실행 중에 결정되므로 런타임에 계산된다.

따라서 `constexpr` 함수는 컴파일 시간 계산과 런타임 계산을 모두 지원하는 함수라고 이해할 수 있다.

## `constexpr` 함수의 반복문과 조건문

현대 C++에서는 `constexpr` 함수 안에 조건문과 반복문을 작성할 수 있다.

```cpp
constexpr int factorial(int value)
{
    int result = 1;

    for (int i = 1; i <= value; ++i)
    {
        result *= i;
    }

    return result;
}

constexpr int value = factorial(5);

static_assert(value == 120);
```

이 함수는 컴파일 시간에 호출될 수 있으며, `factorial(5)`의 결과는 `120`으로 계산된다.

## `constexpr` 생성자와 객체

생성자와 멤버 함수에도 `constexpr`를 지정할 수 있다. 이를 이용하면 객체를 컴파일 시간에 생성하고 사용할 수 있다.

```cpp
class Point
{
private:
    int x;
    int y;

public:
    constexpr Point(int x, int y)
        : x(x), y(y)
    {
    }

    constexpr int x_value() const
    {
        return x;
    }

    constexpr int y_value() const
    {
        return y;
    }
};

constexpr Point point(3, 4);

static_assert(point.x_value() == 3);
static_assert(point.y_value() == 4);
```

이 방식은 템플릿 매개변수, 고정된 설정값, 컴파일 시간 검증이 필요한 작은 객체를 구성할 때 사용할 수 있다.

TODO: 2026.10.08 

## `std::string_view`와 `constexpr`

문자열 리터럴은 `std::string_view`와 함께 컴파일 시간 상수로 표현할 수 있다.

```cpp
#include <string_view>

constexpr std::string_view message = "hello";
```

`std::string_view`는 문자열을 소유하지 않고 기존 문자열을 바라보는 타입이다. 문자열 리터럴은 프로그램 전체 수명을 가지므로 위와 같은 사용은 안전하다.

다만 `std::string_view`를 지역 변수나 임시 객체의 문자열에 연결하면 원본 문자열이 먼저 소멸하여 유효하지 않은 뷰가 될 수 있다. `constexpr`는 객체의 수명 문제나 포인터의 안전성을 자동으로 해결하지 않는다.

## `if constexpr`

`constexpr`와 `if constexpr`는 서로 다른 기능이다.

- `constexpr`: 값이나 함수 호출을 컴파일 시간에 계산할 수 있도록 한다.
- `if constexpr`: 템플릿에서 컴파일 시간 조건에 따라 사용할 코드를 선택한다.

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
void print_type(const T& value)
{
    if constexpr (std::is_integral_v<T>)
    {
        std::cout << "integer: " << value << '\n';
    }
    else
    {
        std::cout << "other: " << value << '\n';
    }
}
```

`print_type(10)`을 호출하면 정수에 해당하는 분기가 선택되고, `print_type(3.14)`를 호출하면 다른 분기가 선택된다. 선택되지 않은 분기는 해당 템플릿 인스턴스에서 사용되지 않는다.

## `consteval`과의 차이

C++20부터 제공되는 `consteval`은 함수를 반드시 컴파일 시간에 호출하도록 강제한다.

```cpp
consteval int square_at_compile_time(int value)
{
    return value * value;
}

constexpr int result = square_at_compile_time(5);
```

실행 중에 결정되는 값을 `consteval` 함수에 전달하면 컴파일 오류가 발생한다.

```cpp
int input;
std::cin >> input;

// int result = square_at_compile_time(input); // 컴파일 오류
```

두 키워드의 차이는 다음과 같다.

| 키워드 | 컴파일 시간 호출 | 런타임 호출 |
| --- | --- | --- |
| `constexpr` | 가능 | 가능 |
| `consteval` | 반드시 필요 | 불가능 |

일반적으로 컴파일 시간과 런타임 양쪽에서 사용할 수 있는 함수에는 `constexpr`를 사용하고, 반드시 컴파일 시간에 계산되어야 하는 함수에는 `consteval`을 사용한다.

## 사용 목적

`constexpr`는 다음과 같은 목적으로 사용할 수 있다.

- 고정된 수식이나 설정값을 컴파일 시간에 계산
- 배열 크기와 템플릿 인자에 사용할 상수 정의
- `static_assert`를 사용한 컴파일 시간 검증
- 컴파일 시간에 객체 구성
- 값이 변경되지 않는다는 의도 표현

`constexpr`를 사용한다고 해서 항상 눈에 띄는 성능 향상이 발생하는 것은 아니다. 현대 컴파일러는 일반적인 코드도 최적화할 수 있기 때문이다. 따라서 `constexpr`는 단순한 성능 최적화 도구보다는 **컴파일 시간에 계산하고 검증할 수 있는 코드의 의도를 표현하는 기능**으로 이해하는 것이 적절하다.

## 요약

```cpp
#include <iostream>

constexpr int square(int value)
{
    return value * value;
}

int main()
{
    constexpr int result = square(5);

    static_assert(result == 25);

    std::cout << result << '\n';
}
```

위 예제의 핵심은 다음과 같다.

1. `constexpr` 변수는 컴파일 시간에 결정되는 상수이다.
2. `constexpr` 함수는 컴파일 시간과 런타임 모두에서 호출할 수 있다.
3. `static_assert`는 컴파일 시간에 조건을 검증한다.
4. `if constexpr`는 템플릿에서 컴파일 시간 분기를 수행한다.
5. `consteval`은 반드시 컴파일 시간에 호출되어야 하는 함수를 정의한다.

## QnA

### Q. `constexpr` 함수는 런타임에도 호출할 수 있는데, 왜 사용하는가?

`constexpr` 함수는 컴파일 시간에 계산할 수 있는 상황과 런타임에 계산해야 하는 상황을 모두 지원하기 때문에 사용한다.

일반 함수는 런타임 호출만 가능하지만, `constexpr` 함수는 인수가 컴파일 시간에 알려져 있으면 컴파일 시간에 호출할 수 있다. 따라서 `static_assert`, 배열 크기, 템플릿 인자처럼 컴파일 시간 상수가 필요한 위치에서도 사용할 수 있다.

```cpp
constexpr int square(int value)
{
    return value * value;
}

constexpr int compile_time_result = square(5);
static_assert(compile_time_result == 25);
```

반면 인수가 실행 중에 결정되면 동일한 함수를 런타임에 호출할 수 있다.

```cpp
int input;
std::cin >> input;

int runtime_result = square(input);
```

따라서 `constexpr`는 함수를 항상 컴파일 시간에 실행하라는 의미가 아니다. 다음과 같은 가능성을 추가하는 기능이다.

> 입력이 컴파일 시간에 알려져 있다면 미리 계산할 수 있도록 하고, 입력이 실행 중에 결정되면 일반 함수처럼 런타임에 호출할 수 있도록 한다.

이 특성 덕분에 컴파일 시간용 함수와 런타임용 함수를 별도로 작성하지 않아도 되며, 컴파일 시간 검증과 템플릿 계산에도 같은 함수를 사용할 수 있다.

### Q. `constexpr`를 사용하면 항상 성능이 향상되는가?

항상 그런 것은 아니다. 현대 컴파일러는 일반 함수 호출도 최적화할 수 있으므로, `constexpr`를 사용했다고 해서 런타임 성능이 반드시 향상되는 것은 아니다.

`constexpr`의 핵심적인 장점은 다음과 같다.

- 컴파일 시간에 계산 가능한 값을 미리 계산할 수 있다.
- `static_assert`를 사용하여 결과를 컴파일 시간에 검증할 수 있다.
- 배열 크기와 템플릿 인자처럼 컴파일 시간 상수가 필요한 위치에서 사용할 수 있다.
- 코드가 컴파일 시간 계산을 지원한다는 의도를 표현할 수 있다.
