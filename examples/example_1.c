/**
 * MIT License
 *
 * Copyright (c) 2024 R. D. Poor <rdpoor # gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @brief This is a simple example of using mu_json_parse_c_str(). 

In lieu of a Makefile, to compile, run and clean the example:

cc -Wall -I../src example_1.c -o example_1 ../src/mu_json.c ../src/mu_str.c ; ./example_1 ; rm ./example_1
*/

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
    const char *json = " {\"a\":111, \"b\":[22.2, true], \"c\":{}}  ";

    int n_tokens = mu_json_parse_c_str(tokens, N_DEMO_TOKENS, json, NULL);
    for (int i = 0; i < n_tokens; i++) {
        mu_json_token_t *t = &tokens[i];
        printf("token[%d]: %7s depth=%d, ", i, token_type_string(t),
               mu_json_token_depth(t));
        print_token_slice(t);
    }
}
