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
#ifndef AUGSUBPP_UTILITY_HPP
#define AUGSUBPP_UTILITY_HPP

#include "augsubpp/types.hpp"

namespace aug {

    class inner_base;
    class node_base;

    AUGSUBPP_API unsigned
    exdepth(const node_base& node);

    AUGSUBPP_API match
    foldmatch(match l, match r);

    AUGSUBPP_API nodetype
    tonodetype(const std::string& value);

    AUGSUBPP_API void
    erasenode(const std::string& head, path_iterator pit,
              path_iterator pend, inner_base& node);

    AUGSUBPP_API void
    erasenode(const std::string& s, inner_base& node);

    AUGSUBPP_API void
    insertnode(const std::string& head, path_iterator pit,
               path_iterator pend, inner_base& node);

    AUGSUBPP_API void
    insertnode(const std::string& s, inner_base& node);
}

#endif // AUGSUBPP_UTILITY_HPP
