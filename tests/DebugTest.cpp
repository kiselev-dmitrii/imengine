#include <GL/glew.h>
#include <imCore/Utils/Debug.h>
#include <imCore/Utils/Types.h>

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

        return 0;
}
