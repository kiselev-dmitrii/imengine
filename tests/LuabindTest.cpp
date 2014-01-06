#include <luabind/luabind.hpp>
#include <imEngine/Utils/Types.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

void render(const String& str) {
        std::cout << "Render: " << str << std::endl;
}

int main() {
        lua_State *L = luaL_newstate();
        luabind::open(L);

        // Биндим функцию render
        luabind::module(L) [
                        luabind::def("render", render)
        ];

        luaL_dostring(L,
                "function add(first, second)\n"
                "       render('Hello, from LUA')"
                "       return first + second\n"
                "end\n"
        );

        IM_VAR(luabind::call_function<int>(L, "add", 2, 3));

        lua_close(L);
}
