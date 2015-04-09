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
#include "Test_GameOfLife.h"
#include "CFake.h"

#define inline
#define main _target_main_
#include "GameOfLife.c"
#undef inline
#undef main

static int Stub_LifeCopy(SLife * life)
{
    int ret;

    printf("<<< Stub_LifeCopy called, ");

    check_expected(life);
    printf("check in-param life: 0x%X, ", (unsigned int)life);

    ret = (int)mock();
    printf("return 0x%x >>>\n", ret);

    return ret;
}

static void Stub_AbortLife(char * format, ...)
{
    printf("<<< Stub_AbortLife called >>>\n");
    check_expected(format);
    mock_assert(0, "\"Life is not aborted\"", __FILE__, __LINE__);
}

static void Stub_LifeUpdate(SLife * newLife, SLife * oldLife)
{
    printf("<<< Stub_LifeUpdate called, newLife: 0x%x, oldLife: 0x%x >>>\n",
           (int)newLife,
           (int)oldLife);
    check_expected(newLife);
    check_expected(oldLife);
}

static void inline Stub_LifeFree(SLife * life)
{
    printf("<<< Stub_LifeFree called, life: 0x%x >>>\n", (int)life);
    check_expected(life);
}

void Test_LifeNext_NotEnoughMemoryToCopyOldLife(void **state)
{
    SLife * oldLife = (SLife *)0x80000000; /* only used for checking in-param */

    FAKE_ON(LifeCopy, Stub_LifeCopy);
    FAKE_ON(AbortLife, Stub_AbortLife);

    /* Stub action defines for Stub_LifeCopy */
    will_return(Stub_LifeCopy, NULL);
    expect_value(Stub_LifeCopy, life, oldLife);

    /* Stub action defines for Stub_AbortLife */
    expect_not_value(Stub_AbortLife, format, NULL);

    expect_assert_failure(LifeNext(oldLife));

    FAKE_OFF(LifeCopy);
    FAKE_OFF(AbortLife);
}

void Test_LifeNext_Success(void **state)
{
    SLife * oldLife = (SLife *)0x80000000; /* only used for checking in-param */
    SLife * tmpLife = (SLife *)0x90000000; /* only used for checking in-param */

    FAKE_ON(LifeCopy,   Stub_LifeCopy);
    FAKE_ON(LifeUpdate, Stub_LifeUpdate);
    FAKE_ON(LifeFree,   Stub_LifeFree);

    /* Stub action defines for LifeCopy */
    will_return(Stub_LifeCopy, tmpLife);
    expect_value(Stub_LifeCopy, life, oldLife);

    /* Stub action defines for LifeUpdate */
    expect_value(Stub_LifeUpdate, oldLife, tmpLife);
    expect_value(Stub_LifeUpdate, newLife, oldLife);

    /* Stub action defines for LifeFree */
    expect_value(Stub_LifeFree, life, tmpLife);

    LifeNext(oldLife);

    FAKE_RESET();
}

