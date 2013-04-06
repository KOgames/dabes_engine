#ifndef __scene_h
#define __scene_h
#include "../prefix.h"
#include "../core/audio.h"
#include "../core/engine.h"
#include "../graphics/game_entity.h"
#include "../graphics/camera.h"
#include "../graphics/tile_map.h"
#include "../physics/world.h"

struct Scene;
typedef struct SceneProto {
    int (*create)(struct Scene *scene, Engine *engine);
    void (*start)(struct Scene *scene, Engine *engine);
    void (*stop)(struct Scene *scene, Engine *engine);
    void (*destroy)(struct Scene *scene, Engine *engine);
    void (*update)(struct Scene *scene, Engine *engine);
    void (*render)(struct Scene *scene, Engine *engine);
    void (*control)(struct Scene *scene, Engine *engine);
} SceneProto;

typedef struct Scene {
    SceneProto proto;
    GfxTexture *bg_texture;

    Music *music;
    Camera *camera;
    World *world;
    TileMap *tile_map;
    List *entities;

    short int draw_grid;
    int started;
} Scene;

Scene *Scene_create(Engine *engine, SceneProto proto);
void Scene_restart(Scene *scene, Engine *engine);
void Scene_load_tile_map(Scene *scene, Engine *engine, char *map_file,
                         int abs_path);
void Scene_set_tile_map(Scene *scene, Engine *engine, TileMap *tile_map);
void Scene_reset_camera(Scene *scene);
void Scene_draw_debug_grid(Scene *scene, Graphics *graphics);

#endif
