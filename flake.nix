{
  description = "WASMCarts flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    blocksds = {
      url = "github:pgattic/blocksds-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    devkitnix = {
      url = "github:bandithedoge/devkitNix/";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = inputs: inputs.flake-parts.lib.mkFlake { inherit inputs; } {
    imports = [
      ./engine/module.nix
    ];
    systems = [ "x86_64-linux" "aarch64-linux" ];
  };
}

