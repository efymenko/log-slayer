#include <random>

void foo() {}

void bar() {}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);

    while (true) {
        int random_bit = distrib(gen);
        if (random_bit)
            foo();
        else
            bar();
    }

    return 0;
}

