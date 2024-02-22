/**
 * @file mu_json.h
 *
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
 * TODO:
 * * Start numbers as INTEGER type, promote to NUMBER type only as needed.
 * * Extend `finish_token()` to check that the token type being finished 
 *   matches the expected type, and write unit test to verify.
 * * Decide what to do with the `void *arg` argument on the parsers.  Provide
 *   more detailed error reporting if non-null?
 * * mu_json_token_t *mu_json_find_key(mu_json_token_t *object, const char *c_str, bool deep);
 * * mu_json_token_t *mu_json_find_key_value(mu_json_token_t *object, const char *c_str, bool deep);
 */

#ifndef _MU_JSON_H_
#define _MU_JSON_H_

// *****************************************************************************
// Includes

#include "mu_str.h"
#include <stddef.h>
#include <stdint.h>

// *****************************************************************************
// C++ Compatibility

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// Public types and definitions

/**
 * @brief Enumeration of error codes returned by mu_json functions.
 */
typedef enum {
    MU_JSON_ERR_NONE = 0,        /**< No error */
    MU_JSON_ERR_BAD_FORMAT = -1, /**< Illegal JSON format */
    MU_JSON_ERR_NO_TOKENS = -2,  /**< Not enough tokens provided */
    MU_JSON_ERR_INCOMPLETE = -3  /**< JSON ended with unterminated form */
} mu_json_err_t;

/**
 * @brief Enumeration of token flags used by mu_json.
 */
typedef enum {
    MU_JSON_TOKEN_FLAG_IS_FIRST = 1,  /**< Token is first in token list */
    MU_JSON_TOKEN_FLAG_IS_LAST = 2,   /**< Token is last in token list */
    MU_JSON_TOKEN_FLAG_IS_SEALED = 4  /**< Token end has been found */
} mu_json_token_flags_t;

#define DEFINE_MU_JSON_TOKEN_TYPES(M)                                          \
    M(MU_JSON_TOKEN_TYPE_UNKNOWN) /* ?       */                                \
    M(MU_JSON_TOKEN_TYPE_ARRAY)   /* [ ... ] */                                \
    M(MU_JSON_TOKEN_TYPE_OBJECT)  /* { ... } */                                \
    M(MU_JSON_TOKEN_TYPE_STRING)  /* "..."   */                                \
    M(MU_JSON_TOKEN_TYPE_NUMBER)  /* 123.45  */                                \
    M(MU_JSON_TOKEN_TYPE_INTEGER) /* 12345 (specialized number) */             \
    M(MU_JSON_TOKEN_TYPE_TRUE)    /* true    */                                \
    M(MU_JSON_TOKEN_TYPE_FALSE)   /* false   */                                \
    M(MU_JSON_TOKEN_TYPE_NULL)    /* null    */

/**
 * @brief Enumeration of JSON token types.
 */
#define EXPAND_MU_JSON_TOKEN_TYPE_ENUMS(_name) _name,
typedef enum {
    DEFINE_MU_JSON_TOKEN_TYPES(EXPAND_MU_JSON_TOKEN_TYPE_ENUMS)
} mu_json_token_type_t;

/**
 * @brief Structure representing a JSON token.
 */
typedef struct {
    mu_str_t json; /**< Slice of the original JSON string */
    uint8_t type;  /**< mu_json_token_type cast to uint8_t */
    uint8_t flags; /**< mu_json_token_flags_t cast to uint8_t */
    int16_t depth; /**< 0 = toplevel, n+1 = child of n... */
} mu_json_token_t;

// *****************************************************************************
// Public declarations

/**
 * @defgroup json_parsing JSON Parsing
 *
 * @brief Functions for parsing JSON-formatted strings.
 *
 * This group contains functions for parsing JSON-formatted strings.
 */

/**
 * @brief Parse a JSON-formatted string provided as a null-terminated C string.
 * 
 * @ingroup json_parsing
 *
 * Parse the JSON-formatted string `json` into a series of tokens stored in 
 * user-supplied `token_store` containing `max_tokens`. The JSON string
 * is expected to be null-terminated.
 *
 * @param token_store A user-supplied array of tokens for receiving the parsed
 *        results.
 * @param max_tokens Number of tokens in `token_store`.
 * @param json The JSON-formatted string to be parsed, provided as a 
 *        null-terminated C string.
 * @param arg Reserved for future features.  For now, pass NULL.
 * @return Returns the number of parsed tokens if parsing is successful, or a
 *         negative error code if an error occurs.
 */
int mu_json_parse_c_str(mu_json_token_t *token_store, size_t max_tokens,
                        const char *json, void *arg);

/**
 * @brief Parse a JSON-formatted string provided as a mu_str_t object.
 * 
 * @ingroup json_parsing
 *
 * Parse the JSON-formatted string in mu_json` into a series of tokens stored in 
 * user-supplied `token_store` containing `max_tokens` elements.
 *
 * @param token_store A user-supplied array of tokens for receiving the parsed
 *        results.
 * @param max_tokens Number of tokens in `token_store`.
 * @param mu_json Pointer to a mu_str_t object containing the JSON-formatted 
 *        string to be parsed.
 * @param arg Reserved for future features.  For now, pass NULL.
 * @return Returns the number of parsed tokens if parsing is successful, or a
 *         negative error code if an error occurs.
 */
int mu_json_parse_mu_str(mu_json_token_t *token_store, size_t max_tokens,
                         mu_str_t *mu_json, void *arg);

/**
 * @brief Parse a JSON-formatted string stored in a buffer.
 * 
 * @ingroup json_parsing
 *
 * Parse the JSON-formatted string in mu_json` into a series of tokens stored in 
 * user-supplied `token_store` containing `max_tokens` elements.
 * Psrse the JSON-formatted buffer `buf` of length `buflen` into a series of
 * tokens stored in the user-supplied `token_store` containing `max_tokens`
 * elements.  
 * 
 * @param token_store A user-supplied array of tokens for receiving the parsed
 *        results.
 * @param max_tokens Number of tokens in `token_store`.
 * @param buf Pointer to a uint8_t array containing the JSON-formatted buffer.
 * @param buflen Length of the JSON-formatted buffer `buf`.
 * @param arg Reserved for future features.  For now, pass NULL.
 * @return Returns the number of parsed tokens if parsing is successful, or a
 *         negative error code if an error occurs.
 */
int mu_json_parse_buffer(mu_json_token_t *token_store, size_t max_tokens,
                         const uint8_t *buf, size_t buflen, void *arg);

/**
 * @defgroup token_accessor Accessors for parsed tokens
 * 
 * These functions access various fields from parsed tokens, valid only after
 * a successful call to one of the @ref json_parsing functions.
 */

/**
 * @brief Retrieve the mu_str_t slice associated with a JSON token.
 *
 * @ingroup token_accessor
 * 
 * @param token Pointer to the JSON token.
 * @return Pointer to the mu_str_t slice associated with the JSON token.
 */
mu_str_t *mu_json_token_slice(mu_json_token_t *token);

/**
 * @brief Retrieve the JSON type of a JSON token.
 *
 * @ingroup token_accessor
 * 
 * @param token Pointer to the JSON token.
 * @return JSON type of the JSON token.
 */
mu_json_token_type_t mu_json_token_type(mu_json_token_t *token);

/**
 * @brief Retrieve the depth of a JSON token in the JSON hierarchy.
 *
 * @ingroup token_accessor
 * 
 * Return the depth of the JSON token `token` in the JSON hierarchy.
 * The depth is 0 for the top-level tokens, and increases for nested tokens.
 *
 * @param token Pointer to the JSON token.
 * @return Depth of the JSON token.
 */
int mu_json_token_depth(mu_json_token_t *token);

/**
 * @brief Check if a JSON token is the first token in the list of parsed tokens.
 *
 * @ingroup token_accessor
 * 
 * This is useful for traversing a list of parsed tokens sequentially.
 * 
 * @param token Pointer to a parsed JSON token.
 * @return true if the token is the first token in the token list, false 
 *         otherwise.
 */
bool mu_json_token_is_first(mu_json_token_t *token);

/**
 * @brief Check if a JSON token is the last token in the list of parsed tokens.
 *
 * @ingroup token_accessor
 * 
 * This is useful for traversing a list of parsed tokens sequentially.
 * 
 * @param token Pointer to a parsed JSON token.
 * @return true if the token is the last token in the token list, false 
 *         otherwise.
 */
bool mu_json_token_is_last(mu_json_token_t *token);

/**
 * @defgroup json_navigation Navigating parsed JSON tokens
 *
 * @brief Functions for navigating a parsed colletion of JSON tokens.
 * 
 * Following a successful call to one of the json_parsing functions
 * (e.g. mu_json_parse_c_str(), etc), `token_store` will contain the
 * parsed tokens stored in preorder: `tokens[0]` is always the root
 * of the JSON tree.
 * 
 * You can navigate the parsed tree using sequential methods:
 * 
 * * mu_json_token_prev()
 * * mu_json_token_next()
 * 
 * ... or by using the structured methods:
 * 
 * * mu_json_token_root()
 * * mu_json_token_parent()
 * * mu_json_token_child()
 * * mu_json_token_prev_sibling()
 * * mu_json_token_next_sibling()
 */

/**
 * @brief Retrieves the previous JSON token in a token list.
 *
 * @ingroup json_navigation
 * 
 * Return a pointer to the previous JSON token in the token list, or NULL
 * if already at the beginning of the list.
 *
 * @param token Pointer to the JSON token. * 
 * @return Pointer to the previous JSON token, or NULL if at start of list.
 */
mu_json_token_t *mu_json_token_prev(mu_json_token_t *token);

/**
 * @brief Retrieves the next JSON token in a token list.
 *
 * @ingroup json_navigation
 * 
 * Return a pointer to the next JSON token in the token list, or NULL
 * if already at the beginning of the list.
 *
 * @param token Pointer to the JSON token. * 
 * @return Pointer to the next JSON token, or NULL if at the end of the list.
 */
mu_json_token_t *mu_json_token_next(mu_json_token_t *token);

/**
 * @brief Retrieves the root token of a JSON token list
 *
 * @ingroup json_navigation
 * 
 * This is always equivalent to tokens[0], assuming at least one token was
 * found.
 *
 * @param token Pointer to the JSON token.
 * @return Pointer to the root JSON token.
 */
mu_json_token_t *mu_json_token_root(mu_json_token_t *token);

/**
 * @brief Retrieves the parent token of a JSON token.
 *
 * @ingroup json_navigation
 * 
 * Return a pointer to the parent token of a JSON token, or NULL if this token
 * has no parent (e.g. is already at the root).
 *
 * @param token Pointer to the JSON token.
 * @return Pointer to the parent JSON token, or NULL if token is the root token.
 */
mu_json_token_t *mu_json_token_parent(mu_json_token_t *token);

/**
 * @brief Retrieves the first child token of a JSON token.
 *
 * @ingroup json_navigation
 * 
 * Return a pointer to the first child token of the JSON token `token`.
 *
 * @param token Pointer to the JSON token.
 * @return Pointer to the first child JSON token, or NULL if `token` has no children.
 */
mu_json_token_t *mu_json_token_child(mu_json_token_t *token);

/**
 * @brief Retrieves the previous sibling token of a JSON token.
 *
 * @ingroup json_navigation
 * 
 * Return a pointer to the previous sibling token of the JSON token `token`.
 *
 * @param token Pointer to the JSON token.
 * @return Pointer to the previous sibling JSON token, or NULL if no previous sibling exists.
 */
mu_json_token_t *mu_json_token_prev_sibling(mu_json_token_t *token);

/**
 * @brief Retrieves the next sibling token of a JSON token.
 *
 * @ingroup json_navigation
 * 
 * Return a pointer to the next sibling token of the JSON token `token`.
 *
 * @param token Pointer to the JSON token.
 * @return Pointer to the next sibling JSON token, or NULL if no next sibling exists.
 */
mu_json_token_t *mu_json_token_next_sibling(mu_json_token_t *token);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_JSON_H_ */
