#include <GL/glew.h>
#include <imCore/Utils/Debug.h>
#include <imCore/Utils/Types.h>
#include <map>

using namespace imCore;

void foo() {
        IM_TRACE("Nothing to do");
}

void bar() {
        IM_TODO;
}

int main() {
        int a = 10;
        Vec2 vec(10, 30);

        IM_VAR(a);
        IM_VAR(vec);

        foo();
        IM_TRACE("Yeah, it's main");

        bar();

        IM_ERROR("There are not error, just test");

        IM_ASSERT(a == vec.x);

        IM_GLCALL(glGenTextures(1, NULL));


        std::map<String, int> myMap;
        for (int i=0; i < 1000; ++i) myMap.insert(std::pair<String, int>(std::to_string(i), i));
        IM_PROFILE("TEST SEARCH 1 000 000 STRING IN MAP",
                for (int i=0; i < 1000; ++i) {
                        for (int j=0; j < 1000; ++j) {
                                myMap.find(std::to_string(j));
                        }
                }
        )

        return 0;
}
