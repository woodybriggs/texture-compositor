#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/stat.h>


#define EA_INIT_CAP 64

#define typedef_ExapandableArray(type, name)\
  typedef struct {\
    type *data;\
    size_t count; \
    size_t capacity;\
  } name;


typedef_ExapandableArray(char, String_Builder);
typedef_ExapandableArray(char*, Cmd);


#define ea_append(ea, item)\
  do {\
    if ((ea)->count >= (ea)->capacity) {\
      (ea)->capacity = (ea)->capacity == 0 ? EA_INIT_CAP : (ea)->capacity*2;\
      (ea)->data = realloc((ea)->data, (ea)->capacity*sizeof(*(ea)->data));\
      assert((ea)->data != NULL && "not enough available memory to expand");\
    }\
    (ea)->data[(ea)->count++] = (item);\
  } while(0)

#define ea_foreach(type, item, ea)\
  for (type *item = (ea)->data; item < ((ea)->data + (ea)->count); item++)


void sb_append_null(String_Builder *sb) {
  ea_append(sb, NULL);
}


void sb_append_str(String_Builder *sb, const char * str) {
  while (*str != '\0') {
    ea_append(sb, *str);
    str += 1;
  }
}

void cmd_append_null(Cmd *cmd, ...) {
  va_list args;
  va_start(args, cmd);

  const char * arg = va_arg(args, const char *);
  while(arg != NULL) {
    ea_append(cmd, arg);
    arg = va_arg(args, const char *);
  }

  va_end(args);
  (void) cmd;
}

#define cmd_append(cmd, ...) cmd_append_null(cmd, __VA_ARGS__, NULL)

char * cstr_array_join(char * sep, char ** cstrs, size_t count)
{
    if (count == 0) {
      return "";
    }

    const size_t sep_len = strlen(sep);
    size_t len = 0;
    for (size_t i = 0; i < count; ++i) {
      len += strlen(cstrs[i]);
    }

    const size_t result_len = (count - 1) * sep_len + len + 1;
    char *result = malloc(sizeof(char) * result_len);
    if (result == NULL) {
      exit(69);
    }

    len = 0;
    for (size_t i = 0; i < count; ++i) {
        if (i > 0) {
            memcpy(result + len, sep, sep_len);
            len += sep_len;
        }

        size_t elem_len = strlen(cstrs[i]);
        memcpy(result + len, cstrs[i], elem_len);
        len += elem_len;
    }
    result[len] = '\0';

    return result;
}

void shell_escape(const char * str, String_Builder *out) {
  if (strchr(str, ' ')) {
    ea_append(out, '\'');
    sb_append_str(out, str);
    ea_append(out, '\'');
  } else {
    sb_append_str(out, str);
  }
}


char * cmd_render(Cmd *cmd, String_Builder *out) {
  ea_foreach(const char *, arg, cmd) {
    shell_escape(*arg, out);
    sb_append_str(out, " ");
  }
}

int cmd_run_async(Cmd *cmd) {
  String_Builder sb = {0};
  cmd_render(cmd, &sb);
  sb_append_null(&sb);

  fprintf(stderr, "[CMD] %s\n", sb.data);

  pid_t cpid = fork();
  if (cpid < 0) {
    fprintf(stderr, "[ERROR] could not fork child process %s\n", strerror(errno));
    return 0;
  }

  if (cpid == 0) {
    if (execvp(cmd->data[0], (char * const*) cmd->data) < 0) {
      fprintf(stderr, "[ERROR] could not exec child process %s\n", strerror(errno));
      exit(1);
    }
    assert(0 && "unreachable");
  }

  return cpid;
}

void pid_wait(pid_t pid) {
  int stat_loc;
  waitpid(pid, &stat_loc, 0);
}

int cmd_run_sync (Cmd *cmd) {
  pid_wait(cmd_run_async(cmd));
}