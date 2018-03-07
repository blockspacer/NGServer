#include "LoadLuaFile.h"

#include <map>
#include <utility>
#include <fstream>
#include <sstream>

#include <gtest/gtest.h>
#include <glog/logging.h>

namespace {

const std::string lua_root = "/home/me/CLionProjects/ngserver/ngserver/LuaState/Lua";
const std::string lua_ini = lua_root + "/lua.ini";

}

namespace LuaState {

std::map<int, std::string> GetLuaPath() {
    std::map<int, std::string> lua_map;
    std::fstream fi(lua_ini);
    std::string line;
    bool flag = false;
    while (std::getline(fi, line)) {
        if (flag) {
            lua_map.insert(std::pair<int, std::string>
                                   (std::stoi(line.substr(0, line.find('='))),
                                    line.substr(line.find('=') + 1)));
        }
        if (line == "[MOD]") { flag = true; }
    }
    fi.close();
    return lua_map;
};

std::string GetLuaRoot() {
    return lua_root;
}

std::string GetLuaIni() {
    return lua_ini;
}

TEST(LuaState, GetLuaPath) {
    std::map<int, std::string> lua_map;
    lua_map = GetLuaPath();

    std::map<int, std::string>::iterator it;
    for (it = lua_map.begin(); it != lua_map.end(); it++) {
        // LOG(INFO) << it->first << " " << it->second;
    }
}

}
