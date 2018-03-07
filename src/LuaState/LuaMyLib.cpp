#ifndef MASSGATE_MOD_LUAMYLIB_H
#define MASSGATE_MOD_LUAMYLIB_H

#include <lua.hpp>


static int C_Func(lua_State *l) {
    double d = luaL_checknumber(l, 1);  /* get argument */
    lua_pushnumber(l, d * 2);       /* push result */
    return 1;                       /* number of results */
}

static const struct luaL_Reg libmylib[] = {
        {"C_Func", C_Func},
        {nullptr,  nullptr} /* sentinel */
};

static int luaopen_libmylib(lua_State *l) {
    luaL_newlib(l, libmylib);
    return 1;
}


#endif //MASSGATE_MOD_LUAMYLIB_H
