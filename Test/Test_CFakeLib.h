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
 * @File    Test_CFakeLib.c
 * @Brief   Unit Testing for CFakeLib
 ******************************************************************************/

#ifndef __TEST_C_FAKE_LIB_H__
#define __TEST_C_FAKE_LIB_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmockery.h" /* <== A great && free UT framework given by Google. */

#define UT_CASES_GROUP_TEST_C_FAKE_LIB      \
    unit_test(Test_Fake),                   \

void Test_Fake(void **state);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_C_FAKE_LIB_H__ */

