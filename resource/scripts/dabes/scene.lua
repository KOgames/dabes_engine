--- The logical encapsulation of a single game space and everything in it.
--
-- `Scene` is comparable to the idea of a "level" or "screen", but could be
-- either of the two.
--
-- The `Scene` renders its contents in ascending Z order, back to front.
--
-- Z values are in the range `-256.0` (back) to `0.0` (front)
--
-- @{scene|Scene} extends @{bound_object|BoundObject}
--
-- @module scene
-- @type Scene

require 'dabes.bound_object'
require 'dabes.camera'
require 'dabes.easer'
require 'dabes.music'
require 'dabes.parallax'

Scene = BoundObject:extend({
    lib = dab_scene,

--- Configuration.
-- Required by subclass declarations. Used when instantiating
-- concrete subclasses.
-- @section configuration

    --- The `Scene` prototype to use.
    --
    -- Must be one of these values:
    --
    -- * `"static"` - A scene not backed by a physics engine. Any updates
    -- happen purely in script hooks.
    --
    -- * `"chipmunk"` - A scene backed by the Chipmunk physics engine.
    -- It behaves in a side-scroller style like Mario or Sonic.
    --
    -- Default `"static"`
    kind = "static",

    --- The ratio by which the screen space (pixels) relates to the
    -- physics space (meters). Units are important!
    --
    -- Default `32`
    pixels_per_meter = 32.0,

--- Properties.
-- Significant fields on an instance.
-- @section properties

    _getters = {
        --- An `{r, g, b, a}` vector of the background color.
        --
        -- Default `{0, 0, 0, 1}` (solid black)
        bg_color = BoundObject.fwd_func("get_bg_color"),

        --- The z value for the `Scene`'s background color rectangle.
        --
        -- Default `-256.0`
        bg_z = BoundObject.fwd_func("get_bg_z"),

        --- *(read only)* The `Scene`'s @{camera|Camera}. This is implicit
        -- (constructed automatically).
        -- @name camera
        -- @class field
        camera = function(self)
            local raw_getter = BoundObject.fwd_func("get_camera")
            local got = raw_getter(self)
            if got == nil then
                got = Camera:new(self)
                self._cache.camera = got
            end
            return got
        end,

        --- A @{canvas|Canvas} that is rendered with the `Scene`.
        canvas = BoundObject.fwd_func("get_canvas"),

        --- **(bool)** Whether to render camera debug outlines.
        debug_camera = BoundObject.fwd_func("get_debug_camera"),

        --- **(bool)** Whether to render a debug wireframe tile grid.
        draw_grid = BoundObject.fwd_func("get_draw_grid"),

        --- An `{r, g, b, a}` vector of the background color.
        --
        -- Modified by `fade_in` and `fade_out`
        --
        -- Default `{0, 0, 0, 0}`
        cover_color = BoundObject.fwd_func("get_cover_color"),

        --- An `{x, y}` vector of the `Scene`'s gravity in meters per
        -- seconds squared. This doesn't do anything in `static` scenes, but
        -- the property can still be used.
        gravity = BoundObject.fwd_func("get_gravity"),

        --- The @{music|Music} bound to the `Scene`. Use this property
        -- if you want the `Music` to be managed along with the `Scene`.
        -- e.g. Stop the music automatically when the `Scene` disappears.
        music = BoundObject.fwd_func("get_music"),

        --- A @{parallax|Parallax} that is rendered with the `Scene`.
        parallax = BoundObject.fwd_func("get_parallax"),

        -- TODO
        space = BoundObject.fwd_func("get_space"),

        --- When the `Scene` started, in "ticks" (milliseconds since the
        -- engine booted).
        --
        -- @see util.ticks
        started_at = BoundObject.fwd_func("get_started_at")
    },

    _setters = {
        bg_color = BoundObject.fwd_func("set_bg_color"),
        bg_z = BoundObject.fwd_func("set_bg_z"),
        cover_color = BoundObject.fwd_func("set_cover_color"),
        canvas = BoundObject.fwd_func_real("set_canvas"),
        debug_camera = BoundObject.fwd_func("set_debug_camera"),
        draw_grid = BoundObject.fwd_func("set_draw_grid"),
        gravity = BoundObject.fwd_func("set_gravity"),
        music = BoundObject.fwd_zeroing_setter("set_music", "music"),
        parallax = BoundObject.fwd_func_real("set_parallax"),
        space = BoundObject.readonly,
        started_at = BoundObject.readonly
    },

    _collections = {
        --- The @{collection|Collection} of the Scene's @{entity|Entities}
        -- @class field
        -- @name entities
        entities = {
            adder = BoundObject.fwd_func("add_entity"),
            remover = BoundObject.fwd_func("remove_entity")
        },

        --- The @{collection|Collection} of the Scene's @{overlay|Overlays}
        -- @class field
        -- @name overlays
        overlays = {
            adder = BoundObject.fwd_func("add_overlay"),
            remover = BoundObject.fwd_func("remove_overlay")
        },

        --- The @{collection|Collection} of the Scene's @{recorder|Recorders}
        -- @class field
        -- @name recorders
        recorders = {
            adder = BoundObject.fwd_func("add_recorder"),
            remover = BoundObject.fwd_func("remove_recorder")
        },
    },

--- Class Methods.
-- Must be called on `Class`, with a capital leading character.
-- e.g. `Class:method("foo")`
-- @section classmethods

    --- Create a new `Scene`
    -- @function Scene:new
    -- @treturn Scene
    realize = function(class)
        local realized = class.lib.new(class.kind, class.pixels_per_meter)
        if realized == nil then
            error("Scene: Invalid .kind `" .. class.kind .. "`", 3)
        end

        return realized
    end,

--- Instance Methods.
-- Must be called on an instance of `Class`.
-- e.g. `instance:method("foo")`
-- @section instancemethods

    --- Perform a 500ms fade-in transition.
    --
    -- This actually just animates the `cover` from `{0, 0, 0, 1}` (black)
    -- to `{0, 0, 0, 0}` (clear).
    --
    -- @function scene:fade_in
    -- @tparam function more_update An update callback, of type
    -- `function(Scene, Easer)`
    -- @tparam function completion A completion callback of type
    -- `function(Scene)`
    -- @treturn nil
    fade_in = function(scene, more_update, completion)
        if scene._fade_in_easer ~= nil then return end

        local easer = Easer:new(500)
        easer.update = function(e)
            local color = scene.cover_color
            color[4] = 1.0 - e.value
            scene.cover_color = color
            if more_update ~= nil then more_update(scene, e) end
        end
        easer.finish = function(e)
            if completion ~= nil then completion(scene) end
            scene._fade_in_easer = nil
            collectgarbage("collect")
        end
        scene._fade_in_easer = easer
    end,

    --- Perform a 500ms fade-out transition
    --
    -- This actually just animates the `cover` from `{0, 0, 0, 0}` (clear)
    -- to `{0, 0, 0, 1}` (black).
    --
    -- @function scene:fade_out
    -- @tparam function more_update An update callback, of type
    -- `function(Scene, Easer)`
    -- @tparam function completion A completion callback of type
    -- `function(Scene)`
    -- @treturn nil
    fade_out = function(scene, more_update, completion)
        if scene._fade_out_easer ~= nil then return end

        local easer = Easer:new(500)
        easer.update = function(e)
            local color = scene.cover_color
            color[4] = e.value
            scene.cover_color = color
            if more_update ~= nil then more_update(scene, e) end
        end
        easer.finish = function(e)
            if completion ~= nil then completion(scene) end
            scene._fade_out_easer = nil
        end
        scene._fade_out_easer = easer
    end,

    --- Start the scene. Usually called automatically by
    -- @{scene_manager|SceneManager}.
    --
    -- @function scene:start
    -- @treturn nil
    start = BoundObject.fwd_func("start"),

    --- Stop the scene. Usually called automatically by
    -- @{scene_manager|SceneManager}.
    --
    -- @function scene:stop
    -- @treturn nil
    stop = BoundObject.fwd_func("stop"),


    --- Load a TMX tile map.
    --
    -- @function scene:load_map
    -- @tparam string map The filename of the `.tmx` file. **Map must be
    -- base64/gzip encoded!**
    -- @tparam number meters_per_tile Length of the edge of a single tile in
    -- meters
    -- @tparam number z The z value of the map. Default `-150.0`
    -- @treturn nil
    load_map = BoundObject.fwd_func("load_map"),

--- Hooks.
-- Callbacks implemented in subclasses to customize behavior. Hooks are called
-- on individual instances.
-- @section hooks

    --- Called by the game engine when the scene starts. Overload this to
    -- do things like add entities, parallax layers, maps, etc.
    --
    -- Configure may be called multiple times in the Scene's lifecycle.
    -- If you want to create objects that persist across stopping and starting,
    -- create them in @{bound_object.init|init}
    --
    -- @tparam Scene self The `Scene` instance
    started = function(self) end,

    --- Called by the game engine once per frame, after any physics solving
    -- and before the individual `main` functions on @{entity|Entities}.
    --
    -- @tparam Scene self The `Scene` instance
    main = function(self) end,

    --- Called by the game engine when the scene stops.
    --
    -- This has a default implementation. If you overload this, call the
    -- `Scene` implementation explicitly:
    --     Scene.cleanup(scene)
    --
    -- @tparam Scene self The `Scene` instance
    cleanup = function(self)
        self._add_entity_cache = nil
        self:_cleancache()
        collectgarbage("collect")
    end,

    --- Called by the game engine when the scene needs to reset its camera.
    --
    -- This has a default implementation, but if you overload it you
    -- are not required to call the default implementation.
    --
    -- @tparam Scene self The `Scene` instance
    reset_camera = function(self)
        local camera = self.camera
        camera.scale = 1
        camera.rotation = 0
        camera.translation = {0, 0}
        camera.snap_to_scene = true
        camera.max_scale = 1
        camera:snap_tracking()
    end,

    --- Called every update during a `fade_in` transition.
    --
    -- This can be used to add more effects to a fade in.
    --
    -- @function fade_in_effect
    -- @tparam Scene scene The `Scene` instance
    -- @tparam Easer easer The `Easer` controlling the transition
    fade_in_effect = nil,

    --- Called every update during a `fade_out` transition.
    --
    -- This can be used to add more effects to a fade out.
    --
    -- @function fade_out_effect
    -- @tparam Scene scene The `Scene` instance
    -- @tparam Easer easer The `Easer` controlling the transition
    fade_out_effect = nil,

    --- Called whenever a pair of bodies collides.
    --
    -- This only applies in the `chipmunk` scene.
    --
    -- @function bodies_collided
    -- @tparam Scene scene The `Scene` instance
    -- @tparam Body a
    -- @tparam Body b
    -- @tparam table info Additional info about the collision
    -- @tparam boolean is_first_contact whether the collision step is first
    -- contact between the two bodies
    collision_begin = function(self, a, b, info, is_first_contact)
    end,

    --- Called pre-solve each step a pair of bodies is colliding
    --
    -- This only applies in the `chipmunk` scene.
    --
    -- @function bodies_collided
    -- @tparam Scene scene The `Scene` instance
    -- @tparam Body a
    -- @tparam Body b
    -- @tparam table info Additional info about the collision
    -- @tparam boolean is_first_contact whether the collision step is first
    -- contact between the two bodies
    collision_pre_solve = function(self, a, b, info, is_first_contact)
    end,

    --- Called post-solve each step a pair of bodies is colliding
    --
    -- This only applies in the `chipmunk` scene.
    --
    -- @function bodies_collided
    -- @tparam Scene scene The `Scene` instance
    -- @tparam Body a
    -- @tparam Body b
    -- @tparam table info Additional info about the collision
    -- @tparam boolean is_first_contact whether the collision step is first
    -- contact between the two bodies
    collision_post_solve = function(self, a, b, info, is_first_contact)
    end,

    --- Called when a pair of bodies separates
    --
    -- This only applies in the `chipmunk` scene.
    --
    -- @function bodies_collided
    -- @tparam Scene scene The `Scene` instance
    -- @tparam Body a
    -- @tparam Body b
    -- @tparam table info Additional info about the collision
    -- @tparam boolean is_first_contact whether the collision step is first
    -- contact between the two bodies
    collision_separate = function(self, a, b, info, is_first_contact)
    end,

    --- Called after each step of the physics solve.
    --
    -- May be called multiple times per frame.
    -- @function postsolve
    -- @tparam Scene scene The `Scene` instance
    postsolve = function(self)
    end
})
