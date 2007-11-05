/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGUTIL_LEXER_H
#define AUGUTIL_LEXER_H

#include "augutil/config.h"
#include "augsys/types.h"

#define AUG_LEXLABEL  0x01
#define AUG_LEXWORD   0x02
#define AUG_LEXPHRASE 0x04

typedef struct aug_lexer_* aug_lexer_t;

AUGUTIL_API aug_lexer_t
aug_createnetlexer(size_t size);

AUGUTIL_API aug_lexer_t
aug_createshelllexer(size_t size);

AUGUTIL_API int
aug_destroylexer(aug_lexer_t lexer);

AUGUTIL_API unsigned
aug_appendlexer(aug_lexer_t lexer, char ch);

AUGUTIL_API unsigned
aug_finishlexer(aug_lexer_t lexer);

AUGUTIL_API const char*
aug_lexertoken(aug_lexer_t lexer);

#endif /* AUGUTIL_LEXER_H */
