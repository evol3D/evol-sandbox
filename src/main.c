#include <evol/evol.h>
#include <evol/common/ev_log.h>
#include <evol/common/ev_macros.h>
#include <evol/common/ev_profile.h>
#include <evol/utils/sleep.h>

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
#define IMPORT_MODULE evmod_renderer
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

void
init_scenes()
{
  { // Scene 0
    scenes[0] = Game->newScene();

    GameObject playerBox = Scene->createObject(scenes[0]);
    GameObject childBox = Scene->createChildObject(scenes[0], playerBox);
    GameObject ground = Scene->createObject(scenes[0]);

    GameObject camera = Scene->createCamera(scenes[0], EV_CAMERA_VIEWTYPE_PERSPECTIVE);

    assert(Scene->getActiveCamera(scenes[0]) == camera);
    Camera->setHFOV(scenes[0], camera, 120);
    Camera->setNearPlane(scenes[0], camera, 0.001);
    Camera->setFarPlane(scenes[0], camera, 1000);
    Camera->setAspectRatio(scenes[0], camera, (F32)width / (F32)height);
    Object->setPosition(scenes[0], camera, Vec3new(0, 0, -5));

    AssetHandle childBoxScriptAsset = Asset->load("scripts://Scene0/child.lua");
    TextAsset childScript = TextLoader->loadAsset(childBoxScriptAsset);
    ScriptHandle childBoxScript = Script->new("ChildScriptScene0", childScript.text);
    Asset->free(childBoxScriptAsset);

    AssetHandle playerBoxScriptAsset = Asset->load("scripts://Scene0/player.lua");
    TextAsset playerScript = TextLoader->loadAsset(playerBoxScriptAsset);
    ScriptHandle playerBoxScript = Script->new("PlayerScriptScene0", playerScript.text);
    Asset->free(playerBoxScriptAsset);

    AssetHandle cameraScriptAsset = Asset->load("scripts://Scene0/camera.lua");
    TextAsset cameraScriptText = TextLoader->loadAsset(cameraScriptAsset);
    ScriptHandle cameraScript = Script->new("CameraScriptScene0", cameraScriptText.text);
    Asset->free(cameraScriptAsset);

    GenericHandle ecs_handle = Scene->getECSWorld(scenes[0]);
    PhysicsWorldHandle physics_handle = Scene->getPhysicsWorld(scenes[0]);

    Script->addToEntity(scenes[0], playerBox, playerBoxScript);
    Script->addToEntity(scenes[0], childBox, childBoxScript);
    Script->addToEntity(scenes[0], camera, cameraScript);

    CollisionShapeHandle boxCollider = CollisionShape->newBox(physics_handle, Vec3new(1., 1., 1.));
    CollisionShapeHandle groundCollider = CollisionShape->newBox(physics_handle, Vec3new(5., 5., 5.));

    Object->setPosition(scenes[0], childBox, Vec3new(1, 7, -10   ));
    Object->setPosition(scenes[0], playerBox, Vec3new(-1, 5, -10   ));
    Object->setPosition(scenes[0], ground, Vec3new(0, -10, -10   ));

    RigidbodyHandle childRigidbody = Rigidbody->addToEntity(scenes[0], childBox, &(RigidbodyInfo) {
      .type = EV_RIGIDBODY_KINEMATIC,
      .collisionShape = boxCollider,
    });

    RigidbodyHandle playerRigidbody = Rigidbody->addToEntity(scenes[0], playerBox, &(RigidbodyInfo) {
      .type = EV_RIGIDBODY_DYNAMIC,
      .collisionShape = boxCollider,
      .mass = 1.0,
    });

    RigidbodyInfo groundRbInfo = {
      .type = EV_RIGIDBODY_STATIC,
      .collisionShape = groundCollider,
    };

    RigidbodyHandle groundRigidbody = Rigidbody->addToEntity(scenes[0], ground, &groundRbInfo);
  }
  { // Scene 1
    scenes[1] = Game->newScene();

    GameObject playerBox = Scene->createObject(scenes[1]);
    GameObject childBox = Scene->createObject(scenes[1]);
    GameObject ground = Scene->createObject(scenes[1]);

    GameObject camera = Scene->createCamera(scenes[1], EV_CAMERA_VIEWTYPE_PERSPECTIVE);

    assert(Scene->getActiveCamera(scenes[1]) == camera);
    Camera->setHFOV(scenes[1], camera, 120);
    Camera->setNearPlane(scenes[1], camera, 0.001);
    Camera->setFarPlane(scenes[1], camera, 1000);
    Camera->setAspectRatio(scenes[1], camera, (F32)width / (F32)height);
    Object->setPosition(scenes[1], camera, Vec3new(0, 0, -5));

    AssetHandle childBoxScriptAsset = Asset->load("scripts://Scene1/child.lua");
    TextAsset childScript = TextLoader->loadAsset(childBoxScriptAsset);
    ScriptHandle childBoxScript = Script->new("ChildScriptScene1", childScript.text);
    Asset->free(childBoxScriptAsset);

    AssetHandle playerBoxScriptAsset = Asset->load("scripts://Scene1/player.lua");
    TextAsset playerScript = TextLoader->loadAsset(playerBoxScriptAsset);
    ScriptHandle playerBoxScript = Script->new("PlayerScriptScene1", playerScript.text);
    Asset->free(playerBoxScriptAsset);

    AssetHandle cameraScriptAsset = Asset->load("scripts://Scene1/camera.lua");
    TextAsset cameraScriptText = TextLoader->loadAsset(cameraScriptAsset);
    ScriptHandle cameraScript = Script->new("CameraScriptScene1", cameraScriptText.text);
    Asset->free(cameraScriptAsset);

    GenericHandle ecs_handle = Scene->getECSWorld(scenes[1]);
    PhysicsWorldHandle physics_handle = Scene->getPhysicsWorld(scenes[1]);

    Script->addToEntity(scenes[1], playerBox, playerBoxScript);
    Script->addToEntity(scenes[1], childBox, childBoxScript);
    Script->addToEntity(scenes[1], camera, cameraScript);

    CollisionShapeHandle boxCollider = CollisionShape->newBox(physics_handle, Vec3new(1., 1., 1.));
    CollisionShapeHandle groundCollider = CollisionShape->newBox(physics_handle, Vec3new(5., 5., 5.));

    Object->setPosition(scenes[1], childBox, Vec3new(1, 7, -10   ));
    Object->setPosition(scenes[1], playerBox, Vec3new(-1, 5, -10   ));
    Object->setPosition(scenes[1], ground, Vec3new(0, -10, -10   ));


    RigidbodyHandle childRigidbody = Rigidbody->addToEntity(scenes[1], childBox, &(RigidbodyInfo) {
      .type = EV_RIGIDBODY_DYNAMIC,
      .collisionShape = boxCollider,
      .mass = 1.0,
    });

    RigidbodyHandle playerRigidbody = Rigidbody->addToEntity(scenes[1], playerBox, &(RigidbodyInfo) {
      .type = EV_RIGIDBODY_DYNAMIC,
      .collisionShape = boxCollider,
      .mass = 1.0,
    });

    RigidbodyInfo groundRbInfo = {
      .type = EV_RIGIDBODY_STATIC,
      .collisionShape = groundCollider,
    };

    RigidbodyHandle groundRigidbody = Rigidbody->addToEntity(scenes[1], ground, &groundRbInfo);
  }
}

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
  evolmodule_t renderer_mod= evol_loadmodule("renderer");       DEBUG_ASSERT(renderer_mod);

  imports(script_mod , (Script))
  imports(game_mod   , (Game, Object, Camera, Scene))
  imports(window_mod , (Window))
  imports(input_mod  , (Input))
  imports(physics_mod, (PhysicsWorld, Rigidbody, CollisionShape))
  imports(asset_mod  , (AssetManager, Asset, TextLoader))
  imports(renderer_mod,(Renderer))

  AssetManager->mount("../res", "res:/");
  AssetManager->mount("../res/scripts", "scripts:/");

  IMPORT_EVENTS_evmod_glfw(window_mod);

  WindowHandle windowHandle = Window->create(width, height, "Main Window");
  Input->setActiveWindow(windowHandle);
  Renderer->setWindow(windowHandle);

  ACTIVATE_EVENT_LISTENER(keyPressedListener, KeyPressedEvent);

  init_scenes();

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
    Renderer->run();
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
