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
            MAGICK="${pkgs.imagemagick}/bin/magick"
            ROOT=$1

            # Copy of the SVG version
            cp $ROOT/icon.svg $ROOT/engine/assets/icon.svg

            # Nintendo DS icon
            $MAGICK -background none $ROOT/icon.svg \
              -resize 32x32 \
              -gravity center \
              -extent 32x32 \
              +dither \
              -colors 15 \
              PNG8:$ROOT/engine/assets/nds-icon.png

            # Wii Homebrew Channel banner
            $MAGICK -background none $ROOT/icon.svg \
              -resize 128x48 \
              -gravity center \
              -extent 128x48 \
              $ROOT/engine/assets/wii-banner.png

            # App icon used by SDL
            $MAGICK -background none $ROOT/icon.svg \
              -resize 64x64 \
              -gravity center \
              -extent 64x64 \
              $ROOT/engine/assets/icon-64.png

          '';
        };
      })
    ];
    systems = [ "x86_64-linux" "aarch64-linux" ];
  };
}

