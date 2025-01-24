{
  description = "C++20 corotine library";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
  };

  outputs = { self, nixpkgs, flake-parts, ... }@inputs:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [ "x86_64-linux" "aarch64-linux" ];

      imports = [ ./nix/development.nix ];

      perSystem = { system, config, pkgs-dev, ... }: {
        # With this, you can run `nix fmt` to format all nix files in this repo.
        formatter = pkgs-dev.nixfmt-classic;
      };
    };
}
