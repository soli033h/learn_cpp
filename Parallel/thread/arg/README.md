# `std::thread`에 인자 전달하기

`std::thread`는 첫 번째 인자로 실행할 함수(또는 함수 객체)를 받고, 그 뒤에 함수에 전달할 인자를 받는다.

```cpp
std::thread worker(function, argument1, argument2);
```

일반 함수 호출이 다음과 같다면:

```cpp
function(argument1, argument2);
```

스레드에서는 다음처럼 별도의 스레드에서 실행한다.

```cpp
std::thread worker(function, argument1, argument2);
worker.join();
```

## 1. 값으로 전달하기

기본적으로 인자는 복사되어 스레드에 전달된다.

```cpp
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
```

스레드 함수 안에서 변경한 `score`는 복사본이므로 `main()`의 원본에 영향을 주지 않는다.

```text
worker score: 110
main score: 100
```

## 2. 여러 인자 전달하기

실행할 함수 뒤에 인자를 필요한 순서대로 나열한다.

```cpp
#include <iostream>
#include <thread>

void feed_cat(const char* name, int food_count, bool favorite)
{
    std::cout << name
              << " eats " << food_count
              << " snacks. Favorite: "
              << std::boolalpha << favorite << '\n';
}

int main()
{
    std::thread worker(feed_cat, "Luna", 5, true);
    worker.join();
}
```

이는 다음 일반 함수 호출과 같은 인자 순서를 사용한다.

```cpp
feed_cat("Luna", 5, true);
```

## 3. 참조로 전달하기: `std::ref`

원본 객체를 스레드 함수에서 직접 수정하려면 `std::ref()`를 사용한다.

```cpp
#include <functional>
#include <iostream>
#include <thread>

void increase(int& value)
{
    ++value;
}

int main()
{
    int number = 0;

    std::thread worker(increase, std::ref(number));
    worker.join();

    std::cout << number << '\n'; // 1
}
```

`std::ref(number)`는 `number`를 복사하지 않고 원본을 참조하라는 뜻이다.

다음처럼 작성하면 안 된다.

```cpp
std::thread worker(increase, number); // 컴파일 오류
```

`std::thread`가 `number`를 복사해서 전달하려고 하는데, `increase()`는 `int&`를 요구하기 때문이다.

### 참조와 동기화

여러 스레드가 같은 객체를 수정하면 데이터 경쟁이 발생할 수 있다.

```cpp
int counter = 0;

std::thread t1(increase, std::ref(counter));
std::thread t2(increase, std::ref(counter));
```

이런 경우에는 `std::mutex` 또는 `std::atomic`을 사용해야 한다.

```cpp
#include <atomic>
#include <iostream>
#include <thread>

void increase(std::atomic<int>& value)
{
    ++value;
}

int main()
{
    std::atomic<int> counter = 0;

    std::thread t1(increase, std::ref(counter));
    std::thread t2(increase, std::ref(counter));

    t1.join();
    t2.join();

    std::cout << counter <<'\n';
}
```

## 4. 읽기 전용 참조: `std::cref`

복사하지 않고 `const` 참조로 전달하려면 `std::cref()`를 사용한다.

```cpp
#include <functional>
#include <iostream>
#include <string>
#include <thread>

void print_name(const std::string& name)
{
    std::cout << name << '\n';
}

int main()
{
    std::string name = "Luna";

    std::thread worker(print_name, std::cref(name));
    worker.join();
}
```

참조로 전달한 객체는 스레드가 끝날 때까지 살아 있어야 한다.

## 5. 객체를 이동하기: `std::move`

복사할 수 없는 객체나 소유권을 넘겨야 하는 객체는 `std::move()`를 사용한다.
>> 물론 앞에서 봤던 것처럼 참조를 써도 됨

```cpp
#include <iostream>
#include <memory>
#include <thread>

void use_data(std::unique_ptr<int> data)
{
    std::cout << *data << '\n';
}

int main()
{
    auto data = std::make_unique<int>(42);

    std::thread worker(use_data, std::move(data));
    worker.join();

    // data는 소유권을 잃는다.
}
```

`std::unique_ptr`는 복사할 수 없으므로 다음은 불가능하다.

```cpp
std::thread worker(use_data, data); // 컴파일 오류
```

`std::move(data)`는 `data`의 소유권을 스레드로 이동하겠다는 의미다. 이동 후 원래 `data`는 더 이상 자원을 소유하지 않는다.

## 6. 문자열과 객체의 수명

문자열 리터럴은 프로그램이 실행되는 동안 살아 있으므로 다음 코드는 안전하다.

```cpp
std::thread worker(feed_cat, "Luna");
```

반면, 지역 배열의 주소를 스레드에 전달하면 위험할 수 있다.

```cpp
void print_name(const char* name)
{
    std::cout << name << '\n';
}

std::thread create_thread()
{
    char name[] = "Luna";

    return std::thread(print_name, name); // 위험
}
```

`create_thread()`가 끝나면 `name`이 사라지는데, 스레드가 그 이후에 실행되면 이미 소멸한 메모리를 참조할 수 있다.

문자열 객체를 값으로 전달하면 수명 문제가 줄어든다.

```cpp
#include <string>
#include <thread>

void print_name(std::string name)
{
    // name은 스레드가 소유하는 복사본이다.
}

std::thread create_thread()
{
    std::string name = "Luna";
    return std::thread(print_name, name);
}
```

## 7. 람다에 인자 전달하기

람다를 스레드 함수로 직접 사용할 수 있다.

```cpp
#include <iostream>
#include <string>
#include <thread>

int main()
{
    std::thread worker(
        [](const std::string& name, int food_count)
        {
            std::cout << name << " eats "
                      << food_count << " snacks\n";
        },
        "Luna",
        5
    );

    worker.join();
}
```

외부 변수를 람다로 전달할 때는 값 캡처와 참조 캡처의 차이를 확인해야 한다.

```cpp
int food_count = 5;

std::thread by_value(
    [food_count]()
    {
        // food_count의 복사본을 사용한다.
    }
);

std::thread by_reference(
    [&food_count]()
    {
        // 원본 food_count를 사용한다.
    }
);

by_value.join();
by_reference.join();
```

참조 캡처를 사용하면 캡처한 객체가 스레드보다 오래 살아 있어야 한다. 여러 스레드가 동시에 수정한다면 동기화도 필요하다.

## 8. 멤버 함수에 인자 전달하기

멤버 함수 포인터와 객체를 순서대로 전달한다.

```cpp
#include <iostream>
#include <string>
#include <thread>

class Cat
{
public:
    void feed(const std::string& food)
    {
        std::cout << "Cat eats " << food << '\n';
    }
};

int main()
{
    Cat cat;

    std::thread worker(
        &Cat::feed,
        &cat,
        "fish"
    );

    worker.join();
}
```

구조는 다음과 같다.

```cpp
std::thread worker(
    &ClassName::member_function,
    object_pointer,
    member_function_argument
);
```

객체를 복사하지 않고 원본 객체를 사용하려면 `std::ref()`를 사용할 수도 있다.

```cpp
std::thread worker(&Cat::feed, std::ref(cat), "fish");
```

이 경우에도 `cat`은 스레드가 끝날 때까지 살아 있어야 한다.

## 9. 함수 포인터 전달하기

함수 이름 대신 함수 포인터를 전달할 수도 있다.

```cpp
#include <iostream>
#include <thread>

void print_number(int number)
{
    std::cout << number << '\n';
}

int main()
{
    void (*function_pointer)(int) = print_number;

    std::thread worker(function_pointer, 42);
    worker.join();
}
```

## 10. 전달 방식 요약

| 표현 | 의미 |
| --- | --- |
| `worker(func, value)` | `value`를 복사해서 전달 |
| `worker(func, std::ref(value))` | `value`를 non-const 참조로 전달 |
| `worker(func, std::cref(value))` | `value`를 const 참조로 전달 |
| `worker(func, std::move(value))` | `value`를 이동해서 전달 |
| `worker(func, pointer)` | 포인터가 가리키는 객체를 사용 |

기본 원칙은 다음과 같다.

1. 기본적으로 값 전달을 사용한다.
2. 원본 객체를 수정해야 할 때만 `std::ref()`를 사용한다.
3. 읽기 전용 원본을 복사하고 싶지 않을 때 `std::cref()`를 사용한다.
4. 복사할 수 없거나 소유권을 넘겨야 할 때 `std::move()`를 사용한다.
5. 참조, 포인터, 람다 캡처 대상의 수명이 스레드보다 긴지 확인한다.
6. `std::thread` 객체는 작업이 끝나기 전에 반드시 `join()`하거나, 의도적으로 분리할 때만 `detach()`한다.


## `std::forward`와의 관계

`std::thread`는 전달받은 인자를 저장했다가 새로운 스레드에서 실행할 때 함수에 전달한다. 따라서 내부적으로 인자의 복사, 이동, 값 범주와 관련된 템플릿 기술이 사용된다.

직접 사용하는 입장에서는 다음 세 가지를 우선 기억하면 된다.

```cpp
// 복사
std::thread t1(func, value);

// 참조
std::thread t2(func, std::ref(value));

// 이동
std::thread t3(func, std::move(value));
```
