#include "buffer.h"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xfixes.h>
#include <boost/filesystem.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/string.hpp>
#include <fstream>
#include <string>
#include <array>
#include <glib.h>
#include <lua.hpp>

#define LUA_CLIP "luaclip"

typedef struct la_error {
  lua_State *L;
  std::string message;
}la_error;

class clip {
public:
  clip(std::array<std::string, 5> ret, lua_State *L) : ret(ret), L(L) {}
  std::array<std::string, 5> ret;
  lua_State *L;
};

class luaclip {
public:
  luaclip(lua_State *);
  ~luaclip();

  void search(const std::string &);
  void daemon();
  void select(std::string);

private:
  lua_State *L;
  buffer_t clipboard;
  std::fstream file;
  std::string current;
  bool isOwner = false;

  Display *display;
  Window window;
  Atom selection;
  void run();

  void writeBuffer();
  void loadBuffer();

  static gboolean sendClip(gpointer data) {
    clip *c = static_cast<clip *>(data);
    lua_getglobal(c->L, "awesome");
    lua_getfield(c->L, -1, "emit_signal");
    lua_remove(c->L, -2);
    lua_pushstring(c->L, "clipboard::selection");
    lua_pushstring(c->L, c->ret[0].c_str());
    lua_pushstring(c->L, c->ret[1].c_str());
    lua_pushstring(c->L, c->ret[2].c_str());
    lua_pushstring(c->L, c->ret[3].c_str());
    lua_pushstring(c->L, c->ret[4].c_str());
    lua_call(c->L, 6, 0);
    delete c;
    return G_SOURCE_REMOVE;
  }

  static gboolean sendError(gpointer data) {
    la_error *e = static_cast<la_error *>(data);
    lua_getglobal(e->L, "awesome");
    lua_getfield(e->L, -1, "emit_signal");
    lua_remove(e->L, -2);
    lua_pushstring(e->L, "display::error");
    lua_pushstring(e->L, e->message.c_str());
    lua_call(e->L, 3, 0);
    delete e;
    return G_SOURCE_REMOVE;
  }
};
