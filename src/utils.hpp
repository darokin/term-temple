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

// ============================================================================================================
template<typename Type>
struct v2d {
    Type x{0};
    Type y{0};
    friend v2d operator+(v2d l, const v2d& r) { l.x += r.x; l.y += r.y; return l; }
    friend v2d operator-(v2d l, const v2d& r) { l.x -= r.x; l.y -= r.y; return l; }
    template<typename FromType> v2d<Type>& operator+=(const v2d<FromType>& r) { x += r.x; y += r.y; return *this; }
    template<typename FromType> v2d<Type>& operator-=(const v2d<FromType>& r) { x -= r.x; y -= r.y; return *this; }
    template<typename FromType> v2d<Type>& operator*=(const v2d<FromType>& r) { x *= r.x; y *= r.y; return *this; }
    template<typename FromType> v2d<Type>& operator/=(const v2d<FromType>& r) { x /= r.x; y /= r.y; return *this; }
    template<typename FromType> v2d<Type>& operator+=(const FromType& v) { x += v; y += v; return *this; }
    template<typename FromType> v2d<Type>& operator-=(const FromType& v) { x -= v; y -= v; return *this; }
    template<typename FromType> v2d<Type>& operator*=(const FromType& v) { x *= v; y *= v; return *this; }
    template<typename FromType> v2d<Type>& operator/=(const FromType& v) { x /= v; y /= v; return *this; }
    // == For conversion between standard types 
    //template<typename FromType> v2d<Type>& operator=(const v2d<FromType>& r) { x = r.x; y = r.y; return *this; }
    //template<typename FromType> v2d(const v2d<FromType> &r) { x = r.x; y = r.y; }
};

template<typename Type> inline bool operator==(const v2d<Type>& l, const v2d<Type>& r) { return (l.x == r.x && l.y == r.y); }
template<typename Type> inline bool operator!=(const v2d<Type>& l, const v2d<Type>& r) { return !(l == r); }
template<typename Type> inline bool operator< (const v2d<Type>& l, const v2d<Type>& r) { return (l.x < r.x && l.y < r.y); }
template<typename Type> inline bool operator> (const v2d<Type>& l, const v2d<Type>& r) { return r < l; }
template<typename Type> inline bool operator<=(const v2d<Type>& l, const v2d<Type>& r) { return !(l > r); }
template<typename Type> inline bool operator>=(const v2d<Type>& l, const v2d<Type>& r) { return !(l < r); }

using i2d = v2d<int>;
using u2d = v2d<uint16_t>; // not used yet may replace most
// ============================================================================================================

namespace Utils {

    // == DATE and TIME =======================================================
    inline std::string getCurrentDateTime(const std::string& s){
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
    inline void Log(const std::string& logMsg){
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

    // == WSTRING FORMAT =====================================
    // == Grosse triche après fail avec variadic arg (cf. + haut) j'ai demandé 
    // == honteusement à ChatGPT (3.5) de faire un format_wstring() (ci-après)

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

    // == Format WSTRING 
    template<typename... Args>
    std::wstring format_wstring(const std::wstring& fmt, Args... args) {
        std::wstringstream ss;
        size_t pos = 0;
        format_arg(ss, fmt, pos, args...);
        ss << fmt.substr(pos); // Ajouter la partie restante de la chaîne de format
        return ss.str();
    }

    // == EASING =====================================
    // https://github.com/nicolausYes/easing-functions/
    // Test : https://easings.net/
    #ifndef PI
    #define PI 3.1415926545
    #endif

    inline double easeInSine(double t)      { return sin( 1.5707963 * t ); }
    inline double easeOutSine(double t)     { return 1 + sin( 1.5707963 * (--t) ); }
    inline double easeInOutSine(double t)   { return 0.5 * (1 + sin( PI * (t - 0.5) ) ); }
    inline double easeInQuad(double t)      { return t * t; }
    inline double easeOutQuad(double t)     { return t * (2 - t); }
    inline double easeInOutQuad(double t)   { return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1; }
    inline double easeInCubic(double t)     { return t * t * t; }
    //inline double easeOutCubic(double t)    { return 1 + (--t) * t * t; }
    inline double easeOutCubic(double t)    { --t; return 1 + t * t * t; }
    //inline double easeInOutCubic(double t)  { return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t); }
    inline double easeInQuart(double t)     { t *= t; return t * t; }
    //inline double easeOutQuart(double t)    { t = (--t) * t; return 1 - t * t; }
    inline double easeOutQuart(double t)    { t--; t *= t; return 1 - t * t; }
    inline double easeInQuint(double t)     { double t2 = t * t; return t * t2 * t2; }
    //inline double easeOutQuint(double t)    { double t2 = (--t) * t; return 1 + t * t2 * t2; }
    inline double easeInExpo(double t)      { return (pow( 2, 8 * t ) - 1) / 255; }
    inline double easeOutExpo(double t)     { return 1 - pow( 2, -8 * t ); }
    inline double easeInCirc(double t)      { return 1 - sqrt( 1 - t ); }
    inline double easeOutCirc(double t)     { return sqrt( t ); }
    inline double easeInBack(double t)      { return t * t * (2.70158 * t - 1.70158); }
    //inline double easeOutBack(double t)     { return 1 + (--t) * t * (2.70158 * t + 1.70158); }
    inline double easeOutBack(double t)     { --t; return 1 + t * t * (2.70158 * t + 1.70158); }
    inline double easeInElastic(double t)   { double t2 = t * t; return t2 * t2 * sin( t * PI * 4.5 ); }
    inline double easeOutElastic(double t)  { double t2 = (t - 1) * (t - 1); return 1 - t2 * t2 * cos( t * PI * 4.5 ); }
    inline double easeInBounce(double t )   { return pow( 2, 6 * (t - 1) ) * abs( sin( t * PI * 3.5 ) ); }
    inline double easeOutBounce(double t )  { return 1 - pow( 2, -6 * t ) * abs( cos( t * PI * 3.5 ) ); }
    inline double easeInOutQuart(double t ) {
        if( t < 0.5 ) {
            t *= t;
            return 8 * t * t;
        } else {
            //t = (--t) * t;
            --t; t *= t;
            return 1 - 8 * t * t;
        }
    }
    inline double easeInOutQuint( double t ) {
        double t2;
        if( t < 0.5 ) {
            t2 = t * t;
            return 16 * t * t2 * t2;
        } else {
            //t2 = (--t) * t;
            --t; t2 = t * t;
            return 1 + 16 * t * t2 * t2;
        }
    }
    inline double easeInOutExpo( double t ) {
        if( t < 0.5 ) {
            return (pow( 2, 16 * t ) - 1) / 510;
        } else {
            return 1 - 0.5 * pow( 2, -16 * (t - 0.5) );
        }
    }
    inline double easeInOutCirc( double t ) {
        if( t < 0.5 ) {
            return (1 - sqrt( 1 - 2 * t )) * 0.5;
        } else {
            return (1 + sqrt( 2 * t - 1 )) * 0.5;
        }
    }
    inline double easeInOutBack( double t ) {
        if( t < 0.5 ) {
            return t * t * (7 * t - 2.5) * 2;
        } else {
            //return 1 + (--t) * t * 2 * (7 * t + 2.5);
            --t; return 1 + t * t * 2 * (7 * t + 2.5);
        }
    }
    inline double easeInOutElastic( double t ) {
        double t2;
        if( t < 0.45 ) {
            t2 = t * t;
            return 8 * t2 * t2 * sin( t * PI * 9 );
        } else if( t < 0.55 ) {
            return 0.5 + 0.75 * sin( t * PI * 4 );
        } else {
            t2 = (t - 1) * (t - 1);
            return 1 - 8 * t2 * t2 * sin( t * PI * 9 );
        }
    }
    inline double easeInOutBounce( double t ) {
        if( t < 0.5 ) {
            return 8 * pow( 2, 8 * (t - 1) ) * abs( sin( t * PI * 7 ) );
        } else {
            return 1 - 8 * pow( 2, -8 * t ) * abs( sin( t * PI * 7 ) );
        }
    }
}

#endif