#include "luaclip.h"
#include <mutex>
#include <thread>

luaclip::luaclip(lua_State *Lua) : L(Lua) {}

luaclip::~luaclip() { file.close(); }

// Start main clipboard loop
void luaclip::daemon() {
  display = XOpenDisplay(nullptr);

  if (!display) {
    sendError(new la_error{L, "XOpenDisplay failed"});
    return;
  }

  selection = XInternAtom(display, "CLIPBOARD", False);
  int N = DefaultScreen(display);
  Window window =
      XCreateSimpleWindow(display, RootWindow(display, N), 0, 0, 1, 1, 0,
                          BlackPixel(display, N), WhitePixel(display, N));
  std::thread t = std::thread(&luaclip::run, this);
}

// Load clipboard history from disk
void luaclip::loadBuffer() {
  boost::filesystem::path path =
      strcat(getenv("HOME"), "/.config/luaclip/buffer.data");
  if (!boost::filesystem::exists(path.parent_path())) {
    boost::filesystem::create_directories(path);
    file.open(path,
              std::fstream::in | std::fstream::out | std::fstream::binary);
  } else {
    file.open(path,
              std::fstream::in | std::fstream::out | std::fstream::binary);
    boost::archive::binary_iarchive iarchive(file);
    iarchive >> clipboard;
  }
}

// Write cliboard history to disk
void luaclip::writeBuffer() {
  boost::archive::binary_oarchive oarchive(file);
  oarchive << clipboard;
}

// Search clipboard history and return up to five results
void luaclip::search(const std::string &str) {
  clip *c = new clip(clipboard.search(str), L);
  sendClip(c);
}

// Take ownership of clipboard share current string
void luaclip::select(std::string str) {
  XSetSelectionOwner(display, selection, window, CurrentTime);
  if (XGetSelectionOwner(display, selection) == window) {
    current = str;
    isOwner = true;
  }
}

// Main X11 clipboard loop
void luaclip::run() {

  XEvent event;
  XSelectionEvent ev;
  XSelectionRequestEvent *xsr;
  int format;
  int req;
  char *data;
  unsigned long n, size;

  std::mutex io_mutex;

  Atom target;
  Atom UTF8 = XInternAtom(display, "UTF8_STRING", True);
  Atom XSEL_DATA = XInternAtom(display, "XSEL_DATA", False);
  Atom targets = XInternAtom(display, "TARGETS", False);
  Atom text = XInternAtom(display, "TEXT", False);
  if (UTF8 == None) {
    UTF8 = XA_STRING;
  }

  while (true) {
    XNextEvent(display, &event);
    switch (event.type) {
    case SelectionRequest: {
      if (isOwner) {
        if (event.xselectionrequest.selection != selection) {
          break;
        }
        xsr = &event.xselectionrequest;
        ev = {0};
        req = 0;
        ev.type = SelectionNotify;
        ev.display = xsr->display;
        ev.requestor = xsr->requestor;
        ev.selection = xsr->selection;
        ev.time = xsr->time;
        ev.target = xsr->target;
        ev.property = xsr->property;
        if (ev.target == targets) {
          req = XChangeProperty(ev.display, ev.requestor, ev.property, XA_ATOM,
                                32, PropModeReplace, (unsigned char *)&UTF8, 1);
        } else if (ev.target == text || ev.target == XA_STRING) {
          req = XChangeProperty(ev.display, ev.requestor, ev.property,
                                XA_STRING, 8, PropModeReplace,
                                (unsigned char *)current.c_str(),
                                (int)current.size());
        } else if (ev.target == UTF8) {
          req = XChangeProperty(
              ev.display, ev.requestor, ev.property, UTF8, 8, PropModeReplace,
              (unsigned char *)current.c_str(), (int)current.size());
        } else {
          ev.property = None;
          if ((req & 2) == 8)
            XSendEvent(display, ev.requestor, 0, 0, (XEvent *)&ev);
        }
      }
      break;
    }
    case SelectionNotify:
      if (event.xselection.selection == selection) {
        if (event.xselection.property) {
          XGetWindowProperty(
              event.xselection.display, event.xselection.requestor,
              event.xselection.property, 0, LONG_MAX, False, AnyPropertyType,
              &target, &format, &size, &n, (unsigned char **)&data);
          if (target == UTF8 || target == XA_STRING) {
            std::lock_guard<std::mutex> _(io_mutex);
            clipboard.insert(std::string(data, size));
            writeBuffer();
          }
          XDeleteProperty(display, event.xselection.requestor,
                          event.xselection.property);
        }
      }
      break;
    case SelectionClear:
      if (event.xselectionclear.display == display) {
        isOwner = false;
        current = "";
      }
      break;
    }
  }
}

static int luaclip_new(lua_State *L) {
  (*reinterpret_cast<luaclip **>(lua_newuserdata(L, sizeof(luaclip *)))) =
      new luaclip(L);
  luaL_setmetatable(L, LUA_CLIP);
  return 1;
}

static int luaclip_delete(lua_State *L) {
  (*reinterpret_cast<luaclip **>(luaL_checkudata(L, 1, LUA_CLIP)))->~luaclip();
  return 0;
}

static int luaclip_daemon(lua_State *L) {
  (*reinterpret_cast<luaclip **>(luaL_checkudata(L, 1, LUA_CLIP)))->daemon();
  return 0;
}

static int luaclip_search(lua_State *L) {
  (*reinterpret_cast<luaclip **>(luaL_checkudata(L, 1, LUA_CLIP)))
      ->search(luaL_checkstring(L, 2));
  return 0;
}

static int luaclip_select(lua_State *L) {
  (*reinterpret_cast<luaclip **>(luaL_checkudata(L, 1, LUA_CLIP)))
      ->select(luaL_checkstring(L, 2));
  return 0;
}

static void register_clipboard(lua_State *L) {
  static const luaL_Reg meta[] = {
      {"__gc", luaclip_delete},
      {NULL, NULL},
  };

  static const luaL_Reg funcs[] = {
      {"search", luaclip_search},
      {"daemon", luaclip_daemon},
      {"select", luaclip_select},
      {NULL, NULL},
  };
  luaL_newmetatable(L, LUA_CLIP);
  luaL_setfuncs(L, meta, 0);
  luaL_newlib(L, funcs);
  lua_setfield(L, -2, "__index");
  lua_pop(L, 1);

  lua_pushcfunction(L, luaclip_new);
}
extern "C" int luaopen_libluaclip(lua_State *L) {
  luaL_openlibs(L);
  register_clipboard(L);
  return 1;
}
