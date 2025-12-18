/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#pragma once

#include "../src/types.h"
#include "../src/util/conf.h"

void test_conf_procbuf(const char *restrict buf, const char *restrict expect_key, const char *restrict expect_val, int expect_return);
void test_conf_matchopt(struct conf_entry *restrict opts, usize optc, const char *restrict key, int expect_index);
void test_conf_procval_int(const char *val, u64 expect_value, int type);
void test_conf_procval_f32(const char *val, f32 expect_value);
void test_conf_procval_fstr(const char *val, u64 expect_value, int type);
void test_conf_procval_str(const char *val, u64 expect_value, int type);
void test_procval_str(void);
void test_procval_str_predef(void);
void test_procval_fstr(void);
void test_procval_fstr_trunc(void);
void test_procval_eparse(void);
void test_conf_getpat(void);
