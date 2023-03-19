# PNG Tools

A list of useful tools for reading, creating and modifying PNG files.
This may be helpful for creating test images, or for analyzing files.

## PNG manipulation tools (read/write)

### SNG

[Readme](https://github.com/axot/SNG#readme) and maintained at [axot/SNG](https://github.com/axot/SNG)

This is a textual format to represent PNG files, which can be edited and then converted back to PNG.
Ancient code, depends on libpng so no support for APNG; and eXIf also is not supported by SNG, despite libpng support.

### hdr4png

[Readme](https://github.com/sandflow/hdr4png/blob/master/README.md) and maintained at [sandflow/hdr4png](https://github.com/sandflow/hdr4png)

This is a Python script to add a specific `iCCP` chunk to PNG.

## Checkers (read)

### pngcheck

[Homepage](http://www.libpng.org/pub/png/apps/pngcheck.html), no repo, [source tarball](http://www.libpng.org/pub/png/src/pngcheck-3.0.3.tar.gz).

Depends on libpng so currently no APNG support. Older versions bundled with some Linux distros have no eXIf support.

### PNG file chunk inspector

[Homepage](https://www.nayuki.io/page/png-file-chunk-inspector)

Browser-based checker written in Javascript. No libpng dependency.

Chris Lilley has experimentally extended it to add cICP, APNG support (easy since it is all JavaScript).

## Libraries

### libpng

Maintained at [pnggroup/libpng](https://github.com/pnggroup/libpng/),
although most of the issues are at [the older GlennRP repo](https://github.com/glennrp/libpng)
([issues](https://github.com/glennrp/libpng/issues)).

This is the original, C, reference library. Depends on zlib. It supports reading and writing PNG files. It has been criticized for being hard to use, and difficult to extend.

Does not support APNG, or any of the Third Edition chunks

### libspng

[Homepage](https://libspng.org/) and maintained at [randy408/libspng](https://github.com/randy408/libspng).

A faster C library for reading and writing PNG. Does not depend on zlib.

Does not support APNG (although that is [planned](https://github.com/randy408/libspng/issues/4)), or any of the Third Edition chunks,
although there is an open issue on [cICP support](https://github.com/randy408/libspng/issues/218).

### PNG-library

[Homepage](https://www.nayuki.io/page/png-library) and maintained at [nayuki/PNG-library](https://github.com/nayuki/PNG-library)

This is a recent Java library for read and write.

### Sixlegs Java PNG Decoder

[Homepage](https://code.google.com/archive/p/javapng/) and maintained [on Google code](https://code.google.com/archive/p/javapng/source).

Seems to support reading only.

