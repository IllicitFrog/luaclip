#include "luaclip.h"
#include <X11/extensions/Xfixes.h>

luaclip::luaclip(lua_State *Lua) : L(Lua), running(true) {
  display = XOpenDisplay(NULL);
  if (!display) {
    lc_error *e = new lc_error{L, "XOpenDisplay failed"};
    sendError(e);
  }
  int N = DefaultScreen(display);
  window = XCreateSimpleWindow(display, RootWindow(display, N), 0, 0, 1, 1, 0,
                               BlackPixel(display, N), WhitePixel(display, N));
}

luaclip::~luaclip() {
  running = false;
  owned = false;
  t.join();
}

// Start main clipboard loop
void luaclip::daemon() { t = std::thread(&luaclip::run, this); }

// Search clipboard history and return up to five results
void luaclip::search(std::string str) {
  std::array<std::string, 5> ret = clipboard.search(str);
  lc_clip *c = new lc_clip{L, ret[0], ret[1], ret[2], ret[3], ret[4]};
  sendClip(c);
}

void luaclip::remove(std::string str) { clipboard.remove(str); }

void luaclip::recent() {
  std::array<std::string, 5> ret = clipboard.recent();
  lc_clip *c = new lc_clip{L, ret[0], ret[1], ret[2], ret[3], ret[4]};
  sendClip(c);
}

// Take ownership of clipboard share current string
void luaclip::select(std::string str) {
  Atom selection = XInternAtom(display, "CLIPBOARD", False);
  clipboard.insert(str);
  current = str;
  XSetSelectionOwner(display, selection, window, CurrentTime);
}

// Main X11 clipboard loop
void luaclip::run() {
  XEvent event = {0};
  int event_base, error_base;

  Atom UTF8 = XInternAtom(display, "UTF8_STRING", True);
  Atom XSEL_DATA = XInternAtom(display, "XSEL_DATA", False);
  Atom targets = XInternAtom(display, "TARGETS", False);
  Atom text = XInternAtom(display, "TEXT", False);
  Atom selection = XInternAtom(display, "CLIPBOARD", False);

  XFixesQueryExtension(display, &event_base, &error_base);

  XFixesSelectSelectionInput(display, DefaultRootWindow(display), selection,
                             XFixesSetSelectionOwnerNotifyMask);

  while (running) {
    if (XPending(display)) {
      XNextEvent(display, &event);
      if (event.type == event_base + XFixesSelectionNotify &&
          ((XFixesSelectionNotifyEvent *)&event)->selection == selection) {
        if (XGetSelectionOwner(display, selection) == window) {
          owned = true;
          while (owned) {
            if (XPending(display)) {
              while (XPending(display)) {
                XNextEvent(display, &event);
                if (event.type == SelectionRequest) {
                  XSelectionEvent ev = {0};
                  int req = 0;
                  ev.type = SelectionNotify;
                  ev.display = event.xselectionrequest.display;
                  ev.requestor = event.xselectionrequest.requestor;
                  ev.selection = event.xselectionrequest.selection;
                  ev.time = event.xselectionrequest.time;
                  ev.target = event.xselectionrequest.target;
                  ev.property = event.xselectionrequest.property;

                  if (ev.target == targets) {
                    req = XChangeProperty(ev.display, ev.requestor, ev.property,
                                          XA_ATOM, 32, PropModeReplace,
                                          (unsigned char *)&UTF8, 1);
                  } else if (ev.target == text || ev.target == XA_STRING) {
                    req = XChangeProperty(ev.display, ev.requestor, ev.property,
                                          XA_STRING, 8, PropModeReplace,
                                          (unsigned char *)current.c_str(),
                                          current.length());
                  } else if (ev.target == UTF8) {
                    req = XChangeProperty(ev.display, ev.requestor, ev.property,
                                          UTF8, 8, PropModeReplace,
                                          (unsigned char *)current.c_str(),
                                          current.length());
                  } else {
                    ev.property = None;
                  }
                  if ((req & 2) == 0) {
                    XSendEvent(display, ev.requestor, 0, 0, (XEvent *)&ev);
                  }
                } else if (event.type == SelectionClear) {
                  owned = false;
                }
              }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
          }
        } else {
          Atom target;
          unsigned char *data;
          int format;
          unsigned long n, size;
          XConvertSelection(display, selection, UTF8, XSEL_DATA, window,
                            CurrentTime);
          XSync(display, False);
          XNextEvent(display, &event);
          if (event.type == SelectionNotify &&
              event.xselection.selection == selection) {
            if (event.xselection.property) {
              XGetWindowProperty(
                  event.xselection.display, event.xselection.requestor,
                  event.xselection.property, 0L, (~0L), 0, AnyPropertyType,
                  &target, &format, &size, &n, &data);

              if (target == UTF8 || target == XA_STRING) {
                std::string str((char *)data);
                if (!str.empty()) {
                  clipboard.insert(str);
                }
              }
              XDeleteProperty(display, event.xselection.requestor,
                              event.xselection.property);
            }
          }
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

static int luaclip_remove(lua_State *L) {
  (*reinterpret_cast<luaclip **>(luaL_checkudata(L, 1, LUA_CLIP)))
      ->remove(luaL_checkstring(L, 2));
  return 0;
}

static int luaclip_recent(lua_State *L) {
  (*reinterpret_cast<luaclip **>(luaL_checkudata(L, 1, LUA_CLIP)))->recent();
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
      {"search", luaclip_search}, {"recent", luaclip_recent},
      {"daemon", luaclip_daemon}, {"select", luaclip_select},
      {"remove", luaclip_remove}, {NULL, NULL},
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
