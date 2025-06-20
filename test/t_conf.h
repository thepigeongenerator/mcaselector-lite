#pragma once
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../src/util/conf.h"
#include "../src/util/types.h"
#include "test.h"

// Environment saver structure for conf_getpat tests
#if defined(__linux__) || defined(__APPLE__) || defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
struct test_getpat_envdat {
	char* xdg_config_home;
	char* home;
	char* appdata;
	char* userprofile;
};

/* save the current environment variables */
static void env_save(struct test_getpat_envdat* s) {
	char const* tmp;

	tmp = getenv("XDG_CONFIG_HOME");
	s->xdg_config_home = tmp ? strdup(tmp) : NULL;

	tmp = getenv("HOME");
	s->home = tmp ? strdup(tmp) : NULL;

	tmp = getenv("APPDATA");
	s->appdata = tmp ? strdup(tmp) : NULL;

	tmp = getenv("USERPROFILE");
	s->userprofile = tmp ? strdup(tmp) : NULL;
}

/* restores the environment variables to what they were before */
static void env_restore(struct test_getpat_envdat* s) {
#ifdef _WIN32
	if (s->xdg_config_home) _putenv_s("XDG_CONFIG_HOME", s->xdg_config_home);
	else _putenv("XDG_CONFIG_HOME=");
	if (s->home) _putenv_s("HOME", s->home);
	else _putenv("HOME=");
	if (s->appdata) _putenv_s("APPDATA", s->appdata);
	else _putenv("APPDATA=");
	if (s->userprofile) _putenv_s("USERPROFILE", s->userprofile);
	else _putenv("USERPROFILE=");
#else
	if (s->xdg_config_home) setenv("XDG_CONFIG_HOME", s->xdg_config_home, 1);
	else unsetenv("XDG_CONFIG_HOME");
	if (s->home) setenv("HOME", s->home, 1);
	else unsetenv("HOME");
	if (s->appdata) setenv("APPDATA", s->appdata, 1);
	else unsetenv("APPDATA");
	if (s->userprofile) setenv("USERPROFILE", s->userprofile, 1);
	else unsetenv("USERPROFILE");
#endif
	free(s->xdg_config_home);
	free(s->home);
	free(s->appdata);
	free(s->userprofile);
}
#endif

/* check procbuf's functionality */
struct test_procbuf {
	char const* in;   // data in
	char const* xkey; // expected key
	char const* xval; // expected value
	int xret;         // expected return type
};
int test_procbuf(void* arg) {
	struct test_procbuf* dat = arg;
	size_t len = strlen(dat->in) + 1;
	char k[len], v[len];
	*k = '\0', *v = '\0';
	return assert_true(conf_procbuf(dat->in, k, v, len) == dat->xret) ||
		assert_true(!strcmp(k, dat->xkey)) ||
		assert_true(!strcmp(v, dat->xval));
}

/* check matchopt functionality */
struct test_matchopt {
	char const* key; // key to search for (key1, key2, key3)
	int xidx;        // expect index (<0 is NULL, may not be more than 2)
};
int test_matchopt(void* arg) {
	struct test_matchopt* dat = arg;
	struct conf_entry opts[] = {
		{"key1", NULL, 0},
		{"key2", NULL, 0},
		{"key3", NULL, 0},
	};
	struct conf_entry* xopt = dat->xidx < 0 ? NULL : opts + dat->xidx;
	return assert_true(conf_matchopt(opts, 3, dat->key) == xopt);
}

struct test_procval_int {
	char const* val;
	u64 xres;
	u8 type;
};
int test_procval_int(void* arg) {
	struct test_procval_int* dat = arg;
	u64 out = 0;
	return assert_true(!conf_procval(&(struct conf_entry){NULL, &out, dat->type}, dat->val)) ||
		assert_true(out == dat->xres);
}

int test_procval_f32(void* arg) {
	(void)arg;
	f32 out;
	return assert_true(!conf_procval(&(struct conf_entry){NULL, &out, CONF_F32}, "3.14159265")) ||
		assert_true(fabsf(out - 3.14159265F) < 1e-6F);
}

int test_procval_str(void* arg) {
	(void)arg;
	char* out = NULL;
	int ret = assert_true(!conf_procval(&(struct conf_entry){NULL, (void*)&out, CONF_STR}, "here comes the sowon")) ||
		assert_false(strcmp("here comes the sowon", out));
	free(out);
	return ret;
}

int test_procval_str_predef(void* arg) {
	(void)arg;
	char* out = strdup("owo");
	int ret = assert_true(!conf_procval(&(struct conf_entry){NULL, (void*)&out, CONF_STR}, "i leak if I don't free")) ||
		assert_true(!strcmp("i leak if I don't free", out));
	free(out);
	return ret;
}

int test_procval_fstr(void* arg) {
	(void)arg;
	char buf[16];
	struct conf_fstr str = {sizeof(buf), buf};
	return assert_true(!conf_procval(&(struct conf_entry){NULL, &str, CONF_FSTR}, "hewwoo wowld")) ||
		assert_true(!strcmp(str.out, "hewwoo wowld"));
}

int test_procval_fstr_trunc(void* arg) {
	(void)arg;
	char buf[8];
	struct conf_fstr str = {sizeof(buf), buf};
	return assert_true(!conf_procval(&(struct conf_entry){NULL, &str, CONF_FSTR}, "hewwooo wowld")) ||
		assert_true(!strcmp(str.out, "hewwooo"));
}

int test_procval_eparse(void* arg) {
	(void)arg;
	i32 out;
	return assert_true(conf_procval(&(struct conf_entry){NULL, &out, CONF_I32}, "owo") == CONF_EPARSE);
}

/* ensure paths are being set correctly */
int test_getpat(void* arg) {
	(void)arg;
	struct test_getpat_envdat envs;
	env_save(&envs);
	int ret = 0;
	char* path = NULL;

#ifdef __linux__
	setenv("XDG_CONFIG_HOME", "/test/config", 1);
	path = conf_getpat("/app");
	ret |= assert_true(path && !strcmp(path, "/test/config/app"));
	free(path);
	unsetenv("XDG_CONFIG_HOME");

	setenv("HOME", "/test/home", 1);
	path = conf_getpat("/app");
	ret |= assert_true(path && !strcmp(path, "/test/home/.config/app"));
	free(path);

	unsetenv("HOME");
#elif defined(__APPLE__)
	setenv("HOME", "/test/home", 1);
	path = conf_getpat("/app");
	ret |= assert_true(path && !strcmp(path, "/test/home/Library/Application Support/app"));
	free(path);

	unsetenv("HOME");
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
	_putenv("APPDATA=C:\\test\\appdata");
	path = conf_getpat("\\app");
	ret |= assert_true(path && !strcmp(path, "C:\\test\\appdata\\app"));
	free(path);

	_putenv("APPDATA=");
	_putenv("USERPROFILE=C:\\test\\user");
	path = conf_getpat("\\app");
	ret |= assert_true(path && !strcmp(path, "C:\\test\\user\\AppData\\Roaming\\app"));
	free(path);

	_putenv("USERPROFILE=");
#endif
	void* ptr;
	ret |= assert_true(!(ptr = conf_getpat("anything")));
	free(ptr);

	env_restore(&envs);
	return ret;
}
