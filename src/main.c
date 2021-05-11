#include <evol/evol.h>
#include <evol/common/ev_log.h>
#include <evol/common/ev_macros.h>
#include <evol/common/ev_profile.h>
#include <evol/utils/sleep.h>

#define TYPE_MODULE evmod_glfw
#include <evol/meta/type_import.h>
#define NAMESPACE_MODULE evmod_glfw
#include <evol/meta/namespace_import.h>
#define EVENT_MODULE evmod_glfw
#include <evol/meta/event_include.h>

#define TYPE_MODULE evmod_ecs
#include <evol/meta/type_import.h>
#define NAMESPACE_MODULE evmod_ecs
#include <evol/meta/namespace_import.h>

#define TYPE_MODULE evmod_physics
#include <evol/meta/type_import.h>
#define NAMESPACE_MODULE evmod_physics
#include <evol/meta/namespace_import.h>

#define TYPE_MODULE evmod_script
#include <evol/meta/type_import.h>
#define NAMESPACE_MODULE evmod_script
#include <evol/meta/namespace_import.h>

#define TYPE_MODULE evmod_assetsystem
#include <evol/meta/type_import.h>
#define NAMESPACE_MODULE evmod_assetsystem
#include <evol/meta/namespace_import.h>

// Close window when Q is pressed
DECLARE_EVENT_LISTENER(keyPressedListener, (KeyPressedEvent *event) {
  if(event->keyCode == 81) // tests if Q was pressed
    Window->setShouldClose(event->handle, true);
})

#define IMPORT_NAMESPACES do {                 \
    IMPORT_NAMESPACE(ECS, ecs_module);         \
    IMPORT_NAMESPACE(Window, window_module);   \
    IMPORT_NAMESPACE(Physics, physics_module); \
    IMPORT_NAMESPACE(Rigidbody, physics_module); \
    IMPORT_NAMESPACE(CollisionShape, physics_module); \
    IMPORT_NAMESPACE(Script, script_module);   \
    IMPORT_NAMESPACE(AssetSystem, asset_module); \
  } while (0)

int main(int argc, char **argv) 
{
  evolengine_t *engine = evol_create();
  evol_parse_args(engine, argc, argv);
  evol_init(engine);

  evolmodule_t script_module  = evol_loadmodule("script");  DEBUG_ASSERT(script_module);
  evolmodule_t ecs_module     = evol_loadmodule("ecs");     DEBUG_ASSERT(ecs_module);
  evolmodule_t window_module  = evol_loadmodule("window");  DEBUG_ASSERT(window_module);
  evolmodule_t physics_module = evol_loadmodule("physics"); DEBUG_ASSERT(physics_module);
  evolmodule_t asset_module   = evol_loadmodule("asset-importer"); DEBUG_ASSERT(asset_module);

  IMPORT_NAMESPACES;
  IMPORT_EVENTS_evmod_glfw(window_module);

  WindowHandle windowHandle = Window->create(800, 600, "Main Window");

  ACTIVATE_EVENT_LISTENER(keyPressedListener, KeyPressedEvent);

  ECS->newScene();

  Script->initECS();
  Physics->initECS();

  ECSEntityID ent1 = ECS->createEntity();
  ECSEntityID ent2 = ECS->createEntity();

  ScriptHandle ent1ScriptHandle = Script->new("Entity1Script",
    "this.on_update = function ()\n"
    "  rb = this:getComponent(Rigidbody)\n"
    "  rb:addForce(Vec3:new(2, 0, 0))\n"
    "end"
  );

  ScriptHandle ent2ScriptHandle = Script->new("Entity2Script",
    "this.on_update = function ()\n"
    "  rb = this:getComponent(Rigidbody)\n"
    "  rb:addForce(Vec3:new(-2, 0, 0))\n"
    "end"
  );

  Script->addToEntity(ent1, ent1ScriptHandle);
  Script->addToEntity(ent2, ent2ScriptHandle);

  CollisionShapeHandle boxCollider = CollisionShape->newBox(Vec3new(1., 1., 1.));
  CollisionShapeHandle groundCollider = CollisionShape->newBox(Vec3new(5., 5., 5.));

  RigidbodyInfo rbInfo = {
    .type = EV_RIGIDBODY_DYNAMIC,
    .collisionShape = boxCollider,
    .mass = 1.0,
    .restitution = 1.0
  };

  RigidbodyHandle box = Rigidbody->new(&rbInfo);
  Rigidbody->setPosition(box, Vec3new(1, 5, -10));
  Rigidbody->addToEntity(ent1, box);

  RigidbodyHandle box2 = Rigidbody->new(&rbInfo);
  Rigidbody->setPosition(box2, Vec3new(-1, 5, -10));
  Rigidbody->addToEntity(ent2, box2);

  RigidbodyInfo groundRbInfo = {
    .type = EV_RIGIDBODY_STATIC,
    .collisionShape = groundCollider,
    .restitution = 1.0
  };
  RigidbodyHandle ground = Rigidbody->new(&groundRbInfo);
  Rigidbody->setPosition(ground, Vec3new(0, -10, -10));

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

    // Remove the following section
    RigidbodyHandle testHandle = Rigidbody->getFromEntity(ent1);
    Vec3 pos = Rigidbody->getPosition(testHandle);
    printf("Current position of Entity1 = (%f, %f, %f)\n", pos.x, pos.y, pos.z);

    sleep_ms(17);
  }

  evol_unloadmodule(physics_module);
  evol_unloadmodule(asset_module);
  evol_unloadmodule(window_module);
  evol_unloadmodule(ecs_module);
  evol_unloadmodule(script_module);
  evol_deinit(engine);
  evol_destroy(engine);
  return 0;
}
