{ inputs, ... }: {
  perSystem = { pkgs, system, ... }: let
    # Shared CMake flags
    cmakeCommon = [
      "-DCMAKE_BUILD_TYPE=Release"
      "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    ];

    blocksDS = pkgs.blocksdsNix.blocksdsSlim;
    devkitPPC = pkgs.devkitNix.devkitPPC;
    wasm3-version = "0.5.0";
    wasmcarts-version = "0.1.0";

    wasm3-source = pkgs.stdenv.mkDerivation {
      pname = "wasm3-source";
      version = wasm3-version;

      src = pkgs.fetchFromGitHub {
        owner = "wasm3"; repo = "wasm3"; rev = "v${wasm3-version}";
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
        inputs.blocksds-nix.overlays.default
        inputs.devkitNix.overlays.default
      ];
    };

    packages = rec {
      engine-linux = pkgs.stdenv.mkDerivation {
        pname = "wasmcarts";
        version = wasmcarts-version;
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
          cp -v build/linux/wasmcarts $out/bin/
        '';
      };

      engine-nds = pkgs.blocksdsNix.stdenvBlocksdsSlim.mkDerivation {
        pname = "wasmcarts-nds";
        version = wasmcarts-version;
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

      engine-wii = pkgs.devkitNix.stdenvPPC.mkDerivation {
        pname = "wasmcarts-wii";
        version = wasmcarts-version;
        src = ./.;

        WASM3_SOURCE = wasm3-source;

        configurePhase = ''
          ${pkgs.cmake}/bin/cmake -S . -B build/wii -DTARGET=wii \
            ${pkgs.lib.concatStringsSep " " cmakeCommon} \
            -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/Wii.cmake"
        '';

        buildPhase = ''
          ${pkgs.cmake}/bin/cmake --build build/wii
        '';

        installPhase = ''
          mkdir -p $out
          cp build/wii/*.dol $out/
        '';
      };

      engine-gamecube = pkgs.devkitNix.stdenvPPC.mkDerivation {
        pname = "wasmcarts-gamecube";
        version = wasmcarts-version;
        src = ./.;

        WASM3_SOURCE = wasm3-source;

        configurePhase = ''
          ${pkgs.cmake}/bin/cmake -S . -B build/gamecube -DTARGET=gamecube \
            ${pkgs.lib.concatStringsSep " " cmakeCommon} \
            -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/GameCube.cmake"
        '';

        buildPhase = ''
          ${pkgs.cmake}/bin/cmake --build build/gamecube
        '';

        installPhase = ''
          mkdir -p $out
          cp build/gamecube/*.dol $out/
        '';
      };

      default = engine-linux;
    };

    devShells.engine = pkgs.mkShell {
      packages = with pkgs; [
        blocksDS # NDS build dependencies
        devkitPPC # Wii, GC build dependencies
        sdl3 # Linux build dependencies
        # alsa-lib jack2 pipewire libpulseaudio libx11 libxcb libxau libxdmcp

        cmake
        gnumake
        pkg-config
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

