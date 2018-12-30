//
// INITOK C++
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
// This is C++ implementation of INITOK.
//
// It is meant for C++ only! For C units, you must use "initok.h" file instead!
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
//
// USAGE:
// - contruct new INITokenizer instance
// - call <initok>.SetData(<ini_buff>) where <initok> is INITokenizer instance and <ini_buff> contains INI data
// - call <initok>.GetNextToken() where <initok> is INITokenizer instance while token is not NULL (means EOF)

#ifndef FSL_INITOK_HPP
#define FSL_INITOK_HPP

#ifndef __cplusplus
#error "Included \"initok.hpp\" in C unit! Include and use \"initok.h\" instead!"
#endif

#include "initok.h"

typedef ini_token_t INIToken;

struct INITokenizer {
  public:
    INITokenizer() : m_Data{} {}
    explicit INITokenizer(char* ini_buff) : m_Data{} { SetData(ini_buff); }
    virtual ~INITokenizer() = default;

    void SetData(char* ini_buff) {
      INITOK_SetData(&m_Data, ini_buff);
    }
    INIToken* GetNextToken() {
      return INITOK_GetNextToken(&m_Data);
    }

  private:
    initok_t m_Data;
};

#endif // FSL_INITOK_HPP