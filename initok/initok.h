//
// INITOK C
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
// This is C implementation of INITOK.
//
// It is meant for C only! You can still use it in C++ if you define "INITOK_USE_C" before including this header, but
// you should use "initok.hpp" file instead!
//
// In exactly one unit file, define "INITOK_IMPL" before including this header.
//
// INITOK depends on three std functions that can be replaced by defining one of the following macros:
// - INITOK_TOUPPER - toupper, used to convert chars to uppercase when parsing section/entry names
// - INITOK_ISSPACE - isspace, used to check and skip whitespace characters
// - INITOK_ASSERT  - assert, used for assertions
//
// INITOK works inside memory on given buffer.
// Buffer shall be filled with contents of INI file.
// Buffer is expected to be NULL-terminated and writable.
// INITOK doesn't "own" the buffer and user is expected to free it after it is no longer needed! Take in mind though
// that all tokens are pointing directly to this buffer - save yourself from some headaches :)
//
// All tokens are writable, so user is able to make modifications to returned tokens as needed.
// Also, as tokens are all by design located in exact position where they occur in given buffer, they can be cached
// inside some table in some wrapper and used directly (provided wrapper owns the buffer and keeps it while tokens are
// of use). Also, as size of tokens is being returned along its type, if someone creates smart wrapper, there shall be
// no need for "strlen" and similar functions in it, saving some cycles.
//
// LIMITATIONS:
// Right now, can't tokenize Unicode encoded INI files.
// Another limitation is that there is no comment support whatsoever.
// One minor limitation affecting parsing of all tokens is that all leading whitespace characters before each token are removed, leading to following minor drawbacks:
// - spaces inside "[]" are parsed as part of section name, as leading whitespace are removed only from before '[' sign and further spaces are ignored
// - values can't have leading whitespace, trailing whitespaces characters are left as-is (excluding "end of line" ones)
// - '=' is expected to be located right behind the entry name, as otherwise trailing whitespace characters will be included as part of entry name
//
// USAGE:
// - zero-initalize new "initok_t" variable
// - call INITOK_SetData(<initok>, <ini_buff>) where <initok> is reference to initok_t and <ini_buff> contains INI data
// - call INITOK_GetNextToken(<initok>) where <initok> is reference to initok_t while token is not NULL (means EOF)

#ifndef FSL_INITOK_H
#define FSL_INITOK_H

#ifdef __cplusplus
#if !defined(FSL_INITOK_HPP) && !defined(INITOK_USE_C)
#warning "Included \"initok.h\" in C++ unit! You should include \"initok.hpp\" instead! To disable this warning, define \"INITOK_USE_C\" before including header."
#endif // FSL_INITOK_HPP

extern "C" {
#endif

#include <stddef.h>

#ifndef INITOK_TOUPPER
#include <ctype.h>
#define INITOK_TOUPPER toupper
#endif // INITOK_TOUPPER

#ifndef INITOK_ISSPACE
#define INITOK_ISSPACE isspace
#endif // INITOK_ISSPACE

#ifndef INITOK_ASSERT
#include <assert.h>
#define INITOK_ASSERT assert
#endif // INITOK_ASSERT

#ifdef INITOK_IMPL
#define INITOK_INLINE extern inline
#else // INITOK_IMPL
#define INITOK_INLINE inline
#endif // INITOK_IMPL

typedef enum {
  ITT_STATE_EOF,
  ITT_STATE_SECTENT,
  ITT_STATE_VALUE
} itt_state_t;

typedef enum {
  ITT_TYPE_NONE,
  ITT_TYPE_SECTION,
  ITT_TYPE_ENTRY,
  ITT_TYPE_VALUE
} itt_type_t;

typedef struct {
  char*      token;
  size_t     size;
  itt_type_t type;
} ini_token_t;

typedef struct {
  char*       data;
  char*       data_ptr;
  itt_state_t state;
  ini_token_t token;
} initok_t;

INITOK_INLINE void INITOK_SetData(initok_t* initok, char* ini_buff) {
  // init assertions
  INITOK_ASSERT(initok);
  // set data
  initok->data     = ini_buff;
  initok->data_ptr = ini_buff;
  initok->state    = ITT_STATE_SECTENT;
}

INITOK_INLINE ini_token_t* INITOK_GetNextToken(initok_t* initok) {
  // init assertions
  INITOK_ASSERT(initok && initok->data && initok->data_ptr);
  // if EOF, nothing to parse
  if (initok->state == ITT_STATE_EOF) {
    return NULL;
  }
  // temporary variables
  itt_state_t curState = initok->state;
  char endC = '\0';
  char* ptr = initok->data_ptr;
  // cleanup whitespace
  while (*ptr && INITOK_ISSPACE(*ptr)) {
    ++ptr;
  }
  // check validity
  if (!ptr[0]) {
    initok->state = ITT_STATE_EOF;
    return NULL;
  }
  // null token
  initok->token.token = NULL;
  initok->token.size  = 0;
  initok->token.type  = ITT_TYPE_NONE;
  // parse according to state
  switch (initok->state) {
    case ITT_STATE_SECTENT: // section/entry
      if (*ptr == '[') {  // section
        // beginning of new section - read head
        ++ptr;
        // init token
        initok->token.token = ptr;
        initok->token.type  = ITT_TYPE_SECTION;
        // set terminator
        endC = ']';
      }
      else {  // entry 
        // init token
        initok->token.token = ptr;
        initok->token.type  = ITT_TYPE_ENTRY;
        // set terminator
        endC = '=';
        // change state
        initok->state = ITT_STATE_VALUE;
      }
    break;
    case ITT_STATE_VALUE: // value
      // init token
      initok->token.token = ptr;
      initok->token.type  = ITT_TYPE_VALUE;
      // set terminator
      endC = '\r';
      // change state
      initok->state = ITT_STATE_SECTENT;
    break;
  }
  // parse token
  if (curState == ITT_STATE_SECTENT) {
    while (*ptr && (*ptr != endC) && (*ptr != '\n')) {
      *ptr = (char)INITOK_TOUPPER((unsigned char)*ptr);
      ++ptr;
    }
  }
  else {
    while (*ptr && (*ptr != endC) && (*ptr != '\n')) {
      ++ptr;
    }
  }
  // check for EOF
  if (!ptr[0]) {
    // EOF
    initok->state = ITT_STATE_EOF;
  }
  // calc token size
  initok->token.size = (size_t)(ptr - initok->token.token) + 1;
  // set NULL and advance
  *(ptr++) = '\0';
  // assign ptr
  initok->data_ptr = ptr;
  // return token
  return &initok->token;
}

#ifdef __cplusplus
}
#endif

#endif // FSL_INITOK_H