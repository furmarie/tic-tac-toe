#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

#define LIST_OF_FUNCS                   \
    FUNC(ttt_init, void, int, int, int) \
    FUNC(ttt_pre_reload, void*, void)   \
    FUNC(ttt_post_reload, void, void*)  \
    FUNC(ttt_draw, void, float)         \
    FUNC(ttt_reset, void, void)

#define FUNC(func, ret, ...) typedef ret func##_t(__VA_ARGS__);
LIST_OF_FUNCS
#undef FUNC
