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

DECLARE_EVENT_LISTENER(keyListener, (KeyEvent *event) {
    if(event->type == EVENT_TYPE(KeyPressedEvent)) {
      ev_log_info("Key pressed: %d", ((KeyPressedEvent*)event)->keyCode);
    } else if(event->type == EVENT_TYPE(KeyReleasedEvent)) {
      ev_log_info("Key released: %d", ((KeyReleasedEvent*)event)->keyCode);
    } else {

    }
})

DECLARE_EVENT_LISTENER(mouseMovedListener, (MouseMovedEvent *event) {
    ev_log_info("Mouse Moved: (%f, %f)", event->position.x, event->position.y);
})

int main(int argc, char **argv) {
  evolengine_t *engine = evol_create();
  evol_parse_args(engine, argc, argv);
  evol_init(engine);

  evolmodule_t window_module = evol_loadmodule("window");
  assert(window_module);
  IMPORT_EVENTS_evmod_glfw(window_module);

  ACTIVATE_EVENT_LISTENER(windowResizedListener, WindowResizedEvent);
  ACTIVATE_EVENT_LISTENER(keyListener, KeyEvent);
  ACTIVATE_EVENT_LISTENER(mouseMovedListener, MouseMovedEvent);

  MODULE_NAMESPACE(Window) *Window = evol_getmodnamespace(window_module, "window");
  EV_BREAK_IF(!Window || !Window.update);

  while(true) {
    if(Window.update(0.0) == 1) {
      break;
    }
    EventSystem.progress();
  }

  evol_unloadmodule(window_module);
  evol_deinit(engine);
  evol_destroy(engine);
  return 0;
}
