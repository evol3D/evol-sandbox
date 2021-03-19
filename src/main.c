#include <evol/evol.h>

#define NAMESPACE_MODULE evmod_glfw
#include <evol/meta/namespace_import.h>

int main(int argc, char **argv) {
  evolengine_t *engine = evol_create();
  evol_parse_args(engine, argc, argv);
  evol_init(engine);

  evolmodule_t window_module = evol_loadmodule("window");
  IMPORT_NAMESPACE(Window, window_module);

  while(!Window->update(0.0)); 

  evol_unloadmodule(window_module);
  evol_deinit(engine);
  evol_destroy(engine);
  return 0;
}