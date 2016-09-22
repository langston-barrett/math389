# See README.md for explanation.
let
  pkgs = import <nixpkgs> {};
  stdenv = pkgs.stdenv;
in rec {
  math389 = stdenv.mkDerivation rec {
    name = "math389";
    version = "1";
    src = ./.;
    buildInputs = with pkgs; [
      gcc6
      gnumake42
    ];
  };
}
