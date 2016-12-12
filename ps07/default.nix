# See README.md for explanation.
# To use: nix-shell --run zsh
# Optimally we'd use: nix-shell --pure --run zsh
# https://nixos.org/wiki/Development_Environments
# http://nixos.org/nix/manual/#sec-nix-shell
with import <nixpkgs> {}; stdenv.mkDerivation {
  name = "sand";
  src = null;
  shellHook = ''
    export CC=clang
  '';
  buildInputs = [
    clang
    R
    gnumake42
    rPackages.codetools
    rPackages.dplyr
    rPackages.ggplot2
    rPackages.knitr
    pythonPackages.pygments # latex package minted
    texlive.combined.scheme-full # lualatex, etc.
    valgrind # debugging
  ];
}
