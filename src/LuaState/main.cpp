#include <iostream>
#include <fstream>

#include <lua.hpp>
#include <glog/logging.h>
#include <gtest/gtest.h>

#include "LoadLuaFile.h"

namespace LuaState {

// Basic error handle
void Error(lua_State *l,
           const char *fmt,
           ...) {
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(l);
    exit(EXIT_FAILURE);
}

// Print all stack value
// stack depth is increase, eg: 1,2,3,4,5,6,7...
void StackDump(lua_State *l) {
    int i;
    int top = lua_gettop(l); /* depth of the stack */
    for (i = 1; i <= top; i++) { /* repeat for each level */
        int t = lua_type(l, i);
        switch (t) {
            case LUA_TSTRING: { /* strings */
                printf("'%s'", lua_tostring(l, i));
                break;
            }
            case LUA_TBOOLEAN: { /* Booleans */
                printf(lua_toboolean(l, i) ? "true" : "false");
                break;
            }
            case LUA_TNUMBER: { /* numbers */
                if (lua_isinteger(l, i)) /* integer? */
                    printf("%lld", lua_tointeger(l, i));
                else /* float */
                    printf("%g", lua_tonumber(l, i));
                break;
            }
            default: { /* other values */
                printf("%s", lua_typename(l, t));
                break;
            }
        }
        printf(" "); /* put a separator */
    }
    printf("\n"); /* end the listing */
}

// A bare-bones stand-alone Lua interpreter
int BasicLuaInterpreter() {
    char buff[256];
    int error;
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);
    while (fgets(buff, sizeof(buff), stdin) != nullptr) {
        error = luaL_loadstring(l, buff) || lua_pcall(l, 0, 0, 0);
        if (error) {
            fprintf(stderr, "%s\n", lua_tostring(l, -1));
            lua_pop(l, 1);
        }
    }
    lua_close(l);
    return 0;
}

// Generic call lua function
// e.g.: call_va("f", "dd>d", x, y, &z);
void Call_va(lua_State *l,
             const char *func,
             const char *sig, ...) {
    va_list vl;
    int narg, nres; /* number of arguments and results */

    va_start(vl, sig);
    lua_getglobal(l, func); /* push function */

    // Pushing arguments for the generic call function
    for (narg = 0; *sig; narg++) { /* repeat for each argument */
        luaL_checkstack(l, 1, "too many arguments"); /* check stack space */
        switch (*sig++) {
            case 'd': /* double argument */
                lua_pushnumber(l, va_arg(vl, double));
                break;
            case 'i': /* int argument */
                lua_pushinteger(l, va_arg(vl, int));
                break;
            case 's': /* string argument */
                lua_pushstring(l, va_arg(vl, char *));
                break;
            case '>': /* end of arguments */
                goto endargs; /* break the loop */
            default:
                Error(l, "invalid option (%c)", *(sig - 1));
        }
    }

    endargs:
    nres = (int) strlen(sig); /* number of expected results */
    if (lua_pcall(l, narg, nres, 0) != 0) { /* do the call */
        Error(l, "error calling 1 '%s': %s",
              func, lua_tostring(l, -1));
    }

    // Retrieving results for the generic call function
    nres = -nres; /* stack index of first result */
    while (*sig) { /* repeat for each result */
        switch (*sig++) {
            case 'd': { /* double result */
                int isnum;
                double n = lua_tonumberx(l, nres, &isnum);
                if (!isnum) { Error(l, "wrong result type number"); }
                *va_arg(vl, double *) = n;
                break;
            }
            case 'i': { /* int result */
                int isnum;
                auto n = (int) lua_tointegerx(l, nres, &isnum);
                if (!isnum) { Error(l, "wrong result type integer"); }
                *va_arg(vl, int *) = n;
                break;
            }
            case 's': { /* string result */
                const char *s = lua_tostring(l, nres);
                if (s == nullptr) { Error(l, "wrong result type string"); }
                *va_arg(vl, const char **) = s;
                break;
            }
            default:
                Error(l, "invalid option (%c)", *(sig - 1));
        }
        nres++;
    }

    va_end(vl);

}

// C Function, return num * 2
static int C_Func(lua_State *l) {
    double d = luaL_checknumber(l, 1);  /*` get argument */
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

static int C_AddOne(lua_State *l) {
    double d = luaL_checknumber(l, 1);  /* get argument */
    lua_pushnumber(l, d + 1);           /* push result */
    return 1;                           /* number of results */
}

// return a, b, a+b
static int C_GetSum(lua_State *l) {
    auto a = (int) luaL_checkinteger(l, 1);
    auto b = (int) luaL_checkinteger(l, 2);
    auto sum = a + b;
    lua_pushnumber(l, a);
    lua_pushnumber(l, b);
    lua_pushnumber(l, sum);
    return 3;
}

TEST(LuaState, stack_value) {
    lua_State *l = luaL_newstate();
    luaL_openlibs(l); // opens all standard libraries

    lua_pushnil(l);             // nil
    lua_pushboolean(l, false);  // bool
    lua_pushnumber(l, 1.2345);  // float
    lua_pushinteger(l, 123);    // int
    lua_pushlstring(l, "hello world length", 18);   // string length
    lua_pushstring(l, "hello world string long");   // string

    ASSERT_EQ(lua_toboolean(l, 1), false);
    ASSERT_EQ(lua_toboolean(l, -6), false);

    ASSERT_EQ(lua_toboolean(l, 2), false);
    ASSERT_EQ(lua_toboolean(l, -5), false);

    ASSERT_EQ(lua_tonumber(l, 3), 1.2345);
    ASSERT_EQ(lua_tonumber(l, -4), 1.2345);

    ASSERT_EQ(lua_tointeger(l, 4), 123);
    ASSERT_EQ(lua_tointeger(l, -3), 123);

    size_t size1;
    ASSERT_STREQ(lua_tolstring(l, 5, &size1), "hello world length");
    ASSERT_EQ(size1, 18);
    ASSERT_STREQ(lua_tolstring(l, -2, &size1), "hello world length");
    ASSERT_EQ(size1, 18);

    size_t size2;
    ASSERT_STREQ(lua_tolstring(l, 6, &size2), "hello world string long");
    ASSERT_EQ(size2, 23);
    ASSERT_STREQ(lua_tolstring(l, -1, &size2), "hello world string long");
    ASSERT_EQ(size2, 23);
}

TEST(LuaState, stack_size) {
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);
    ASSERT_EQ(lua_checkstack(l, 20), 1);
}

// lua call c function, use lua_register
TEST(LuaState, C_GetSum) {
    int error;
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);

    lua_register(l, "C_GetSum", C_GetSum);

    auto *buffer = (char *) "a,b,sum = C_GetSum(10, 100)";
    error = luaL_loadstring(l, buffer) || lua_pcall(l, 0, 0, 0);
    if (error) {
        LOG(ERROR) << lua_tostring(l, -1);
        lua_pop(l, 1); /* pop error message from the stack */
    }

    lua_getglobal(l, "a");
    lua_getglobal(l, "b");
    lua_getglobal(l, "sum");

    ASSERT_EQ(lua_tointeger(l, 1), 10);
    ASSERT_EQ(lua_tointeger(l, 2), 100);
    ASSERT_EQ(lua_tointeger(l, 3), 110);

    lua_close(l);
}

// lua call c function, use lua_register
TEST(LuaState, lua_register) {
    int error;
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);

    lua_register(l, "C_AddOne", C_AddOne);

    auto *buffer = (char *) "a = C_AddOne(100)";
    error = luaL_loadstring(l, buffer) || lua_pcall(l, 0, 0, 0);
    if (error) {
        LOG(ERROR) << lua_tostring(l, -1);
        lua_pop(l, 1); /* pop error message from the stack */
    }

    lua_getglobal(l, "a");
    ASSERT_EQ(lua_tointeger(l, 1), 101);

    lua_close(l);
}

// lua call c function, use lua_pushcfunction
TEST(LuaState, lua_pushcfunction) {
    int error;
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);

    lua_pushcfunction(l, C_AddOne);
    lua_setglobal(l, "C_AddOne");

    auto *buffer = (char *) "a = C_AddOne(100)";
    error = luaL_loadstring(l, buffer) || lua_pcall(l, 0, 0, 0);
    if (error) {
        LOG(ERROR) << lua_tostring(l, -1);
        lua_pop(l, 1); /* pop error message from the stack */
    }

    lua_getglobal(l, "a");
    ASSERT_EQ(lua_tointeger(l, -1), 101);

    lua_close(l);
}

// c call lua funtion, 2 param and return 3 value, basic way
TEST(LuaState, call_lua_func) {
    int error;
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);
    auto *buffer = (char *)
            "function l_sum(a, b)           \
                 return a, b, a+b           \
             end";

    error = luaL_loadstring(l, buffer) || lua_pcall(l, 0, 0, 0);
    if (error) {
        LOG(ERROR) << lua_tostring(l, -1);
        lua_pop(l, 1);
    }

    lua_getglobal(l, "l_sum");
    lua_pushinteger(l, 10);
    lua_pushinteger(l, 100);

    lua_pcall(l, 2, 3, 0);

    ptrdiff_t a = luaL_checkinteger(l, -3);
    ptrdiff_t b = luaL_checkinteger(l, -2);
    ptrdiff_t sum = luaL_checkinteger(l, -1);

    ASSERT_EQ(a, 10);
    ASSERT_EQ(b, 100);
    ASSERT_EQ(sum, 110);

    lua_pop(l, 1);
    lua_close(l);
}

// c call lua function, use generic call lua function
TEST(LuaState, call_lua_func_generic) {
    int error;
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);
    auto *buffer = (char *)
            "function sum(a, b)     \
                 return a + b       \
             end";

    error = luaL_loadstring(l, buffer) || lua_pcall(l, 0, 0, 0);
    if (error) {
        LOG(ERROR) << lua_tostring(l, -1);
        lua_pop(l, 1);
    }

    int result;
    Call_va(l, "sum", "ii>i", 3, 4, &result);

    ASSERT_EQ(result, 7);

    lua_close(l);
}

// load lua file by lua.ini
TEST(LuaState, load_lua_ini) {
    int error;
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);

    // load all lua file
    std::string line;
    std::string total_lua_line;

    std::map<int, std::string> lua_map = GetLuaPath();
    std::map<int, std::string>::iterator it;
    for (it = lua_map.begin(); it != lua_map.end(); it++) {
        // LOG(INFO) << it->first << " " << it->second;

        std::ifstream file(GetLuaRoot() + it->second);
        while (std::getline(file, line)) {
            total_lua_line += line + "\n";
        }
        const char *buffer = total_lua_line.c_str();

        // load lua script to lua_state
        error = luaL_loadstring(l, buffer) || lua_pcall(l, 0, 0, 0);
        if (error) {
            LOG(ERROR) << lua_tostring(l, -1);
            lua_pop(l, 1); /* pop error message from the stack */
        }
    }

    int result = 0;
    Call_va(l, "sum", "ii>i", 3, 4, &result);
    ASSERT_EQ(result, 7);

    lua_close(l);
}

// TODO modules load error
TEST(LuaState, C_Modules_Func) {
    int error;
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);

    auto *buffer = (char *)
            "package.cpath = package.cpath  \
             require 'libmylib'";
    error = luaL_loadstring(l, buffer) || lua_pcall(l, 0, 0, 0);
    if (error) {
        LOG(ERROR) << lua_tostring(l, -1);
        lua_pop(l, 1); /* pop error message from the stack */
    }

    lua_close(l);
}

}

GTEST_API_ int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);

    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_colorlogtostderr = true;

    // LuaState::BasicLuaInterpreter();

    return RUN_ALL_TESTS();
}

