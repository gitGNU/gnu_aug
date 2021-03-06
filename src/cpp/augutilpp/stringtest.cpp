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
#include "augctx/defs.h"

AUG_RCSID("$Id$");

#include "augutilpp/string.hpp"

#include <iostream>

using namespace aug;
using namespace std;

namespace {

    typedef runtime_error error;

    const char DECODED[] = "\"hello, world!\"";
    const char ENCODED[] = "%22hello%2C+world!%22";
    const char LTRIMMED[] = "hello, world!  \r\n";
    const char RTRIMMED[] = "\t  hello, world!";
    const char TRIMMED[] = "hello, world!";
    const char UNTRIMMED[] = "\t  hello, world!  \r\n";

    void
    encodeurltest()
    {
        if (encodeurl(DECODED, DECODED + strlen(DECODED)) != ENCODED)
            throw error("encodeurl() failed");
    }

    void
    decodeurltest()
    {
        if (decodeurl(ENCODED, ENCODED + strlen(ENCODED)) != DECODED)
            throw error("decodeurl() failed");
    }

    void
    ltrimtest()
    {
        string s(UNTRIMMED);
        if (ltrim(s) != LTRIMMED)
            throw error("ltrim() failed");

        if (ltrimcopy(UNTRIMMED) != LTRIMMED)
            throw error("ltrimcopy() failed");
    }

    void
    rtrimtest()
    {
        string s(UNTRIMMED);
        if (rtrim(s) != RTRIMMED)
            throw error("rtrim() failed");

        if (rtrimcopy(UNTRIMMED) != RTRIMMED)
            throw error("rtrimcopy() failed");
    }

    void
    trimtest()
    {
        string s(UNTRIMMED);
        if (trim(s) != TRIMMED)
            throw error("trim() failed");

        if (trimcopy(UNTRIMMED) != TRIMMED)
            throw error("trimcopy() failed");
    }

    void
    copyiftest()
    {
        string in("one:two"), out;
        string::iterator it(in.begin());

        it = copyif(it, in.end(), back_inserter(out),
                    bind2nd(not_equal_to<char>(), ':'));
        if (out != "one" || it == in.end() || *it != ':')
            throw error("copyif() failed");

        it = copyif(++it, in.end(), back_inserter(out),
                    bind2nd(not_equal_to<char>(), ':'));
        if (out != "onetwo" || it != in.end())
            throw error("copyif() failed");
    }

    void
    tokenisetest()
    {
        string in("one:two:three"), tok;
        vector<string> toks(tokenise(in.begin(), in.end(), tok, ':'));
        if (2 != toks.size() || toks[0] != "one" || toks[1] != "two"
            || tok != "three")
            throw error("tokenise() failed");
    }

    void
    splitntest()
    {
        string in("one:two");
        vector<string> toks(splitn(in.begin(), in.end(), ':'));
        if (2 != toks.size() || toks[0] != "one" || toks[1] != "two")
            throw error("splitn() failed");
    }

    enum scheme {
        NONE,
        FTP,
        HTTP
    };

    pair<scheme, string>
    splituri(const string& uri)
    {
        string x, y;
        scheme s(NONE);
        if (split2(uri.begin(), uri.end(), x, y, ':')) {
            if (x == "ftp")
                s = FTP;
            else if (x == "http")
                s = HTTP;
            else
                throw error("invalid scheme");
        } else
            y = uri;
        return make_pair(s, y);
    }

    void
    split2test()
    {
        if (NONE != splituri("/foo").first
            || FTP != splituri("ftp://foo").first
            || HTTP != splituri("http://foo").first)
        throw error("split2() failed");
    }
}

int
main(int argc, char* argv[])
{
    try {
        encodeurltest();
        decodeurltest();
        ltrimtest();
        rtrimtest();
        trimtest();
        copyiftest();
        tokenisetest();
        splitntest();
        split2test();
        return 0;
    } catch (const exception& e) {
        cerr << "error: " << e.what() << endl;
    }
    return 1;
}
