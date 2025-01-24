{ inputs, ... }:

let self = inputs.self;

in {
  # Configure per-system settings  
  perSystem = { system, config, pkgs-dev, ... }: {
    # Define `pkgs-dev` to be a nixpkgs instance with the custom overlays
    # applied. This ensures all `perSystem` across all parts files can
    # consistently access `pkgs-dev`.
    _module.args.pkgs-dev = import inputs.nixpkgs {
      inherit system;
    };

    devShells.default = let name = "libcoro";
    in pkgs-dev.mkShell {
      inherit name;

      packages = with pkgs-dev; [
        cmake
        cmakeCurses
        
        c-ares
        openssl.dev
      ];

      shellHook = let icon = "f121";
      in ''
        export PS1="$(echo -e '\u${icon}') {\[$(tput sgr0)\]\[\033[38;5;228m\]\w\[$(tput sgr0)\]\[\033[38;5;15m\]} (${name}) \\$ \[$(tput sgr0)\]"
      '';
    };
  };
}
