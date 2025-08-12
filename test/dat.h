#include <stdint.h>

#include "../src/util/conf.h"
#include "../src/util/intdef.h"
#include "t_arith.h"
#include "t_conf.h"
#include "test.h"

testdat tests[] = {
	{"ensure SAR",  test_sar,                NULL									    },
	{"k=v",         test_procbuf,            &(struct test_procbuf){"key=val", "key", "val", 0}                              },
	{"sometxt",     test_procbuf,            &(struct test_procbuf){"sometxt", "sometxt", "", CONF_ESYNTAX}                  },
	{"comment",     test_procbuf,            &(struct test_procbuf){"# comment", "", "", CONF_ENODAT}                        },
	{"empty",       test_procbuf,            &(struct test_procbuf){"", "", "", CONF_ENODAT}                                 },
	{"LF",          test_procbuf,            &(struct test_procbuf){"\n", "", "", CONF_ENODAT}                               },
	{"CRLF",        test_procbuf,            &(struct test_procbuf){"\r\n", "", "", CONF_ENODAT}                             },
	{"k=v (LF)",    test_procbuf,            &(struct test_procbuf){"k=v\na", "k", "v", 0}                                   },
	{"k=v (CRLF)",  test_procbuf,            &(struct test_procbuf){"k=v\r\na", "k", "v", 0}                                 },
	{"get",         test_matchopt,           &(struct test_matchopt){"key3", 2}                                              },
	{"invalid",     test_matchopt,           &(struct test_matchopt){"nono", -1}                                             },
	// NOTE: formatter is fucking with alignment making it use tabs @.@
	{"i32",         test_procval_int,        &(struct test_procval_int){"42", 42, CONF_I32}                                  },
	{"i32_neg",     test_procval_int,        &(struct test_procval_int){"-42", (u32)-42, CONF_I32}                           },
	{"u32_max",     test_procval_int,        &(struct test_procval_int){"4294967295", UINT32_MAX, CONF_U64}                  },
	{"u64",         test_procval_int,        &(struct test_procval_int){"3141592653589793238", 3141592653589793238, CONF_U64}},
	{"u8_overflow", test_procval_int,        &(struct test_procval_int){"256", 255, CONF_U8}                                 },
	{"",            test_procval_f32,        NULL									    },
	{"",            test_procval_str,        NULL									    },
	{"",            test_procval_str_predef, NULL									    },
	{"",            test_procval_fstr,       NULL									    },
	{"",            test_procval_fstr_trunc, NULL									    },
	{"",            test_procval_eparse,     NULL									    },
	{"",            test_getpat,             NULL									    },
};
