#include <evol/evol.h>
#include <assert.h>

#define NAMESPACE_MODULE evmod_glfw
#include <evol/meta/namespace_import.h>

#define TYPE_MODULE evmod_glfw
#include <evol/meta/type_import.h>

#define NAMESPACE_MODULE evmod_world
#include <evol/meta/namespace_import.h>

#define IMPORT_NAMESPACES do {               \
    IMPORT_NAMESPACE(World, world_module);   \
    IMPORT_NAMESPACE(Window, window_module); \
  } while (0)

int main(int argc, char **argv) 
{
  evolengine_t *engine = evol_create();
  evol_parse_args(engine, argc, argv);
  evol_init(engine);

  evolmodule_t window_module = evol_loadmodule("window"); assert(window_module);
  evolmodule_t world_module = evol_loadmodule("world");   assert(world_module);

  IMPORT_NAMESPACES;

  bool result = 0;
  while(true) {
    result |= Window->update(0.0); 
    result |= World->update(0.0);

    if(result) break;
  }

  evol_unloadmodule(world_module);
  evol_unloadmodule(window_module);
  evol_deinit(engine);
  evol_destroy(engine);
  return 0;
}