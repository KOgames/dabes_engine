#include "parallax_bindings.h"
#include "../core/engine.h"

const char *luab_ParallaxLayer_lib = "dab_parallaxlayer";
const char *luab_ParallaxLayer_metatable = "DaBes.parallaxlayer";

Scripting_num_getter(ParallaxLayer, p_factor);
Scripting_num_setter(ParallaxLayer, p_factor);

int luab_ParallaxLayer_new(lua_State *L) {
    ParallaxLayer_userdata *ud = NULL;
    Engine *engine = luaL_get_engine(L);

    ud = lua_newuserdata(L, sizeof(ParallaxLayer_userdata));
    check(ud != NULL, "Could not make ParralaxLayer userdata");
    ud->p = NULL;

    luaL_getmetatable(L, luab_ParallaxLayer_metatable);
    lua_setmetatable(L, -2);

    const char *texname = lua_tostring(L, -2);
    char *ppath = engine->project_path(texname);
    GfxTexture *tex = Graphics_texture_from_image(engine->graphics, ppath, 0);
    free(ppath);
    check(tex != NULL, "Couldn't load image %s", texname);

    ParallaxLayer *layer = ParallaxLayer_create(tex);

    luaL_register_ud(L, -1, (void **)&ud->p, layer);
    return 1;
error:
    return 0;
}

int luab_ParallaxLayer_close(lua_State *L) {
    ParallaxLayer_userdata *ud = (ParallaxLayer_userdata *)
        luaL_testudata(L, 1, luab_ParallaxLayer_metatable);
    if (ud->p) {
        // TODO: unload texture?
        free(ud->p);
    }
    ud->p = NULL;
    return 0;
}

int luab_ParallaxLayer_p_cascade(lua_State *L) {
    ParallaxLayer *layer = luaL_toparallaxlayer(L, 1);
    check(layer != NULL, "ParallaxLayer required");

    double top = lua_tonumber(L, 2);
    double bot = lua_tonumber(L, 3);

    ParallaxLayer_p_cascade(layer, top, bot);

    return 0;
error:
    return 0;
}

Scripting_VPoint_getter(ParallaxLayer, offset);
Scripting_VPoint_setter(ParallaxLayer, offset);
Scripting_GfxSize_getter(ParallaxLayer, texture_size);

Scripting_num_getter(ParallaxLayer, y_wiggle);
Scripting_num_setter(ParallaxLayer, y_wiggle);
Scripting_num_getter(ParallaxLayer, z);
Scripting_num_setter(ParallaxLayer, z);

static const struct luaL_Reg luab_ParallaxLayer_meths[] = {
    {"__gc", luab_ParallaxLayer_close},
    {"p_cascade", luab_ParallaxLayer_p_cascade},
    {"get_texture_size", luab_ParallaxLayer_get_texture_size},
    {"get_p_factor", luab_ParallaxLayer_get_p_factor},
    {"set_p_factor", luab_ParallaxLayer_set_p_factor},
    {"get_offset", luab_ParallaxLayer_get_offset},
    {"set_offset", luab_ParallaxLayer_set_offset},
    {"get_y_wiggle", luab_ParallaxLayer_get_y_wiggle},
    {"set_y_wiggle", luab_ParallaxLayer_set_y_wiggle},
    {"get_z", luab_ParallaxLayer_get_z},
    {"set_z", luab_ParallaxLayer_set_z},
    {NULL, NULL}
};

static const struct luaL_Reg luab_ParallaxLayer_funcs[] = {
    {"new", luab_ParallaxLayer_new},
    {NULL, NULL}
};

////////////////////////////////////////////////////////////////////////////////

const char *luab_Parallax_lib = "dab_parallax";
const char *luab_Parallax_metatable = "DaBes.parallax";

int luab_Parallax_new(lua_State *L) {
    Parallax_userdata *ud = NULL;
    Engine *engine = luaL_get_engine(L);

    int c = lua_gettop(L);

    ud = lua_newuserdata(L, sizeof(Parallax_userdata));
    check(ud != NULL, "Could not make Parralax userdata");
    ud->p = NULL;

    luaL_getmetatable(L, luab_Parallax_metatable);
    lua_setmetatable(L, -2);

    Parallax *parallax = Parallax_create(engine);

    int i = 0;
    for (i = 0; i < c; i++) {
        ParallaxLayer *layer = luaL_toparallaxlayer(L, 1);
        Parallax_add_layer(parallax, layer);
        lua_remove(L, 1);
    }

    luaL_register_ud(L, -1, (void **)&ud->p, parallax);
    return 1;
error:
    return 0;
}

int luab_Parallax_add_layer(lua_State *L) {
    Parallax *parallax = luaL_toparallax(L, 1);
    check(parallax != NULL, "Parallax required");
    lua_getfield(L, 2, "real");
    ParallaxLayer *layer = luaL_toparallaxlayer(L, -1);
    Parallax_add_layer(parallax, layer);

    return 0;
error:
    return 0;
}

int luab_Parallax_close(lua_State *L) {
    Parallax_userdata *ud = (Parallax_userdata *)
        luaL_testudata(L, 1, luab_Parallax_metatable);
    if (ud->p) {
        Parallax_destroy(ud->p);
    }
    ud->p = NULL;
    return 0;
}

Scripting_VVector4_getter(Parallax, sky_color);
Scripting_VVector4_setter(Parallax, sky_color);

Scripting_VVector4_getter(Parallax, sea_color);
Scripting_VVector4_setter(Parallax, sea_color);

Scripting_num_getter(Parallax, y_wiggle);
Scripting_num_setter(Parallax, y_wiggle);

Scripting_num_getter(Parallax, sea_level);
Scripting_num_setter(Parallax, sea_level);

Scripting_num_getter(Parallax, bg_z);
Scripting_num_setter(Parallax, bg_z);

static const struct luaL_Reg luab_Parallax_meths[] = {
    {"__gc", luab_Parallax_close},
    {"add_layer", luab_Parallax_add_layer},
    {"get_sky_color", luab_Parallax_get_sky_color},
    {"set_sky_color", luab_Parallax_set_sky_color},
    {"get_sea_color", luab_Parallax_get_sea_color},
    {"set_sea_color", luab_Parallax_set_sea_color},
    {"get_y_wiggle", luab_Parallax_get_y_wiggle},
    {"set_y_wiggle", luab_Parallax_set_y_wiggle},
    {"get_sea_level", luab_Parallax_get_sea_level},
    {"set_sea_level", luab_Parallax_set_sea_level},
    {"get_bg_z", luab_Parallax_get_bg_z},
    {"set_bg_z", luab_Parallax_set_bg_z},
    {NULL, NULL}
};

static const struct luaL_Reg luab_Parallax_funcs[] = {
    {"new", luab_Parallax_new},
    {NULL, NULL}
};

int luaopen_dabes_parallax(lua_State *L) {
    luaL_newmetatable(L, luab_ParallaxLayer_metatable);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, luab_ParallaxLayer_meths, 0);
    luaL_newlib(L, luab_ParallaxLayer_funcs);
    lua_pushvalue(L, -1);
    lua_setglobal(L, luab_ParallaxLayer_lib);

    luaL_newmetatable(L, luab_Parallax_metatable);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, luab_Parallax_meths, 0);
    luaL_newlib(L, luab_Parallax_funcs);
    lua_pushvalue(L, -1);
    lua_setglobal(L, luab_Parallax_lib);

    return 1;
}
