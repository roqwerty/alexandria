# Alexandria
A collection and standardization of utilities that I often re-implement.

Important Functionality:
- Vector3 implementation with operator addition, subtraction, and scalar multiplication, as well as magnitude(), normalize(), cross(), dot(), and angle() functions.
- Standardization of RGB Color, RGBA ColorAlpha, and HSV ColorHSV structures.
- Color manipulation functionality, such as linear interpolation, fast RGB <-> HSV conversion, color-as-data native vector memory storage, and saving to ARGB bitmap.
- Base64 encoding and decoding using native C++ strings.
- Base-independent number digit indexing.
- Multidimensional array index collapse helper functions.
- General-purpose simple structure serial saving/loading, as well as native vector-of-things support.
- Many, many easing functions, as well as a Tween helper class to make use of them easily.
- Really, *really* fast random boolean generator.

For more detail, as well as a current list of included structs, functions, and classes, see the comment at the top of `alexandria.h`.
