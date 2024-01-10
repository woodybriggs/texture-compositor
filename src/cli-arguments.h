#ifndef CLI_ARGUMENTS_H
#define CLI_ARGUMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  CompositeRequestType_Compose,
  CompositeRequestType_AOMR,
} CompositeRequestType;

typedef struct {

  CompositeRequestType reqType;

  const char * id_map_filename;

  const char * red_ch_filename;
  const char * grn_ch_filename;
  const char * blu_ch_filename;

  const char * output_filename;
} CompositeRequest;

CompositeRequest cli_args_parse(int argc, char ** argv) {

  CompositeRequest result = {0};

  for (int i = 1; i < argc; i += 2) {
    const char * argKey = argv[i];
    const char * argVal = argv[i+1];
    if (argVal == NULL) {
      fprintf(stdout, "missing value for argument %s\n", argKey);
      exit(1);
    }

    if (strncmp(argKey, "-type", 5) == 0){
      if (strncmp(argVal, "compose", 7) == 0) {
        result.reqType = CompositeRequestType_Compose;
      }
      else if (strncmp(argVal, "aomr", 4) == 0) {
        result.reqType = CompositeRequestType_AOMR;
      }
      else {
        fprintf(stdout, "no valid value for -type 'compose' | 'aomr' got: %s\n", argVal);
      }

    }

    if (strncmp(argKey, "-idmap", 6) == 0) {
      result.id_map_filename = argVal;
    }

    if (strncmp(argKey, "-red", 4) == 0) {
      result.red_ch_filename = argVal;
    }

    if (strncmp(argKey, "-green", 6) == 0) {
      result.grn_ch_filename = argVal;
    }

    if (strncmp(argKey, "-blue", 5) == 0) {
      result.blu_ch_filename = argVal;
    }

    if (strncmp(argKey, "-out", 4) == 0) {
      result.output_filename = argVal;
    }
  }

  if (result.id_map_filename == NULL) {
    fprintf(stdout, "missing value for -idmap\n");
    exit(1);
  }

  if (result.red_ch_filename == NULL) {
    fprintf(stdout, "missing value for -red\n");
    exit(1);
  }

  if (result.grn_ch_filename == NULL) {
    fprintf(stdout, "missing value for -green\n");
    exit(1);
  }

  if (result.blu_ch_filename == NULL) {
    fprintf(stdout, "missing value for -blue\n");
    exit(1);
  }

  if (result.output_filename == NULL) {
    fprintf(stdout, "missing value for -out\n");
    exit(1);
  }

  return result;
}

#endif
