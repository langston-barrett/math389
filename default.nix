# See README.md for explanation.
# To use: nix-shell --run zsh
# Optimally we'd use: nix-shell --pure --run zsh
# https://nixos.org/wiki/Development_Environments
# http://nixos.org/nix/manual/#sec-nix-shell
with import <nixpkgs> {}; stdenv.mkDerivation {
  name = "math389";
  src = null;
  shellHook = ''
    export CC=clang
  '';
  buildInputs = [
    clang
    clang-tools # for clang-format
    gnumake42
    valgrind # debugging
  ];
}
