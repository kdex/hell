# hell
Set of compression and decompression tools for HAL Laboratory games.
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
   5   | LZ77 for exact matches in reversed order
   6   | LZ77 for bit-reversed matches
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
## Development
This project uses the Meson build system.
### How to build
In the project root, run:
```bash
$ mkdir build
$ cd $_
$ meson .. && ninja
```
This will build the following assets for you:
- `hell` binary
- a static (.a) and shared (.so) library
- headers to include into your projects
- a package configuration for `pkgconfig`

### Installation
Installing `hell` will make the aforementioned assets globally available. In the build directory, run:
```bash
$ ninja install
```
### Uninstallation
If you don't need `hell` anymore, you can remove all the aforementioned assets like so:
```bash
$ ninja uninstall
```
### Testing
In the build directory, you can run the unit tests via:
```bash
$ ninja test
```