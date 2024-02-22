# mu_json
`mu_json` is a compact, efficient JSON parser designed with embedded systems
in mind.

`mu_json`'s features include:
* Written in pure C
* Conforms to IETF RFC-7159
* No calls to `malloc()` or `free()`
* In-place, copy-free parsing of JSON strings
* Provides functions to traverse parsed JSON tree (parent, child, sibling)
* Includes `mu_str` module for safe copy-free string processing

`mu_json` takes a JSON formatted string and parses it into its constituent 
tokens.  Once parsed, `mu_json` provides functions to navigate among the
resulting tokens via parent, child, next_sibling, prev_sibling, etc.

**in place**: Unlike many parsers, `mu_json` does not try to interpret the 
parsed values.  Rather, it simply slices each token into a substring and 
associates a JSON type to it (ARRAY, OBJECT, NUMBER, TRUE, FALSE, NULL) and the 
token's depth in the parsed tree.

**malloc-free**: The caller provies token storage, so mu_json never calls 
malloc() or free().

## Very special thanks to:

Douglas Crockford for creating JSON in the first place, and for the compact
[JSON_checker.c](https://github.com/douglascrockford/JSON-c) design on which 
much of mu_json is based.

Serge Zaitsev for creating [jsmn](https://github.com/zserge/jsmn), an efficient
copy-free JSON parser, which inspired the "sliced token" approach used by 
`mu_json`.

Nicolas Seriot for creating the comprehensive 
[JSONTestSuite](https://github.com/nst/JSONTestSuite), used as a basis for the
`mu_json` unit tests.

## A simple example:

```
#include "mu_json.h"
#include "mu_str.h"
#include <stdio.h>

// Helper functions
const char *token_type_string(mu_json_token_t *t) {
    switch (mu_json_token_type(t)) {
    case MU_JSON_TOKEN_TYPE_UNKNOWN: return "UNKNOWN";
    case MU_JSON_TOKEN_TYPE_ARRAY: return "ARRAY";
    case MU_JSON_TOKEN_TYPE_OBJECT: return "OBJECT";
    case MU_JSON_TOKEN_TYPE_STRING: return "STRING";
    case MU_JSON_TOKEN_TYPE_NUMBER: return "NUMBER";
    case MU_JSON_TOKEN_TYPE_INTEGER: return "INTEGER";
    case MU_JSON_TOKEN_TYPE_TRUE: return "TRUE";
    case MU_JSON_TOKEN_TYPE_FALSE: return "FALSE";
    case MU_JSON_TOKEN_TYPE_NULL: return "NULL";
    }
}

void print_token_slice(mu_json_token_t *t) {
    mu_str_t *s = mu_json_token_slice(t);
    printf("'%.*s'\n", (int)mu_str_length(s), mu_str_buf(s));
}

#define N_DEMO_TOKENS 10

int main() {
    mu_json_token_t tokens[N_DEMO_TOKENS];
    const char *json = " {\"a\":111, \"b\":[222, true], \"c\":{}}  ";

    int n_tokens = mu_json_parse_c_str(tokens, N_DEMO_TOKENS, json, NULL);
    for (int i = 0; i < n_tokens; i++) {
        mu_json_token_t *t = &tokens[i];
        printf("token[%d]: %6s depth=%d, ", i, token_type_string(t),
               mu_json_token_depth(t));
        print_token_slice(t);
    }
}
```
Compiling and running the above results in:
```
token[0]: OBJECT depth=0, '{"a":111, "b":[222, true], "c":{}}'
token[1]: STRING depth=1, '"a"'
token[2]: NUMBER depth=1, '111'
token[3]: STRING depth=1, '"b"'
token[4]:  ARRAY depth=1, '[222, true]'
token[5]: NUMBER depth=2, '222'
token[6]:   TRUE depth=2, 'true'
token[7]: STRING depth=1, '"c"'
token[8]: OBJECT depth=1, '{}'
```