#include <imCore/Utils/Timer.h>
#include <imCore/Debug/Debug.h>
#include <iostream>

int main() {
        imCore::Timer timer;

        timer.reset();
        int sum = 0;
        for (int i = 1; i <= 1000000000; ++i) sum += i;
        IM_SHOW(timer.timestamp());

        IM_SHOW(sum);

        return 0;
}
