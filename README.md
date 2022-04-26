# Alexandria
A collection and standardization of utilities that I often re-implement.

Important Functionality:
- Vector3 implementation with operator addition, subtraction, and scalar multiplication, as well as magnitude(), normalize(), cross(), dot(), and angle() functions. additionally, some basic matrix integration through 2D float vectors.
- Standardization of RGB Color, RGBA ColorAlpha, and HSV ColorHSV structures.
- Color manipulation functionality, such as linear interpolation, fast RGB <-> HSV conversion, color-as-data native vector memory storage, and saving to ARGB bitmap.
- Base64 encoding and decoding using native C++ strings.
- Base-independent number digit indexing.
- Multidimensional array index collapse helper functions.
- General-purpose simple structure serial saving/loading, as well as native vector-of-things support.
- Many, many easing functions, as well as a Tween helper class to make use of them as a near-native data structure.
- Simple loading bar streaming function with modular support for titles, progress bars, and (completed/total) counts.
- Really, *really* fast random boolean generator.

For more detail, as well as a current list of included structs, functions, and classes, see the comment at the top of `alexandria.h`.

# Licensing
Code in this file was acquired from a wide variety of sources in addition to being hand-made myself. If I added a section from somwhere, the source is mentioned in a comment above their section. If there is no explicit source, assume that I wrote it and released it freely under the [unlicense](https://unlicense.org/).
