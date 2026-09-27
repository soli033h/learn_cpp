FROM gcc:14

WORKDIR /app

COPY . .

RUN mkdir -p /app/bin \
    && g++ -std=c++20 -Wall -Wextra -pedantic /app/value_category/main.cpp -o /app/bin/value_category

CMD ["/app/bin/value_category"]
