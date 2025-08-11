#ifndef LUA_GRAPHICS_H
#define LUA_GRAPHICS_H

#include "Lua/LuaHandle.h"

struct lua_State;

/** Backend-neutral Lua graphics bindings */
class LuaGraphics {
public:
    static bool PushEntries(lua_State* L);

    static bool IsDrawingEnabled(lua_State* L) { return GetLuaContextData(L)->drawingEnabled; }

    static int GetScreenViewTrans(lua_State* L);
    static int GetViewSizes(lua_State* L);
    static int GetViewRange(lua_State* L);
    static int SlaveMiniMap(lua_State* L);
    static int ConfigMiniMap(lua_State* L);
    static int SwapBuffers(lua_State* L);
};

#endif
