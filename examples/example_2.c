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
 * @brief Demonstrate traversal functinos. 

In lieu of a Makefile, to compile, run and clean the example:

cc -Wall -I../src example_2.c -o example_2 ../src/mu_json.c ../src/mu_str.c ; ./example_2 ; rm ./example_2
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
    default: return "ILLEGAL";
    }
}

/**
 * @brief Return a string representation of a token in user supplied buffer.
 */
char *tts(mu_json_token_t *t, char *buf, size_t buflen) {
    if (t == NULL) {
        snprintf(buf, buflen, "<null token>");
    } else {
        mu_str_t *s = mu_json_token_slice(t);
        snprintf(buf, buflen, "<token: %6s depth=%d value='%.*s'>",
                 token_type_string(t), mu_json_token_depth(t),
                 (int)mu_str_length(s), mu_str_buf(s));
    }
    return buf;
}

#define BUfSIZ 40
/**
 * @brief Print parent, child, prev sibling, next sibling of a node.
 */
void explore(int index, mu_json_token_t *tokens) {
    char buf[BUFSIZ];
    mu_json_token_t *t = &tokens[index];

    printf("Exploring[%d]: %s:\n", index, tts(t, buf, BUFSIZ));
    // "structured" navigation
    printf("        parent = %s\n", tts(mu_json_token_parent(t), buf, BUFSIZ));
    printf("         child = %s\n", tts(mu_json_token_child(t), buf, BUFSIZ));
    printf("  prev sibling = %s\n",
           tts(mu_json_token_next_sibling(t), buf, BUFSIZ));
    printf("  next sibling = %s\n",
           tts(mu_json_token_next_sibling(t), buf, BUFSIZ));
    // "sequential" navigation
    printf("          prev = %s\n", tts(mu_json_token_prev(t), buf, BUFSIZ));
    printf("          next = %s\n", tts(mu_json_token_next(t), buf, BUFSIZ));
    printf("      is first = %s\n",
           mu_json_token_is_first(t) ? "true" : "false");
    printf("       is last = %s\n",
           mu_json_token_is_last(t) ? "true" : "false");
}

#define N_DEMO_TOKENS 10

int main() {
    mu_json_token_t tokens[N_DEMO_TOKENS];
    const char *json = " {\"a\":111, \"b\":[222, true], \"c\":{}}  ";
    int n_tokens = mu_json_parse_c_str(tokens, N_DEMO_TOKENS, json, NULL);

    for (int i = 0; i < n_tokens; i++) {
        printf("=======\n");
        explore(i, tokens);
    }
}
