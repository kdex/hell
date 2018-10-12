<p align="center">
	<img width="300" src="https://raw.githubusercontent.com/kdex/hell/master/hell.svg?sanitize=true">
</p>

# hell
[![Build status](https://travis-ci.org/kdex/hell.svg?branch=master)](https://travis-ci.org/kdex/hell)
[![Build status](https://ci.appveyor.com/api/projects/status/l09ktnael2mpgp53/branch/master?svg=true)](https://ci.appveyor.com/project/kdex/hell/branch/master)

A cross-platform set of (de)compression tools for HAL Laboratory games.
## Compression
In order to compress the file `INPUT` and place the result at `OUTPUT`, you can use the `compress` subcommand:
```bash
$ hell compress INPUT OUTPUT
```
## Decompression
Analogously, in order to decompress the file `INPUT` and place the result at `OUTPUT`, you can use the `decompress` subcommand:
```bash
$ hell decompress INPUT OUTPUT
```
## About the format
### Compression modes
HAL Laboratory's lossless compression format is the result of mixing different compression modes. It uses:

 Mode  | Description
-------|-------------
   0   | Uncompressed
   1   | Run-length-encoded (repeating byte)
   2   | Run-length-encoded (repeating byte pair)
   3   | Run-length-encoded (incrementally growing byte)
   4   | LZ77 for exact matches
   5   | LZ77 for bit-reversed matches
   6   | LZ77 for exact matches in reversed order
   7   | Meta mode, indicates large header
### Chunks
The input bytes are compressed into chunks; one compression mode will result in one compressed chunk, which consists of a header and a payload. After the chunk stream, there will be a compression terminator symbol (`0xff`) denoting that no more chunks will follow. It should be noted that this terminator **is** allowed as part of a chunk's payload.
### Headers
The header is used to store information about the chunk that is needed to decompress it. This entails the compression mode and the uncompressed length.

Further, there are two different header formats that one needs to distinguish.
#### Small headers
Small headers consist of a single byte.
##### Memory layout
```
MMMIIIII

  MMM = Compression mode
IIIII = Maximum index of compressed data
```
As becomes clear with the memory layout, small headers can carry maximum payloads of 2⁵ B = 32 B, since there are only five bits that can be used to denote the maximum index (which implies the uncompressed length).
#### Large headers
Large headers use two bytes.
##### Memory layout
```
111MMMJJ IIIIIIII

  111 = Constant 1
  MMM = Compression mode
   JJ = Maximum index of compressed data (upper 2 bits)
IIIII = Maximum index of compressed data (lower 8 bits)
```
Since we have ten bits available to index our data, large headers must be used for up to 2¹⁰ B = 1 KiB.

The constant `111` is necessary, since the decompressor can't know up front how large a header is. Therefore, it looks only at the first three bits of a chunk to determine the mode and will see it as mode `0b111 == 7`. Mode 7 should hence be considered a special mode that instructs the decompressor to also consider the upcoming byte as part of the header.
### Limitations
Modes 4, 5 and 6 are closely related to Lempel-Ziv compression. However, HAL Laboratory chose to deviate from LZ77 and **did not** store offsets relatively to the current look-ahead buffer position; instead, they chose to store absolute offsets. Since these modes use two bytes to store the offset, LZ compression can only work if the offset is in `[0, 2¹⁶ - 1 = 65535]`.

Effectively, this limits the input file size to **2¹⁶ B = 64 KiB**.

This also implies that the "sliding window" you may know from LZ77 can have a maximum size of `2¹⁶ B = 64 KiB`, except that this "sliding window" *does not* actually slide, since its size will always cover the entire input file.
## Development
This project uses the CMake build system.
### How to build
In the project root, run:
```bash
$ mkdir build
$ cd $_
$ cmake .. && make -j
```
This will build the following assets for you:
- `hell` binary
- `libhell` (as a static and a shared library)
- headers for your projects

### Installation
Installing `hell` will make the aforementioned assets globally available. In the build directory, run:
```bash
# make -j install
```
If you would like the assets to be installed in a local directory, you should instead invoke `cmake` like so:
```bash
$ cmake -DCMAKE_INSTALL_PREFIX=YOUR_PREFIX_HERE ..
$ make -j install
```
### Testing
In the build directory, you can run the unit tests via:
```bash
$ make -j test
```