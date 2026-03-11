{
  description = "WASMCarts flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    blocksds-nix = {
      url = "github:pgattic/blocksds-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    devkitNix = {
      url = "github:bandithedoge/devkitNix/";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = inputs: inputs.flake-parts.lib.mkFlake { inherit inputs; } {
    imports = [
      ./engine/module.nix
      ({
        perSystem = { pkgs, ... }: {
          packages.gen-icons = pkgs.writeShellScriptBin "gen-icons" ''
            # Generate icons for their places across the repo
            CONVERT="${pkgs.librsvg}/bin/rsvg-convert"
            ROOT=$1

            # $CONVERT -w 32 -h 32 -o $ROOT/engine/assets/nds-icon.bmp $ROOT/icon.svg

            # ${pkgs.imagemagick}/bin/magick -background "#FF00FF" -size 32x32 $ROOT/icon.svg \
            # \( +clone -colors 16 -unique-colors \
            #   xc:"#FF00FF" +swap +append -write mpr:palette +delete \) \
            # -remap mpr:palette -type Palette -colors 16 BMP3:$ROOT/engine/assets/nds-icon.bmp

            # ${pkgs.imagemagick}/bin/magick -background "#FF00FF" -size 1024x1024 $ROOT/icon.svg \
            #   \( xc:"#FF00FF" \) \
            #   \( +clone -colors 15 -unique-colors \) \
            #   +insert +append -write mpr:palette +delete \
            #   -remap mpr:palette -type Palette -colors 16 BMP3:$ROOT/engine/assets/nds-icon.bmp

            # ${pkgs.imagemagick}/bin/magick -background "#FF00FF" -size 32x32 $ROOT/icon.svg \
            #   \( +clone -colors 15 -unique-colors -write mpr:colors +delete \) \
            #   \( xc:"#FF00FF" mpr:colors +append -write mpr:palette +delete \) \
            #   -remap mpr:palette BMP3:$ROOT/engine/assets/nds-icon.bmp

            ${pkgs.imagemagick}/bin/magick $ROOT/icon.svg \
              -background none \
              -resize 32x32 \
              -gravity center \
              -extent 32x32 \
              -background "#FF00FF" \
              -alpha background \
              +dither \
              -colors 15 \
              -type Palette \
              BMP3:$ROOT/engine/assets/nds-icon.bmp


          '';
        };
      })
    ];
    systems = [ "x86_64-linux" "aarch64-linux" ];
  };
}

