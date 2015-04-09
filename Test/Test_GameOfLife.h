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
 * @File    Test_GameOfLife.c
 * @Brief   Unit Testing for GameOfLife.c
 ******************************************************************************/

#ifndef __TEST_GAME_OF_LIFE_H__
#define __TEST_GAME_OF_LIFE_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmockery.h" /* <== A great && free UT framework given by Google. */

#define UT_CASES_GROUP_TEST_GAME_OF_LIFE                    \
    unit_test(Test_LifeNext_NotEnoughMemoryToCopyOldLife),  \
    unit_test(Test_LifeNext_Success),                       \

void Test_LifeNext_NotEnoughMemoryToCopyOldLife(void **state);
void Test_LifeNext_Success(void **state);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_GAME_OF_LIFE_H__ */

