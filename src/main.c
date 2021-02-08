#include "evol/common/ev_macros.h"
#include "evol/core/eventlistener.h"
#include <evol/evol.h>
#include <evol/evolmod.h>
#include <evol/common/ev_log.h>
#include <assert.h>

#define EVENT_MODULE evmod_glfw
#include <evol/meta/event_include.h>

DECLARE_EVENT_LISTENER(windowResizedListener, (WindowResizedEvent *event) {
    ev_log_info("Window resized: (%d, %d)", event->width, event->height);
})

int main(int argc, char **argv) {
  evolengine_t *engine = evol_create();
  evol_parse_args(engine, argc, argv);
  evol_init(engine);

  evolmodule_t window_module = evol_loadmodule("window");
  assert(window_module);
  IMPORT_EVENTS_evmod_glfw(window_module);

  ACTIVATE_EVENT_LISTENER(windowResizedListener, WindowResizedEvent);

  FN_PTR window_start_fn = evol_getmodfunc(window_module, EV_STRINGIZE(EV_START_FN_NAME));
  if(window_start_fn) {
    window_start_fn();
  }

  evol_unloadmodule(window_module);
  evol_deinit(engine);
  evol_destroy(engine);
  return 0;
}
