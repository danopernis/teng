/*
 * Teng -- a general purpose templating engine.
 * Copyright (C) 2004  Seznam.cz, a.s.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Seznam.cz, a.s.
 * Naskove 1, Praha 5, 15000, Czech Republic
 * http://www.seznam.cz, mailto:teng@firma.seznam.cz
 *
 *
 * $Id: tengdictionary.h,v 1.1 2004-07-28 11:36:55 solamyl Exp $
 *
 * DESCRIPTION
 * Teng dictionary.
 *
 * AUTHORS
 * Vaclav Blazek <blazek@firma.seznam.cz>
 *
 * HISTORY
 * 2003-09-18  (vasek)
 *             Created.
 */


#ifndef _TENGDICTIONARY_H
#define _TENGDICTIONARY_H

#include <string>
#include <vector>
#include <map>
#include <stdio.h>

#include "tengerror.h"
#include "tengsourcelist.h"

using namespace std;

namespace Teng {

/**
 * @short Dictionary -- mapping of string to string value.
 *
 * Used for language and parametric dictionaries.
 */
class Dictionary_t {
public:
    /**
     * @short Creates new dictionary object.
     *
     * @param root path of root for locating files
     */
    Dictionary_t(const string &root = string())
        : root(root), level(0), sources(), err()
    {}

    /**
     * @short Destroy dictionary object.
     */
    virtual ~Dictionary_t();

    /**
     * @short Parses dicionary from given file.
     *
     * @param filename name of file to parse
     * @return 0 OK !0 error
     */
    int parse(const string &filename);

    /**
     * @short Adds new entry into dictionary. Doesn't replace
     *        existing entry.
     *
     * @param name name of entry
     * @param value value of entry
     * @return 0 OK !0 error
     */
    virtual int add(const string &name, const string &value);

    /**
     * @short Searches for key in dictionary.
     *
     * @param key the key
     * @return found value or 0 when key not found
     */
    virtual const string* lookup(const string &key) const;
    
    /**
     * @short Check source files for change.
     *
     * @return 0 OK !0 changed
     */
    inline int check() const {
        return sources.check();
    }

    /**
     * @short Dumps dictionary into string. For debugging purposes.
     *
     * @param out output string
     * @return 0 OK !0 error
     */
    virtual int dump(string &out) const;

    /**
     * @short Return source list.
     *
     * @return source list
     */
    inline const SourceList_t& getSources() const {
        return sources;
    }

    /**
     * @short Get error logger.
     *
     * @return error logger
     */
    inline const Error_t& getErrors() const {
        return err;
    }

protected:
    /**
     * @short Parses dictionary from given string. Worker function.
     *
     * @param data
     * @param pos position in current file
     * @return 0 OK !0 error
     */
    virtual int _parseString(const string &data, Error_t::Position_t &pos);

    /**
     * @short Parses value line.
     *
     * @param line parsed line
     * @param value returend value
     * @param pos position in current file
     * @return 0 OK !0 error
     */
    int parseValueLine(const string &line, string &value,
                          Error_t::Position_t &pos);
    
    /**
     * @short Parses line beginning with identifier.
     *
     * @param line parsed line
     * @param name name of identifier
     * @param value value of ifentifier
     * @param pos position in current file
     * @return 0 OK !0 error
     */
    virtual int parseIdentLine(const string &line, string &name, string &value,
                          Error_t::Position_t &pos);
    
    /**
     * @short Parses dicionary from given file. Worker function.
     *
     * @param filename name of file to parse
     * @param pos position in current file
     * @return 0 OK !0 error
     */
    int _parse(const string &filename, Error_t::Position_t &pos);

    /**
     * @short Extracts line from string.
     *
     * Removes terminating <LF> and (optional) preceding <CR>.
     *
     * @param str input string
     * @param line found line
     * @param begin start of line
     * @return position of terminating <LF> in input string
     */
    static string::size_type getLine(const string &str, string &line,
                                     string::size_type begin);
    
    /**
     * @short Maximal number of dictionary file inclusion.
     */
    static const unsigned int MAX_RECURSION_LEVEL = 10;

    /**
     * @short Root directory for file lookup.
     */
    string root;

    /**
     * @short Current level of recursion. Valid only when parsing.
     */
    unsigned int level;

    /**
     * @short Sources of this dictionary.
     */
    SourceList_t sources;

    /**
     * @short Error logger.
     */
    Error_t err;

private:
    /**
     * @short Copy constructor intentionally private -- copying
     *        disabled.
     */
    Dictionary_t(const Dictionary_t&);

    /**
     * @short Assignment operator intentionally private -- assignment
     *        disabled.
     */
    Dictionary_t operator=(const Dictionary_t&);

    /**
     * @short Parses dicionary from given file. Worker function.
     *
     * @param file file open for reading
     * @param pos position in current file
     * @return 0 OK !0 error
     */
    int _parse(FILE *file, Error_t::Position_t &pos);

    /**
     * @short Parses and processes processing directive.
     *
     * @param directive whole directive string
     * @param pos position in current file
     * @return 0 OK !0 error
     */
    int processDirective(const string &directive,
                         Error_t::Position_t &pos);

    /**
     * @short The dictionary itself.
     */
    map<string, string> dict;
};

} // namespace Teng

#endif // _TENGDICTIONARY_H