{
  description = "WASMCarts dev shell (Linux-only)";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
    };
    lib = pkgs.lib;
  in {
    devShells.${system}.default = pkgs.mkShell {
      packages = with pkgs; [
        gcc15
        binutils
        cmake
        gnumake
        pkg-config
        sdl3
        git # required by CMake to download wasm3 source
      ];
    };
  };
}

