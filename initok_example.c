//
// INITOK C Example
// Created by Andrej Redeky.
// Copyright (c) 2019 Feldarian Softworks
//
// LICENSE: (MIT)
// Permission is hereby granted, free of charge, to any person obtaining a copy
//     of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
//     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
//     copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// DESCRIPTION:
// This file is meant as an usage example for INITOK.
// It takes one input INI file, tokenizes it (printing all tokens to stdout) and terminates.
//

#define INITOK_IMPL
#include "initok/initok.h"

#include "stdio.h"
#include "stdint.h"
#include "malloc.h"

#define INITOK_EXAMPLE_USAGE "INITOK Example usage:\n"   \
                             "  initok_cxx <ini_path>\n" \

static const char* ITT_TYPE_STR[] = {
    "NONE",
    "SECTION",
    "ENTRY",
    "VALUE"
};

int main(int argc, char** argv) {
  if (argc < 2) {
    fputs("ERROR: Missing commandline argument!", stderr);
    fputs(INITOK_EXAMPLE_USAGE, stdout);
    return -1;
  }

  FILE* ini = fopen(argv[1], "rb");
  if (!ini) {
    fprintf(stderr, "ERROR: Couldn't open file \"%s\"!\n", argv[1]);
    fputs(INITOK_EXAMPLE_USAGE, stdout);
    return -2;
  }
  _fseeki64(ini, 0, SEEK_END);
  size_t ini_size = (size_t)_ftelli64(ini);
  _fseeki64(ini, 0, SEEK_SET);

  char* ini_buff = (char*)malloc(ini_size + 1);
  if (fread(ini_buff, 1, ini_size, ini) != ini_size) {
    fprintf(stderr, "ERROR: Failed to read from file \"%s\"!\n", argv[1]);
    fputs(INITOK_EXAMPLE_USAGE, stdout);
    return -3;
  }
  ini_buff[ini_size] = '\0';
  fclose(ini);

  initok_t initok = { 0 };
  INITOK_SetData(&initok, ini_buff);
  ini_token_t* token = INITOK_GetNextToken(&initok);
  while (token) {
    fprintf(stdout, "TYPE: %s SIZE: %u TOKEN: \"%s\"\n", ITT_TYPE_STR[token->type], (uint32_t)token->size, token->token);
    token = INITOK_GetNextToken(&initok);
  }
  free(ini_buff);

  return 0;
}
