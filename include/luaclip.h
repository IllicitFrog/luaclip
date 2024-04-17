#include "buffer.h"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <atomic>
#include <glib.h>
#include <lua.hpp>
#include <string>
#include <thread>

#define LUA_CLIP "luaclip"

typedef struct lc_error {
  lua_State *L;
  std::string message;
} lc_error;

typedef struct lc_clip {
  lua_State *L;
  std::string ret1;
  std::string ret2;
  std::string ret3;
  std::string ret4;
  std::string ret5;
} lc_clip;

class luaclip {
public:
  luaclip(lua_State *);
  ~luaclip();

  void search(std::string);
  void recent();
  void daemon();
  void select(std::string);
  void remove(std::string);

private:
  lua_State *L;
  buffer_t clipboard;
  std::string current;
  std::thread t;
  std::atomic<bool> running;
  std::atomic<bool> owned;

  Display *display;
  Window window;
  void run();

  static gboolean sendClip(gpointer data) {
    lc_clip *c = static_cast<lc_clip *>(data);
    lua_getglobal(c->L, "awesome");
    lua_getfield(c->L, -1, "emit_signal");
    lua_remove(c->L, -2);
    lua_pushstring(c->L, "clipboard::selection");
    lua_newtable(c->L);
    lua_pushnumber(c->L, 1);
    lua_pushstring(c->L, c->ret1.c_str());
    lua_rawset(c->L, -3);
    lua_pushnumber(c->L, 2);
    lua_pushstring(c->L, c->ret2.c_str());
    lua_rawset(c->L, -3);
    lua_pushnumber(c->L, 3);
    lua_pushstring(c->L, c->ret3.c_str());
    lua_rawset(c->L, -3);
    lua_pushnumber(c->L, 4);
    lua_pushstring(c->L, c->ret4.c_str());
    lua_rawset(c->L, -3);
    lua_pushnumber(c->L, 5);
    lua_pushstring(c->L, c->ret5.c_str());
    lua_rawset(c->L, -3);
    lua_call(c->L, 2, 0);
    delete c;
    return G_SOURCE_REMOVE;
  }

  static gboolean sendError(gpointer data) {
    lc_error *e = static_cast<lc_error *>(data);
    lua_getglobal(e->L, "awesome");
    lua_getfield(e->L, -1, "emit_signal");
    lua_remove(e->L, -2);
    lua_pushstring(e->L, "error::notification");
    lua_pushstring(e->L, "luaclip error");
    lua_pushstring(e->L, e->message.c_str());
    lua_call(e->L, 3, 0);
    delete e;
    return G_SOURCE_REMOVE;
  }
};
