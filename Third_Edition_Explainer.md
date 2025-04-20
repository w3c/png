# PNG Third Edition, Explained

## Authors:

- Chris Lilley

## Participate

- [Issue tracker](https://github.com/w3c/PNG-spec/issues)

## Table of Contents

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
- [References](#references)

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

_Figure 1: Animated GIF (left) and animated truecolor PNG (right)_

PNG Second Edition was limited to
Standard Dynamic Range (SDR) imagery.
With the growing use of **High Dynamic Range (HDR)**
there was a need to extend PNG to support HDR images
during **content creation workflow**
for **display** on HDR or SDR displays.

This requirement was [incubated](https://github.com/w3c/ColorWeb-CG/blob/main/hdr-in-png-requirements.md)
in the Color on the Web CG.

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

The PNG development group, after much discussion, rejected the APNG proposal in 2007
on the grounds that PNG should remain a static format and that APNG, unlike MNG, did
not clearly identify itself as a separate format.

Two attempts to revisit the decision in the following years failed.

APNG is documented on the [Mozilla wiki](https://wiki.mozilla.org/APNG_Specification)
and some browsers and other tools added a variety of patches to their own versions of libpng
to support APNG.

The adoption of APNG is documented by [Alexis Deveria](http://a.deveria.com/) on the "can I use"
web page [Animated PNG (APNG)](https://caniuse.com/apng).

Some specific examples of the addition of support for APNG include:

- Opera 9 in 2009
- Safari 8 in 2014
- iOS in 2016
- [Chrome 59](https://chromestatus.com/feature/6691520493125632) in 2017
- Edge 79 in 2019

Authoring support for APNG also improved over time.

## APNG

The PNG working group was chartered in 2021 to maintain and extend the PNG specification specifically
to add the widely adopted APNG extensions to the core PNG specification.

This has now been achieved;

 - General introduction [APNG: frame-based animation](https://www.w3.org/TR/png-3/#apng-frame-based-animation)
    - ['acTL' Animation Control Chunk](https://www.w3.org/TR/png-3/#acTL-chunk)
    - ['fcTL' Frame Control Chunk](https://www.w3.org/TR/png-3/#fcTL-chunk)
    - ['fdAT' Frame Data Chunk](https://www.w3.org/TR/png-3/#fdAT-chunk)

These are identical to the existing the (Mozilla)[https://www.mozilla.org/] (documentation)[https://wiki.mozilla.org/APNG_Specification],
bringing the PNG specification into line with a
decade of deployed APNG content and implementations.

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

_Note:_ The third byte of 'cICP' always has the value '0',
because PNG stores only RGB format images, not YCbCr or ICtCp.
Explicit storage of this value was retained 
for compatibility with other video and image formats,
and to allow for potential future expansion.

### Mastering Color Volume

Video and image content is typically finalized
on some reference display, called the _mastering display_.
The content will not contain color values
that **cannot be reproduced** on the mastering display,
which sets an **upper bound** on the range of colors
in the image.

For example, content transmitted in the BT.2100 PQ colorspace,
which has a theoretical peak luminance of 10,000 cd/m²,
is often mastered on a display
whose color primaries are similar to Display P3,
with a maximum peak luminance of 2,000 cd/m²
and a minimum black luminance of 0.02 cd/m.

![3 gamuts](./img/3gamuts-oklab.svg)

_Figure 2: The color volumes of BT.2100, Display P3 and sRGB
projected onto the _a,b_ plane of the Oklab colorspace._

When displaying the content to the end user, on some other display,
this mastering color volume information can be used for
**gamut mapping** (saturation reduction of un-displayable colors)
and **tone mapping**
(compression of luminance to the darkest and brightest values supported by a display).

It is more efficient to provide this information
than to expect the final display system to compute it on the fly.
Especially for an image sequence,
where analysis of all frames must be completed
prior to displaying the first frame.

PNG stores this optional information in the
same order, and using the same encoding, as
**SMPTE ST 2086**, a standard which is widely used
in the streaming, broadcast and TV industries.
This is expected to ease deployment
by fitting smoothly into existing industry workflows.

The content of the 'mDCV' chunk is only 24 bytes.

Specification link:

- ['mDCV' Mastering Display Color Volume](https://w3c.github.io/PNG-spec/#mDCV-chunk)

### Content Luminance Levels

Tone mapping 
of an HDR image or animation
onto some end-user display
is made easier
by knowing the **luminance levels** in the actual image data.

Two items are of interest;
firstly the average luminance level of the **brightest frame**,
and secondly the luminance of the **brightest pixel**.

The industry standard terms for these are
**MaxFALL** (Maximum Frame Average Light Level)
and
**MaxCLL** (Maximum Content Light Level),
and these are _required metatada_
for HDR10 content.

As with 'mDCV',
it is more efficient to provide this information
than to expect the final display system to compute it on the fly.
Especially for an image sequence,
where analysis of all frames must be completed
prior to displaying the first frame.

The content of the 'cLLI' chunk is only 8 bytes.

Specification link:

- ['cLLI' Content Light Level Information](https://w3c.github.io/PNG-spec/#cLLI-chunk)

## Stakeholder Feedback / Opposition

- [Mozilla] : Positive, implements APNG and CICP
- [Chromium] : Positive, implements APNG and CICP
- [WebKit] : Positive, implements APNG and CICP
- [Ladybird] : Positive, implements APNG and CICP
- [HDR content creators] : Positive, re-use of existing workflows

## References

[**APNG**]
    _APNG Specification_ version 1.0.
    Mozilla Wiki.
    URL: https://wiki.mozilla.org/APNG_Specification

[**HDR10**]
    _Best Practices: How to Calculate HDR10 Metadata (MaxFALL, MaxCLL)_
    Dolby Vision Post Production knowledge base.
    Dolby, 2020-10.
    URL: https://professionalsupport.dolby.com/s/article/Calculation-of-MaxFALL-and-MaxCLL-metadata?language=en_US

[**ITU-R BT.2100**]
    ITU-R BT.2100, SERIES BT: BROADCASTING SERVICE (TELEVISION). _Image parameter values for high dynamic range television for use in production and international programme exchange._ ITU. 2018-07. URL: https://www.itu.int/rec/R-REC-BT.2100

[**PNG-HDR**]
    _Adding support for HDR imagery to the PNG format_
    W3C Color on the Web CG.
    URL: https://github.com/w3c/ColorWeb-CG/blob/main/hdr-in-png-requirements.md

[**SMPTE ST 2086**]
    _Mastering Display Color Volume Metadata Supporting High Luminance and Wide Color Gamut Images_. Society of Motion Picture and Television Engineers. 27 April 2018. URL: https://ieeexplore.ieee.org/document/8353899
