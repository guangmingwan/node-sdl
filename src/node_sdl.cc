#ifdef __APPLE__
#include <objc/objc.h>
#include <objc/objc-runtime.h>
#endif

#include "SDL.h"
#include "node_sdl.h"
#include "struct_wrappers.h"
#include "window.h"
#include "texture.h"
#include "surface.h"
#include <v8.h>
#include <string>
#include <iostream>
#include <ctime>
#include <sys/time.h>

using namespace v8;

static std::map<uint32_t, std::string> event_type_to_string_;
static std::map<SDL_WindowEventID, std::string> window_event_to_string_;

extern "C" void
init(Handle<Object> target)
{
// #ifdef __APPLE__
//   // on the mac it is necessary to create to call [NSApplication sharedApplication]
//   // before we can create a rendering window
//   objc_msgSend(objc_getClass("NSApplication"), sel_getUid("sharedApplication"));
// #endif
  std::cout << "Starting init." << std::endl;

  // Initialize the SDL event type to string mappings.
  std::cout << "Initializing SDL event type to string mappings." << std::endl;
  event_type_to_string_[SDL_DOLLARGESTURE] = "dollarGesture";
  event_type_to_string_[SDL_DROPFILE] = "dropFile";
  event_type_to_string_[SDL_FINGERMOTION] = "fingerMotion";
  event_type_to_string_[SDL_FINGERDOWN] = "fingerDown";
  event_type_to_string_[SDL_FINGERUP] = "fingerUp";
  event_type_to_string_[SDL_KEYDOWN] = "keyDown";
  event_type_to_string_[SDL_KEYUP] = "keyUp";
  event_type_to_string_[SDL_JOYAXISMOTION] = "joyAxisMotion";
  event_type_to_string_[SDL_JOYBALLMOTION] = "joyBallMotion";
  event_type_to_string_[SDL_JOYHATMOTION] = "joyHatMotion";
  event_type_to_string_[SDL_JOYBUTTONDOWN] = "joyButtonDown";
  event_type_to_string_[SDL_JOYBUTTONUP] = "joyButtonUp";
  event_type_to_string_[SDL_MOUSEMOTION] = "mouseMotion";
  event_type_to_string_[SDL_MOUSEBUTTONDOWN] = "mouseButtonDown";
  event_type_to_string_[SDL_MOUSEBUTTONUP] = "mouseButtonUp";
  event_type_to_string_[SDL_MOUSEWHEEL] = "mouseWheel";
  event_type_to_string_[SDL_MULTIGESTURE] = "multiGesture";
  event_type_to_string_[SDL_QUIT] = "quit";
  event_type_to_string_[SDL_SYSWMEVENT] = "sysWMEvent";
  event_type_to_string_[SDL_TEXTEDITING] = "textEditing";
  event_type_to_string_[SDL_TEXTINPUT] = "textInput";
  event_type_to_string_[SDL_USEREVENT] = "userEvent";
  event_type_to_string_[SDL_WINDOWEVENT] = "windowEvent";
  std::cout << "Finished initializing event mappings." << std::endl;

  // Initialize the SDL WindowEvent type to string mappings.
  std::cout << "Initializing SDL window event type to string mappings." << std::endl;
  window_event_to_string_[SDL_WINDOWEVENT_SHOWN] = "shown";
  window_event_to_string_[SDL_WINDOWEVENT_HIDDEN] = "hidden";
  window_event_to_string_[SDL_WINDOWEVENT_EXPOSED] = "exposed";
  window_event_to_string_[SDL_WINDOWEVENT_MOVED] = "moved";
  window_event_to_string_[SDL_WINDOWEVENT_RESIZED] = "resized";
  window_event_to_string_[SDL_WINDOWEVENT_SIZE_CHANGED] = "sizeChanged";
  window_event_to_string_[SDL_WINDOWEVENT_MINIMIZED] = "minimized";
  window_event_to_string_[SDL_WINDOWEVENT_MAXIMIZED] = "maximized";
  window_event_to_string_[SDL_WINDOWEVENT_RESTORED] = "restored";
  window_event_to_string_[SDL_WINDOWEVENT_ENTER] = "enter";
  window_event_to_string_[SDL_WINDOWEVENT_LEAVE] = "leave";
  window_event_to_string_[SDL_WINDOWEVENT_FOCUS_GAINED] = "focusGained";
  window_event_to_string_[SDL_WINDOWEVENT_FOCUS_LOST] = "focusLost";
  window_event_to_string_[SDL_WINDOWEVENT_CLOSE] = "close";
  std::cout << "Finished initializing window event mappings." << std::endl;

  sdl::InitWrappers(target);
  sdl::WindowWrapper::Init(target);
  sdl::RendererWrapper::Init(target);
  sdl::TextureWrapper::Init(target);
  sdl::SurfaceWrapper::Init(target);

  // Initialization and Shutdown.
  NODE_SET_METHOD(target, "init", sdl::Init);
  NODE_SET_METHOD(target, "initSubSystem", sdl::InitSubSystem);
  NODE_SET_METHOD(target, "wasInit", sdl::WasInit);
  NODE_SET_METHOD(target, "quit", sdl::Quit);
  NODE_SET_METHOD(target, "quitSubSystem", sdl::QuitSubSystem);

  // Display and Window Management.

  NODE_SET_METHOD(target, "clearError", sdl::ClearError);
  NODE_SET_METHOD(target, "getError", sdl::GetError);
  NODE_SET_METHOD(target, "setError", sdl::SetError);

  NODE_SET_METHOD(target, "waitEvent", sdl::WaitEvent);
  NODE_SET_METHOD(target, "waitEventTimeout", sdl::WaitEventTimeout);
  NODE_SET_METHOD(target, "pollEvent", sdl::PollEvent);

  NODE_SET_METHOD(target, "numJoysticks", sdl::NumJoysticks);
  NODE_SET_METHOD(target, "joystickOpen", sdl::JoystickOpen);
  NODE_SET_METHOD(target, "joystickName", sdl::JoystickName);
  NODE_SET_METHOD(target, "joystickNumAxes", sdl::JoystickNumAxes);
  NODE_SET_METHOD(target, "joystickNumButtons", sdl::JoystickNumButtons);
  NODE_SET_METHOD(target, "joystickNumBalls", sdl::JoystickNumBalls);
  NODE_SET_METHOD(target, "joystickNumHats", sdl::JoystickNumHats);
  NODE_SET_METHOD(target, "joystickClose", sdl::JoystickClose);
  NODE_SET_METHOD(target, "joystickUpdate", sdl::JoystickUpdate);
  NODE_SET_METHOD(target, "joystickEventState", sdl::JoystickEventState);

  NODE_SET_METHOD(target, "mapRGB", sdl::MapRGB);
  NODE_SET_METHOD(target, "mapRGBA", sdl::MapRGBA);
  NODE_SET_METHOD(target, "getRGB", sdl::GetRGB);
  NODE_SET_METHOD(target, "getRGBA", sdl::GetRGBA);

  Local<Object> INIT = Object::New();
  target->Set(String::New("INIT"), INIT);
  INIT->Set(String::New("TIMER"), Number::New(SDL_INIT_TIMER));
  INIT->Set(String::New("AUDIO"), Number::New(SDL_INIT_AUDIO));
  INIT->Set(String::New("VIDEO"), Number::New(SDL_INIT_VIDEO));
  INIT->Set(String::New("JOYSTICK"), Number::New(SDL_INIT_JOYSTICK));
  INIT->Set(String::New("HAPTIC"), Number::New(SDL_INIT_HAPTIC));
  INIT->Set(String::New("GAMECONTROLLER"), Number::New(SDL_INIT_GAMECONTROLLER));
  INIT->Set(String::New("EVENTS"), Number::New(SDL_INIT_EVENTS));
  INIT->Set(String::New("EVERYTHING"), Number::New(SDL_INIT_EVERYTHING));
  INIT->Set(String::New("NOPARACHUTE"), Number::New(SDL_INIT_NOPARACHUTE));

  Local<Object> EVENT = Object::New();
  target->Set(String::New("EVENT"), EVENT);
  EVENT->Set(String::New("DOLLARGESTURE"), Number::New(SDL_DOLLARGESTURE));
  EVENT->Set(String::New("DROPFILE"), Number::New(SDL_DROPFILE));
  EVENT->Set(String::New("FINGERMOTION"), Number::New(SDL_FINGERMOTION));
  EVENT->Set(String::New("FINGERUP"), Number::New(SDL_FINGERUP));
  EVENT->Set(String::New("KEYDOWN"), Number::New(SDL_KEYDOWN));
  EVENT->Set(String::New("KEYUP"), Number::New(SDL_KEYUP));
  EVENT->Set(String::New("JOYAXISMOTION"), Number::New(SDL_JOYAXISMOTION));
  EVENT->Set(String::New("JOYBALLMOTION"), Number::New(SDL_JOYBALLMOTION));
  EVENT->Set(String::New("JOYHATMOTION"), Number::New(SDL_JOYHATMOTION));
  EVENT->Set(String::New("JOYBUTTONDOWN"), Number::New(SDL_JOYBUTTONDOWN));
  EVENT->Set(String::New("JOYBUTTONUP"), Number::New(SDL_JOYBUTTONUP));
  EVENT->Set(String::New("MOUSEMOTION"), Number::New(SDL_MOUSEMOTION));
  EVENT->Set(String::New("MOUSEBUTTONDOWN"), Number::New(SDL_MOUSEBUTTONDOWN));
  EVENT->Set(String::New("MOUSEBUTTONUP"), Number::New(SDL_MOUSEBUTTONUP));
  EVENT->Set(String::New("MOUSEWHEEL"), Number::New(SDL_MOUSEWHEEL));
  EVENT->Set(String::New("MULTIGESTURE"), Number::New(SDL_MULTIGESTURE));
  EVENT->Set(String::New("QUIT"), Number::New(SDL_QUIT));
  EVENT->Set(String::New("SYSWMEVENT"), Number::New(SDL_SYSWMEVENT));
  EVENT->Set(String::New("TEXTEDITING"), Number::New(SDL_TEXTEDITING));
  EVENT->Set(String::New("TEXTINPUT"), Number::New(SDL_TEXTINPUT));
  EVENT->Set(String::New("USEREVENT"), Number::New(SDL_USEREVENT));
  EVENT->Set(String::New("WINDOWEVENT"), Number::New(SDL_WINDOWEVENT));

  Local<Object> SURFACE = Object::New();
  target->Set(String::New("SURFACE"), SURFACE);
  // SURFACE->Set(String::New("ANYFORMAT"), Number::New(SDL_ANYFORMAT));
  // SURFACE->Set(String::New("ASYNCBLIT"), Number::New(SDL_ASYNCBLIT));
  // SURFACE->Set(String::New("DOUBLEBUF"), Number::New(SDL_DOUBLEBUF));
  // SURFACE->Set(String::New("HWACCEL"), Number::New(SDL_HWACCEL));
  // SURFACE->Set(String::New("HWPALETTE"), Number::New(SDL_HWPALETTE));
  // SURFACE->Set(String::New("HWSURFACE"), Number::New(SDL_HWSURFACE));
  // SURFACE->Set(String::New("FULLSCREEN"), Number::New(SDL_FULLSCREEN));
  // SURFACE->Set(String::New("OPENGL"), Number::New(SDL_OPENGL));
  // SURFACE->Set(String::New("RESIZABLE"), Number::New(SDL_RESIZABLE));
  SURFACE->Set(String::New("RLEACCEL"), Number::New(SDL_RLEACCEL));
  // SURFACE->Set(String::New("SRCALPHA"), Number::New(SDL_SRCALPHA));
  // SURFACE->Set(String::New("SRCCOLORKEY"), Number::New(SDL_SRCCOLORKEY));
  SURFACE->Set(String::New("SWSURFACE"), Number::New(SDL_SWSURFACE));
  SURFACE->Set(String::New("PREALLOC"), Number::New(SDL_PREALLOC));

  // SDL Enumerations start:

  Local<Object> AUDIOFORMAT = Object::New();
  target->Set(String::New("AUDIOFORMAT"), AUDIOFORMAT);
  AUDIOFORMAT->Set(String::New("MASK_BITSIZE"), Number::New(SDL_AUDIO_MASK_BITSIZE));
  AUDIOFORMAT->Set(String::New("MASK_DATATYPE"), Number::New(SDL_AUDIO_MASK_DATATYPE));
  AUDIOFORMAT->Set(String::New("MASK_ENDIAN"), Number::New(SDL_AUDIO_MASK_ENDIAN));
  AUDIOFORMAT->Set(String::New("MASK_SIGNED"), Number::New(SDL_AUDIO_MASK_SIGNED));

  Local<Object> TEXTUREACCESS = Object::New();
  target->Set(String::New("TEXTUREACCESS"), TEXTUREACCESS);
  TEXTUREACCESS->Set(String::New("STATIC"), Number::New(SDL_TEXTUREACCESS_STATIC));
  TEXTUREACCESS->Set(String::New("STREAMING"), Number::New(SDL_TEXTUREACCESS_STREAMING));

  Local<Object> TTF = Object::New();
  target->Set(String::New("TTF"), TTF);
  NODE_SET_METHOD(TTF, "init", sdl::TTF::Init);
  NODE_SET_METHOD(TTF, "openFont", sdl::TTF::OpenFont);
  // NODE_SET_METHOD(TTF, "renderTextBlended", sdl::TTF::RenderTextBlended);

  Local<Object> IMG = Object::New();
  target->Set(String::New("IMG"), IMG);

  NODE_SET_METHOD(IMG, "load", sdl::IMG::Load);

  Local<Object> WM = Object::New();
  target->Set(String::New("WM"), WM);

  Local<Object> GL = Object::New();
  target->Set(String::New("GL"), GL);


  NODE_SET_METHOD(GL, "setAttribute", sdl::GL::SetAttribute);
  NODE_SET_METHOD(GL, "getAttribute", sdl::GL::GetAttribute);

  GL->Set(String::New("RED_SIZE"), Number::New(SDL_GL_RED_SIZE));
  GL->Set(String::New("GREEN_SIZE"), Number::New(SDL_GL_GREEN_SIZE));
  GL->Set(String::New("BLUE_SIZE"), Number::New(SDL_GL_BLUE_SIZE));
  GL->Set(String::New("ALPHA_SIZE"), Number::New(SDL_GL_ALPHA_SIZE));
  GL->Set(String::New("DOUBLEBUFFER"), Number::New(SDL_GL_DOUBLEBUFFER));
  GL->Set(String::New("BUFFER_SIZE"), Number::New(SDL_GL_BUFFER_SIZE));
  GL->Set(String::New("DEPTH_SIZE"), Number::New(SDL_GL_DEPTH_SIZE));
  GL->Set(String::New("STENCIL_SIZE"), Number::New(SDL_GL_STENCIL_SIZE));
  GL->Set(String::New("ACCUM_RED_SIZE"), Number::New(SDL_GL_ACCUM_RED_SIZE));
  GL->Set(String::New("ACCUM_GREEN_SIZE"), Number::New(SDL_GL_ACCUM_GREEN_SIZE));
  GL->Set(String::New("ACCUM_BLUE_SIZE"), Number::New(SDL_GL_ACCUM_BLUE_SIZE));
  GL->Set(String::New("ACCUM_ALPHA_SIZE"), Number::New(SDL_GL_ACCUM_ALPHA_SIZE));

  Local<Object> HINT = Object::New();
  target->Set(String::New("HINT"), HINT);

  HINT->Set(String::New("FRAMEBUFFER_ACCELERATION"), String::New(SDL_HINT_FRAMEBUFFER_ACCELERATION));
  HINT->Set(String::New("IDLE_TIMER_DISABLED"), String::New(SDL_HINT_IDLE_TIMER_DISABLED));
  HINT->Set(String::New("ORIENTATIONS"), String::New(SDL_HINT_ORIENTATIONS));
  HINT->Set(String::New("RENDER_DRIVER"), String::New(SDL_HINT_RENDER_DRIVER));
  HINT->Set(String::New("RENDER_OPENGL_SHADERS"), String::New(SDL_HINT_RENDER_OPENGL_SHADERS));
  HINT->Set(String::New("SCALE_QUALITY"), String::New(SDL_HINT_RENDER_SCALE_QUALITY));
  HINT->Set(String::New("RENDER_VSYNC"), String::New(SDL_HINT_RENDER_VSYNC));

  Local<Object> RENDERER = Object::New();
  target->Set(String::New("RENDERER"), RENDERER);

  RENDERER->Set(String::New("SOFTWARE"), Number::New(SDL_RENDERER_SOFTWARE));
  RENDERER->Set(String::New("ACCELERATED"), Number::New(SDL_RENDERER_ACCELERATED));
  RENDERER->Set(String::New("PRESENTVSYNC"), Number::New(SDL_RENDERER_PRESENTVSYNC));
  RENDERER->Set(String::New("TARGETTEXTURE"), Number::New(SDL_RENDERER_TARGETTEXTURE));

  Local<Object> BLENDMODE = Object::New();
  target->Set(String::New("BLENDMODE"), BLENDMODE);

  BLENDMODE->Set(String::New("NONE"), Number::New(SDL_BLENDMODE_NONE));
  BLENDMODE->Set(String::New("BLEND"), Number::New(SDL_BLENDMODE_BLEND));
  BLENDMODE->Set(String::New("ADD"), Number::New(SDL_BLENDMODE_ADD));
  BLENDMODE->Set(String::New("MOD"), Number::New(SDL_BLENDMODE_MOD));

  Local<Object> FLIP = Object::New();
  target->Set(String::New("FLIP"), FLIP);

  FLIP->Set(String::New("NONE"), Number::New(SDL_FLIP_NONE));
  FLIP->Set(String::New("HORIZONTAL"), Number::New(SDL_FLIP_HORIZONTAL));
  FLIP->Set(String::New("VERTICAL"), Number::New(SDL_FLIP_VERTICAL));
}


////////////////////////////////////////////////////////////////////////////////
// Initialization and Shutdown.

Handle<Value> sdl::Init(const Arguments& args) {
  HandleScope scope;
  SDL_SetMainReady();
  int init = (args[0]->IsUndefined() || !args[0]->IsNumber()) ? SDL_INIT_EVERYTHING : args[0]->Int32Value();
  // std::cout << "sdl::Init got: " << init << std::endl;
  if (SDL_Init(init) < 0) {
    return ThrowSDLException(__func__);
  }
  return Undefined();
}

Handle<Value> sdl::InitSubSystem(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected InitSubSystem(Number)")));
  }

  if (SDL_InitSubSystem(args[0]->Int32Value()) < 0) return ThrowSDLException(__func__);

  return Undefined();
}

Handle<Value> sdl::Quit(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Quit()")));
  }

  SDL_Quit();

  return Undefined();
}

Handle<Value> sdl::QuitSubSystem(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected QuitSubSystem(Number)")));
  }

  SDL_QuitSubSystem(args[0]->Int32Value());

  return Undefined();
}

Handle<Value> sdl::WasInit(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected WasInit(Number)")));
  }

  return Number::New(SDL_WasInit(args[0]->Int32Value()));
}

////////////////////////////////////////////////////////////////////////////////
// Display and Window Management.


////////////////////////////////////////////////////////////////////////////////
// Global SDL functions.
Handle<Value> sdl::DisableScreenSaver(const Arguments& args) {
  SDL_DisableScreenSaver();
  return Undefined();
}
Handle<Value> sdl::EnableScreenSaver(const Arguments& args) {
  SDL_EnableScreenSaver();
  return Undefined();
}
Handle<Value> sdl::IsScreenSaverEnabled(const Arguments& args) {
  HandleScope scope;

  SDL_bool ret = SDL_IsScreenSaverEnabled();
  return scope.Close(Boolean::New(ret));
}

Handle<Value> sdl::GetClosestDisplayMode(const Arguments& args) {
  HandleScope scope;

  if(args.Length() < 2) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected getClosestDisplayMode(Number, DisplayMode)")));
  }

  int index = args[0]->Int32Value();
  SDL_DisplayMode* mode = UnwrapDisplayMode(Handle<Object>::Cast(args[1]));
  SDL_DisplayMode* closest = new SDL_DisplayMode;
  SDL_DisplayMode* err = SDL_GetClosestDisplayMode(index, mode, closest);
  if(NULL == err) {
    delete closest;
    return ThrowSDLException(__func__);
  }

  return scope.Close(WrapDisplayMode(closest));
}
Handle<Value> sdl::GetCurrentDisplayMode(const Arguments& args) {
  HandleScope scope;

  if(args.Length() < 1) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected getCurrentDisplayMode(Number)")));
  }

  int index = args[0]->Int32Value();
  SDL_DisplayMode* current = new SDL_DisplayMode;
  int err = SDL_GetCurrentDisplayMode(index, current);
  if(err < 0) {
    delete current;
    return ThrowSDLException(__func__);
  }

  return scope.Close(WrapDisplayMode(current));
}
Handle<Value> sdl::GetCurrentVideoDriver(const Arguments& args) {
  HandleScope scope;

  const char* ret = SDL_GetCurrentVideoDriver();
  if(NULL == ret) {
    return ThrowSDLException(__func__);
  }

  return scope.Close(String::New(ret));
}
Handle<Value> sdl::GetDesktopDisplayMode(const Arguments& args) {
  HandleScope scope;

  if(args.Length() < 1) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected getCurrentDisplayMode(Number)")));
  }

  int index = args[0]->Int32Value();
  SDL_DisplayMode* current = new SDL_DisplayMode;
  int err = SDL_GetDesktopDisplayMode(index, current);
  if(err < 0) {
    delete current;
    return ThrowSDLException(__func__);
  }

  return scope.Close(WrapDisplayMode(current));
}
Handle<Value> sdl::GetDisplayBounds(const Arguments& args) {
  HandleScope scope;

  if(args.Length() < 1) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected getDisplayBounds(Number)")));
  }

  int index = args[0]->Int32Value();
  SDL_Rect* bounds = new SDL_Rect;
  int err = SDL_GetDisplayBounds(index, bounds);
  if(err < 0) {
    delete bounds;
    return ThrowSDLException(__func__);
  }

  return scope.Close(WrapRect(bounds));
}
Handle<Value> sdl::GetDisplayMode(const Arguments& args) {
  HandleScope scope;

  if(args.Length() < 2) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected getDisplayMode(Number, Number)")));
  }

  int displayIndex = args[0]->Int32Value();
  int modeIndex = args[1]->Int32Value();
  SDL_DisplayMode* mode = new SDL_DisplayMode;
  int err = SDL_GetDisplayMode(displayIndex, modeIndex, mode);
  if(err < 0) {
    delete mode;
    return ThrowSDLException(__func__);
  }

  return scope.Close(WrapDisplayMode(mode));
}
Handle<Value> sdl::GetDisplayName(const Arguments& args) {
  HandleScope scope;

  if(args.Length() < 0) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: expected getDisplayName(Number)")));
  }

  int index = args[0]->Int32Value();
  const char* ret = SDL_GetDisplayName(index);
  if(NULL == ret) {
    return ThrowSDLException(__func__);
  }

  return scope.Close(String::New(ret));
}
Handle<Value> sdl::GetNumDisplayModes(const Arguments& args) {
  HandleScope scope;

  if(args.Length() < 0) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: expected getNumDisplayModes(Number)")));
  }

  int index = args[0]->Int32Value();
  int ret = SDL_GetNumDisplayModes(index);
  if(ret < 0) {
    return ThrowSDLException(__func__);
  }

  return scope.Close(Number::New(ret));
}
Handle<Value> sdl::GetNumVideoDisplays(const Arguments& args) {
  HandleScope scope;

  int num = SDL_GetNumVideoDisplays();
  if(num < 0) {
    return ThrowSDLException(__func__);
  }
  return scope.Close(Number::New(num));
}
Handle<Value> sdl::GetNumVideoDrivers(const Arguments& args) {
  HandleScope scope;

  int num = SDL_GetNumVideoDrivers();
  if(num < 0) {
    return ThrowSDLException(__func__);
  }
  return scope.Close(Number::New(num));
}
Handle<Value> sdl::GetVideoDriver(const Arguments& args) {
  HandleScope scope;

  if(args.Length() < 1) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: expected getVideoDriver(Number)")));
  }

  int index = args[0]->Int32Value();
  const char* driver = SDL_GetVideoDriver(index);
  if(NULL == driver) {
    return ThrowSDLException(__func__);
  }
  return scope.Close(String::New(driver));
}

// Handle<Value> sdl::ShowMessageBox(const Arguments& args) {

// }
Handle<Value> sdl::ShowSimpleMessageBox(const Arguments& args) {
  HandleScope scope;

  if(args.Length() < 3) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: expected showSimpleMessageBox(Number, String, String, [Window])")));
  }
  else if(args.Length() < 4) {
    int flags = args[0]->Int32Value();
    String::Utf8Value title(args[1]);
    String::Utf8Value message(args[2]);
    int err = SDL_ShowSimpleMessageBox(flags, *title, *message, NULL);
    if(err < 0) {
      return ThrowSDLException(__func__);
    }
  }
  else {
    int flags = args[0]->Int32Value();
    String::Utf8Value title(args[1]);
    String::Utf8Value message(args[2]);
    WindowWrapper* window = node::ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(args[3]));
    int err = SDL_ShowSimpleMessageBox(flags, *title, *message, window->window_);
    if(err < 0) {
      return ThrowSDLException(__func__);
    }
  }

  return Undefined();
}

Handle<Value> sdl::VideoInit(const Arguments& args) {
  HandleScope scope;

  const char* driver = args[0]->IsUndefined() ? NULL : *(String::Utf8Value(args[0]));
  int err = SDL_VideoInit(driver);
  if(err < 0) {
    return ThrowSDLException(__func__);
  }
  return scope.Close(String::New(driver));
}
Handle<Value> sdl::VideoQuit(const Arguments& args) {
  SDL_VideoQuit();
  return Undefined();
}

////////////////////////////////////////////////////////////////////////////////

Handle<Value> sdl::ClearError(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected ClearError()")));
  }

  SDL_ClearError();

  return Undefined();
}

Handle<Value> sdl::GetError(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetError()")));
  }

  return String::New(SDL_GetError());
}

Handle<Value> sdl::SetError(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsString())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetError(String)")));
  }

  String::Utf8Value message(args[1]);

  SDL_SetError(*message);

  return Undefined();
}

Handle<Value> sdl::WaitEvent(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsFunction())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected WaitEvent(Function)")));
  }

  SDL_Event e;
  int err = SDL_WaitEvent(&e);
  if(0 == err) {
    std::string err = "WaitEvent failed: ";
    err += SDL_GetError();
    return ThrowException(MakeSDLException(err.c_str()));
  }
  Handle<Value> argv[1];
  argv[0] = sdl::SDLEventToJavascriptObject(e);
  Handle<Function>::Cast(args[0])->Call(Context::GetCurrent()->Global(), 1, argv);
  return Undefined();
}

Handle<Value> sdl::WaitEventTimeout(const Arguments& args) {
  HandleScope scope;

  if(!(args.Length() == 2 && args[0]->IsFunction() && args[1]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected WaitEventTimeout(Function, Number)")));
  }

  SDL_Event e;
  int timeout = args[1]->Int32Value();
  int err = SDL_WaitEventTimeout(&e, timeout);
  if(0 == err) {
    std::string err = "WaitEventTimeout failed: ";
    err += SDL_GetError();
    return ThrowException(MakeSDLException(err.c_str()));
  }
  Handle<Value> argv[1];
  argv[0] = sdl::SDLEventToJavascriptObject(e);
  Handle<Function>::Cast(args[0])->Call(Context::GetCurrent()->Global(), 1, argv);
  return Undefined();
}

Handle<Value> sdl::PollEvent(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected PollEvent()")));
  }

  SDL_Event event;
  if (!SDL_PollEvent(&event)) {
    return Undefined();
  }

  Local<Object> evt = SDLEventToJavascriptObject(event);
  return scope.Close(evt);
}

Handle<Value> sdl::NumJoysticks(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected NumJoysticks()")));
  }

  return Number::New(SDL_NumJoysticks());
}

Handle<Value> sdl::JoystickOpen(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickOpen(Number)")));
  }

  SDL_Joystick* joystick = SDL_JoystickOpen(args[0]->Int32Value());
  if (!joystick) return ThrowSDLException(__func__);
  return scope.Close(WrapJoystick(joystick));
}

Handle<Value> sdl::JoystickName(const Arguments& args) {
  return Undefined();
  // HandleScope scope;

  // if (!(args.Length() == 1 && args[0]->IsNumber())) {
  //   return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickName(Number)")));
  // }

  // return String::New(SDL_JoystickName(UnwrapJoystick(args[0])));
}

Handle<Value> sdl::JoystickNumAxes(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickNumAxes(Joystick)")));
  }

  return Number::New(SDL_JoystickNumAxes(UnwrapJoystick(args[0]->ToObject())));
}

Handle<Value> sdl::JoystickNumButtons(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickNumButtons(Joystick)")));
  }

  return Number::New(SDL_JoystickNumButtons(UnwrapJoystick(args[0]->ToObject())));
}

Handle<Value> sdl::JoystickNumBalls(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickNumBalls(Joystick)")));
  }

  return Number::New(SDL_JoystickNumBalls(UnwrapJoystick(args[0]->ToObject())));
}

Handle<Value> sdl::JoystickNumHats(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickNumHats(Joystick)")));
  }

  return Number::New(SDL_JoystickNumHats(UnwrapJoystick(args[0]->ToObject())));
}

Handle<Value> sdl::JoystickClose(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickClose(Joystick)")));
  }

  SDL_JoystickClose(UnwrapJoystick(args[0]->ToObject()));

  return Undefined();
}

Handle<Value> sdl::JoystickUpdate(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickUpdate()")));
  }

  SDL_JoystickUpdate();
  return Undefined();
}

Handle<Value> sdl::JoystickEventState(const Arguments& args) {
  HandleScope scope;

  int state;
  if (args.Length() == 0) {
    state = SDL_QUERY;
  } else {
    if (!(args.Length() == 1 && args[0]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickEventState([Boolean])")));
    }
    state = args[0]->BooleanValue() ? SDL_ENABLE : SDL_IGNORE;
  }
  return Boolean::New(SDL_JoystickEventState(state));
}

Handle<Value> sdl::MapRGB(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 4 && args[0]->IsObject() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected MapRGB(PixelFormat, Number, Number, Number)")));
  }

  SDL_PixelFormat* fmt = UnwrapPixelFormat(args[0]->ToObject());
  int r = args[1]->Int32Value();
  int g = args[2]->Int32Value();
  int b = args[3]->Int32Value();

  return Number::New(SDL_MapRGB(fmt, r, g, b));
}

Handle<Value> sdl::MapRGBA(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 5 && args[0]->IsObject() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber() && args[4]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected MapRGBA(PixelFormat, Number, Number, Number, Number)")));
  }

  SDL_PixelFormat* fmt = UnwrapPixelFormat(args[0]->ToObject());
  int r = args[1]->Int32Value();
  int g = args[2]->Int32Value();
  int b = args[3]->Int32Value();
  int a = args[4]->Int32Value();

  return Number::New(SDL_MapRGBA(fmt, r, g, b, a));
}

Handle<Value> sdl::GetRGB(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsNumber() && args[1]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetRGB(Number, PixelFormat)")));
  }

  int pixel = args[0]->Int32Value();
  SDL_PixelFormat* fmt = UnwrapPixelFormat(args[1]->ToObject());
  ::Uint8 r, g, b;

  SDL_GetRGB(pixel, fmt, &r, &g, &b);

  Local<Object> rgb = Object::New();
  rgb->Set(String::New("r"), Number::New(r));
  rgb->Set(String::New("g"), Number::New(g));
  rgb->Set(String::New("b"), Number::New(b));

  return scope.Close(rgb);
}

Handle<Value> sdl::GetRGBA(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsNumber() && args[1]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetRGBA(Number, PixelFormat)")));
  }

  int pixel = args[0]->Int32Value();
  SDL_PixelFormat* fmt = UnwrapPixelFormat(args[1]->ToObject());
  ::Uint8 r, g, b, a;

  SDL_GetRGBA(pixel, fmt, &r, &g, &b, &a);

  Local<Object> rgba = Object::New();
  rgba->Set(String::New("r"), Number::New(r));
  rgba->Set(String::New("g"), Number::New(g));
  rgba->Set(String::New("b"), Number::New(b));
  rgba->Set(String::New("a"), Number::New(a));

  return scope.Close(rgba);
}

Handle<Value> sdl::TTF::Init(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TTF::Init()")));
  }

  if (TTF_Init() < 0) {
    return ThrowException(Exception::Error(String::Concat(
      String::New("TTF::Init: "),
      String::New(TTF_GetError())
    )));
  }

  return Undefined();
}

Handle<Value> sdl::TTF::OpenFont(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TTF::OpenFont(String, Number)")));
  }

  String::Utf8Value file(args[0]);
  int ptsize = (args[1]->Int32Value());

  TTF_Font* font = TTF_OpenFont(*file, ptsize);
  if (font == NULL) {
    return ThrowException(Exception::Error(String::Concat(
      String::New("TTF::OpenFont: "),
      String::New(TTF_GetError())
    )));
  }
  return scope.Close(WrapFont(font));
}

// TODO: Rewrite for SDL2.
// static Handle<Value> sdl::TTF::RenderTextBlended(const Arguments& args) {
//   HandleScope scope;

//   if (!(args.Length() == 3 && args[0]->IsObject() && args[1]->IsString() && args[2]->IsNumber())) {
//     return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TTF::RenderTextBlended(Font, String, Number)")));
//   }

//   SDL_PixelFormat* vfmt = SDL_GetVideoInfo()->vfmt;
//   TTF_Font* font = UnwrapFont(args[0]->ToObject());
//   String::Utf8Value text(args[1]);
//   int colorCode = args[2]->Int32Value();

//   Uint8 r, g, b;
//   SDL_GetRGB(colorCode, vfmt, &r, &g, &b);

//   SDL_Color color;
//   color.r = r;
//   color.g = g;
//   color.b = b;

//   SDL_Surface *resulting_text;
//   resulting_text = TTF_RenderText_Blended(font, *text, color);
//   if (!resulting_text) {
//     return ThrowException(Exception::Error(String::Concat(
//       String::New("TTF::RenderTextBlended: "),
//       String::New(TTF_GetError())
//     )));
//   }
//   return scope.Close(WrapSurface(resulting_text));
// }

// TODO: make an async version so this can be used in loops or parallel load images
Handle<Value> sdl::IMG::Load(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsString())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected IMG::Load(String)")));
  }

  String::Utf8Value file(args[0]);

  SDL_Surface *image;
  image=IMG_Load(*file);
  if(!image) {
    return ThrowException(Exception::Error(String::Concat(
      String::New("IMG::Load: "),
      String::New(IMG_GetError())
    )));
  }

  Handle<Object> ret = Object::New();
  SurfaceWrapper* wrap = new SurfaceWrapper(ret);
  wrap->surface_ = image;
  return scope.Close(ret);
}

Handle<Value> sdl::GL::SetAttribute(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsNumber() && args[1]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetAttribute(Number, Number)")));
  }

  int attr = args[0]->Int32Value();
  int value = args[1]->Int32Value();

  if (SDL_GL_SetAttribute((SDL_GLattr)attr, value)) return ThrowSDLException(__func__);
  return Undefined();
}

Handle<Value> sdl::GL::GetAttribute(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetAttribute(Number)")));
  }

  int attr = args[0]->Int32Value();
  int value;

  if (SDL_GL_GetAttribute((SDL_GLattr)attr, &value)) return ThrowSDLException(__func__);

  return Number::New(value);
}

NODE_MODULE(node_sdl, init)
