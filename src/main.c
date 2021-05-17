#include <evol/evol.h>
#include <evol/common/ev_log.h>
#include <evol/common/ev_macros.h>
#include <evol/common/ev_profile.h>
#include <evol/utils/sleep.h>

#define IMPORT_MODULE evmod_glfw
#include IMPORT_MODULE_H
#define IMPORT_MODULE evmod_ecs
#include IMPORT_MODULE_H
#define IMPORT_MODULE evmod_physics
#include IMPORT_MODULE_H
#define IMPORT_MODULE evmod_script
#include IMPORT_MODULE_H
#define IMPORT_MODULE evmod_assetsystem
#include IMPORT_MODULE_H
#define IMPORT_MODULE evmod_game
#include IMPORT_MODULE_H

// Close window when Q is pressed
DECLARE_EVENT_LISTENER(keyPressedListener, (KeyPressedEvent *event) {
  if(event->keyCode == 81) // tests if Q was pressed
    Window->setShouldClose(event->handle, true);
})

#define IMPORT_NAMESPACES do {                 \
    IMPORT_NAMESPACE(ECS           , ecs_module);         \
    IMPORT_NAMESPACE(Window        , window_module);   \
    IMPORT_NAMESPACE(Input         , input_module);   \
    IMPORT_NAMESPACE(Physics       , physics_module); \
    IMPORT_NAMESPACE(Rigidbody     , physics_module); \
    IMPORT_NAMESPACE(CollisionShape, physics_module); \
    IMPORT_NAMESPACE(Script        , script_module);   \
    IMPORT_NAMESPACE(AssetSystem   , asset_module); \
    IMPORT_NAMESPACE(Game          , game_module); \
    IMPORT_NAMESPACE(Object        , game_module); \
  } while (0)

int main(int argc, char **argv) 
{
  evolengine_t *engine = evol_create();
  evol_parse_args(engine, argc, argv);
  evol_init(engine);

  evolmodule_t script_module  = evol_loadmodule("script");         DEBUG_ASSERT(script_module);
  evolmodule_t ecs_module     = evol_loadmodule("ecs");            DEBUG_ASSERT(ecs_module);
  evolmodule_t window_module  = evol_loadmodule("window");         DEBUG_ASSERT(window_module);
  evolmodule_t input_module   = evol_loadmodule("input");          DEBUG_ASSERT(input_module);
  evolmodule_t physics_module = evol_loadmodule("physics");        DEBUG_ASSERT(physics_module);
  evolmodule_t asset_module   = evol_loadmodule("asset-importer"); DEBUG_ASSERT(asset_module);
  evolmodule_t game_module    = evol_loadmodule("game");           DEBUG_ASSERT(game_module);

  IMPORT_NAMESPACES;
  IMPORT_EVENTS_evmod_glfw(window_module);

  WindowHandle windowHandle = Window->create(800, 600, "Main Window");
  Input->setActiveWindow(windowHandle);

  ACTIVATE_EVENT_LISTENER(keyPressedListener, KeyPressedEvent);

  ECS->newScene();

  Script->initECS();
  Physics->initECS();
  Game->initECS();

  ECSEntityID playerBox = ECS->createEntity();
  ECSEntityID childBox = ECS->createEntity();

  ECS->addChild(playerBox, childBox);

  ECSEntityID ground = ECS->createEntity();

  ScriptHandle childBoxScript = Script->new("Entity1Script", 
    "this.on_init = function ()                           "
    "  this.custom_eulerangles = Vec3:new()               "
    "  this.custom_angularvelocity = Vec3:new(0, 0.01, 0) "
    "end                                                  "
    "                                                     "
    "this.on_fixedupdate = function ()                    "
    "  if Input.getKeyDown(Input.KeyCode.Left) then       "
    "    this.custom_eulerangles:add(Vec3:new(0,0.01,0))  "
    "  end                                                "
    "  if Input.getKeyDown(Input.KeyCode.Right) then      "
    "    this.custom_eulerangles:sub(Vec3:new(0,0.01,0))  "
    "  end                                                "
    "  this.eulerAngles = this.custom_eulerangles         "
    "end                                                  "
  );

  ScriptHandle playerBoxScript = Script->new("Entity2Script",
    "this.on_collisionenter = function(other)                "
    "  other.position = other.position + Vec3:new(0.2, 0, 0) "
    "end                                                     "
    "                                                        "
    "this.on_update = function ()                            "
    "  rb = this:getComponent(Rigidbody)                     "
    "  if Input.getKeyDown(Input.KeyCode.Space) then         "
    "    rb:addForce(Vec3:new(0, 100, 0))                    "
    "  end                                                   "
    "  if Input.getKeyDown(Input.KeyCode.D) then             "
    "    rb:addForce(Vec3:new(10, 0, 0))                     "
    "  end                                                   "
    "  if Input.getKeyDown(Input.KeyCode.A) then             "
    "    rb:addForce(Vec3:new(-10, 0, 0))                    "
    "  end                                                   "
    "  if Input.getKeyDown(Input.KeyCode.W) then             "
    "    rb:addForce(Vec3:new(0, 0, -10))                    "
    "  end                                                   "
    "  if Input.getKeyDown(Input.KeyCode.S) then             "
    "    rb:addForce(Vec3:new(0, 0, 10))                     "
    "  end                                                   "
    "end                                                     "
  );

  Script->addToEntity(playerBox, playerBoxScript);
  Script->addToEntity(childBox, childBoxScript);

  CollisionShapeHandle boxCollider = CollisionShape->newBox(Vec3new(1., 1., 1.));
  CollisionShapeHandle groundCollider = CollisionShape->newBox(Vec3new(5., 5., 5.));

  Object->setPosition(childBox, Vec3new(1, 7, -10   ));
  Object->setRotation(childBox, Vec4new(0, 0,   0, 1));
  Object->setScale(childBox, Vec3new(1, 1,   1   ));

  Object->setPosition(playerBox, Vec3new(-1, 5, -10   ));
  Object->setRotation(playerBox, Vec4new( 0, 0,   0, 1));
  Object->setScale(playerBox, Vec3new( 1, 1,   1   ));

  Object->setPosition(ground, Vec3new(0, -10, -10   ));
  Object->setRotation(ground, Vec4new(0,   0,   0, 1));
  Object->setScale(ground, Vec3new(1,   1,   1   ));

  RigidbodyHandle childRigidbody = Rigidbody->addToEntity(childBox, &(RigidbodyInfo) {
    .type = EV_RIGIDBODY_KINEMATIC,
    .collisionShape = boxCollider,
  });

  RigidbodyHandle playerRigidbody = Rigidbody->addToEntity(playerBox, &(RigidbodyInfo) {
    .type = EV_RIGIDBODY_DYNAMIC,
    .collisionShape = boxCollider,
    .mass = 1.0,
  });

  RigidbodyInfo groundRbInfo = {
    .type = EV_RIGIDBODY_STATIC,
    .collisionShape = groundCollider,
  };

  RigidbodyHandle groundRigidbody = Rigidbody->addToEntity(ground, &groundRbInfo);

  rmt_SetCurrentThreadName("Main Thread");

  AssetSystem->load_mesh("MESH_0_ToyCar.mesh");

  U32 result = 0;
  while(result == 0) {
    ev_ProfileCPU(EventSystemProgress, 0) {
      result |= EventSystem.progress();
    }

    ev_ProfileCPU(WindowUpdate, 0) {
      result |= Window->update(windowHandle);
    }

    ev_ProfileCPU(PhysicsUpdate, 0) {
      result |= Physics->update(0.017);
    }

    ev_ProfileCPU(ECSUpdate, 0) {
      result |= ECS->update(0);
    }

    sleep_ms(17);
  }

  evol_unloadmodule(game_module);
  evol_unloadmodule(physics_module);
  evol_unloadmodule(input_module);
  evol_unloadmodule(asset_module);
  evol_unloadmodule(ecs_module);
  evol_unloadmodule(script_module);
  evol_unloadmodule(window_module);
  evol_deinit(engine);
  evol_destroy(engine);
  return 0;
}
