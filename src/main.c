#include <evol/evol.h>
#include <evol/common/ev_log.h>
#include <evol/common/ev_macros.h>
#include <evol/common/ev_profile.h>
#include <evol/utils/sleep.h>
#include <evjson.h>
#include <evol/core/configloader.h>

#define IMPORT_MODULE evmod_glfw
#include IMPORT_MODULE_H
#define IMPORT_MODULE evmod_physics
#include IMPORT_MODULE_H
#define IMPORT_MODULE evmod_script
#include IMPORT_MODULE_H
#define IMPORT_MODULE evmod_assets
#include IMPORT_MODULE_H
#define IMPORT_MODULE evmod_game
#include IMPORT_MODULE_H

GameScene scenes[2];

U32 width = 800;
U32 height = 600;

// Close window when Q is pressed
DECLARE_EVENT_LISTENER(keyPressedListener, (KeyPressedEvent *event) {
  if(event->keyCode == 81) // tests if Q was pressed
    Window->setShouldClose(event->handle, true);

  else if(event->keyCode == 49) // Numrow 1
    Game->setActiveScene(scenes[0]);
  else if(event->keyCode == 50) // Numrow 2
    Game->setActiveScene(scenes[1]);

  /* else if(event->keyCode == 45) // Numrow - */
  /* else if(event->keyCode == 61) // Numrow = */
})

int main(int argc, char **argv) 
{
  evolengine_t *engine = evol_create();
  evol_parse_args(engine, argc, argv);
  evol_init(engine);


  evolmodule_t script_mod  = evol_loadmodule("script");         DEBUG_ASSERT(script_mod);
  evolmodule_t window_mod  = evol_loadmodule("window");         DEBUG_ASSERT(window_mod);
  evolmodule_t input_mod   = evol_loadmodule("input");          DEBUG_ASSERT(input_mod);
  evolmodule_t physics_mod = evol_loadmodule("physics");        DEBUG_ASSERT(physics_mod);
  evolmodule_t asset_mod   = evol_loadmodule("assetmanager");   DEBUG_ASSERT(asset_mod);
  evolmodule_t game_mod    = evol_loadmodule("game");           DEBUG_ASSERT(game_mod);

  imports(script_mod , (Script))
  imports(game_mod   , (Game, Object, Camera, Scene))
  imports(window_mod , (Window))
  imports(input_mod  , (Input))
  imports(physics_mod, (PhysicsWorld, Rigidbody, CollisionShape))
  imports(asset_mod  , (AssetManager, Asset, TextLoader, JSONLoader))

  IMPORT_EVENTS_evmod_glfw(window_mod);

  WindowHandle windowHandle = Window->create(width, height, "Main Window");
  Input->setActiveWindow(windowHandle);

  ACTIVATE_EVENT_LISTENER(keyPressedListener, KeyPressedEvent);
  evstring project_dir = NULL;
  EvConfigLoaderResult project_dir_get_res = ev_configloader_get("project_dir", EV_TYPE_NAME(STRING), &project_dir);
  if(project_dir_get_res != EV_CONFIGLOADER_SUCCESS) {
    ev_log_error("[sandbox] Could not get project_dir from config file. Error: %s", EvConfigLoaderResultStrings[project_dir_get_res]);
  }

  assert(project_dir);

  evstring project_mountpoint = evstring_new("project");
  AssetManager->mount(&project_dir, &project_mountpoint);
  evstring_free(project_mountpoint);


  EV_DEFER(
      AssetHandle project_config = Asset->load("project://game.proj"),
      Asset->free(project_config))
  {
    JSONAsset project_desc = JSONLoader->loadAsset(project_config);

    // Loading filesystem mounts
    double mounts_count = evjs_get(project_desc.json_data, "mounts.len")->as_num;
    for(int i = 0; i < (int)mounts_count;i++) {
      evstring path_id = evstring_newfmt("mounts[%d].path", i);
      evstring mountpoint_id = evstring_newfmt("mounts[%d].mountpoint", i);

      evstr_ref path_ref = evjs_get(project_desc.json_data, path_id)->as_str;
      evstr_ref mountpoint_ref = evjs_get(project_desc.json_data, mountpoint_id)->as_str;

      evstring path = evstring_newfmt("%s/%.*s", project_dir, path_ref.len, path_ref.data + path_ref.offset);
      evstring mountpoint = evstring_refclone(mountpoint_ref);

      AssetManager->mount(&path, &mountpoint);

      evstring_free(mountpoint);
      evstring_free(path);

      evstring_free(mountpoint_id);
      evstring_free(path_id);
    }

    // Loading Scenes
    double scene_count = evjs_get(project_desc.json_data, "scenes.len")->as_num;
    for(int i = 0; i < (int)scene_count;i++) {
      evstring scenepath_id = evstring_newfmt("scenes[%d].path", i);
      evstring scenepath = evstring_refclone(evjs_get(project_desc.json_data, scenepath_id)->as_str);

      scenes[0] = Game->loadSceneFromFile(scenepath);

      evstring_free(scenepath);
      evstring_free(scenepath_id);
    }
  }

  evstring_free(project_dir);

  rmt_SetCurrentThreadName("Main Thread");

  U32 result = 0;
  while(result == 0) {
    ev_ProfileCPU(EventSystemProgress, 0) {
      result |= EventSystem.progress();
    }

    ev_ProfileCPU(WindowUpdate, 0) {
      result |= Window->update(windowHandle);
    }

    ev_ProfileCPU(GameProgress, 0) {
      result |= Game->progress(0.01666667f);
    }

    sleep_ms(17);
  }

  evol_unloadmodule(game_mod);
  evol_unloadmodule(physics_mod);
  evol_unloadmodule(input_mod);
  evol_unloadmodule(asset_mod);
  evol_unloadmodule(script_mod);
  evol_unloadmodule(window_mod);
  evol_deinit(engine);
  evol_destroy(engine);
  return 0;
}
