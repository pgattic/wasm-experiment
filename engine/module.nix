{ inputs, ... }: {
  perSystem = { pkgs, system, ... }: let
    # Shared CMake flags
    cmakeCommon = [
      "-DCMAKE_BUILD_TYPE=Release"
      "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    ];

    blocksDS = pkgs.blocksdsNix.blocksdsSlim;
    devkitPPC = pkgs.devkitNix.devkitPPC;
    wasm3-ver = "0.5.0";

    wasm3-source = pkgs.stdenv.mkDerivation {
      pname = "wasm3-source";
      version = wasm3-ver;

      src = pkgs.fetchFromGitHub {
        owner = "wasm3"; repo = "wasm3"; rev = "v${wasm3-ver}";
        sha256 = "sha256-QkXOBJ5luml6VCYLowQKv0K4mI7S2gy7bx5Jc86s/x8=";
      };

      installPhase = ''
        mkdir -p $out
        cp -r . $out/
      '';
    };
  in {
    _module.args.pkgs = import inputs.nixpkgs {
      inherit system;
      overlays = [
        inputs.blocksds.overlays.default
        inputs.devkitnix.overlays.default
      ];
    };

    packages = rec {
      engine-linux = pkgs.stdenv.mkDerivation {
        pname = "wasmcarts";
        version = "0.1.0";
        src = ./.;

        WASM3_SOURCE = wasm3-source;

        nativeBuildInputs = with pkgs; [
          cmake
          gnumake
          pkg-config
          sdl3
        ];

        configurePhase = ''
          cmake -S . -B build/linux -DTARGET=linux \
            ${pkgs.lib.concatStringsSep " " cmakeCommon}
        '';

        buildPhase = ''
          cmake --build build/linux
        '';

        installPhase = ''
          mkdir -p $out/bin
          # Adjust if your target name differs
          cp -v build/linux/wasmcarts $out/bin/
        '';
      };

      engine-nds = pkgs.blocksdsNix.stdenvBlocksdsSlim.mkDerivation {
        pname = "wasmcarts-nds";
        version = "0.1.0";
        src = ./.;

        WASM3_SOURCE = wasm3-source;

        configurePhase = ''
          ${pkgs.cmake}/bin/cmake -S . -B build/nds -DTARGET=nds \
            ${pkgs.lib.concatStringsSep " " cmakeCommon} \
            -DCMAKE_TOOLCHAIN_FILE="$BLOCKSDS/cmake/BlocksDS.cmake"
        '';

        buildPhase = ''
          ${pkgs.cmake}/bin/cmake --build build/nds
        '';

        installPhase = ''
          mkdir -p $out
          cp build/nds/*.nds $out/
        '';
      };

      default = engine-linux;
    };

    devShells.engine = pkgs.mkShell {
      packages = with pkgs; [
        blocksDS # NDS build dependencies
        devkitPPC # Wii, GC build dependencies
        sdl3 # Linux build dependencies
        # alsa-lib
        # jack2
        # pipewire
        # libpulseaudio
        # libx11
        # libxcb
        # libxau
        # libxdmcp

        cmake
        gnumake
        pkg-config
        # grrlib-wii
        # grrlib-gc
      ];

      WONDERFUL_TOOLCHAIN = blocksDS.passthru.WONDERFUL_TOOLCHAIN;
      BLOCKSDS = blocksDS.passthru.BLOCKSDS;
      BLOCKSDSEXT = blocksDS.passthru.BLOCKSDSEXT;
      # CPATH = devkitPPC.passthru.CPATH;
      DEVKITPRO = "${devkitPPC}/opt/devkitpro";
      DEVKITPPC = "${devkitPPC}/opt/devkitpro/devkitPPC";
      WASM3_SOURCE = wasm3-source;
    };
  };
}

