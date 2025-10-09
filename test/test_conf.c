/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#include "test_conf.h"

#include <math.h>
#include <string.h>

#include "../src/types.h"
#include "../src/util/conf.h"
#include "test.h"

void test_conf_procbuf(const char *restrict buf, const char *restrict expect_key, const char *restrict expect_val, int expect_return) {
	usize len = strlen(buf) + 1;
	char k[len], v[len];
	*k = '\0', *v = '\0';
	(void)(assert_true(conf_procbuf(buf, k, v, len) == expect_return) &&
		assert_true(!strcmp(k, expect_key)) &&
		assert_true(!strcmp(v, expect_val)));
}

void test_conf_matchopt(struct conf_entry *opts, usize optc, const char *restrict key, int expect_index) {
	usize idx = opts - conf_matchopt(opts, optc, key);
	idx = (ssize)idx < 0 ? -idx : idx;
	int i = idx < optc ? (int)idx : -1;
	assert_true(i == expect_index);
}

void test_conf_procval_int(const char *val, u64 expect_value, int type) {
	u8 out[sizeof(u64)] = {0};
	assert_true(!conf_procval(&(struct conf_entry){NULL, out, type}, val));
	assert_true(memcmp(out, &expect_value, sizeof(u64)) == 0);
}

void test_conf_procval_f32(const char *val, f32 expect_value) {
	u8 out[4];
	f32 result;
	conf_procval(&(struct conf_entry){NULL, out, CONF_F32}, val);
	memcpy(&result, out, 4);
	assert_true(fabsf(expect_value - result) < 1e-9f);
}

void test_procval_str(void) {
	char *out = NULL;
	(void)(assert_true(!conf_procval(&(struct conf_entry){NULL, (void *)&out, CONF_STR}, "here comes the sowon")) &&
		assert_false(strcmp("here comes the sowon", out)));
	free(out);
}

void test_procval_str_predef(void) {
	char *out = strdup("owo");
	(void)(assert_true(!conf_procval(&(struct conf_entry){NULL, (void *)&out, CONF_STR}, "i leak if I don't free")) &&
		assert_true(!strcmp("i leak if I don't free", out)));
	free(out);
}

void test_procval_fstr(void) {
	char buf[16];
	struct conf_fstr str = {sizeof(buf), buf};
	(void)(assert_true(!conf_procval(&(struct conf_entry){NULL, &str, CONF_FSTR}, "hewwoo wowld")) &&
		assert_true(!strcmp(str.out, "hewwoo wowld")));
}

void test_procval_fstr_trunc(void) {
	char buf[8];
	struct conf_fstr str = {sizeof(buf), buf};
	(void)(assert_true(!conf_procval(&(struct conf_entry){NULL, &str, CONF_FSTR}, "hewwooo wowld")) &&
		assert_true(!strcmp(str.out, "hewwooo")));
}

void test_procval_eparse(void) {
	i32 out;
	assert_true(conf_procval(&(struct conf_entry){NULL, &out, CONF_I32}, "owo") == CONF_EPARSE);
}

void test_conf_getpat(void) {
	char *path;
#if defined(__linux__)
	/* test without setting environment variables. */
	unsetenv("XDG_CONFIG_HOME");
	unsetenv("HOME");
	path = conf_getpat("/mypath");
	assert_true(path == NULL);
	free(path);

	/* test with setting HOME. */
	setenv("HOME", "/home/test", 1);
	path = conf_getpat("/mypath");
	assert_true(path != NULL && strcmp(path, "/home/test/.config/mypath") == 0);
	free(path);
	conf_getpat("/mypath");

	/* test with setting XDG_CONFIG_HOME. */
	setenv("XDG_CONFIG_HOME", "/etc/xdg", 1);
	path = conf_getpat("/mypath");
	assert_true(path != NULL && strcmp(path, "/etc/xdg/mypath") == 0);
	free(path);
#elif defined(__APPLE__)
	/* test without setting environment variables. */
	unsetenv("HOME");
	path = conf_getpat("/mypath");
	assert_true(path == NULL);
	free(path);

	/* test with setting HOME. */
	setenv("HOME", "/home/test", 1);
	path = conf_getpat("/mypath");
	assert_true(path != NULL && strcmp(path, "/home/test/Library/Application Support/mypath") == 0);
	free(path);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
	/* test without setting environment variables. */
	_putenv("APPDATA=");
	_putenv("USERPROFILE=");
	path = conf_getpat("\\mypath");
	assert_true(path == NULL);
	free(path);

	/* test with setting USERPROFILE */
	_putenv("USERPROFILE=C:\\Users\\test");
	path = conf_getpat("\\mypath");
	assert_true(path != NULL && strcmp(path, "C:\\Users\\test\\AppData\\Roaming\\mypath") == 0);
	free(path);

	/* test with setting APPDATA */
	_putenv("APPDATA=C:\\Users\\test\\AppData\\Roaming");
	path = conf_getpat("\\mypath");
	assert_true(path != NULL && strcmp(path, "C:\\Users\\test\\AppData\\Roaming\\mypath") == 0);
	free(path);
#else
	assert_fail("unsupported operating system; no tests have been written.");
#endif
}
