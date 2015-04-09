/*
 * Copyright 2015 Nokia
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/******************************************************************************
 * @File    CFakeTypes.h
 * @Brief   It provide the type defines for internal use in CFake.
 ******************************************************************************/


#ifndef __C_FAKE_TYPES_H__
#define __C_FAKE_TYPES_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "CFake.h"

#define FAKE_FLEXIBLE_ARRAY_SIZE    0

#ifdef __GNUC__
#define FAKE_ATTR(attrs) __attribute__(attrs)
#else
#define FAKE_ATTR(attrs)
#endif

typedef unsigned char       TFakeU8;
typedef unsigned short int  TFakeU16;
typedef unsigned long  int  TFakeU32;
typedef unsigned int        TFakeUInt;

typedef enum EFakeBool {
    EFakeBool_False = 0,
    EFakeBool_True  = 1,
} EFakeBool;

#ifdef __cplusplus
}
#endif

#endif /* __C_FAKE_TYPES_H__ */


