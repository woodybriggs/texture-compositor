
#include "cbuild.h"

#define CFLAGS "-Wall", "-Wextra", "-std=c11", "-pedantic", "-Wno-strict-prototypes", "-Wno-deprecated", "-Ithirdparty/glfw/include", "-g"
#define LFLAGS "-framework", "IOKit", "-framework", "OpenGL", "-framework", "Cocoa", "-Lthirdparty/glfw/lib-arm64", "-lglfw3"

void build_compositor()
{
  Cmd cmd = {0};
  cmd_append(&cmd, "cc", "-g");
  cmd_append(&cmd, "-Wall", "-Wextra", "-std=c11", "-pedantic", "-Wno-strict-prototypes", "-Wno-deprecated");

  cmd_append(&cmd, "-Ithirdparty/glfw/include");

  cmd_append(&cmd, "-framework", "IOKit");
  cmd_append(&cmd, "-framework", "OpenGL");
  cmd_append(&cmd, "-framework", "Cocoa");

  cmd_append(&cmd, "-Lthirdparty/glfw/lib-arm64", "-lglfw3");

  cmd_append(&cmd, "-o", "bin/compositor");
  
  cmd_append(&cmd, "compositor.c");

  cmd_run_sync(&cmd);
}

int main(int argc, char **argv)
{
  build_compositor();

  return 0;
}
