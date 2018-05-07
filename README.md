# hell
Set of compression and decompression tools for HAL Laboratory games.
## Decompression
In order to decompress the file `INPUT` and place the result at `OUTPUT`, you can use the `decompress` subcommand:
```bash
$ hell decompress INPUT OUTPUT
```
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