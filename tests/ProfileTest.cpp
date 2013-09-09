#include <imCore/Debug/Debug.h>
#include <random>
#include <algorithm>

using namespace std;

void func() {
        IM_START_PROFILER("func");
        srand(time(0));

        vector<int> vec;
        for (int i=0; i < 1000; ++i)  vec.push_back(rand());
        std::sort(begin(vec), end(vec));

        IM_END_PROFILER("func");
}

int main() {
        IM_START_PROFILER("cycle");
        for (int i=0; i < 100; ++i) {
                func();
        }
        IM_END_PROFILER("cycle");
        return 0;
}
