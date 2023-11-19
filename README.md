# bfif: brute force image finder

A very simple app that walks over a file or block device attempting to
interpret the data as an image [supported by
stb_image](https://github.com/nothings/stb/blob/master/stb_image.h#L19)
(some but not all PNG, JPG, BMP, PIC, PNM, GIF variants) and writes
out anything found as PNGs.


## Usage

Simply takes the file to work on and then will output PNG files
for every image found that passes some basic sanity checks. Use
`--ignore-false-positives=false` (short version `-i false`.)

```
$ ./bfif --help
bfif: brute force image finder (v1.0.0)
Usage: ./build/bfif_linux_x86_64 [OPTIONS] filepath

Positionals:
  filepath PATH:FILE REQUIRED Path to input file

Options:
  -h,--help                   Print this help message and exit
  -o,--output-dirpath PATH:DIR
                              Path to directory to write found images (default: input file directory)
  -i,--ignore-false-positives BOOLEAN [true]
                              Ignore suspected false positives


$ ./bfif PHOTO_ALBUM
Reading ./PHOTO_ALBUM into memory
Found images will be written to .
Found at offset 5914: x = 64, y = 64 - writing PHOTO_ALBUM_000_5914_64x64.png
Found at offset 22298: x = 64, y = 64 - writing PHOTO_ALBUM_001_22298_64x64.png
Found at offset 53452: x = 1280, y = 720 - writing PHOTO_ALBUM_002_53452_1280x720.png
Found at offset 258252: x = 1280, y = 720 - writing PHOTO_ALBUM_003_258252_1280x720.png
Found at offset 463052: x = 1280, y = 720 - writing PHOTO_ALBUM_004_463052_1280x720.png
Found at offset 667852: x = 1280, y = 720 - writing PHOTO_ALBUM_005_667852_1280x720.png
Found at offset 889036: x = 1280, y = 720 - writing PHOTO_ALBUM_006_889036_1280x720.png
Found at offset 1093836: x = 1280, y = 720 - writing PHOTO_ALBUM_007_1093836_1280x720.png
Found at offset 1298636: x = 1280, y = 720 - writing PHOTO_ALBUM_008_1298636_1280x720.png
Found at offset 1511628: x = 1280, y = 720 - writing PHOTO_ALBUM_009_1511628_1280x720.png
Found at offset 1716428: x = 1280, y = 720 - writing PHOTO_ALBUM_010_1716428_1280x720.png
Found at offset 1921228: x = 1280, y = 720 - writing PHOTO_ALBUM_011_1921228_1280x720.png
Found at offset 2126028: x = 1280, y = 720 - writing PHOTO_ALBUM_012_2126028_1280x720.png
Found at offset 2155232: x = 18657, y = 3427 - seems like a false positive - skipping
Found at offset 2166260: x = 8755, y = 18951 - seems like a false positive - skipping
Found at offset 2178923: x = 308, y = 31012 - seems like a false positive - skipping
Found at offset 2339020: x = 1280, y = 720 - writing PHOTO_ALBUM_013_2339020_1280x720.png
Found at offset 2543820: x = 1280, y = 720 - writing PHOTO_ALBUM_014_2543820_1280x720.png
Found at offset 2748620: x = 1280, y = 720 - writing PHOTO_ALBUM_015_2748620_1280x720.png
Found at offset 2961612: x = 1280, y = 720 - writing PHOTO_ALBUM_016_2961612_1280x720.png
Found at offset 3166412: x = 1280, y = 720 - writing PHOTO_ALBUM_017_3166412_1280x720.png
```

# Donations / Sponsorship

If you find bfif useful please consider supporting its ongoing development.

https://github.com/trapexit/support
