#ifndef ALEXANDRIA_LIBRARY_INCLUDED
#define ALEXANDRIA_LIBRARY_INCLUDED

/*

The Alexandria Library
Built by Ethan Brucker
Latest:
    https://github.com/roqwerty/alexandria

Released freely under the Unlicense, except where sources are cited.

#---------------------------------------------------------------------------------------------------#
#""--..__|  |            | |         ||     _.---------._     ||         | |            |  |__..--""#
#        ""--..__        | |         ||   .'             '.   ||         | |        __..--""        #
#                ""--..__| |         ||  /                 \  ||         | |__..--""                #
#                        ""--..__    || |    -""-. .-""-    | ||    __..--""                        #
#....____                        ""--||_|   | "" .|   "-|   |_||--''                        ____....#
#        ;"";----....____               "|  |-  - |.-  -|  |"               ____....----;"";        # 
#        |  |            ;";"----....____|  |-"  .|  ""-|  |____....----";";            |  |        # 
#        |  |            | |         || |   |-""-.|.-""-|   | ||         | |            |  |        #
#        |  |            |_|         [] |        \V/        | []         |_|            |  |        #
#        |__|            |_|         || |  ___:__|||__:___  | ||         |_|            |__|        #
#        |  |            | |         || |_|______.^.______|_| ||         | |            |  |        #
#        |__|            | |         ||//                   \\||         | |            |__|        #
#        |  |            | |         |//                     \\|         | |            |  |        #
#---------------------------------------------------------------------------------------------------#

*/

// Compilation Settings
//#define USE_ALEXANDRIA_NAMESPACE // If uncommented or otherwise defined, will make ALL of the following (except macros and defines) part of the Alexandria:: namespace
//#define USE_ALEXANDRIA_COLORLESS // Use colorless testing and output for other functions, useful for OSs like Windows that just don't play nice with color :<
//#define DEBUG // If uncommented, logs, errors, and warnings will be printed to the console

/*
The purpose of the Alexandria library is to collect and standardize functions and utilities that I find myself using somewhat often
Basically it's a mini-Boost, so that's fun :)

--- Notes ---
Remember to #include <stdlib.h> and srand(time(NULL)) if doing anything with randomization

--- Currently Included ---

Macros / Defines:
    T_<COLOR> and TB_<COLOR> for setting terminal foreground and background colors on bash-like terminals (not Windows, so define USE_ALEXANDRIA_COLORLESS above)
    INFO(x): Prints a line of debug code containing information about a variable, including name in code, memory address, variable type, and value (through ostream <<)
    INFO_NOVALUE(x): Prints a line identical to ^, but does not require a defined << operator for custom structs and classes
    INFO_BASIC(x): Prints variable/value name and type ONLY. Able to be used with atomic types
    LOCATION: A std::string "filename:line:function" corresponding to where this macro is used in the source code
    COMPILE_TIME: A std::string containing the time and date of program compilation
    MONO_<CHARACTER/SYMBOL>: An array of Point2s referring to the (x,y) locations of color for that character in the monospace font. Origin is top-left. Dimensions: 5px ascender, 5x5px lowercase, 2px descender, add 1px space
    TIME(x): Times whatever takes place within the parentheses and prints the elapsed time to the console
    BLACK, WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, GRAY: Color struct definitions for basic colors

Debug Printing: (NOTE That these will only print if DEBUG is defined. Else, they compile as nothing)
    LOG(x): Prints the string x as a blue log message
    PASS(x): Prints the string x as a green pass message
    WARNING(x): Prints the string x as a yellow warning message
    ERROR(x): Prints the string x as a red error message

Testing Macros and Values:
    Unit test macros:
        TEST(x): Tests if a value or expression x is true. Basic unit test. Grants the least amount of information (only file/line location)
        TEST_NAMED(n, x): Same as above, but adds a string name n for easier tracking through output and statistics
        TEST_EQ(x, y): Tests if (x == y), basic unit test. Stores and prints the values of x and y and test output in color according to pass/fail along with file and line location
        TEST_NAMED_EQ(n, x, y): See above, but also stores the name string n to the test for future reference alongside file and line numbers
        TEST_EPSILON_EQ(x, y, e): Tests whether two values are within epsilon of each other-s values (abs(x - y) <= e)
        TEST_NEQ(x, y): Same as TEST_EQ for inequality
        TEST_NAMED_NEQ(n, x, y): Same as TEST_NAMED_EQ for inequality
        TEST_EPSILON_NEQ(x, y, e): Same as TEST_EPSILON_EQ for inequality
    Meta-test variables and macros:
        TESTS_SILENT: A boolean variable, default false. If set to true, tests will be silent other than TEST_SUMMARY() below
            Note that this can be toggled true/false multiple times within the code base, allowing verbose explanation and printing of a select subset of tests
        TEST_SUMMARY(): Print a number of test summary statistics, such as passed/total tests, test percentage, and a list of all failed tests, locations, and names (if any)

Structs:
    Point2: A basic 2D point (of integers)
        Supported operators: <<
    Point3: A basic 3D point (of integers)
        Supported operators: <<
    Vector3: A basic 3D floating-point vector
        Supported operators: +, -, *, /, <<
        Supported functions: magnitude(Vector3), normalize(Vector3), cross(Vector3, Vector3), dot(Vector3, Vector3), angle(Vector3, Vector3)
        Supported matrix helpers: native * operator (mat*vec3), make_matrix_3x3(bool identity)
    Color: A small RGB 256-bit color
        Supported operators: <<
        Supported functions: to_grayscale()
    ColorAlpha: A small RGBA 256-bit color
        Supported operators: <<
        Supported functions:
            to_grayscale()
            ColorAlpha::from_float(float), ColorAlpha::to_float() -> casts to/from a floating point number for serial transportation
    ColorHSV: A small HSV 256-bit color
        Supported operators: <<
    BMPHeader: Header for all Bitmap image data, as well as default values, constructor provided

Functions:
    save_bmp(const std::string& filepath, const std::vector<std::vector<ColorAlpha>> pixels, bool origin_at_top_left = true)
        Saves a ColorAlpha vector as an image
    make_matrix_3x3(bool identity = true)
        Blank or identity 3x3 matrix creation
        returns std::vector<std::vector<float>>
    matrix_multiply(std::vector<std::vector<float>>& lhs, std::vector<std::vector<float>>& rhs)
        Matrix multiplication
        returns std::vector<std::vector<float>>
    make_image_array(int width, int height)
        Makes and returns a blank (white) ColorAlpha array of the given dimensions
        returns std::vector<std::vector<ColorAlpha>>
    load_file(const std::string& filepath)
        Loads all data within a file and returns as a C++ string
        returns std::string
    heatmap(float val)
        Changes a normalized val in range [0.0, 1.0] into a 7-color heatmap color
        returns Color
    doppler(float val)
        Changes a normalized val in range [-1.0, 1.0] into either a red or blue color with intensity based on the magnitude of the value
        Negative values become blue, and positive values become red
        returns Color
    random_color(const Color& c1, const Color& c2)
        Returns a random color linearly interpolated between the two given colors
        returns Color
    linear_color(float percent, const Color& c1, const Color& c2)
        Returns the color percent of the way through the linear range between the colors. Percent in range [0.0, 1.0]
        returns Color
    hsv_to_rgb(ColorHSV hsv)
        Quickly (not accurately) converts an HSV color to a RGB color
        returns Color
    rgb_to_hsv(Color rgb)
        Quickly (not accurately) converts an RGB color to a HSV color
        returns ColorHSV
    base64_encode(const std::string &in)
        Converts an input string into base64 data using standard encoding
        returns std::string
    base64_decode(const std::string &in)
        Converts an input string from base64 data using standard encoding
        returns std::string
    crypt(std::string input, std::string key)
        Encrypts/decrypts a string using a key
        returns std::string
    trim_spaces(const std::string& source)
        Trims leading / trailing spaces (and only spaces)
        returns std::string
    get_digit_at_index(int source, int index, int base = 10)
        Returns the number at the given index of the base number (from right)
        returns int
    get_number_length(int source, int base = 10)
        Returns the length of the given number, in base-10 digits by default
        returns int
    collapse_index(unsigned int x, unsigned int y, unsigned int width)
        Collapses a 2D array index into a 1D array index in row-major order
            also available as collapse_index(x, y, z, width, height) for 3D -> 1D conversion
        returns unsigned int
    write_pod(std::ofstream& out, T& t), read_pod(std::ofstream& in, T& t), write_pod_vector(std::ofstream& out, std::vector<T>& vect), and read_pod_vector(std::ofstream& in, std::vector<T>& vect)
        templated general-purpose simple structure serialization
    easeIn/Out double functions for every easing function found at https://easings.net/
        examples: easeLinear(), easeInQuad(), easeInOutExpo(), etc.
    stream_loading_bar(std::ostream& out, float percent, const std::string& title = "", int bar_width = 0, int count_finished = -1, int count_total = -1)
        Sends a loading bar to stream (carriage return, no terminating newline), somewhat slow at the moment
    extract_vector(const std::string& input, char delimiter = ',', const std::string& ignored_characters = " \n\t[](){}")
        Turns an input std::string into a vector of extracted value strings, including an intelligent delimiter and a section of ignored characters
        returns std::vector<std::string>
    extract_map(const std::string& input, char keyval_delimiter = '=', char entry_delimiter = '\n', const std::string& ignored_characters = " \t[](){}")
        Splits an input std::string into a map of key/value pairs based on input delimiters and ignored characters
        returns std::map<std::string, std::string>
    split(const std::string& input, char delimiter = '\n')
        Splits an input std::string into a vector of strings based on the given delimiter
        returns std::vector<std::string>
    get_os():
        Returns a string that is the name of the compiled OS
        returns std::string
    get_abs_path(std::string file):
        Returns the absolute path to the given file's absolute or relative path
            Executable directory: get_abs_path("./")
            Executable path: get_abs_path(std::string(argv[0]))
        returns std::string


Classes:
    FastBoolGenerator: A really, really fast boolean value generator with pretty random distribution. Use () operator for use.
    Tween: A data structure to hold and use the above easing functions to have default behavior as a basic double in range [0.0, 1.0]
        Has built-in double casting, time stretching, and scalar output multiplication, for ease of use
    Rect: A (x, y, w, h) structure built on and for Tweens
        Built-in support for casting to SDL_Rect (also defines SDL_Rect struct if library is not included)
    Circle<type>: A wrapper for std::vector that behaves as a circular buffer with a changeable zero index (moves iterator, NOT whole contents of buffer)
        Supported operations:
            clear, insert(element), remove: Change the contents of the buffer (at current zero index)
            +=, -=, ++circle, circle++, --circle, circle--: Alter the position of the zero index safely, with looping. O(1)
            []: Access the element at offset within the buffer, safely, with looping. Supports negative indexes.
    PythonicVector<type>: A wrapper for std::vector that allows python-like indexing and slicing natively
        Supported operations:
            [index] (indexing): Access the element at offset within the vector. Supports negative indexes.
            (start, end) (slicing): Access the elements between the two offsets within the vector, non-inclusive.

--- Recently Completed And Undocumented ---

--- NOW ---
String-based diff function
A simple function to turn a string of text into a pretty text-based PDF (Courier for monospacing, perhaps line numbers, perhaps title header/page nums)
    save_pdf()
    Needed pre-parse info:
        intelligent line numbers and word wrapping
        length in bytes of all page streams
        total number of pages
        locations of everything in bytes within the file

--- Future Work ---
DEBUG-define-only log to file
A rate-based main loop and callback strucure that can activate functions based on a rate in Hz
Multidimensional vector extraction (copy work from HCR class)
Simple code documentation maker (in markdown)
Function to get a vector list in strings of all filepaths in a directory, even recursively
A wrapper for fprintf(stdout, string, ...) just as "print()"
A function to turn integers of a count of bytes into software/hardware KB/MB/GB summary strings
Generalized structure message construction for serialization, network communication, events, pub/sub models, etc.
ROS-like event-driven topic publisher and subscriber model
A point struct and a system to iterate over certain coordinates, such as all in a circle or within a letter or the like
a define-based system that can remove large sections of code for different debug/release compilations, such as unit tests, print statements, and anything else
to-stream functions for all classes
ultra-basic 2D/3D physics engine wrapper using Verlet Integration: https://www.youtube.com/watch?v=lS_qeBy3aQI
    and friction: https://stackoverflow.com/questions/10270875/verlet-integrator-friction
This color lookup map function: https://www.youtube.com/watch?v=HsOKwUwL1bE
SDICL support and fast kernel interfacing
Make the extract functions templated like the string format function
Regular expression expansion / mapping with returned variable environment map
Fast calculation and return of a set of points in a circle around another point (from SDL_wrapper)
Angle calculation between three points, with one as center. Multidimensional it.
Plotting functionality, even if basic. Save to BMP or image vector for users to display themselves.
Serialization and un-serialization of N-dimensional vectors of basic types
N-dimensional index to 1-dimensional index collapse
Reading from bitmap files, even if only ones written by this file
General-purpose raycasting structure and library, implemented to be as fast as possible
    https://github.com/ssloy/tinyraycaster/wiki
BigInt, like https://stackoverflow.com/questions/4507121/c-big-integer
---
Benched:
Add a debug macro that just returns that string instead of printing it (requires different overloads, so tabled for now)
Numpy-like zeros and ones functions for making vectors automatically of different shapes and sizes (return types are nightmarish)
*/

////////// SETUP //////////

#define PI 3.14159265358979

#define _USE_MATH_DEFINES
#include <cmath> // Math functions, used throughout
#include <string> // C++ string library, used throughout
#include <vector> // C++ vector library, used throughout
#include <map> // C++ map library, used throughout
#include <random> // Random number generation, used only for "random" functions
#include <iostream> // C++ stream input/output, used throughout
#include <fstream> // C++ finestreaming, used for saving/loading functions
#include <functional> // C++ functions-as-variables, used for some classes
#include <stdexcept> // Clean and pretty exception throwing
#include <iomanip> // std::setprecision
#include <stdlib.h> // Pathnames and other utilities
#include <chrono> // Time-based functions


////////// MACROS //////////
#ifndef USE_ALEXANDRIA_COLORLESS
// Terminal colors (source: https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html)
#define T_RESET "\e[0m"
// Terminal Foreground Colors
#define T_BLACK "\e[30m"
#define T_RED "\e[31m"
#define T_GREEN "\e[32m" // equivalently "\e[0;32m"
#define T_YELLOW "\e[33m"
#define T_BLUE "\e[34m"
#define T_MAGENTA "\e[35m"
#define T_CYAN "\e[36m"
#define T_WHITE "\e[37m"
// Terminal Background Colors
#define TB_BLACK "\e[40m"
#define TB_RED "\e[41m"
#define TB_GREEN "\e[42m"
#define TB_YELLOW "\e[43m"
#define TB_BLUE "\e[44m"
#define TB_MAGENTA "\e[45m"
#define TB_CYAN "\e[36m"
#define TB_WHITE "\e[37m"
#else // Below is placeholder values for colorless compilation
#define T_RESET ""
#define T_BLACK ""
#define T_RED ""
#define T_GREEN ""
#define T_YELLOW ""
#define T_BLUE ""
#define T_MAGENTA ""
#define T_CYAN ""
#define T_WHITE ""
// Terminal Background Colors
#define TB_BLACK ""
#define TB_RED ""
#define TB_GREEN ""
#define TB_YELLOW ""
#define TB_BLUE ""
#define TB_MAGENTA ""
#define TB_CYAN ""
#define TB_WHITE ""
#endif

// Debugging macros
#define INFO(x) std::cout << #x << " @ " << &x <<  " (" << typeid(x).name() << ") = " << x << std::endl;
#define INFO_NOVALUE(x) std::cout << #x << " @ " << &x << " (" << typeid(x).name() << ")" << std::endl;
#define INFO_BASIC(x) std::cout << #x << " (" << typeid(x).name() << ")" << std::endl;
#define LOCATION (std::string)__FILE__ + ":" + std::to_string(__LINE__) + ":" + std::to_string(__func__)
#define COMPILE_TIME (std::string)(__TIME__) + " on " + (std::string)(__DATE__)

// Debug printing macros
#define PRINT_VECTOR(x) std::cout << #x << " = "; for (auto i : x) { std::cout << i << " "; } std::cout << std::endl;

// Debug logging macros, only actually print if DEBUG is defined
#ifdef DEBUG
#define LOG(x) std::cout << T_BLUE << "LOG: " << T_RESET << x << std::endl;
#define PASS(x) std::cout << T_GREEN << "PASS: " << T_RESET << x << std::endl;
#define WARNING(x) std::cout << T_YELLOW << "WARNING: " << T_RESET << x << std::endl;
#define ERROR(x) std::cout << T_RED << "ERROR: " << T_RESET << x << std::endl;
#else
#define LOG(x)
#define PASS(x)
#define WARNING(x)
#define ERROR(x)
#endif

// Testing macros
unsigned int TESTS_TOTAL = 0;
unsigned int TESTS_SUCCESSFUL = 0;
bool TESTS_SILENT = false; // If set true, tests other than summary will have no output
std::vector<std::string> TESTS_FAILURES;
#define TEST(x) TESTS_TOTAL++; if (x) {TESTS_SUCCESSFUL++; if (!TESTS_SILENT) {std::cout << T_GREEN << "test passed @ " << LOCATION << T_RESET << std::endl;}} else {TESTS_FAILURES.push_back(LOCATION); if (!TESTS_SILENT) {std::cout << T_RED << "TEST FAILED @ " << LOCATION << T_RESET << std::endl << "\twhere " << #x << " (" << (x) << ") was FALSE" << std::endl;}}
#define TEST_NAMED(n, x) TESTS_TOTAL++; if (x) {TESTS_SUCCESSFUL++; if (!TESTS_SILENT) {std::cout << T_GREEN << "test \"" << n << "\" passed @ " << LOCATION << T_RESET << std::endl;}} else {TESTS_FAILURES.push_back(LOCATION + " (" + n + ")"); if (!TESTS_SILENT) {std::cout << T_RED << "TEST \"" << n << "\" FAILED @ " << LOCATION << T_RESET << std::endl << "\twhere " << #x << " (" << (x) << ") was FALSE" << std::endl;}}
#define TEST_EQ(x, y) TESTS_TOTAL++; if (x == y) {TESTS_SUCCESSFUL++; if (!TESTS_SILENT) {std::cout << T_GREEN << "test passed @ " << LOCATION << T_RESET << std::endl;}} else {TESTS_FAILURES.push_back(LOCATION); if (!TESTS_SILENT) {std::cout << T_RED << "TEST FAILED @ " << LOCATION << T_RESET << std::endl << "\tequating " << #x << " (" << x << ") to " << #y << " (" << y << ")" << std::endl;}}
#define TEST_NAMED_EQ(n, x, y) TESTS_TOTAL++; if (x == y) {TESTS_SUCCESSFUL++; if (!TESTS_SILENT) {std::cout << T_GREEN << "test \"" << n << "\" passed @ " << LOCATION << T_RESET << std::endl;}} else {TESTS_FAILURES.push_back(LOCATION + " (" + n + ")"); if (!TESTS_SILENT) {std::cout << T_RED << "TEST \"" << n << "\" FAILED @ " << LOCATION << T_RESET << std::endl << "\tequating " << #x << " (" << x << ") to " << #y << " (" << y << ")" << std::endl;}}
#define TEST_EPSILON_EQ(x, y, e) TESTS_TOTAL++; if (abs(x - y) <= e) {TESTS_SUCCESSFUL++; if (!TESTS_SILENT) {std::cout << T_GREEN << "test passed @ " << LOCATION << T_RESET << std::endl;}} else {TESTS_FAILURES.push_back(LOCATION); if (!TESTS_SILENT) {std::cout << T_RED << "TEST FAILED @ " << LOCATION << T_RESET << std::endl << "\tequating " << #x << " (" << x << ") to " << #y << " (" << y << ")" << " with epsilon " << e << std::endl;}}
#define TEST_NEQ(x, y) TESTS_TOTAL++; if (x != y) {TESTS_SUCCESSFUL++; if (!TESTS_SILENT) {std::cout << T_GREEN << "test passed @ " << LOCATION << T_RESET << std::endl;}} else {TESTS_FAILURES.push_back(LOCATION); if (!TESTS_SILENT) {std::cout << T_RED << "TEST FAILED @ " << LOCATION << T_RESET << std::endl << "\tinequating " << #x << " (" << x << ") to " << #y << " (" << y << ")" << std::endl;}}
#define TEST_NAMED_NEQ(n, x, y) TESTS_TOTAL++; if (x != y) {TESTS_SUCCESSFUL++; if (!TESTS_SILENT) {std::cout << T_GREEN << "test \"" << n << "\" passed @ " << LOCATION << T_RESET << std::endl;}} else {TESTS_FAILURES.push_back(LOCATION + " (" + n + ")"); if (!TESTS_SILENT) {std::cout << T_RED << "TEST \"" << n << "\" FAILED @ " << LOCATION << T_RESET << std::endl << "\tinequating " << #x << " (" << x << ") to " << #y << " (" << y << ")" << std::endl;}}
#define TEST_EPSILON_NEQ(x, y, e) TESTS_TOTAL++; if (abs(x - y) > e) {TESTS_SUCCESSFUL++; if (!TESTS_SILENT) {std::cout << T_GREEN << "test passed @ " << LOCATION << T_RESET << std::endl;}} else {TESTS_FAILURES.push_back(LOCATION); if (!TESTS_SILENT) {std::cout << T_RED << "TEST FAILED @ " << LOCATION << T_RESET << std::endl << "\tinequating " << #x << " (" << x << ") to " << #y << " (" << y << ")" << " with epsilon " << e << std::endl;}}
#define TEST_SUMMARY() std::cout << T_CYAN << "+--------------+\n| TEST SUMMARY |\n+--------------+\n" << T_GREEN << "Passed " << TESTS_SUCCESSFUL << "/" << TESTS_TOTAL << " tests (" << (TESTS_SUCCESSFUL/(float)TESTS_TOTAL)*100.0 << "%)" << T_RESET << std::endl; if (TESTS_FAILURES.size() > 0) {std::cout << T_RED << "Failed tests:" << T_RESET << std::endl; for (const std::string& s : TESTS_FAILURES) {std::cout << "    " << s << std::endl;}}

// Other Macros
#define TIME(x) {auto start = std::chrono::high_resolution_clock::now(); x; auto end = std::chrono::high_resolution_clock::now(); std::cout << T_CYAN << "+-----------------+\n|    TIME TEST    |\n" << #x << "\n+-----------------+\n" << T_GREEN << "Time taken: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << T_RESET << std::endl;}

// Begin Alexandria namespace
#ifdef USE_ALEXANDRIA_NAMESPACE
namespace Alexandria {
#endif

////////// STRUCTS //////////

// Ultra-basic 2D point
struct Point2 {
    int x;
    int y;
};

// Point2 streaming/printing
std::ostream& operator << (std::ostream& out, const Point2& rhs) {
    out << "(" << rhs.x << ", " << rhs.y << ")";
    return out;
}

// Ultra-basic 3D point
struct Point3 {
    int x;
    int y;
    int z;
};

// Point3 streaming/printing
std::ostream& operator << (std::ostream& out, const Point3& rhs) {
    out << "(" << rhs.x << ", " << rhs.y << ", " << rhs.z << ")";
    return out;
}

// A basic 3D floating-point vector
struct Vector3 {
    float x;
    float y;
    float z;
};

// Vector streaming/printing
std::ostream& operator << (std::ostream& out, const Vector3& rhs) {
    out << "<" << rhs.x << ", " << rhs.y << ", " << rhs.z << ">";
    return out;
}

// Vector-matrix functions
Vector3 operator*(std::vector<std::vector<float>> const& lhs, Vector3 const& rhs) {
    // Throw an error if the vector is not the right size
    if (lhs.size() != 3 || lhs[0].size() != 3) {
        throw std::invalid_argument("Vector3 matrix multiplication only defined for 3x3 floating-point matrices.");
    }

    return (Vector3){
        lhs[0][0]*rhs.x + lhs[1][0]*rhs.y + lhs[2][0]*rhs.z,
        lhs[0][1]*rhs.x + lhs[1][1]*rhs.y + lhs[2][1]*rhs.z,
        lhs[0][2]*rhs.x + lhs[1][2]*rhs.y + lhs[2][2]*rhs.z,
    };
}

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

Vector3 operator/(Vector3 const& lhs, float scalar) {
    return {lhs.x / scalar, lhs.y / scalar, lhs.z / scalar};
}

// Returns the magnitude of a Vector3
float magnitude(Vector3 v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Returns a normalized Vector3
Vector3 normalize(Vector3 v) { return v * (1.0f / magnitude(v)); }

// Returns the cross product of two Vector3
Vector3 cross(Vector3 const& lhs, Vector3 const& rhs) {
    return {lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.x * rhs.y - lhs.y * rhs.x};
}

// Returns the dot product of two Vector3
float dot(Vector3 const& lhs, Vector3 const& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

// Returns the angle between two Vector3
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

// A small RGB 256-bit color
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    // Returns a float in range [0, 1] representing the luminance of the color
    float to_grayscale() {
        return static_cast<float>(r) * 0.299f + static_cast<float>(g) * 0.587f + static_cast<float>(b) * 0.114f;
    }
};

// Basic Color defines for 9 or so basic colors
#define BLACK = (Color){0, 0, 0}
#define WHITE = (Color){255, 255, 255}
#define RED = (Color){255, 0, 0}
#define GREEN = (Color){0, 255, 0}
#define BLUE = (Color){0, 0, 255}
#define YELLOW = (Color){255, 255, 0}
#define CYAN = (Color){0, 255, 255}
#define MAGENTA = (Color){255, 0, 255}
#define GRAY = (Color){128, 128, 128}

// Color streaming/printing
std::ostream& operator << (std::ostream& out, const Color& rhs) {
    out << "(" << std::to_string(rhs.r) << ", " << std::to_string(rhs.g) << ", " << std::to_string(rhs.b) << ")";
    return out;
}

// A small RGBA 256-bit color
struct ColorAlpha {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    // Returns a float in range [0, 1] representing the luminance of the color
    float to_grayscale() {
        return static_cast<float>(r) * 0.299f + static_cast<float>(g) * 0.587f + static_cast<float>(b) * 0.114f;
    }

    // Makes this color from a float
    void from_float(float f) {
        memcpy(this, &f, 4);
    }

    // Returns this but as a float
    float to_float() {
        float f = 0.0;
        memcpy(&f, this, 4);
        return f;
    }
};

// ColorAlpha streaming/printing
std::ostream& operator << (std::ostream& out, const ColorAlpha& rhs) {
    out << "(" << std::to_string(rhs.r) << ", " << std::to_string(rhs.g) << ", " << std::to_string(rhs.b) << ", " << std::to_string(rhs.a) << ")";
    return out;
}

// A small HSV 256-bit color
struct ColorHSV {
    uint8_t h;
    uint8_t s;
    uint8_t v;
};

// ColorHSV streaming/printing
std::ostream& operator << (std::ostream& out, const ColorHSV& rhs) {
    out << "(" << std::to_string(rhs.h) << ", " << std::to_string(rhs.s) << ", " << std::to_string(rhs.v) << ")";
    return out;
}

#pragma pack(push, 1) // Align to exact bytes

// Header for all Bitmap image data, as well as default values and a QoL constructor
// Source: https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/
// And: https://elcharolin.wordpress.com/2018/11/28/read-and-write-bmp-files-in-c-c/
struct BMPHeader {
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

// Saves a double array of pixels as a bitmap image
// Filepath should end in ".bmp", pixels should be rectangular, and origin_at_top_left toggles between the expected and bottom left (if false)
void save_bmp(const std::string& filepath, const std::vector<std::vector<ColorAlpha>> pixels, bool origin_at_top_left = true) {
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

// Makes and returns a 3x3 float vector, for use with Vector3 math.
// If identity is set, it will be an identity matrix. Otherwise, it will be a zero matrix
std::vector<std::vector<float>> make_matrix_3x3(bool identity = true) {
    if (identity) {
        return std::vector<std::vector<float>>{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    } else {
        return std::vector<std::vector<float>>{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
    }
}

// Multiplies two matrices of identical dimensions together
std::vector<std::vector<float>> matrix_multiply(std::vector<std::vector<float>>& lhs, std::vector<std::vector<float>>& rhs) {
    // Validate matrix sizes
    if (lhs[0].size() != rhs.size()) {
        throw std::invalid_argument("Matrix multiplication only defined for matrices of shapes n*m & m*p");
    }

    // Helpful defines
    int n = lhs.size();
    int m = lhs[0].size();
    int p = rhs[0].size();

    // Make a new matrix of the correct size (n*p)
    std::vector<std::vector<float>> result = std::vector<std::vector<float>>(n, std::vector<float>(p, 0.0));

    // Multiply
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            float acc = 0;
            for (int k = 0; k < m; k++) {
                acc += lhs[i][k] * rhs[k][j];
            }
            result[i][j] = acc;
        }
    }

    return result;
}


// Makes and returns a blank (white) ColorAlpha array of the given dimensions
std::vector<std::vector<ColorAlpha>> make_image_array(int width, int height) {
    return std::vector<std::vector<ColorAlpha>>(width, std::vector<ColorAlpha>(height, {255, 255, 255, 255}));
}

// Loads all data within a file and returns as a C++ string
std::string load_file(const std::string& filepath) {
    std::ifstream f(filepath);
    if (f.good()) {
        std::string str;

        f.seekg(0, std::ios::end);   
        str.reserve(f.tellg());
        f.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();
        return str;
    }
    else {
        return ""; // No data
    }
}

// Changes a normalized val in range [0.0, 1.0] into a 7-color heatmap color
// Source: https://andrewnoske.com/wiki/Code_-_heatmaps_and_color_gradients
Color heatmap(float val) {
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

// Changes a normalized val in range [-1.0, 1.0] into a Doppler-effect like red or blue shift
// Negative val will become blue with increasing intensity
// Positive val will become red with increasing intensity
Color doppler(float val) {
    if (val < 0.0) {
        return {0, 0, (uint8_t)(255.0 * abs(val))};
    } else {
        return {(uint8_t)(255.0 * val), 0, 0};
    }
}

// Returns a random color linearly interpolated between the two given colors
inline Color random_color(const Color& c1, const Color& c2) {
    float t = (float)rand() / RAND_MAX; // Between 0-1
    return (Color){(uint8_t)(c1.r + (c2.r - c1.r) * t), (uint8_t)(c1.g + (c2.g - c1.g) * t), (uint8_t)(c1.b + (c2.b - c1.b) * t)}; // Compound literal
}

// Returns the color percent of the way through the linear range between the colors. Percent in range [0.0, 1.0]
inline Color linear_color(float percent, const Color& c1, const Color& c2) {
    return (Color){(uint8_t)(c1.r + (c2.r - c1.r) * percent), (uint8_t)(c1.g + (c2.g - c1.g) * percent), (uint8_t)(c1.b + (c2.b - c1.b) * percent)};
}

// Returns a random color linearly interpolated between the two given colors
inline ColorAlpha random_color(const ColorAlpha& c1, const ColorAlpha& c2) {
    float t = (float)rand() / RAND_MAX; // Between 0-1
    return (ColorAlpha){(uint8_t)(c1.r + (c2.r - c1.r) * t), (uint8_t)(c1.g + (c2.g - c1.g) * t), (uint8_t)(c1.b + (c2.b - c1.b) * t), (uint8_t)(c1.a + (c2.a - c1.a) * t)};
}

// Returns the color percent of the way through the linear range between the colors. Percent in range [0.0, 1.0]
inline ColorAlpha linear_color(float percent, const ColorAlpha& c1, const ColorAlpha& c2) {
    return (ColorAlpha){(uint8_t)(c1.r + (c2.r - c1.r) * percent), (uint8_t)(c1.g + (c2.g - c1.g) * percent), (uint8_t)(c1.b + (c2.b - c1.b) * percent), (uint8_t)(c1.a + (c2.a - c1.a) * percent)};
}

// Quickly (not accurately) converts an HSV color to a RGB color
// RGB <-> HSV functions source: https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
Color hsv_to_rgb(ColorHSV hsv) {
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

// Quickly (not accurately) converts an RGB color to a HSV color
// RGB <-> HSV functions source: https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
ColorHSV rgb_to_hsv(Color rgb) {
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

// Encodes a Base64 C++ string
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

// Decodes a Base64 C++ string
// Base64 functions source: https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
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

// Encrypts/decrypts an input string using a key
std::string crypt(std::string input, std::string key) {
    std::string output;
    for (int i = 0; i < input.length(); i++) {
        output += char(input[i] ^ key[i % key.length()]);
    }
    return output;
}

// Trims leading / trailing spaces (and only spaces)
static std::string trim_spaces(const std::string& source) {
    return source.substr(source.find_first_not_of(" "), source.find_last_not_of(" ") - source.find_first_not_of(" ") + 1);
}

// Returns the number at the given index of the source number (from right)
int get_digit_at_index(int source, int index, int base = 10) {
    int i = 0; // Iterable
    while (i < index)
    {
        source /= base;
        i++;
    }
    return (source % base); // Gives back only the rightmost digit
}

// Returns the length of the given number, in base-base digits
int get_number_length(int source, int base = 10) {
    int digits = 0;
    while (source)
    {
        source /= base;
        digits++;
    }
    return digits;
}

// Collapses a 2D array index into a 1D array index in row-major order
inline unsigned int collapse_index(unsigned int x, unsigned int y, unsigned int width) {
    return y*width + x;
}

// Collapses a 3D array index into a 1D array index in row-major order
inline unsigned int collapse_index(unsigned int x, unsigned int y, unsigned int z, unsigned int width, unsigned int height) {
    return x*width*height + y*width + z;
}

// Saves a simple continuous structure to file
// Template structure serialization source: https://snipplr.com/view/10148/c-structure-how-do-i-write-a-structure-to-a-file
template<typename T>
void write_pod(std::ofstream& out, T& t) {
    out.write(reinterpret_cast<char*>(&t), sizeof(T));
}

// Reads a simple continuous structure to file
// Template structure serialization source: https://snipplr.com/view/10148/c-structure-how-do-i-write-a-structure-to-a-file
template<typename T>
void read_pod(std::ifstream& in, T& t) {
    in.read(reinterpret_cast<char*>(&t), sizeof(T));
}

// Writes an entire vector at once by prefixing it with the size of the vector in elements
// Template structure serialization source: https://snipplr.com/view/10148/c-structure-how-do-i-write-a-structure-to-a-file
template<typename T>
void write_pod_vector(std::ofstream& out, std::vector<T>& vect) {
    long size = vect.size();
    write_pod<long>(out, size);
    out.write(reinterpret_cast<char*>(vect.data()), size * sizeof(T));
}

// Reads an entire vector at once by prefixing it with the size of the vector in elements
// Template structure serialization source: https://snipplr.com/view/10148/c-structure-how-do-i-write-a-structure-to-a-file
template<typename T>
void read_pod_vector(std::ifstream& in, std::vector<T>& vect) {
    long size;
    read_pod(in, size);
    vect.resize(size);
    in.read(reinterpret_cast<char*>(vect.data()), size * sizeof(T));
}

// Easing function defines for faster calculation
// Basically this whole thing is from https://easings.net/
#define C1_EASE 1.70158
#define C2_EASE C1_EASE * 1.525
#define C3_EASE C1_EASE + 1
#define C4_EASE (2 * PI) / 3
#define C5_EASE (2 * PI) / 4.5

double easeLinear(double x) {return x;}
double easeInQuad(double x) {return x * x;}
double easeOutQuad(double x) {return 1.0 - (1.0 - x) * (1.0 - x);}
double easeInOutQuad(double x) {return x < 0.5 ? 2.0 * x * x : 1 - pow(-2.0 * x + 2.0, 2.0) / 2.0;}
double easeInCubic(double x) {return x * x * x;}
double easeOutCubic(double x) {return 1.0 - pow(1.0 - x, 3.0);}
double easeInOutCubic(double x) {return x < 0.5 ? 4.0 * x * x * x : 1.0 - pow(-2.0 * x + 2.0, 3.0) / 2.0;}
double easeInQuart(double x) {return x * x * x * x;}
double easeOutQuart(double x) {return 1.0 - pow(1.0 - x, 4.0);}
double easeInOutQuart(double x) {return x < 0.5 ? 8.0 * x * x * x * x : 1.0 - pow(-2.0 * x + 2.0, 4.0) / 2.0;}
double easeInQuint(double x) {return x * x * x * x * x;}
double easeOutQuint(double x) {return 1.0 - pow(1.0 - x, 5.0);}
double easeInOutQuint(double x) {return x < 0.5 ? 16.0 * x * x * x * x * x : 1.0 - pow(-2.0 * x + 2.0, 5.0) / 2.0;}
double easeInSine(double x) {return 1.0 - cos((x * PI) / 2.0);}
double easeOutSine(double x) {return sin((x * PI) / 2.0);}
double easeInOutSine(double x) {return -(cos(PI * x) - 1.0) / 2.0;}
double easeInExpo(double x) {return x == 0.0 ? 0.0 : pow(2.0, 10.0 * x - 10.0);}
double easeOutExpo(double x) {return x == 1.0 ? 1.0 : 1.0 - pow(2.0, -10.0 * x);}
double easeInOutExpo(double x) {
    return x == 0.0
        ? 0.0
        : x == 1
        ? 1.0
        : x < 0.5
        ? pow(2.0, 20.0 * x - 10.0) / 2.0
        : (2.0 - pow(2.0, -20.0 * x + 10.0)) / 2.0;
}
double easeInCirc(double x) {return 1.0 - sqrt(1.0 - pow(x, 2.0));}
double easeOutCirc(double x) {return sqrt(1.0 - pow(x - 1.0, 2.0));}
double easeInOutCirc(double x) {
    return x < 0.5
        ? (1.0 - sqrt(1.0 - pow(2.0 * x, 2.0))) / 2.0
        : (sqrt(1.0 - pow(-2.0 * x + 2.0, 2.0)) + 1.0) / 2.0;
}
double easeInBack(double x) {return C3_EASE * x * x * x - C1_EASE * x * x;}
double easeOutBack(double x) {return 1.0 + C3_EASE * pow(x - 1.0, 3.0) + C1_EASE * pow(x - 1.0, 2.0);}
double easeInOutBack(double x) {
    return x < 0.5
        ? (pow(2.0 * x, 2.0) * ((C2_EASE + 1.0) * 2.0 * x - C2_EASE)) / 2.0
        : (pow(2.0 * x - 2.0, 2.0) * ((C2_EASE + 1.0) * (x * 2.0 - 2.0) + C2_EASE) + 2.0) / 2.0;
}
double easeInElastic(double x) {
    return x == 0.0
        ? 0.0
        : x == 1.0
        ? 1.0
        : -pow(2.0, 10.0 * x - 10.0) * sin((x * 10.0 - 10.75) * C4_EASE);
}
double easeOutElastic(double x) {
    return x == 0.0
        ? 0.0
        : x == 1.0
        ? 1.0
        : pow(2.0, -10.0 * x) * sin((x * 10.0 - 0.75) * C4_EASE) + 1.0;
}
double easeInOutElastic(double x) {
    return x == 0.0
        ? 0.0
        : x == 1.0
        ? 1.0
        : x < 0.5
        ? -(pow(2.0, 20.0 * x - 10.0) * sin((20.0 * x - 11.125) * C5_EASE)) / 2.0
        : (pow(2.0, -20.0 * x + 10.0) * sin((20.0 * x - 11.125) * C5_EASE)) / 2.0 + 1.0;
}
double easeOutBounce(double x) {
	const double n1 = 7.5625;
	const double d1 = 2.75;

	if (x < 1.0 / d1) {
		return n1 * x * x;
	} else if (x < 2.0 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	} else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	} else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}
double easeInBounce(double x) {
    return 1.0 - easeOutBounce(1.0 - x);
}
double easeInOutBounce(double x) {
    return x < 0.5
        ? (1.0 - easeOutBounce(1.0 - 2.0 * x)) / 2.0
        : (1.0 + easeOutBounce(2.0 * x - 1.0)) / 2.0;
}

// Prints a loading bar based on given inputs:
//  out: output stream
//  percent: percent complete in range [0.0, 1.0]
//  title: Prints a title to the loading bar
//  bar_width: The total length (in characters) of a [===>  ]-style bar, if any (2 + number of = signs)
//  count_finished: The number of finished tasks
//  count_total: The number of total tasks
// NOTE: This is really kinda slow at the moment, but it DOES work. Don't use per-operation, use per-chunk
void stream_loading_bar(std::ostream& out, float percent, const std::string& title = "", int bar_width = 0, int count_finished = -1, int count_total = -1) {
    out << "\r"; // Return to beginning of line

    // Title
    if (title != "") {
        out << title << ": ";
    }

    // Loading bar
    if (bar_width > 2) {
        out << "[";
        int cutoff = (bar_width-2)*percent;
        for (int i = 0; i < bar_width-2; i++) {
            if (i < cutoff) {
                out << "=";
            } else if (i == cutoff) {
                out << ">";
            } else {
                out << " ";
            }
        }
        out << "] ";
    }

    // Percent number
    out << std::setprecision(4) << std::setw(5) << percent*100.0 << "%";

    // Finished counts
    if (count_finished != -1 && count_total != -1) {
        out << " (" << count_finished << "/" << count_total << ")";
    }
}

// Turns an input std::string into a vector of extracted value strings, including an intelligent delimiter and a section of ignored characters
std::vector<std::string> extract_vector(const std::string& input, char delimiter = ',', const std::string& ignored_characters = " \n\t[](){}") {
    std::string current = ""; // The current string selection
    std::vector<std::string> values; // The return values
    for (char c : input) {
        if (ignored_characters.find(c) == std::string::npos) {
            // Is a valid value or delim character
            if (c == delimiter) {
                // Split value
                values.push_back(current);
                current = "";
            } else {
                current += c;
            }
        }
    }
    if (current != "") {
        values.push_back(current);
    }
    return values;
}

// Similar to extract_vector, but is much simpler
std::vector<std::string> split(const std::string& input, char delimiter = '\n') {
    std::vector<std::string> output;
    std::string current = "";
    for (char c : input) {
        if (c == delimiter) {
            output.push_back(current);
            current = "";
        } else {
            current += c;
        }
    }
    output.push_back(current);
    return output;
}

// Splits an input std::string into a map of key/value pairs based on input delimiters and ignored characters
std::map<std::string, std::string> extract_map(const std::string& input, char keyval_delimiter = '=', char entry_delimiter = '\n', const std::string& ignored_characters = " \t[](){}") {
    // Get entry lines
    std::string current = ""; // The current string selection
    std::vector<std::string> entries; // The entry pairs
    for (char c : input) {
        if (ignored_characters.find(c) == std::string::npos) {
            // Is a valid value or delim character
            if (c == entry_delimiter) {
                // Split value
                entries.push_back(current);
                current = "";
            } else {
                current += c;
            }
        }
    }
    if (current != "") {
        entries.push_back(current);
    }

    // Add entries into a map
    std::map<std::string, std::string> keyvals; // The return map
    for (const std::string& e : entries) {
        size_t index = e.find(keyval_delimiter);
        keyvals[e.substr(0, index)] = e.substr(index+1);
    }
    return keyvals;
}

// Returns a string that is the name of the compiled OS
// Source: https://stackoverflow.com/questions/15580179/how-do-i-find-the-name-of-an-operating-system
std::string get_os() {
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Other";
    #endif
}

// Returns the absolute path to the given file's absolute or relative path
//  Executable directory: get_abs_path("./")
//  Executable path: get_abs_path(std::string(argv[0]))
// Source: https://pubs.opengroup.org/onlinepubs/000095399/functions/realpath.html
std::string get_abs_path(std::string file) {
    char* path_ptr = _fullpath(NULL, file.c_str(), 100);
    std::string result(path_ptr);
    free(path_ptr);
    return result;
}

/*// Acts like Linux' diff command, printing the difference between two strings
void get_diff(const std::string& one, const std::string& two) {
    std::vector<std::string> lines_one = split(one, '\n');
    std::vector<std::string> lines_two = split(two, '\n');
    int j = 0;
    for (int i = 0; i < lines_one.size(); i++) {
        // If j is out of bounds, print the rest of one's lines
        if (j >= lines_two.size()) {
            for (; i < lines_one.size(); i++) {
                std::cout << "-" << lines_one[i] << std::endl;
            }
        }

        if (lines_one[i] == lines_two[j]) {
            // Lines are the same, advance
            j++;
        } else {
            // Lines are different, find out the next index where they match again
            // Is there an extra line in one?
            for (int k = i+1; k < lines_one.size(); k++) {
                if (lines_one[k] == lines_two[j]) {
                    // Found a match, print the diff
                    for (int l = i; l < k; l++) {
                        std::cout << "-" << lines_one[l] << std::endl;
                    }
                    // Align indexes
                    i = k;
                    j++;
                    break;
                }
            }
            // Is there an extra line in two?
            for (int k = j+1; k < lines_two.size(); k++) {
                if (lines_one[i] == lines_two[k]) {
                    // Found a match, print the diff
                    for (int l = j; l < k; l++) {
                        std::cout << "+" << lines_two[l] << std::endl;
                    }
                    // Align indexes
                    j = k;
                    j++;
                    break;
                }
            }
            // These are different lines, print them
            std::cout << "@1: " << lines_one[i] << std::endl;
            std::cout << "@2: " << lines_two[j] << std::endl;
            j++;
        }
    }

    // If we still have lines in two, print them
    for (; j < lines_two.size(); j++) {
        std::cout << "+" << lines_two[j] << std::endl;
    }
}*/

// Defines for PDF things
#define PDF_CHARS_BEFORE_WRAP 76 // Number of characters to be printed before a word wrap
#define PDF_LINES_IN_PAGE 54 // Number of lines that fit on a page

// Saves a PDF based off of the given input data to disk. Uses intelligent line wrapping and optionally line numbers
void save_pdf(const std::string& data, const std::string& filepath, bool use_line_numbers = false) {
    // Make a cleaned version of the data based on word wrapping
    //std::vector<std::string> pages; // A collection of cleaned pages
    //std::string page = ""; // The current page
    std::vector<std::vector<std::string>> pages; // A collection of cleaned pages, composed of vectors of lines
    std::vector<std::string> alllines; // A collection of all lines to be split into pages
    int chars_in_line = 0; // Number of characters in the current line
    int lines = 0; // Number of lines in this page
    //int line_num = 1; // The line number of the current line in data

    std::cout << "\n  ---  \nUNFINISHED\n  ---   " << std::endl;

    /*// Clean the input
    for (char c : data) {
        if (c == '\r') {} // Eat carriage returns
        else if (c == '\n') {
            if (lines < PDF_LINES_IN_PAGE) {
                // Add to this page
                page += "\n";
                chars_in_line = 0;
                lines++;
            } else {
                // Move on to next page
                pages.push_back(page);
                page = "";
                chars_in_line = 0;
                lines = 0;
            }
        } else {
            if (chars_in_line < PDF_CHARS_BEFORE_WRAP) {
                page += c;
                chars_in_line++;
            } else {
                // Add line with wrap
                if (lines < PDF_LINES_IN_PAGE) {
                    // Add to this page
                    page += c + "\n";
                    chars_in_line = 0;
                    lines++;
                } else {
                    // Move on to next page
                    pages.push_back(page);
                    page = c + "";
                    chars_in_line = 1;
                    lines = 0;
                }
        }
    }*/
    
    // Setup header information
}

////////// CLASSES //////////

// A really, really fast boolean value generator with pretty random distribution
// Usage: FastBoolGenerator fbg; bool random = fbg();
class FastBoolGenerator {
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

// A class for a double value that can be easily changed over time according to an easing function
class Tween {
public:
    // Constructor
    Tween(std::function<double(double)> easing_function = &easeLinear, double end_time = 1.0, double scale = 1.0) {
        function = easing_function;
        value = 0.0;
        time = end_time;
        current_time = 0.0;
        scalar = scale;
    }
    // Advance the time by the value
    void advance(double delta_time) {
        current_time += delta_time;
        update();
    }
    // Sets the time to a given value
    void set_time(double new_time) {
        current_time = new_time;
        update();
    }
    // Resets this value with new functions and a new start
    void reset(std::function<double(double)> easing_function = &easeLinear, double end_time = 1.0, double scale = 1.0) {
        function = easing_function;
        value = 0.0;
        time = end_time;
        current_time = 0.0;
        scalar = scale;
    }
    operator double() const {return value * scalar;} // Built-in casting to double
    const double operator() () {return value * scalar;} // Getting value 
private:
    // Updates the value based on the now time
    void update() {
        if (current_time > time) {
            value = 1.0;
        } else if (current_time < 0.0) {
            value = 0.0;
        } else {
            value = function(current_time / time);
        }
    }
    double value = 0.0; // The current value of the output, will be mapped to mostly [0.0, 1.0]
    double time = 1.0; // The value of time at which the animation will be complete
    double current_time = 0.0; // The current value of the time
    double scalar = 1.0; // How much to scale the output by
    std::function<double(double)> function; // The easing function used
};

#ifndef SDL_h_ // Only set in SDL.h, so this only triggers if not already defined
// Overload for an SDL_Rect
struct SDL_Rect {
    int x, y, w, h;
};
#endif

class Rect {
public:
    // Basic Constructor
    Rect(double x = 0.0, double y = 0.0, double w = 0.0, double h = 0.0) {
        scale_x = x;
        scale_y = y;
        scale_w = w;
        scale_h = h;
        x = Tween(&easeLinear, 1.0, scale_x);
        y = Tween(&easeLinear, 1.0, scale_y);
        w = Tween(&easeLinear, 1.0, scale_w);
        h = Tween(&easeLinear, 1.0, scale_h);
    }

    // Advanced constructor
    Rect(const Tween& tx, const Tween& ty, const Tween& tw, const Tween& th) {
        x = tx;
        y = ty;
        w = tw;
        h = th;
    }

    // Update function, advances the time by the given time for all elements
    void advance(double delta_time) {
        x.advance(delta_time);
        y.advance(delta_time);
        w.advance(delta_time);
        h.advance(delta_time);
    }

    // Sets the time for all contained Tweens
    void set_time(double new_time) {
        x.set_time(new_time);
        y.set_time(new_time);
        w.set_time(new_time);
        h.set_time(new_time);
    }

    // Casting to SDL_Rect
    operator SDL_Rect() const {return SDL_Rect{(int)(double)x, (int)(double)y, (int)(double)w, (int)(double)h};}

    Tween x, y, w, h; // Tween variables, can be edited independently
    double scale_x, scale_y, scale_w, scale_h; // Default scale of all the values
private:
};

// A wrapper for an std::vector that makes it behave like a circular buffer
template <typename T>
class Circle {
public:
    // Functions

    // Clears the entire structure
    void clear() {
        contents.clear();
        index = 0;
    }

    // Add a new element at the current buffer position, moving current element forwards (new element inserted before)
    void insert(T element) {
        auto it = contents.begin();
        contents.insert(it+index, element);
    }

    // Removes the element at the current position
    void remove() {
        auto it = contents.begin();
        contents.erase(it+index);
    }

    // Advance the zero position
    Circle& operator+=(int delta) {
        if (contents.size() == 0) {throw std::out_of_range("Attempted to advance index of buffer of size 0");} // No data
        index += delta;
        while (index >= contents.size()) {
            index -= contents.size();
        }
        while (index < 0) {
            index += contents.size();
        }
        return (*this);
    }

    // Retreat the zero posiiton
    Circle& operator-=(int delta) {
        if (contents.size() == 0) {throw std::out_of_range("Attempted to retreat index of buffer of size 0");} // No data
        index -= delta;
        while (index >= contents.size()) {
            index -= contents.size();
        }
        while (index < 0) {
            index += contents.size();
        }
        return (*this);
    }

    // Prefix increment position
    Circle& operator++() {
        index++;
        if (index == contents.size()) {
            index = 0;
        }
        return *this;
    }

    // Postfix increment position
    T operator++(int) {
        T og = (*this)[0];
        ++(*this);
        return og;
    }

    // Prefix decrement position
    Circle& operator--() {
        index--;
        if (index == -1) {
            index = contents.size() - 1;
        }
        return *this;
    }

    // Postfix decrement position
    T operator--(int) {
        T og = (*this)[0];
        --(*this);
        return og;
    }

    // Returns the element at the given offset from the current pointer
    T& operator[](int offset) {
        if (contents.size() == 0) {throw std::out_of_range("Attempted to access element of buffer of size 0");} // No data
        int target = index + offset;
        while (target >= contents.size()) {
            target -= contents.size();
        }
        while (target < 0) {
            target += contents.size();
        }
        return contents[target];
    }

    // Size function, use this for looping
    int size() {
        return contents.size();
    }

    // Variables
private:
    // Variables
    std::vector<T> contents; // The underlying data
    int index = 0; // The index of the current data element as the "beginning" of the buffer
    //typename std::vector<T>::iterator it = contents.begin(); // The current position of the "beginning" of the buffer
    // Functions
};

// A wrapper for an std::vector that allows python-like indexing (negative indexes, slicing, etc)
template <typename T>
class PythonicVector {
public:
    // Functions

    // Bracket operator, allows negative indexes
    T& operator [] (int index) {
        if (index < 0) {
            index += contents.size();
        }
        return contents[index];
    }

    // Parenthesis operator, allows slicing into sub-vectors non-inclusively (cannot use negative indexes)
    PythonicVector<T> operator () (int start, int end) {
        PythonicVector<T> new_vector;
        for (int i = start; i < end; i++) {
            new_vector.contents.push_back(contents[i]);
        }
        return new_vector;
    }

    // Vector overloads
    void push_back(T element) {
        contents.push_back(element);
    }

    void pop_back() {
        contents.pop_back();
    }

    void clear() {
        contents.clear();
    }

    void resize(int new_size) {
        contents.resize(new_size);
    }

    void reserve(int new_size) {
        contents.reserve(new_size);
    }

    // Return the size of the vector
    int size() {
        return contents.size();
    }

    // Returns contents.begin()
    typename std::vector<T>::iterator begin() {
        return contents.begin();
    }

    // Returns contents.end()
    typename std::vector<T>::iterator end() {
        return contents.end();
    }

    // Variables
    std::vector<T> contents; // The contained data
};

////////// SUPER-LONG DEFINES //////////

// Monogram font hard-coded pixel defines
// Source: https://datagoblin.itch.io/monogram
#define MONO_0 (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {3, 5}, {4, 5}, {0, 6}, {2, 6}, {4, 6}, {0, 7}, {1, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_1 (Point2[]){{2, 3}, {1, 4}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_2 (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {4, 5}, {3, 6}, {2, 7}, {1, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_3 (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {4, 5}, {2, 6}, {3, 6}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_4 (Point2[]){{1, 3}, {4, 3}, {1, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {4, 7}, {4, 8}, {4, 9}}
#define MONO_5 (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {0, 4}, {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 6}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_6 (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_7 (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {4, 4}, {4, 5}, {3, 6}, {2, 7}, {2, 8}, {2, 9}}
#define MONO_8 (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {1, 6}, {2, 6}, {3, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_9 (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_A (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {4, 9}}
#define MONO_B (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_C (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_D (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_E (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {0, 7}, {0, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_F (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {0, 7}, {0, 8}, {0, 9}}
#define MONO_G (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {0, 6}, {2, 6}, {3, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_H (Point2[]){{0, 3}, {4, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {4, 9}}
#define MONO_I (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_J (Point2[]){{4, 3}, {4, 4}, {4, 5}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_K (Point2[]){{0, 3}, {4, 3}, {0, 4}, {3, 4}, {0, 5}, {2, 5}, {0, 6}, {1, 6}, {0, 7}, {2, 7}, {0, 8}, {3, 8}, {0, 9}, {4, 9}}
#define MONO_L (Point2[]){{0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_M (Point2[]){{0, 3}, {4, 3}, {0, 4}, {1, 4}, {3, 4}, {4, 4}, {0, 5}, {2, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {4, 9}}
#define MONO_N (Point2[]){{0, 3}, {4, 3}, {0, 4}, {4, 4}, {0, 5}, {1, 5}, {4, 5}, {0, 6}, {2, 6}, {4, 6}, {0, 7}, {3, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {4, 9}}
#define MONO_O (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_P (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {0, 7}, {0, 8}, {0, 9}}
#define MONO_Q (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}, {3, 10}, {4, 10}}
#define MONO_R (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {4, 9}}
#define MONO_S (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {1, 6}, {2, 6}, {3, 6}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_T (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 9}}
#define MONO_U (Point2[]){{0, 3}, {4, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_V (Point2[]){{0, 3}, {4, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {4, 6}, {1, 7}, {3, 7}, {1, 8}, {3, 8}, {2, 9}}
#define MONO_W (Point2[]){{0, 3}, {4, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {2, 7}, {4, 7}, {0, 8}, {1, 8}, {3, 8}, {4, 8}, {0, 9}, {4, 9}}
#define MONO_X (Point2[]){{0, 3}, {4, 3}, {0, 4}, {4, 4}, {1, 5}, {3, 5}, {2, 6}, {1, 7}, {3, 7}, {0, 8}, {4, 8}, {0, 9}, {4, 9}}
#define MONO_Y (Point2[]){{0, 3}, {4, 3}, {0, 4}, {4, 4}, {1, 5}, {3, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 9}}
#define MONO_Z (Point2[]){{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {4, 4}, {3, 5}, {2, 6}, {1, 7}, {0, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_a (Point2[]){{1, 5}, {2, 5}, {3, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_b (Point2[]){{0, 3}, {0, 4}, {0, 5}, {1, 5}, {2, 5}, {3, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_c (Point2[]){{1, 5}, {2, 5}, {3, 5}, {0, 6}, {4, 6}, {0, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_d (Point2[]){{4, 3}, {4, 4}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_e (Point2[]){{1, 5}, {2, 5}, {3, 5}, {0, 6}, {4, 6}, {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {0, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_f (Point2[]){{2, 3}, {3, 3}, {1, 4}, {4, 4}, {1, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {1, 7}, {1, 8}, {1, 9}}
#define MONO_g (Point2[]){{1, 5}, {2, 5}, {3, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {4, 10}, {1, 11}, {2, 11}, {3, 11}}
#define MONO_h (Point2[]){{0, 3}, {0, 4}, {0, 5}, {1, 5}, {2, 5}, {3, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {4, 9}}
#define MONO_i (Point2[]){{2, 3}, {1, 5}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_j (Point2[]){{4, 3}, {3, 5}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {4, 9}, {0, 10}, {4, 10}, {1, 11}, {2, 11}, {3, 11}}
#define MONO_k (Point2[]){{0, 3}, {0, 4}, {0, 5}, {4, 5}, {0, 6}, {3, 6}, {0, 7}, {1, 7}, {2, 7}, {0, 8}, {3, 8}, {0, 9}, {4, 9}}
#define MONO_l (Point2[]){{0, 3}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_m (Point2[]){{0, 5}, {1, 5}, {2, 5}, {3, 5}, {0, 6}, {2, 6}, {4, 6}, {0, 7}, {2, 7}, {4, 7}, {0, 8}, {2, 8}, {4, 8}, {0, 9}, {2, 9}, {4, 9}}
#define MONO_n (Point2[]){{0, 5}, {1, 5}, {2, 5}, {3, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {4, 9}}
#define MONO_o (Point2[]){{1, 5}, {2, 5}, {3, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_p (Point2[]){{0, 5}, {1, 5}, {2, 5}, {3, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}, {0, 10}, {0, 11}}
#define MONO_q (Point2[]){{1, 5}, {2, 5}, {3, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {4, 10}, {4, 11}}
#define MONO_r (Point2[]){{0, 5}, {2, 5}, {3, 5}, {0, 6}, {1, 6}, {4, 6}, {0, 7}, {0, 8}, {0, 9}}
#define MONO_s (Point2[]){{1, 5}, {2, 5}, {3, 5}, {4, 5}, {0, 6}, {1, 7}, {2, 7}, {3, 7}, {4, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_t (Point2[]){{1, 3}, {1, 4}, {0, 5}, {1, 5}, {2, 5}, {3, 5}, {1, 6}, {1, 7}, {1, 8}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_u (Point2[]){{0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_v (Point2[]){{0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {1, 8}, {3, 8}, {2, 9}}
#define MONO_w (Point2[]){{0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {2, 7}, {4, 7}, {0, 8}, {2, 8}, {4, 8}, {1, 9}, {3, 9}}
#define MONO_x (Point2[]){{0, 5}, {4, 5}, {1, 6}, {3, 6}, {2, 7}, {1, 8}, {3, 8}, {0, 9}, {4, 9}}
#define MONO_y (Point2[]){{0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {0, 8}, {4, 8}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {4, 10}, {1, 11}, {2, 11}, {3, 11}}
#define MONO_z (Point2[]){{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {3, 6}, {2, 7}, {1, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}
#define MONO_AMPERSAND (Point2[]){{1, 3}, {2, 3}, {0, 4}, {3, 4}, {0, 5}, {3, 5}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {0, 7}, {3, 7}, {0, 8}, {3, 8}, {1, 9}, {2, 9}, {4, 9}}
#define MONO_ANGLE_BRACKET_LEFT (Point2[]){{3, 4}, {4, 4}, {1, 5}, {2, 5}, {0, 6}, {1, 7}, {2, 7}, {3, 8}, {4, 8}}
#define MONO_ANGLE_BRACKET_RIGHT (Point2[]){{0, 4}, {1, 4}, {2, 5}, {3, 5}, {4, 6}, {2, 7}, {3, 7}, {0, 8}, {1, 8}}
#define MONO_APOSTROPHE (Point2[]){{2, 3}, {2, 4}, {2, 5}}
#define MONO_ASTERISK (Point2[]){{2, 4}, {0, 5}, {2, 5}, {4, 5}, {1, 6}, {2, 6}, {3, 6}, {0, 7}, {2, 7}, {4, 7}, {2, 8}}
#define MONO_AT_SYMBOL (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {3, 4}, {4, 4}, {0, 5}, {2, 5}, {4, 5}, {0, 6}, {2, 6}, {4, 6}, {0, 7}, {3, 7}, {4, 7}, {0, 8}, {1, 9}, {2, 9}, {3, 9}}
#define MONO_BACKSLASH (Point2[]){{0, 3}, {0, 4}, {1, 5}, {2, 6}, {3, 7}, {4, 8}, {4, 9}}
#define MONO_CARAT (Point2[]){{2, 3}, {1, 4}, {3, 4}, {0, 5}, {4, 5}}
#define MONO_COLON (Point2[]){{2, 4}, {2, 5}, {2, 8}, {2, 9}}
#define MONO_COMMA (Point2[]){{2, 8}, {2, 9}, {1, 10}}
#define MONO_CURLY_BRACKET_LEFT (Point2[]){{3, 3}, {2, 4}, {2, 5}, {1, 6}, {2, 7}, {2, 8}, {3, 9}}
#define MONO_CURLY_BRACKET_RIGHT (Point2[]){{1, 3}, {2, 4}, {2, 5}, {3, 6}, {2, 7}, {2, 8}, {1, 9}}
#define MONO_DOLLAR_SIGN (Point2[]){{2, 3}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {0, 5}, {2, 5}, {1, 6}, {2, 6}, {3, 6}, {2, 7}, {4, 7}, {0, 8}, {1, 8}, {2, 8}, {3, 8}, {2, 9}}
#define MONO_EQUALS (Point2[]){{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}}
#define MONO_EXCLAMATION_POINT (Point2[]){{2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 9}}
#define MONO_FORWARD_SLASH (Point2[]){{4, 3}, {4, 4}, {3, 5}, {2, 6}, {1, 7}, {0, 8}, {0, 9}}
#define MONO_HYPHEN (Point2[]){{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}}
#define MONO_PARENTHESIS_LEFT (Point2[]){{3, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {3, 9}}
#define MONO_PARENTHESIS_RIGHT (Point2[]){{1, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {1, 9}}
#define MONO_PERCENTAGE (Point2[]){{0, 3}, {4, 3}, {0, 4}, {4, 4}, {3, 5}, {2, 6}, {1, 7}, {0, 8}, {4, 8}, {0, 9}, {4, 9}}
#define MONO_PERIOD (Point2[]){{2, 8}, {2, 9}}
#define MONO_PIPE (Point2[]){{2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 9}}
#define MONO_PLUS (Point2[]){{2, 4}, {2, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {2, 7}, {2, 8}}
#define MONO_POUND_SIGN (Point2[]){{1, 4}, {3, 4}, {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {1, 6}, {3, 6}, {1, 7}, {3, 7}, {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {1, 9}, {3, 9}}
#define MONO_QUESTION_MARK (Point2[]){{1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {4, 5}, {3, 6}, {2, 7}, {2, 9}}
#define MONO_QUOTATION_MARK (Point2[]){{1, 3}, {3, 3}, {1, 4}, {3, 4}, {1, 5}, {3, 5}}
#define MONO_SEMICOLON (Point2[]){{2, 4}, {2, 5}, {2, 8}, {2, 9}, {1, 10}}
#define MONO_SQUARE_BRACKET_LEFT (Point2[]){{2, 3}, {3, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 9}, {3, 9}}
#define MONO_SQUARE_BRACKET_RIGHT (Point2[]){{1, 3}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {1, 9}, {2, 9}}
#define MONO_TILDE (Point2[]){{1, 5}, {4, 5}, {0, 6}, {2, 6}, {3, 6}}
#define MONO_UNDERSCORE (Point2[]){{0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}}

////////// CLEANUP //////////

#ifdef USE_ALEXANDRIA_NAMESPACE
}
#endif

#endif