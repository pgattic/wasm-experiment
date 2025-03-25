#!/usr/bin/env python3
import sys
import os

# File layout constants

FILE_SIZE       = 0x100000 # 1048576
HEADER_SIZE     = 0x10 # 16 (Don't know what this'll be about yet)
PALETTE_SIZE    = 0x30 # 8bits per channel * three channels (RGB) * 16 colors = 48 bytes
SPR_TILES_SIZE  = 0x2000 # 4bit/px * 8px * 8px * 256tiles = 32768bits (8192 bytes)
BG_TILES_SIZE   = 0x2000 # 4bit/px * 8px * 8px * 256tiles = 32768bits (8192 bytes)
METAPROG_SIZE   = HEADER_SIZE + PALETTE_SIZE + SPR_TILES_SIZE + BG_TILES_SIZE # 16448
PRG_CODE_SIZE   = FILE_SIZE - METAPROG_SIZE # 1032128

DEFAULT_PALETTE = [ # PICO-8 Palette
      0,   0,   0,
     29,  43,  83,
    126,  37,  83,
      0, 135,  81,
    171,  82,  54,
     95,  87,  79,
    194, 195, 199,
    255, 241, 232,
    255,   0,  77,
    255, 163,   0,
    255, 236,  39,
      0, 228,  54,
     41, 173, 255,
    131, 118, 156,
    255, 119, 168,
    255, 204, 170,
]

def main():
    if len(sys.argv) < 3:
        print("Usage: {} input.wasm output.bin".format(sys.argv[0]))
        sys.exit(1)

    wasm_file = sys.argv[1]
    output_file = sys.argv[2]

    # Read the WASM file
    try:
        with open(wasm_file, 'rb') as f:
            wasm_data = f.read()
    except IOError as e:
        print(f"Error reading WASM file: {e}")
        sys.exit(1)

    # Check if the WASM file fits into the Program Code section.
    if len(wasm_data) > PRG_CODE_SIZE:
        print(f"Error: WASM file is too large ({len(wasm_data)} bytes), maximum allowed is {PRG_CODE_SIZE} bytes.")
        sys.exit(1)

    # Create the Header section (16 bytes).
    # Here we use a simple magic string 'MYHEADER' padded with zeros.
    header = bytearray(HEADER_SIZE)
    magic = b"MYHEADER"
    header[:len(magic)] = magic

    # Fill the Global Palette section (`bytearray()` can be used with a list of ints too)
    global_palette = bytearray(DEFAULT_PALETTE)

    # Create the Sprite Tiles section (8192 bytes) filled with zeros.
    sprite_tiles = bytearray(SPR_TILES_SIZE)
    smiley_face = bytearray([

        # Smiley
        0x10, 0x11, 0x11, 0x01,
        0x11, 0xaa, 0xaa, 0x11,
        0xa1, 0xa1, 0x1a, 0x1a,
        0xa1, 0xaa, 0xaa, 0x1a,
        0xa1, 0xa1, 0x1a, 0x1a,
        0xa1, 0x1a, 0xa1, 0x1a,
        0x11, 0xaa, 0xaa, 0x11,
        0x10, 0x11, 0x11, 0x01,

        # Palette Test
        0x10, 0x32, 0x54, 0x76,
        0x98, 0xba, 0xdc, 0xfe,
        0x10, 0x32, 0x54, 0x76,
        0x98, 0xba, 0xdc, 0xfe,
        0x10, 0x32, 0x54, 0x76,
        0x98, 0xba, 0xdc, 0xfe,
        0x10, 0x32, 0x54, 0x76,
        0x98, 0xba, 0xdc, 0xfe,
        # 0x11, 0x22, 0x33, 0x44,
        # 0x55, 0x66, 0x77, 0x88,
        # 0x99, 0xAA, 0xBB, 0xCC,
        # 0xDD, 0xEE, 0xFF, 0x00,
        # 0x1A, 0xFA, 0xA1, 0xA1,
        # 0x1A, 0xA1, 0xDA, 0xA1,
        # 0x11, 0xAA, 0xAA, 0x11,
        # 0x01, 0x11, 0x11, 0x10,
    ])
    sprite_tiles[:len(smiley_face)] = smiley_face

    # Create the BG Tiles section (8192 bytes) filled with zeros.
    bg_tiles = bytearray(BG_TILES_SIZE)

    # Create the Program Code section (1032128 bytes).
    # Copy the WASM data into the beginning of the section and leave the rest as zeros.
    # This pads the file.
    # program_code = bytearray(PRG_CODE_SIZE)
    # program_code[:len(wasm_data)] = wasm_data

    # Combine all sections into one final binary blob.
    final_data = header + global_palette + sprite_tiles + bg_tiles + wasm_data

    # Verify final size.
    # if len(final_data) != FILE_SIZE:
    #     print(f"Error: Final file size is {len(final_data)} bytes, expected {FILE_SIZE} bytes.")
    #     sys.exit(1)

    # Write the output file.
    try:
        with open(output_file, 'wb') as f:
            f.write(final_data)
        print(f"Successfully created {output_file} ({len(final_data)} bytes).")
    except IOError as e:
        print(f"Error writing output file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()

