#include <evol/evol.h>
#include <evol/common/ev_log.h>

#define TYPE_MODULE evmod_glfw
#include <evol/meta/type_import.h>
#define TYPE_MODULE evmod_ecs
#include <evol/meta/type_import.h>
#define TYPE_MODULE evmod_physics
#include <evol/meta/type_import.h>

#define NAMESPACE_MODULE evmod_glfw
#include <evol/meta/namespace_import.h>
#define NAMESPACE_MODULE evmod_ecs
#include <evol/meta/namespace_import.h>
#define NAMESPACE_MODULE evmod_physics
#include <evol/meta/namespace_import.h>

#define EVENT_MODULE evmod_glfw
#include <evol/meta/event_include.h>

// Close window when Q is pressed
DECLARE_EVENT_LISTENER(keyPressedListener, (KeyPressedEvent *event) {
  if(event->keyCode == 81) // tests if Q was pressed
    Window->close();
})

#define IMPORT_NAMESPACES do {                 \
    IMPORT_NAMESPACE(ECS, ecs_module);         \
    IMPORT_NAMESPACE(Window, window_module);   \
    IMPORT_NAMESPACE(Physics, physics_module); \
  } while (0)

typedef struct Cmp1 {
  I32 dummy_i32;
} Component1;

typedef struct Cmp2 {
  I16 dummy_i16;
  F32 dummy_f32;
} Component2;

void TestSystem(ECSQuery query)
{
  Component1 *cmp1 = ECS->getQueryColumn(query, sizeof(Component1), 1);
  Component2 *cmp2 = ECS->getQueryColumn(query, sizeof(Component2), 2);
  U32 count = ECS->getQueryMatchCount(query);

  for(int i = 1; i <= count; ++i) {
    ev_log_trace("Iteration #%d, cmp1: {%d}", i, cmp1[i-1].dummy_i32);
  }
}

int main(int argc, char **argv) 
{
  evolengine_t *engine = evol_create();
  evol_parse_args(engine, argc, argv);
  evol_init(engine);

  evolmodule_t window_module  = evol_loadmodule("window");  DEBUG_ASSERT(window_module);
  evolmodule_t ecs_module     = evol_loadmodule("ecs");     DEBUG_ASSERT(ecs_module);
  evolmodule_t physics_module = evol_loadmodule("physics"); DEBUG_ASSERT(physics_module);

  IMPORT_NAMESPACES;  
  IMPORT_EVENTS_evmod_glfw(window_module);

  ACTIVATE_EVENT_LISTENER(keyPressedListener, KeyPressedEvent);

  ECS->newScene();

  ECSEntityID ent1 = ECS->createEntity();
  ECSEntityID ent2 = ECS->createEntity();

  ECSComponentID Cmp1_ID = ECS->registerComponent("Component1", sizeof(Component1), EV_ALIGNOF(Component1));
  ECSComponentID Cmp2_ID = ECS->registerComponent("Component2", sizeof(Component2), EV_ALIGNOF(Component2));

  Component1 c11 = { 555 };
  Component2 c2 = { 123, 456.789 };
  Component1 c12 = { 444 };

  ECS->addComponent(ent1, Cmp1_ID, sizeof(Component1), &c11);
  ECS->addComponent(ent1, Cmp2_ID, sizeof(Component2), &c2);

  ECS->addComponent(ent2, Cmp1_ID, sizeof(Component1), &c12);
  ECS->addComponent(ent2, Cmp2_ID, sizeof(Component2), &c2);

  ECS->registerSystem("Component1, Component2", EV_ECS_PIPELINE_STAGE_UPDATE, TestSystem, "TestSystem");

  bool result = 0;
  while(result == 0) {
    result |= EventSystem.progress();
    result |= Window->update(0.0);
    result |= ECS->update(0.0);
    result |= Physics->update(0.0);
  }

  evol_unloadmodule(physics_module);
  evol_unloadmodule(ecs_module);
  evol_unloadmodule(window_module);
  evol_deinit(engine);
  evol_destroy(engine);
  return 0;
}
