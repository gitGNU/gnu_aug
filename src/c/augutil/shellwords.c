/*
  Copyright (c) 2004, 2005, 2006, 2007, 2008, 2009 Mark Aylett <mark.aylett@gmail.com>

  This file is part of Aug written by Mark Aylett.

  Aug is released under the GPL with the additional exemption that compiling,
  linking, and/or using OpenSSL is allowed.

  Aug is free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation; either version 2 of the License, or (at your option) any later
  version.

  Aug is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
  details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 51
  Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#define AUGUTIL_BUILD
#include "augutil/shellwords.h"
#include "augctx/defs.h"

AUG_RCSID("$Id$");

/*
  Shell Word Semantics

  Escape Character

  A non-quoted backslash [\] is the escape character.  It preserves the
  literal value of the next character that follows, with the exception of
  newline.  If a \newline pair appears, and the backslash itself is not
  quoted, the \newline is treated as a line continuation (that is, it is
  removed from the input stream and effectively ignored).

  Single Quotes

  Enclosing characters in single quotes ['] preserves the literal value of
  each character within the quotes.  A single quote may not occur between
  single quotes, even when preceded by a backslash.

  Double Quotes

  Enclosing characters in double quotes ["] preserves the literal value of all
  characters within the quotes, with the exception of [\].  The backslash
  retains its special meaning only when followed by one of the following
  characters: ["], [\], or newline.  Within double quotes, backslashes that
  are followed by one of these characters are removed.  Backslashes preceding
  characters without a special meaning are left unmodified.  A double quote
  may be quoted within double quotes by preceding it with a backslash.
*/

#include <ctype.h>
#include <stdio.h>

static void
eatws_(struct aug_words*, int);

static void
bare_(struct aug_words*, int);

static void
squot_(struct aug_words*, int);

static void
dquot_(struct aug_words*, int);

static void
comment_(struct aug_words*, int);

static void
eatws_(struct aug_words* st, int ch)
{
    if (st->flags_ & AUG_WORDESCAPE) {
        /* Handle escape. */
        st->flags_ &= ~AUG_WORDESCAPE;
        switch (ch) {
        case '\n': /* New line. */
            /* Line continuation. */
            break;
        default:   /* Else. */
            /* Begin bare-word with literal. */
            st->out_(st->arg_, ch);
            st->fn_ = bare_;
            break;
        }
    } else {
        switch (ch) {
        case '\n':
        case ';':  /* New line. */
            st->out_(st->arg_, AUG_TOKPHRASE);
            break;
        case '\t':
        case '\v':
        case '\f':
        case '\r':
        case ' ':  /* Line space. */
            /* Swallow. */
            break;
        case '"':  /* Double quote. */
            /* Begin double quote. */
            st->fn_ = dquot_;
            break;
        case '#':  /* Comment. */
            st->fn_ = comment_;
            break;
        case '\'': /* Single quote. */
            /* Begin single quote. */
            st->fn_ = squot_;
            break;
        case '\\': /* Escape. */
            /* Begin escape. */
            st->flags_ |= AUG_WORDESCAPE;
            break;
        default:   /* Else. */
            /* Begin bare-word. */
            st->out_(st->arg_, ch);
            st->fn_ = bare_;
            break;
        }
    }
}

static void
bare_(struct aug_words* st, int ch)
{
    if (st->flags_ & AUG_WORDESCAPE) {
        /* Handle escape. */
        st->flags_ &= ~AUG_WORDESCAPE;
        switch (ch) {
        case '\n': /* New line. */
            /* Line continuation. */
            break;
        default:   /* Else. */
            /* Literal. */
            st->out_(st->arg_, ch);
            break;
        }
    } else {
        switch (ch) {
        case '\n':
        case ';':  /* New line. */
            /* End of bare-word. */
            st->out_(st->arg_, AUG_TOKWORD);
            st->fn_ = eatws_;
            if ((st->flags_ & AUG_WORDPAIRS))
                st->flags_ |= AUG_WORDLABEL;
            st->fn_(st, ch);
            break;
        case '\t':
        case '\v':
        case '\f':
        case '\r':
        case ' ':  /* Line space. */
            /* End of bare-word. */
            st->out_(st->arg_, AUG_TOKWORD);
            st->fn_ = eatws_;
            if ((st->flags_ & AUG_WORDPAIRS))
                st->flags_ |= AUG_WORDLABEL;
            break;
        case '"':  /* Double quote. */
            /* Begin double quote. */
            st->fn_ = dquot_;
            break;
        case '\'': /* Single quote. */
            /* Begin single quote. */
            st->fn_ = squot_;
            break;
        case '\\': /* Escape. */
            /* Begin escape. */
            st->flags_ |= AUG_WORDESCAPE;
            break;
        default:   /* Else. */
            if ((st->flags_ & AUG_WORDLABEL) && '=' == ch) {
                /* Label and reset delim. */
                st->out_(st->arg_, AUG_TOKLABEL);
                st->fn_ = eatws_;
                st->flags_ &= ~AUG_WORDLABEL;
            } else {
                /* Append to bare-word. */
                st->out_(st->arg_, ch);
            }
            break;
        }
    }
}

static void
dquot_(struct aug_words* st, int ch)
{
    if (st->flags_ & AUG_WORDESCAPE) {
        /* Handle escape. */
        st->flags_ &= ~AUG_WORDESCAPE;
        switch (ch) {
        case '\n': /* New line. */
            /* Line continuation. */
            break;
        case '"':  /* Double quote. */
        case '\\': /* Escape. */
            /* Literal double quote or backslash. */
            st->out_(st->arg_, ch);
            break;
        default:   /* Else. */
            /* Ignore escape. */
            st->out_(st->arg_, '\\');
            st->out_(st->arg_, ch);
            break;
        }
    } else {
        if ('"' == ch) {
            /* Continue as bare-word. */
            st->fn_ = bare_;
        } else if ('\\' == ch) {
            /* Begin escape. */
            st->flags_ |= AUG_WORDESCAPE;
        } else {
            /* Append to quoted. */
            st->out_(st->arg_, ch);
        }
    }
}

static void
squot_(struct aug_words* st, int ch)
{
    /* No escapes in single quotes. */

    if ('\'' == ch) {
        /* Continue as bare-word. */
        st->fn_ = bare_;
    } else {
        /* Append to quoted. */
        st->out_(st->arg_, ch);
    }
}

static void
comment_(struct aug_words* st, int ch)
{
    /* Ignore until newline. */

    if ('\n' == ch) {
        st->fn_ = eatws_;
        st->fn_(st, ch);
    }
}

AUGUTIL_API void
aug_initshellwords(struct aug_words* st, aug_bool pairs,
                   void (*out)(void*, int), void* arg)
{
    st->out_ = out;
    st->arg_ = arg;
    st->fn_ = eatws_;
    st->flags_ = pairs ? (AUG_WORDLABEL | AUG_WORDPAIRS) : 0;
}

AUGUTIL_API void
aug_putshellwords(struct aug_words* st, int ch)
{
    switch (ch) {
    case EOF:
        st->fn_(st, '\n');
        break;
    case '\r':
        /* Ignore. */
        break;
    default:
        st->fn_(st, ch);
        break;
    }
}
