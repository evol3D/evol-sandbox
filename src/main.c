#include <evol/evol.h>
#include <evol/evolmod.h>

int main(int argc, char **argv) {
  evolengine_t *engine = evol_create();
  evol_parse_args(engine, argc, argv);
  evol_init(engine);
  evolmodule_t window_module = evol_loadmodule("window");

  FN_PTR window_start_fn = evol_getmodfunc(window_module, EV_STRINGIZE(EV_START_FN_NAME));
  if(window_start_fn) {
    window_start_fn();
  }

  evol_unloadmodule(window_module);
  evol_deinit(engine);
  evol_destroy(engine);
  return 0;
}
