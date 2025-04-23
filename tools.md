# PNG Tools

A list of useful tools for reading, creating and modifying PNG files.
This may be helpful for creating test images, or for analyzing files.

## PNG manipulation tools (read/write)

### Photomator

[Homepage](https://apps.apple.com/us/app/photomator-photo-editor/id1444636541) Mac application which can read and generate HDR PNG.

### pngcrush

[Homepage](https://pmt.sourceforge.io/pngcrush/) and maintained [on sourceforge](https://sourceforge.net/projects/pmt/files/pngcrush/).

Used to recompress PNG, optionally remove chunks or add a few such as sRGB.

Depends on libpng. Author, Glenn Randers-Pehrson, is deceased, last update was 2017.

### optipng

[Homepage](https://sourceforge.net/projects/optipng/) and maintained [on sourceforge](https://sourceforge.net/projects/optipng/files/)

Used to recompress PNG, including optionally reducing bit depth, changing color type, and reducing the palette.

Depends on libpng. Originally a fork of pngcrush by Cosmin Truta, now has a different feature set and has added GIF and BMP support.

### OxiPNG

[Homepage](https://github.com/shssoichiro/oxipng/blob/master/README.md) and maintained on [GitHub](https://github.com/shssoichiro/oxipng)

PNG compression optimizer, written in Rust. [Recognizes `cICP` chunks and does not discard them](https://github.com/shssoichiro/oxipng/issues/538#issuecomment-1644266228).

### png_cicp_editor

[Homepage](https://github.com/ProgramMax/png_cicp_editor/blob/tip-of-tree/Docs/README.md) and [maintained on GitHub](https://github.com/ProgramMax/png_cicp_editor)

Command-line tool in C++ to insert (and optionally, overwrite) a `cICP` chunk in an existing PNG file. Maintained by Chris Blume.

### png-itxt

[Homepage](https://github.com/finnp/png-itxt/blob/master/readme.md) and [maintained on GitHub](https://github.com/finnp/png-itxt).

Tool to read textual information in `tEXT`, `zTXT` and `iTXT` chunks, and to write `iTXT` chunks. Use in Node programs, or on command line.

### Animated PNG Maker

[Homepage](https://ezgif.com/apng-maker/sort).

Online tool to convert one or more GIF, JPG, PNG, APNG, HEIC, HEIF, MNG, JP2, AVIF, JXL, BMP, PDF, WebP to APNG. Based on [APNG Assembler](https://apngasm.sourceforge.net/)

### Minimum viable PNG encoder, in Python

[Homepage](https://www.da.vidbuchanan.co.uk/blog/hello-png.html) and maintained [on GitHub](https://github.com/DavidBuchanan314/hello_png).

By @DavidBuchanan314  Easily extensible to spit out new chunks, so great for prototyping and creating test files.

Depnds on zlib, only.

### node pngdefry

[Homepage](https://www.npmjs.com/package/pngdefry)

Repairs corruption in undisplayable Apple PNG icons which were extracted from iOS apps; removes CgBI chunk, converts premultiplied samples back to RGB, etc. Uses node.

### SNG

[Readme](https://github.com/axot/SNG#readme) and maintained at [axot/SNG](https://github.com/axot/SNG)

This is a textual format to represent PNG files, which can be edited and then converted back to PNG.
Ancient code, depends on libpng so no support for APNG; and eXIf also is not supported by SNG, despite libpng support.

### hdr4png

[Readme](https://github.com/sandflow/hdr4png/blob/master/README.md) and maintained at [sandflow/hdr4png](https://github.com/sandflow/hdr4png)

This is a Python script by Pierre-Anthony Lemieux to add a specific `iCCP` chunk to PNG to signal BT.2100 PQ usage. Now historical, use `cICP` instead.

## Checkers and inspectors (read)

### pngcheck

[Homepage](https://github.com/pnggroup/pngcheck/blob/main/README.md), and maintained [on GitHub](https://github.com/pnggroup/pngcheck).

Depends on libpng. Has APNG support. Checks `cICP`, `mDCV` and `cLLI`. Reports if `eXIf` is found after `IDAT`.

Older versions bundled with some Linux distros have no `eXIf`, APNG, `cICP`, `mDCV` or `cLLI` support.

### PNG file chunk inspector

[Homepage](https://www.nayuki.io/page/png-file-chunk-inspector) and source at [nayuki/
Nayuki-web-published-code/png-file-chunk-inspector/](https://github.com/nayuki/Nayuki-web-published-code/tree/master/png-file-chunk-inspector)

Browser-based checker written in Javascript. No libpng dependency. Now supports APNG. Does not yet support other Third Edition chunks.

Chris Lilley had experimentally extended it to add `cICP`, APNG support (easy since it is all JavaScript).

### fotoforensics

[Homepage](https://fotoforensics.com/)

Browser-based tool which displays hidden (by alpha channel, or by incorrect `IHDR` height) data in PNG (and other) images.

### MediaInfo

[Homepage](https://mediaarea.net/en/MediaInfo)

Multi-platform GUI which displays metadata on many image and video formats including PNG. [Recent builds](https://mediaarea.net/download/snapshots/binary/mediainfo-gui/20240106-2/) support `cICP`, `mDCV` and `cLLI`.

### PNG Chunk Extraction

[Homepage](https://www.dcode.fr/png-chunks).

Online tool to extract contents of `tEXT`, `zTXT` and `iTXT` chunks.

## Libraries

### libpng

Maintained at [pnggroup/libpng](https://github.com/pnggroup/libpng/),
although most of the issues are at [the older GlennRP repo](https://github.com/glennrp/libpng)
([issues](https://github.com/glennrp/libpng/issues)).

This is the original, C, reference library. Depends on zlib. It supports reading and writing PNG files. It has been criticized for being hard to use, and difficult to extend.

Does not support APNG. Version 1.6.45 added `cICP` support, and `mDCV` and `cLLI` from Third Edition were added in 1.6.47.

### libspng

[Homepage](https://libspng.org/) and maintained at [randy408/libspng](https://github.com/randy408/libspng). There is [documentation](https://libspng.org/docs/api/)

A faster C library for reading and writing PNG. Does not depend on zlib (or libpng).

Does not support APNG (although that is [planned](https://github.com/randy408/libspng/issues/4)) and there is an [open PR](https://github.com/randy408/libspng/pull/252) which adds APNG support, or any of the Third Edition chunks,
although there is an open issue on [cICP support](https://github.com/randy408/libspng/issues/218).

### image-png

Maintained at [image-rs/image-png](https://github.com/image-rs/image-png). There is [documentation](https://docs.rs/png/latest/png/).

Pure Rust library with a PNG encoder and decoder. Supports APNG. Supports Third Edition chunks `cICP`, `mDCV` and `cLLI`.

### lodepng

[Homepage](https://lodev.org/lodepng/) and [maintained on GitHub](https://github.com/lvandeve/lodepng)

C library and C++ wrapper by Lode Vandevenne.

### pnger

[Readme](https://github.com/masq/pnger#readme) and maintained at [masq/pnger](https://github.com/masq/pnger#readme)

Python library for parsing PNG, mainly for forensic purposes.

Now supports APNG and Third Edition chunks.

### PNG-library

[Homepage](https://www.nayuki.io/page/png-library) and maintained at [nayuki/PNG-library](https://github.com/nayuki/PNG-library)

This is a recent Java library for read and write.

### Sixlegs Java PNG Decoder

[Homepage](https://code.google.com/archive/p/javapng/) and maintained [on Google code](https://code.google.com/archive/p/javapng/source).

Seems to support reading only.

