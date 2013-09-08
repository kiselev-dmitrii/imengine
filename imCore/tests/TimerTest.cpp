#include <imCore/Utils/Timer.h>
#include <iostream>

int main() {
        imCore::Timer timer;

        timer.reset();
        int sum = 0;
        for (int i = 1; i <= 1000000000; ++i) sum += i;
        std::cout << timer.timestamp() << std::endl;

        return 0;
}
