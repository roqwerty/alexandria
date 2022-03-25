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
    BMPHeader: Header for all Bitmap image data, as well as default values, constructor provided

Functions:
    void save_bmp(const std::string& filepath, const std::vector<std::vector<ColorAlpha>> pixels, bool origin_at_top_left = true): Saves a ColorAlpha vector as an image
    std::vector<std::vector<ColorAlpha>> make_image_array(int width, int height): Makes and returns a blank (white) ColorAlpha array of the given dimensions
    std::string load_file(const std::string& filepath): Loads all data within a file and returns as a C++ string
    Color heatmap(float val): Changes a normalized val in range [0.0, 1.0] into a 7-color heatmap color
    Color random_color(const Color& c1, const Color& c2): Returns a random color linearly interpolated between the two given colors
    Color linear_color(float percent, const Color& c1, const Color& c2): Returns the color percent of the way through the linear range between the colors. Percent in range [0.0, 1.0]
    Color hsv_to_rgb(ColorHSV hsv): Quickly (not accurately) converts an HSV color to a RGB color
    ColorHSV rgb_to_hsv(Color rgb): Quickly (not accurately) converts an RGB color to a HSV color
    std::string base64_encode(const std::string &in): Converts an input string into base64 data using standard encoding
    std::string base64_decode(const std::string &in): Converts an input string from base64 data using standard encoding
    std::string trim_spaces(const std::string& source): Trims leading / trailing spaces (and only spaces)
    int get_digit_at_index(int source, int index, int base = 10): Returns the number at the given index of the base number (from right)
    int get_number_length(int source, int base = 10): Returns the length of the given number, in base-10 digits by default

Classes:
    FastBoolGenerator: A really, really fast boolean value generator with pretty random distribution. Use () operator for use.

--- Future Work ---
Reading from bitmap files, even if only ones written by this file
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

#pragma pack(push, 1) // Align to exact bytes

// Source: https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/
// And: https://elcharolin.wordpress.com/2018/11/28/read-and-write-bmp-files-in-c-c/

struct BMPHeader {
    // Header for all Bitmap image data, as well as default values
    // Header
    uint16_t file_type{0x4D42}; // File type, always "BM"
    uint32_t file_size{0};      // Size of the file in total, in bytes
    uint16_t reserved1{0};      // Reserved, always 0
    uint16_t reserved2{0};      // Reserved, always 0
    uint32_t offset_data{0};    // Start position of pixel data, in bytes from beginning of file
    // Info header
    uint32_t size{ 40 };                     // Size of this header (in bytes)
    int32_t width{ 0 };                      // width of bitmap in pixels
    int32_t height{ 0 };                     // height of bitmap in pixels
                                             //       (if positive, bottom-up, with origin in lower left corner)
                                             //       (if negative, top-down, with origin in upper left corner)
    uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
    uint16_t bit_count{ 32 };                // No. of bits per pixel, kept at 32
    uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    uint32_t size_image{ 0 };                // 0 - for uncompressed images
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
    // Color Header
    uint32_t red_mask{ 0x00ff0000 };         // Bit mask for the red channel
    uint32_t green_mask{ 0x0000ff00 };       // Bit mask for the green channel
    uint32_t blue_mask{ 0x000000ff };        // Bit mask for the blue channel
    uint32_t alpha_mask{ 0xff000000 };       // Bit mask for the alpha channel
    uint32_t color_space_type{ 0x73524742 }; // Default "sRGB" (0x73524742)
    uint32_t unused[16]{ 0 };                // Unused data for sRGB color space

    BMPHeader(int image_width, int image_height) {
        // Constructor, because there's a lot of info here
        // NOTE that image_height can be negative for origin at upper left
        int header_size = 14; // Static, kinda hardcoded
        int color_header_size = 84; // Color table length
        width = image_width;
        height = image_height;
        file_size = width*height*4 + header_size + size + color_header_size; // Total file size
        offset_data = header_size + size + color_header_size; // Size of the headers in total
    }
};

#pragma pack(pop)

////////// FUNCTIONS //////////

void save_bmp(const std::string& filepath, const std::vector<std::vector<ColorAlpha>> pixels, bool origin_at_top_left = true) {
    // Saves a double array of pixels as a bitmap image
    // Filepath should end in ".bmp", pixels should be rectangular, and origin_at_top_left toggles between the expected and bottom left (if false)
    if (origin_at_top_left) {
        // Same as usual, just with a negative height the whole way through
        BMPHeader header(pixels.size(), -pixels[0].size());
        std::ofstream output(filepath, std::ofstream::binary);
        output.write((char*)&header, sizeof(header));
        for (int y = 0; y < -header.height; y++) {
            for (int x = 0; x < header.width; x++) {
                output << (char)pixels[x][y].b << (char)pixels[x][y].g << (char)pixels[x][y].r << (char)pixels[x][y].a;
            }
        }
        output.close();
    } else {
        // Buisness as usual
        BMPHeader header(pixels.size(), pixels[0].size());
        std::ofstream output(filepath, std::ofstream::binary);
        output.write((char*)&header, sizeof(header));
        for (int y = 0; y < header.height; y++) {
            for (int x = 0; x < header.width; x++) {
                output << (char)pixels[x][y].b << (char)pixels[x][y].g << (char)pixels[x][y].r << (char)pixels[x][y].a;
            }
        }
        output.close();
    }
}

std::vector<std::vector<ColorAlpha>> make_image_array(int width, int height) {
    // Makes and returns a blank (white) ColorAlpha array of the given dimensions
    return std::vector<std::vector<ColorAlpha>>(width, std::vector<ColorAlpha>(height, {255, 255, 255, 255}));
}

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

inline Color random_color(const Color& c1, const Color& c2)
{
    // Returns a random color linearly interpolated between the two given colors
    float t = (float)rand() / RAND_MAX; // Between 0-1
    return (Color){(uint8_t)(c1.r + (c2.r - c1.r) * t), (uint8_t)(c1.g + (c2.g - c1.g) * t), (uint8_t)(c1.b + (c2.b - c1.b) * t)}; // Compound literal
}

inline Color linear_color(float percent, const Color& c1, const Color& c2)
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

static std::string trim_spaces(const std::string& source) {
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