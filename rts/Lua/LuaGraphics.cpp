#include "LuaGraphics.h"
#include "LuaInclude.h"
#include "LuaUtils.h"
#include "LuaHandle.h"
#include "Game/Camera.h"
#include "Game/CameraHandler.h"
#include "Game/UI/MiniMap.h"
#include "Rendering/GlobalRendering.h"
#include "System/float3.h"
#include <algorithm>

static float3 screenViewTrans;

static void CheckDrawingEnabled(lua_State* L, const char* caller)
{
    if (!LuaGraphics::IsDrawingEnabled(L))
        luaL_error(L, "%s(): graphics calls can only be used in Draw() call-ins, or while creating display lists", caller);
}

bool LuaGraphics::PushEntries(lua_State* L)
{
    REGISTER_LUA_CFUNC(GetScreenViewTrans);
    REGISTER_LUA_CFUNC(GetViewSizes);
    REGISTER_LUA_CFUNC(GetViewRange);
    REGISTER_LUA_CFUNC(SlaveMiniMap);
    REGISTER_LUA_CFUNC(ConfigMiniMap);
    REGISTER_LUA_CFUNC(SwapBuffers);
    return true;
}

/***
 * @function gl.GetScreenViewTrans
 * Metal: same behavior as OpenGL.
 */
int LuaGraphics::GetScreenViewTrans(lua_State* L)
{
    lua_pushnumber(L, screenViewTrans.x);
    lua_pushnumber(L, screenViewTrans.y);
    lua_pushnumber(L, screenViewTrans.z);
    return 3;
}

/***
 * @function gl.GetViewSizes
 * Metal: identical values returned.
 */
int LuaGraphics::GetViewSizes(lua_State* L)
{
    lua_pushnumber(L, globalRendering->viewSizeX);
    lua_pushnumber(L, globalRendering->viewSizeY);
    return 2;
}

/***
 * @function gl.GetViewRange
 * Metal: identical values returned.
 */
int LuaGraphics::GetViewRange(lua_State* L)
{
    constexpr int minCamType = CCamera::CAMTYPE_PLAYER;
    constexpr int maxCamType = CCamera::CAMTYPE_ACTIVE;
    const CCamera* cam = CCameraHandler::GetCamera(std::clamp(luaL_optint(L, 1, CCamera::CAMTYPE_ACTIVE), minCamType, maxCamType));
    lua_pushnumber(L, cam->GetNearPlaneDist());
    lua_pushnumber(L, cam->GetFarPlaneDist());
    lua_pushnumber(L, globalRendering->minViewRange);
    lua_pushnumber(L, globalRendering->maxViewRange);
    return 4;
}

/***
 * @function gl.SlaveMiniMap
 * Metal: identical semantics.
 */
int LuaGraphics::SlaveMiniMap(lua_State* L)
{
    if (minimap == nullptr)
        return 0;

    minimap->SetSlaveMode(luaL_checkboolean(L, 1));
    return 0;
}

/***
 * @function gl.ConfigMiniMap
 * Metal: identical semantics.
 */
int LuaGraphics::ConfigMiniMap(lua_State* L)
{
    if (minimap == nullptr)
        return 0;

    const int px = luaL_checkint(L, 1);
    const int py = luaL_checkint(L, 2);
    const int sx = luaL_checkint(L, 3);
    const int sy = luaL_checkint(L, 4);
    minimap->SetGeometry(px, py, sx, sy);
    return 0;
}

/***
 * @function gl.SwapBuffers
 * Metal: swaps through the active backend.
 */
int LuaGraphics::SwapBuffers(lua_State* L)
{
    CheckDrawingEnabled(L, __func__);
    if (!CLuaHandle::GetHandle(L)->PersistOnReload())
        return 0;

    globalRendering->SwapBuffers(true, true);
    return 0;
}

