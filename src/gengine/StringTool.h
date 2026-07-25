#ifndef HEADER_STRINGTOOL_H
#define HEADER_STRINGTOOL_H

#include <string>
#include <vector>

/**
 * String utils.
 */
class StringTool {
    public:
        using t_args = std::vector<std::string>;
    public:
        static long readInt(const char *text, bool *ok);
        static std::string toString(long value);

        static bool startsWith(const std::string &str,
                const std::string &prefix);
        static void replace(std::string &buffer,
                const std::string &pattern, const std::string &newstring);
        static t_args split(const std::string &str, char separator);
        static int utf8Length(const std::string &str);
};

#endif

