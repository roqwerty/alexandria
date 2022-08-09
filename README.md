# Alexandria

![Library](library.png?raw=true "Alexandria Library")

A collection and standardization of utilities that I often re-implement.

## Important Functionality
- Embeddable and flexible unit testing framework with native support for verbosity (including automatic file:line:function position) and program-wide summary.
- Defines for colored terminal output in bash-based terminals and streams.
- Debug macros, such as info/log/warning/error printing (only if DEBUG is defined), printing variable data (code name, memory address, type, and value), in-code location (file:line), and compile time as native C++ data structures.
- Timing macros, able to print ms-level accuracy for execution times of code sections.
- Point2 and Point3 structures for coordinates.
- Ability to iterate over points within predefined lists, such as the points of characters in the Monogram font.
- Optional `Alexandria::` namespace encapsulation.
- Vector3 implementation with operator addition, subtraction, and scalar multiplication/division, as well as magnitude(), normalize(), cross(), dot(), and angle() functions. additionally, some basic matrix integration through 2D float vectors.
- Standardization of RGB Color, RGBA ColorAlpha, and HSV ColorHSV structures.
- Color manipulation functionality, such as linear interpolation, fast RGB <-> HSV conversion, color-as-data native vector memory storage, and saving to ARGB bitmap.
- Base64 encoding and decoding using native C++ strings.
- Base-independent number digit indexing.
- Multidimensional array index collapse helper functions.
- General-purpose simple structure serial saving/loading, as well as native vector-of-things support.
- Many, many easing functions, as well as a Tween helper class to make use of them as a near-native data structure.
- Simple loading bar streaming function with modular support for titles, progress bars, and (completed/total) counts.
- Generalized extraction of value vectors and maps from C++ strings (with support for JSON lists and maps, CSV/TSV files, and .ini files, as well as many others through use of custom delimiters and ignored characters).
- Ability to save a string as a PDF file with semi-intelligent word wrapping and page breaking.
- A Linux-like diff function for finding the minimum amount of different lines between two strings.
- A wrapper for `std::vector` that makes it behave as a circular buffer data structure.
- A wrapper for `std::vector` that makes it behave like a pythonic vector with support for slicing and negative indexes.
- Really, *really* fast random boolean generator.

For more detail, as well as a current list of included structs, functions, and classes, see the massive comment/documentation at the top of `alexandria.h`.

## Implementation Detail and Examples

### Unit Testing
```c++
// Note that this can be added to existing code structure,
//  shown in main() for ease of explanation
int main(int argc, char *argv[]) {
    int x = 4;
    int y = 5;
    int z = 5;

    // Evaluates an expression and will pass/fail a test accordingly
    TEST(x < y);

    // Also assigns this test a name for later reference alongside position
    TEST_NAMED("test two", y <= z);

    // Provides detailed information on a failed test, like variable name and value
    TEST_EQ(y, z);

    // Provides detailed information on fail and assigns a test name for later
    TEST_NAMED_EQ("1D equality", y, z);

    float fx = 1.000000000000;
    float fy = 1.000000000001;

    // Compares two values within an error margin
    TEST_EPSILON_EQ(fx, fy, 0.00001);

    // Any tests performed when this is true won't print any output, but WILL still
    //  be outlined by the summary macro below
    TESTS_SILENT = true;

    TEST(y > x);
    TEST(y > z);
    TEST(!(x == z));

    // Finally, prints a pretty summary of all tests including overall counts,
    //  percentage of tests passed, and a list of position/names of failed tests
    TEST_SUMMARY();
}
```

### Intelligent Code Metadata
```c++
double variable_name = 4.2;

// Prints "variable_name @ 0x67ea68 (d) = 4.2"
INFO(variable_name);

struct MyStruct {
    int index;
    std::string data;
};
MyStruct my_obj(3, "hi");

// Prints "my_obj @ 0x67ea5c (8MyStruct)"
INFO_NOVALUE(my_obj);

// Defining a stream operator allows printing of a structure's value
std::ostream& operator << (std::ostream& out, const MyStruct& obj) {
    out << obj.index << ": " << obj.data;
    return out;
}

// Prints "my_obj @ 0x67ea5c (8MyStruct) = 3: hi"
INFO(my_obj);

// Prints "4 (i)"
INFO_BASIC(4);

// Capture and print the location of this line (file:line:function) in the codebase
//  and the date of last compilation
std::string location = LOCATION;
std::string build_time = COMPILE_TIME;
std::cout << location << " in build " << build_time << std::endl;
```

### Vector File Saving/Loading
```c++
// Create some data
std::vector<char> data = {'H', 'i', ' ', ':', ')'};

// Write data vector to file
std::ofstream f("vec.data");
if (f.good()) {
    write_pod_vector<char>(f, data);
    f.close();
}

// Read vector from file
std::vector<char> read;
std::ifstream g("vec.data");
if (g.good()) {
    read_pod_vector<char>(g, read);
    g.close();
}

// Print results
for (char a : read) {
    std::cout << a << std::endl;
}
```

### Colored Terminal Output
Note that this only works for terminals that support ANSI escape codes ( e.g. bash, NOT Windows :< ).

Note also that unit testing uses these colors by default. To disable this, define `USE_ALEXANDRIA_COLORLESS`.

```c++
std::cout << TB_YELLOW << "text with a yellow background" << T_CYAN << " and cyan text" << T_RESET << std::endl;
```

### Color Manipulation and Image Saving
```c++
// Color definition
Color white = {255, 255, 255};
Color black = {0, 0, 0};

// Set a gray value equal to 35% of the way from white to black
Color gray = linear_color(0.35, white, black);

// Convert RGB <-> HSV. Fast, not perfectly accurate
ColorHSV grayHSV = rgb_to_hsv(gray);
Color gray2 = hsv_to_rgb(grayHSV); // This will be close, not exactly identical

// Heatmap coloring for values in range [0, 1]
Color cold_heat = heatmap(0.1);
Color avg_heat = heatmap(0.5);
Color hot_heat = heatmap(0.9);

// Image dimensions
int width = 25;
int height = 25;

// This returns a blank image 2D vector of pixels
std::vector<std::vector<ColorAlpha>> pixels = make_image_array(width, height);

// Set some color in the corners
pixels[0][0] = {255, 0, 0, 255}; // Origin is red
pixels[width-1][0] = {0, 255, 0, 255}; // max x is green
pixels[width-1][height-1] = {255, 255, 0, 255}; // Max in both directions is yellow
pixels[0][height-1] = {0, 0, 255, 255}; // Max in y is blue

// Save with origin at bottom left (last val = false, else top right)
save_bmp("example.bmp", pixels, false);
```

### Iteration over Points of a Character
```c++
// Setup image array
int width = 29; // Characters are 5 wide, plus 1 empty pixel between
int height = 12; // Characters are 12 high
std::vector<std::vector<ColorAlpha>> pixels = make_image_array(width, height);

// Write "Hi :)" in black
int offset = 0;
for (const Point2& p : MONO_H) {
    pixels[offset + p.x][p.y] = {0, 0, 0, 255};
}
offset += 6;
for (const Point2& p : MONO_i) {
    pixels[offset + p.x][p.y] = {0, 0, 0, 255};
}
offset += 6; // Do nothing for space
offset += 6;
for (const Point2& p : MONO_COLON) {
    pixels[offset + p.x][p.y] = {0, 0, 0, 255};
}
offset += 6;
for (const Point2& p : MONO_PARENTHESIS_RIGHT) {
    pixels[offset + p.x][p.y] = {0, 0, 0, 255};
}

// Save the image, note origin at top-left
save_bmp("banner.bmp", pixels, true);
```

### Loading Bars
```c++
for (int i = 0; i < 10000; i++) {
    // Output stream, percent, title, width, complete_count, total_count
    //  All but stream and percent are optional
    stream_loading_bar(std::cout, i/9999.0, "Done", 25, i, 9999);
}
```

### Timing Code
```c++
TIME(
    std::cout << "This section of code will be timed" << std::endl;
    std::cout << "and the time (ms) will be printed" << std::endl;
    std::cout << "as well as the location of the block" << std::endl;
)
// Prints: Ended timed section @ file.cpp:20:main in 2ms

TIME_NAMED("Code Section #2",
    std::cout << "In addition to timing this code, the" << std::endl;
    std::cout << "title 'Code Section #2' will also print" << std::endl;
)
// Prints: Ended timed section "Code Section #2" @ file.cpp:26:main in 2ms
```

### Value Vector and Map Extraction from String
```c++
// Extracting string vector from JSON-styled input
std::string json_input = "[1, 2, 4, 5.0, 238, 109]\n";
for (std::string v : extract_vector(json_input)) {
    std::cout << v << std::endl;
}

// Vector extraction with custom delimeters and ignored chars
std::string custom_input = "a|b|c|cat|alligator;";
std::vector<std::string> vals = extract_vector(custom_input, '|', "; \n\t[](){}");

// Extracting map from .ini-style input with extra whitespace
std::string ini_input = "one = 1\n two = 2\n      three = 3";
for (auto kv : extract_map(ini_input)) {
    std::cout << "Key " << kv.first << " is " << kv.second << std::endl;
}
```

### Saving a string as a PDF file
```c++
// The function will automatically break long lines into multiple lines
//  and large collections of lines into separate pages
std::string contents = "";
for (int i = 0; i < 100; i++) {
    contents += "This is a test string.\n";
}

save_pdf("test.pdf", contents);
```

### Circular Buffers
```c++
Circle<std::string> cs;
cs.insert("One"); // Buffer from zero: ["One"]
cs.insert("Two"); // Buffer from zero: ["Two", "One"]
cs.insert("Three");  // Buffer from zero: ["Three", "Two", "One"]
cs++; // Buffer from zero: ["Two", "One", "Three"]

// This will print all elements of the buffer, in order, twice.
for (int i = 0; i < cs.size() * 2; i++) {
    std::cout << cs[i] << std::endl;
}
```

### Pythonic Vectors
```c++
PythonicVector<int> pv;
pv.push_back(1);
pv.push_back(2);
pv.push_back(3);
pv.push_back(4);
pv.push_back(5);

std::cout << "1: " << pv[1] << std::endl;   // 1: 2
std::cout << "-1: " << pv[-1] << std::endl; // -1: 5
std::cout << "4: " << pv[4] << std::endl;   // 4: 5
PRINT_VECTOR(pv)                            // pv = 1 2 3 4 5
std::cout << "slicing" << std::endl;
PRINT_VECTOR(pv(1, 4))                      // pv(1, 4) = 2 3 4
PRINT_VECTOR(pv(1, -1))                     // pv(1, -1) = 2 3 4
```

## Licensing
Code in this file was acquired from a wide variety of sources in addition to being hand-made myself. If I added a section from somwhere, the source is mentioned in a comment above their section. If there is no explicit source, assume that I wrote it and released it freely under the [unlicense](https://unlicense.org/).
