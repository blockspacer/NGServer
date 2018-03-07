#ifndef MASSGATE_MOD_LOADLUAFILE_H
#define MASSGATE_MOD_LOADLUAFILE_H

#include <map>

namespace LuaState {

std::map<int, std::string> GetLuaPath();

std::string GetLuaRoot();

std::string GetLuaIni();

}

#endif //MASSGATE_MOD_LOADLUAFILE_H
