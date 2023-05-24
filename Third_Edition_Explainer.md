# PNG Third Edition, Explained

## Authors:

- Chris Lilley

## Participate

- [Issue tracker](https://github.com/w3c/PNG-spec/issues)

## Table of Contents [if the explainer is longer than one printed page]

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [Introduction](#introduction)
- [Motivating Use Cases](#motivating-use-cases)
- [Non-goals](#non-goals)
- [User research](#user-research)
  - [MNG and Animated PNG](#mng-and-animated-png)
- [APNG](#apng)
- [HDR support](#hdr-support)
  - [Labelling HDR content](#labelling-hdr-content)
  - [Mastering Color Volume](#mastering-color-volume)
  - [Content Luminance Levels](#content-luminance-levels)
- [Stakeholder Feedback / Opposition](#stakeholder-feedback--opposition)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Introduction

PNG is a very widely deployed, lossless, raster image format
for RGB and grayscale images,
with or without an alpha channel.

The [First Edition](https://www.w3.org/TR/REC-png-961001) was published as W3C Recommendation in 1996
and the [Second Edition](https://www.w3.org/TR/2003/REC-PNG-20031110/) in 2003,
also published as ISO/IEC 15948:2003.

This explainer covers solely changes for the 
[Third Edition](https://www.w3.org/TR/png-3/),
currently under development. The Third Edition adds widely-requested features
and folds in errata reported since the
Second Edition.

## Motivating Use Cases

For **short animations** currently better handled by Animated GIF than by video files,
there is a need for an animation format which avoids the **limitations of GIF**
(256 colors, only on-off transparency)
and draws on the **strengths of PNG**
(24 or 48bit truecolor, full alpha transparency
for halo-less display over any background).

![rotating GIF cube](./img/badcube.gif) ![rotating APNG cube](./img/APNG-cube.png)

PNG Second Edition was limited to
Standard Dynamic Range (SDR) imagery.
With the growing use of **High Dynamic Range (HDR)**
there was a need to extend PNG to support HDR images
during **content creation workflow**
for **display** on HDR or SDR displays.

## Non-goals

**Backwards compatibility** with the enormous installed base of PNG images
is considered paramount,
so non-backwards-compatible changes
such as a better compression method,
are [out of scope in the charter](https://www.w3.org/Graphics/PNG/png-2021.html#section-out-of-scope).

## User research

### MNG and Animated PNG

Between 1999 and 2001, a fully-featured animated format called
Multiple-image Network Graphics ([MNG](http://www.libpng.org/pub/mng/spec/)) was developed.
It could contain animations, slide shows, or complex still frames,
comprised of multiple PNG (including delta images) or JNG (JPEG in a PNG-like container) 
single-image datastreams.

This was briefly implemented in Firefox, but was then 
[removed in 2004](https://bugzilla.mozilla.org/show_bug.cgi?id=195280) 
due to complexity,
lack of use,
and lack of maintenance for the library:

> (The) mng decoder module is roughly the same size as _all the other image decoders_ and libpr0n logic combined.

During 2004 and 2005, Vladimir Vukicevic and Stuart Parmenter (then both with Mozilla)
created and refined **Animated PNG (APNG)**, an addition to PNG (rather than an entirely separate format, like MNG)
which combined the existing benefits of PNG
and the limited, but demonstrably sufficient,
animation features of GIF.
**Fallback** to a non-animated PNG 
(which could be, but need not be, the first frame of the animation)
was a feature.

Firefox 3 added APNG support in 2007.

The PNG development group, after much discussion, **rejected APNG in 2007**,
mostly on the grounds that it was not MNG and that PNG should remain a static format.
Two attempts to restart the discussion in the following years failed.

APNG was documented [on the Mozilla wiki](https://wiki.mozilla.org/APNG_Specification)
and browsers and other tools added a variety of patches to their own versions of libpng,
to support APNG.

APNG started to be [**added to other platforms**](https://caniuse.com/apng):

- Opera 9 in 2009
- Safari 8 in 2014
- iOS in 2016
- [Chrome 59](https://chromestatus.com/feature/6691520493125632) in 2017
- Edge 79 in 2019

Authoring support for APNG also improved over time.

## APNG

The PNG working group was chartered in 2021
to maintain and extend the PNG specification;
in particular, to add the widely adopted APNG extensions to the **core PNG specification**.
This has now been done; specification links:

 - General introduction [APNG: frame-based animation](https://www.w3.org/TR/png-3/#apng-frame-based-animation)
    - ['acTL' Animation Control Chunk](https://www.w3.org/TR/png-3/#acTL-chunk)
    - ['fcTL' Frame Control Chunk](https://www.w3.org/TR/png-3/#fcTL-chunk)
    - ['fdAT' Frame Data Chunk](https://www.w3.org/TR/png-3/#fdAT-chunk)

These are precisely the same as the Mozilla documentation,
bringing the PNG specification into line with a
decade of deployed content and implementations.

## HDR support

### Labelling HDR content

For SDR content, the color space is typically labelled
using an **ICC profile** in the ['iCCP' chunk](https://w3c.github.io/PNG-spec/#11iCCP)
or, for sRGB content, with the ['sRGB' chunk](https://w3c.github.io/PNG-spec/#srgb-standard-colour-space).
An ICC profile for an RGB color space typically adds 1k to 10k to the filesize,
while the content of the sRGB chunk is 1 byte.

ICC profiles are rarely used to label HDR content.
Instead, an ITU specification called
**Coding Independent Code Points (CICP)**
is used in HDR video workflows
and also for static image formats such as AVIF or JPEG-XL.

PNG has aligned with this growing industry usage,
adding the ['cICP' chunk](https://w3c.github.io/PNG-spec/#cICP-chunk)
which enables CICP labelling of the two most common HDR color spaces
(**BT.2100 HLG** and **BT.2100 PQ**) with only four bytes.

As a bonus, it can also be used to compactly label
some common Wide Gamut SDR color spaces,
such as **Display P3** and **BT.2020**.

_Note:_ The third byte of 'cICP' always has the value '1',
because PNG stores only RGB format images, not YCbCr or ICtCp.
Explicit storage of this value was retained 
for compatibility with other video and image formats,
and to allow for potential future expansion.

### Mastering Color Volume

Video and image content is typically finalized
on some reference display, called the _mastering display_.
The content will not contain color values
that cannot be reproduced on the mastering display,
which sets an upper bound on the range of colors
in the image.

For example, content transmitted in the BT.2100 PQ colorspace,
which has a theoretical peak luminance of 10,000 cd/m²,
if often mastered on a display
whose color primaries are similar to Display P3,
with a maximum peak luminance of 2,000 cd/m²
and a minimum black luminance of 0.02 cd/m.

The color volumes of BT.2100, Display P3 and sRGB
are illustrated below,
projected onto the _a,b_ plane of the Oklab colorspace.

![3 gamuts](./img/3gamuts-oklab.svg)

When displaying the content to the end user, on some other display,
this mastering color volume information can be used for
**gamut mapping** (saturation reduction of un-displayable colors)
and **tone mapping**
(compression of luminance to the darkest and brightest values supported by a display).

It is more efficient to provide this information
than to expect the final display system to compute it on the fly,
especially for an image sequence,
where analysis of all frames would be required
to display the first frame,
to present the entire sequence in a consistent way.

PNG stores this optional information in the
same order, and using the same encoding, as
SMPTE ST 2086, a standard which is widely used
in the streaming, broadcast and TV industries.

This is expected to ease deployment
by fitting smoothly into existing industry workflows.

The content of the 'mDCv' chunk is only 24 bytes.

Specification link:

- ['mDCv' Mastering Display Color Volume](https://w3c.github.io/PNG-spec/#mDCv-chunk)


### Content Luminance Levels


## Stakeholder Feedback / Opposition

[Implementors and other stakeholders may already have publicly stated positions on this work. If you can, list them here with links to evidence as appropriate.]

- [Implementor A] : Positive
- [Stakeholder B] : No signals
- [Implementor C] : Negative

## References

[ITU-R BT.2100]
    ITU-R BT.2100, SERIES BT: BROADCASTING SERVICE (TELEVISION). _Image parameter values for high dynamic range television for use in production and international programme exchange._ ITU. 2018-07. URL: https://www.itu.int/rec/R-REC-BT.2100

[SMPTE ST 2086]
    _Mastering Display Color Volume Metadata Supporting High Luminance and Wide Color Gamut Images_. Society of Motion Picture and Television Engineers. 27 April 2018. URL: https://ieeexplore.ieee.org/document/8353899
