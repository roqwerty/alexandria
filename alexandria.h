#ifndef ALEXANDRIA_LIBRARY_INCLUDED
#define ALEXANDRIA_LIBRARY_INCLUDED

//#define USE_ALEXANDRIA_NAMESPACE // If uncommented or otherwise defined, will make ALL of the following part of the Alexandria:: namespace

/*
The purpose of the Alexandria library is to collect and standardize functions and utilities that I find myself using somewhat often
Basically it's a mini-Boost, so that's fun :)

--- Notes ---
Remember to #include <stdlib.h> and srand(time(NULL)) if doing anything with randomization

--- Currently Included ---

Structs:
    Vector3: A basic 3D floating-point vector
        Supported operators: +, -, *
        Supported functions: magnitude(Vector3), normalize(Vector3), cross(Vector3, Vector3), dot(Vector3, Vector3), angle(Vector3, Vector3)
    Color: A small RGB 256-bit color
    ColorAlpha: A small RGBA 256-bit color
        Supported functions: ColorAlpha::fromFloat(float), ColorAlpha::toFloat()
    ColorHSV: A small HSV 256-bit color

Functions:
    std::string load_file(const std::string& filepath): Loads all data within a file and returns as a C++ string
    Color heatmap(float val): Changes a normalized val in range [0.0, 1.0] into a 7-color heatmap color
    Color randColor(const Color& c1, const Color& c2): Returns a random color linearly interpolated between the two given colors
    Color linearColor(float percent, const Color& c1, const Color& c2): Returns the color percent of the way through the linear range between the colors. Percent in range [0.0, 1.0]
    Color hsv_to_rgb(ColorHSV hsv): Quickly (not accurately) converts an HSV color to a RGB color
    ColorHSV rgb_to_hsv(Color rgb): Quickly (not accurately) converts an RGB color to a HSV color
    std::string base64_encode(const std::string &in): Converts an input string into base64 data using standard encoding
    std::string base64_decode(const std::string &in): Converts an input string from base64 data using standard encoding
    std::string trimSpaces(const std::string& source): Trims leading / trailing spaces (and only spaces)
    int get_digit_at_index(int source, int index, int base = 10): Returns the number at the given index of the base number (from right)
    int get_number_length(int source, int base = 10): Returns the length of the given number, in base-10 digits by default

Classes:
    FastBoolGenerator: A really, really fast boolean value generator with pretty random distribution. Use () operator for use.

--- Future Work ---
General-purpose raycasting structure and library, implemented to be as fast as possible with trig lookup tables and the like
Big integer work, even if simple addition, subtraction, multiplication, etc., such as #include <boost/multiprecision/cpp_int.hpp>?
Data type built specifically for tweening with various different smoothing functions and an update deltatime function
*/

////////// SETUP //////////

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>

#ifdef USE_ALEXANDRIA_NAMESPACE
namespace Alexandria {
#endif


////////// STRUCTS //////////

struct Vector3 {
    // A basic 3D floating-point vector
    float x;
    float y;
    float z;
};

// Vector 3 math source: audeo C++ library
Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs) {
    return Vector3{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs) {
    return Vector3{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

Vector3 operator*(Vector3 const& lhs, float scalar) {
    return {lhs.x * scalar, lhs.y * scalar, lhs.z * scalar};
}

float magnitude(Vector3 v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 normalize(Vector3 v) { return v * (1.0f / magnitude(v)); }

Vector3 cross(Vector3 const& lhs, Vector3 const& rhs) {
    return {lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x};
}

float dot(Vector3 const& lhs, Vector3 const& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float angle(Vector3 const& lhs, Vector3 const& rhs) {
    // The cosine of the angle between two vectors is equal to the dot product
    // of the vectors divided by the product of the magnitude of the vectors
    //
    // We get rid of the division by normalizing the vectors
    Vector3 a = normalize(lhs);
    Vector3 b = normalize(rhs);
    // The cosine is now equal to the dot product of a and b
    float cos_a = dot(a, b);
    // Convert this cosine to an angle in degrees and we are done
    return static_cast<float>(std::acos(cos_a) * 180.0f / M_PI);
}

struct Color {
    // A small RGB 256-bit color
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct ColorAlpha {
    // A small RGBA 256-bit color
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    void fromFloat(float f) {
        // Makes this color from a float
        memcpy(this, &f, 4);
    }

    float toFloat() {
        // Returns this but as a float
        float f = 0.0;
        memcpy(&f, this, 4);
        return f;
    }
};

struct ColorHSV {
    // A small HSV 256-bit color
    uint8_t h;
    uint8_t s;
    uint8_t v;
};

////////// FUNCTIONS //////////

std::string load_file(const std::string& filepath) {
    // Loads all data within a file and returns as a C++ string
    std::ifstream f(filepath);
    if (f.good()) {
        std::string str;

        f.seekg(0, std::ios::end);   
        str.reserve(f.tellg());
        f.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        return str;
    }
    else {
        return ""; // No data
    }
}

Color heatmap(float val) {
    // Changes a normalized val in range [0.0, 1.0] into a 7-color heatmap color
    // Source: https://andrewnoske.com/wiki/Code_-_heatmaps_and_color_gradients
    Color colors[] = { {0,0,0}, {0,0,255}, {0,255,255}, {0,255,0}, {255,255,0}, {255,0,0}, {255,255,255} };
    int numColors = 7;
    int i1, i2; // Our final color will end between these two color indexes
    float frac = 0.0; // The remainder fraction between the two indexes

    if (val < 0.0) {
        i1 = 0;
        i2 = 0;
    } else if (val >= 1.0) {
        i1 = numColors-1;
        i2 = numColors-1;
    } else {
        val *= numColors - 1;
        i1 = floor(val);
        i2 = i1 + 1;
        frac = val - (float)i1;
    }

    Color retc = {
        (uint8_t)((colors[i2].r - colors[i1].r)*frac + (float)colors[i1].r),
        (uint8_t)((colors[i2].g - colors[i1].g)*frac + (float)colors[i1].g),
        (uint8_t)((colors[i2].b - colors[i1].b)*frac + (float)colors[i1].b)
    };
    return retc;
}

inline Color randColor(const Color& c1, const Color& c2)
{
    // Returns a random color linearly interpolated between the two given colors
    float t = (float)rand() / RAND_MAX; // Between 0-1
    return (Color){(uint8_t)(c1.r + (c2.r - c1.r) * t), (uint8_t)(c1.g + (c2.g - c1.g) * t), (uint8_t)(c1.b + (c2.b - c1.b) * t)}; // Compound literal
}

inline Color linearColor(float percent, const Color& c1, const Color& c2)
{
    // Returns the color percent of the way through the linear range between the colors. Percent in range [0.0, 1.0]
    return (Color){(uint8_t)(c1.r + (c2.r - c1.r) * percent), (uint8_t)(c1.g + (c2.g - c1.g) * percent), (uint8_t)(c1.b + (c2.b - c1.b) * percent)};
}

// RGB <-> HSV functions source: https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
// NOTE That these are not ultra-accurate, but they are fast
Color hsv_to_rgb(ColorHSV hsv)
{
    // Quickly (not accurately) converts an HSV color to a RGB color
    Color rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

ColorHSV rgb_to_hsv(Color rgb)
{
    // Quickly (not accurately) converts an RGB color to a HSV color
    ColorHSV hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

// Base64 functions source: https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
static std::string base64_encode(const std::string &in) {
    std::string out;

    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}

static std::string base64_decode(const std::string &in) {
    std::string out;

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val=0, valb=-8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}

static std::string trimSpaces(const std::string& source) {
    // Trims leading / trailing spaces (and only spaces)
    return source.substr(source.find_first_not_of(" "), source.find_last_not_of(" ") - source.find_first_not_of(" ") + 1);
}

int get_digit_at_index(int source, int index, int base = 10) {
    // Returns the number at the given index of the source number (from right)
    int i = 0; // Iterable
    while (i < index)
    {
        source /= base;
        i++;
    }
    return (source % base); // Gives back only the rightmost digit
}

int get_number_length(int source, int base = 10) {
    // Returns the length of the given number, in base-10 digits
    int digits = 0;
    while (source)
    {
        source /= base;
        digits++;
    }
    return digits;
}

////////// CLASSES //////////

class FastBoolGenerator {
    // A really, really fast boolean value generator with pretty random distribution
    // Usage: FastBoolGenerator fbg; bool random = fbg();
public:
    FastBoolGenerator() { // Constructor
        std::random_device rd;     // Get a random seed from the OS entropy device, or whatever
        eng = new std::mt19937_64(rd());
    }
    ~FastBoolGenerator() { // Destructor
        delete eng;
    }
    bool operator() () {
        if (count == 64) {
            count = 0;
            number = distr(*eng); // New 64-bit integer
        }
        count++;
        bool value = number & mask;
        number = number >> 1;
        return value;
    }
private:
    std::mt19937_64* eng; // Use the 64-bit Mersenne Twister 19937 generator
    std::uniform_int_distribution<uint64_t> distr;
    uint64_t number = 0; // The number storing the bits
    uint8_t count = 64; // 0-63, determines if new random 64-bit number is needed
    uint64_t mask = 1; // Just the last bit
};

////////// CLEANUP //////////

#ifdef USE_ALEXANDRIA_NAMESPACE
}
#endif

#endif