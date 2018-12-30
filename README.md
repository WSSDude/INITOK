# INITOK - Tiny and fast, header-only INI tokenizer for C/C++ INI parsing

## DESCRIPTION

__INITOK__ provides two header files - one with C implementation and one with C++ wrapper structure.  
You should include _initok.h_ in C units and _initok.hpp_ in C++ units.  
You will need both, _initok.h_ and _initok.hpp_ for C++ projects. For C-only projects, only _initok.h_ is required.  
Examples are provided for both, C and C++ in according unit files.

In exactly one unit file, define __INITOK_IMPL__ before including any of above headers to provide definitons for inline functions.  

INITOK depends on three std functions that can be replaced by defining one of the following macros:  
- __INITOK_TOUPPER__ - toupper, used to convert chars to uppercase when parsing section/entry names  
- __INITOK_ISSPACE__ - isspace, used to check and skip whitespace characters  
- __INITOK_ASSERT__  - assert, used for assertions  

INITOK works inside memory on given buffer.  
Buffer shall be filled with contents of INI file.  
Buffer is expected to be NULL-terminated and writable.  
INITOK doesn't _own_ the buffer and user is expected to free it after it is no longer needed! Take in mind though that all tokens are pointing directly to this buffer (save yourself from some headaches :) )  

All tokens are writable, so user is able to make modifications to returned tokens as needed.  
Also, as tokens are all by design located in exact position where they occur in given buffer, they can be cached inside some table in some wrapper structure and used directly (provided wrapper owns the buffer and keeps it while tokens are of use). As size of tokens is being returned along its type, if someone creates smart wrapper, there shall be no need for "strlen" and similar functions in it, saving some cycles.

## LIMITATIONS
Right now, can't tokenize Unicode encoded INI files.

## LICENSE

>MIT License
>
>Copyright (c) 2019 Feldarian Softworks
>
>Permission is hereby granted, free of charge, to any person obtaining a copy
>of this software and associated documentation files (the "Software"), to deal
>in the Software without restriction, including without limitation the rights
>to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
>copies of the Software, and to permit persons to whom the Software is
>furnished to do so, subject to the following conditions:
>
>The above copyright notice and this permission notice shall be included in all
>copies or substantial portions of the Software.
>
>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
>IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
>FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
>AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
>LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
>OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
>SOFTWARE.
