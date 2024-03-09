#ifndef TERMTEMPLE_UTILS_H
#define TERMTEMPLE_UTILS_H

#include <time.h>
#include <sys/time.h>   // for gettimeofday()
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <random>      
#include <locale>       // for string to wstring 
#include <codecvt>      // for string to wstring
#include <cstdarg>      // for va_list
#include "utils.hpp"

#define LOGFILE_PATH    "logs/log.txt"

/*
struct v2d {
    int x{0};
    int y{0};
    friend v2d operator+(v2d l, const v2d& r) { l.x += r.x; l.y += r.y; return l; };
    friend v2d operator-(v2d l, const v2d& r) { l.x -= r.x; l.y -= r.y; return l; };
};

inline bool operator==(const v2d& l, const v2d& r) { return (l.x == r.x && l.y == r.y); };
inline bool operator!=(const v2d& l, const v2d& r) { return !(l == r); };
inline bool operator< (const v2d& l, const v2d& r) { return (l.x < r.x && l.y < r.y); };
inline bool operator> (const v2d& l, const v2d& r) { return r < l; };
inline bool operator<=(const v2d& l, const v2d& r) { return !(l > r); };
inline bool operator>=(const v2d& l, const v2d& r) { return !(l < r); };
*/


template<typename Type>
struct v2d {
    Type x{0};
    Type y{0};
    friend v2d operator+(v2d l, const v2d& r) { l.x += r.x; l.y += r.y; return l; };
    friend v2d operator-(v2d l, const v2d& r) { l.x -= r.x; l.y -= r.y; return l; };
};

template<typename Type> inline bool operator==(const v2d<Type>& l, const v2d<Type>& r) { return (l.x == r.x && l.y == r.y); };
template<typename Type> inline bool operator!=(const v2d<Type>& l, const v2d<Type>& r) { return !(l == r); };
template<typename Type> inline bool operator< (const v2d<Type>& l, const v2d<Type>& r) { return (l.x < r.x && l.y < r.y); };
template<typename Type> inline bool operator> (const v2d<Type>& l, const v2d<Type>& r) { return r < l; };
template<typename Type> inline bool operator<=(const v2d<Type>& l, const v2d<Type>& r) { return !(l > r); };
template<typename Type> inline bool operator>=(const v2d<Type>& l, const v2d<Type>& r) { return !(l < r); };

using i2d = v2d<int>;

namespace Utils {

    // == DATE and TIME =======================================================
    inline std::string getCurrentDateTime(std::string s){
        time_t now = time(0);
        struct tm timeinfo;
        char buf[80];
        timeinfo = *localtime(&now);
        if (s == "now")
            strftime(buf, sizeof(buf), "%Y-%m-%d %X", &timeinfo);
        else if (s == "date")
            strftime(buf, sizeof(buf), "%Y-%m-%d", &timeinfo);
        return std::string(buf);
    };

    inline long long timeInMilliseconds() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
    }

    // == RANDOM =======================================================
    inline uint16_t rand10() {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist10(0,9);
        return dist10(rng);
    }

    // == LOG =======================================================
    inline void Log(std::string logMsg){
        std::string now = getCurrentDateTime("now");
        std::ofstream ofs(LOGFILE_PATH, std::ios_base::out | std::ios_base::app );
        ofs << now << '\t' << logMsg << std::endl;
        ofs.close();
    };

    // == TRIM =======================================================
    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }
    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }
    inline void trim(std::string &s) {
        rtrim(s);
        ltrim(s);
    }
    inline std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }
    inline std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }
    inline std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }

    // == MAXIMUM and MINIMUM ==========================================
    inline int max(int _val, int _max) {
        if (_val > _max)
            return _max;
        return _val;
    }

    inline int min(int _val, int _min) {
        if (_val < _min)
            return _min;
        return _val;
    }

    // == STRINGS and WIDE-STRINGS =====================================
    inline std::wstring str2wstr(const std::string& str) {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converter;
        return converter.from_bytes(str);
    }

    inline std::string wstr2str(const std::wstring& wstr)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converter;
        return converter.to_bytes(wstr);
    }

/*     inline std::wstring format_wstring(const std::wstring& format, ...) {
        va_list args;
        va_start(args, format);

        // Determine the size of the buffer needed
        size_t size = std::vswprintf(nullptr, 0, format.c_str(), args);
        if (size == static_cast<size_t>(-1)) {
            // Error handling
            va_end(args);
            return L"";
        }

        // Allocate buffer and format the string
        std::wstring result(size + 1, L'\0');
        std::vswprintf(&result[0], size + 1, format.c_str(), args);

        va_end(args);
        return result;
    } */

    // Fonction récursive pour formater une chaîne de caractères
    template<typename T>
    void format_arg(std::wstringstream& ss, const std::wstring& fmt, size_t& pos, const T& value) {
        size_t newpos = fmt.find(L"{}", pos);
        if (newpos != std::wstring::npos) {
            ss << fmt.substr(pos, newpos - pos) << value;
            pos = newpos + 2; // Passer au-delà de {}
        }
    }

    template<typename T, typename... Args>
    void format_arg(std::wstringstream& ss, const std::wstring& fmt, size_t& pos, const T& value, Args... args) {
        size_t newpos = fmt.find(L"{}", pos);
        if (newpos != std::wstring::npos) {
            ss << fmt.substr(pos, newpos - pos) << value;
            pos = newpos + 2; // Passer au-delà de {}
            format_arg(ss, fmt, pos, args...); // Appel récursif pour traiter le reste des arguments
        }
    }

    template<typename... Args>
    std::wstring format_wstring(const std::wstring& fmt, Args... args) {
        std::wstringstream ss;
        size_t pos = 0;
        format_arg(ss, fmt, pos, args...);
        ss << fmt.substr(pos); // Ajouter la partie restante de la chaîne de format
        return ss.str();
    }

};

#endif