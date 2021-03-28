#include <evol/evol.h>
#include <assert.h>

#define NAMESPACE_MODULE evmod_glfw
#include <evol/meta/namespace_import.h>

#define TYPE_MODULE evmod_glfw
#include <evol/meta/type_import.h>

#define TYPE_MODULE evmod_ecs
#include <evol/meta/type_import.h>

#define NAMESPACE_MODULE evmod_ecs
#include <evol/meta/namespace_import.h>

#define IMPORT_NAMESPACES do {               \
    IMPORT_NAMESPACE(ECS, ecs_module);   \
    IMPORT_NAMESPACE(Window, window_module); \
  } while (0)

typedef struct Cmp1 {
  I32 dummy_i32;
} Component1;

typedef struct Cmp2 {
  I16 dummy_i16;
  F32 dummy_f32;
} Component2;

int main(int argc, char **argv) 
{
  evolengine_t *engine = evol_create();
  evol_parse_args(engine, argc, argv);
  evol_init(engine);

  evolmodule_t window_module = evol_loadmodule("window"); assert(window_module);
  evolmodule_t ecs_module = evol_loadmodule("ecs");   assert(ecs_module);

  IMPORT_NAMESPACES;

  ECS->newScene();

  ECSEntityID ent1 = ECS->createEntity();

  ECSComponentID Cmp1_ID = ECS->registerComponent("Component1", sizeof(Component1), EV_ALIGNOF(Component1));
  ECSComponentID Cmp2_ID = ECS->registerComponent("Component2", sizeof(Component2), EV_ALIGNOF(Component2));

  Component1 c1 = { 555 };
  Component2 c2 = { 123, 456.789 };

  ECS->addComponent(ent1, Cmp1_ID, sizeof(Component1), &c1);
  ECS->addComponent(ent1, Cmp2_ID, sizeof(Component2), &c2);

  bool result = 0;
  while(true) {
    result |= Window->update(0.0); 
    result |= ECS->update(0.0);

    if(result) break;
  }

  evol_unloadmodule(ecs_module);
  evol_unloadmodule(window_module);
  evol_deinit(engine);
  evol_destroy(engine);
  return 0;
}
