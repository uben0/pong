{
  description = "Flake to develop in C with SDL";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem(system:
      let pkgs = nixpkgs.legacyPackages.${system}; in
      {
        devShell = pkgs.mkShell {
          buildInputs = [
            pkgs.pkg-config
            pkgs.gnumake
            pkgs.SDL2.dev
            pkgs.SDL2_ttf
            pkgs.SDL2_image
          ];
          shellHook = ''
            PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${pkgs.SDL2.dev}/lib/pkgconfig:${pkgs.SDL2_ttf}/lib/pkgconfig:${pkgs.SDL2_image}/lib/pkgconfig" fish_color_user=bryellow fish_color_cwd=bryellow exec ${pkgs.fish}/bin/fish
          '';
        };
      }
    );
}
