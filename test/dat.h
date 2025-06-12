#include <stdint.h>
#include <stdlib.h>

#include "../src/util/conf.h"
#include "../src/util/vec/float3.h"
#include "t_conf.h"
#include "t_util.h"
#include "test.h"

testdat tests[] = {
	{"",		   test_float3_norm,        &(float3){2.0F, 0.67F, 5.0F}                                  },
	{"",		   test_float3_norm,        &(float3){0.2F, 0.4F, 0.1F}                                   },
	{"",		   test_compat_endianess,   (uint32_t[]){0x15F59267}                                      },
	{"k=v",        test_procbuf,            &(struct test_procbuf){"key=val", "key", "val", 0}            },
	{"sometxt",    test_procbuf,            &(struct test_procbuf){"sometxt", "sometxt", "", CONF_ESYNTAX}},
	{"comment",    test_procbuf,            &(struct test_procbuf){"# comment", "", "", CONF_ENODAT}      },
	{"empty",      test_procbuf,            &(struct test_procbuf){"", "", "", CONF_ENODAT}               },
	{"LF",         test_procbuf,            &(struct test_procbuf){"\n", "", "", CONF_ENODAT}             },
	{"CRLF",       test_procbuf,            &(struct test_procbuf){"\r\n", "", "", CONF_ENODAT}           },
	{"k=v (LF)",   test_procbuf,            &(struct test_procbuf){"k=v\na", "k", "v", 0}                 },
	{"k=v (CRLF)", test_procbuf,            &(struct test_procbuf){"k=v\r\na", "k", "v", 0}               },
	{"get",        test_matchopt,           &(struct test_matchopt){"key3", 2}                            },
	{"invalid",    test_matchopt,           &(struct test_matchopt){"nono", -1}                           },
	{"",		   test_colour32_endianess, NULL														  },
	{"",		   test_procval_i32,        NULL														  },
	{"",		   test_procval_u64,        NULL														  },
	{"",		   test_procval_f32,        NULL														  },
	{"",		   test_procval_str,        NULL														  },
	{"",		   test_procval_str_predef, NULL														  },
	{"",		   test_procval_fstr,       NULL														  },
	{"",		   test_procval_fstr_trunc, NULL														  },
	{"",		   test_procval_eparse,     NULL														  },
	{"",		   test_getpat,             NULL														  },
};
