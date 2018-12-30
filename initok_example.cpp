//
// INITOK C++ Example
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
#include "initok/initok.hpp"

#include "iostream"
#include "fstream"

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
    std::cout << INITOK_EXAMPLE_USAGE << std::endl;
    return -1;
  }

  std::ifstream ini(argv[1], std::ios::binary | std::ios::ate);
  if (!ini.is_open()) {
    std::cout << "ERROR: Couldn't open file " << argv[1] << '!' << std::endl
              << INITOK_EXAMPLE_USAGE << std::endl;
    return -2;
  }
  std::streamsize ini_size = ini.tellg();
  ini.seekg(0, std::ios::beg);

  char* ini_buff = new char[ini_size + 1];
  if (!ini.read(ini_buff, ini_size)) {
    std::cout << "ERROR: Failed to read from file " << argv[1] <<'!' << std::endl
              << INITOK_EXAMPLE_USAGE << std::endl;
    return -3;
  }
  ini_buff[ini_size] = '\0';
  ini.close();

  INITokenizer initok(ini_buff);
  INIToken* token = initok.GetNextToken();
  while (token) {
    std::cout << "TYPE: "     << ITT_TYPE_STR[token->type] << " "
              << "SIZE: "     << token->size               << " "
              << "TOKEN: \""  << token->token              << "\""
              << std::endl;
    token = initok.GetNextToken();
  }
  delete[] ini_buff;

  return 0;
}