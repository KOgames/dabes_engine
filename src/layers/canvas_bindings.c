#include "../math/shape_matcher_bindings.h"
#include "canvas_bindings.h"

const char *luab_Canvas_lib = "dab_canvas";
const char *luab_Canvas_metatable = "DaBes.canvas";

int luab_Canvas_new(lua_State *L) {
    Canvas_userdata *ud = NULL;
    Engine *engine = luaL_get_engine(L);

    ud = lua_newuserdata(L, sizeof(Canvas_userdata));
    check(ud != NULL, "Could not make Canvas userdata");
    ud->p = NULL;

    luaL_getmetatable(L, luab_Canvas_metatable);
    lua_setmetatable(L, -2);

    Canvas *canvas = NULL;
    canvas = Canvas_create(engine);
    luaL_register_ud(L, -1, (void **)&ud->p, canvas);
    return 1;
error:
    return 0;
}

int luab_Canvas_close(lua_State *L) {
    Canvas_userdata *ud = (Canvas_userdata *)
        luaL_testudata(L, 1, luab_Canvas_metatable);
    if (ud->p) {
        Canvas_destroy(ud->p);
    }
    ud->p = NULL;
    return 0;
}

int luab_Canvas_empty(lua_State *L) {
    Canvas *canvas = luaL_tocanvas(L, 1);
    check(canvas != NULL, "Canvas required");
    Canvas_empty(canvas);
    return 0;
error:
    return 0;
}

Scripting_num_getter(Canvas, alpha);
Scripting_num_setter(Canvas, alpha);
Scripting_num_getter(Canvas, angle_threshold);
Scripting_num_setter(Canvas, angle_threshold);
Scripting_VVector4_getter(Canvas, angle_color);
Scripting_VVector4_setter(Canvas, angle_color);
Scripting_num_getter(Canvas, angle_z);
Scripting_num_setter(Canvas, angle_z);
Scripting_VVector4_getter(Canvas, bg_color);
Scripting_VVector4_setter(Canvas, bg_color);
Scripting_num_getter(Canvas, bg_z);
Scripting_num_setter(Canvas, bg_z);
Scripting_num_getter(Canvas, distance_threshold);
Scripting_num_setter(Canvas, distance_threshold);
Scripting_VVector4_getter(Canvas, draw_color);
Scripting_VVector4_setter(Canvas, draw_color);
Scripting_num_getter(Canvas, draw_z);
Scripting_num_setter(Canvas, draw_z);
Scripting_num_getter(Canvas, draw_width);
Scripting_num_setter(Canvas, draw_width);
Scripting_VVector4_getter(Canvas, simplified_path_color);
Scripting_VVector4_setter(Canvas, simplified_path_color);
Scripting_num_getter(Canvas, simplified_path_z);
Scripting_num_setter(Canvas, simplified_path_z);

Scripting_bool_getter(Canvas, enabled);

int luab_Canvas_set_enabled(lua_State *L) {
    int enabled = lua_toboolean(L, 2);
    Canvas *canvas = luaL_tocanvas(L, 1);
    Engine *engine = luaL_get_engine(L);
    check(canvas != NULL, "Canvas required");
    Canvas_set_enabled(canvas, engine, enabled);

    return 0;
error:
    return 0;
}

int luab_Canvas_get_shape_matcher(lua_State *L) {
    Canvas *canvas = luaL_tocanvas(L, 1);
    check(canvas != NULL, "Canvas required");
    if (canvas->shape_matcher == NULL) {
        lua_pushnil(L);
        return 1;
    }

    luaL_lookup_instance(L, canvas->shape_matcher);
    return 1;
error:
    return 0;
}

int luab_Canvas_set_shape_matcher(lua_State *L) {
    Canvas *canvas = luaL_tocanvas(L, 1);
    check(canvas != NULL, "Canvas required");

    ShapeMatcher *matcher = luaL_toshapematcher(L, -1);
    canvas->shape_matcher = matcher;

error:
    return 0;
}

static const struct luaL_Reg luab_Canvas_meths[] = {
    {"__gc", luab_Canvas_close},
    {"empty", luab_Canvas_empty},
    {"get_alpha", luab_Canvas_get_alpha},
    {"set_alpha", luab_Canvas_set_alpha},
    {"get_angle_threshold", luab_Canvas_get_angle_threshold},
    {"set_angle_threshold", luab_Canvas_set_angle_threshold},
    {"get_angle_color", luab_Canvas_get_angle_color},
    {"set_angle_color", luab_Canvas_set_angle_color},
    {"get_angle_z", luab_Canvas_get_angle_z},
    {"set_angle_z", luab_Canvas_set_angle_z},
    {"get_bg_color", luab_Canvas_get_bg_color},
    {"set_bg_color", luab_Canvas_set_bg_color},
    {"get_bg_z", luab_Canvas_get_bg_z},
    {"set_bg_z", luab_Canvas_set_bg_z},
    {"get_distance_threshold", luab_Canvas_get_distance_threshold},
    {"set_distance_threshold", luab_Canvas_set_distance_threshold},
    {"get_draw_color", luab_Canvas_get_draw_color},
    {"set_draw_color", luab_Canvas_set_draw_color},
    {"get_draw_width", luab_Canvas_get_draw_width},
    {"set_draw_width", luab_Canvas_set_draw_width},
    {"get_draw_z", luab_Canvas_get_draw_z},
    {"set_draw_z", luab_Canvas_set_draw_z},
    {"get_enabled", luab_Canvas_get_enabled},
    {"set_enabled", luab_Canvas_set_enabled},
    {"get_simplified_path_color", luab_Canvas_get_simplified_path_color},
    {"set_simplified_path_color", luab_Canvas_set_simplified_path_color},
    {"get_simplified_path_z", luab_Canvas_get_simplified_path_z},
    {"set_simplified_path_z", luab_Canvas_set_simplified_path_z},
    {"get_shape_matcher", luab_Canvas_get_shape_matcher},
    {"set_shape_matcher", luab_Canvas_set_shape_matcher},
    {NULL, NULL}
};

static const struct luaL_Reg luab_Canvas_funcs[] = {
    {"new", luab_Canvas_new},
    {NULL, NULL}
};

int luaopen_dabes_canvas(lua_State *L) {
    luaL_newmetatable(L, luab_Canvas_metatable);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, luab_Canvas_meths, 0);
    luaL_newlib(L, luab_Canvas_funcs);
    lua_pushvalue(L, -1);
    lua_setglobal(L, luab_Canvas_lib);

    return 1;
}
