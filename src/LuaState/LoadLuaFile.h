#ifndef NGSERVER_LOADLUAFILE_H
#define NGSERVER_LOADLUAFILE_H

#include <map>

namespace LuaState {

std::map<int, std::string> GetLuaPath();

std::string GetLuaRoot();

std::string GetLuaIni();

}

#endif //NGSERVER_LOADLUAFILE_H
