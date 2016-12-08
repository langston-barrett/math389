# See README.md for explanation.
# To use: nix-shell --run zsh
# Optimally we'd use: nix-shell --pure --run zsh
# https://nixos.org/wiki/Development_Environments
# http://nixos.org/nix/manual/#sec-nix-shell
with import <nixpkgs> {}; stdenv.mkDerivation {
  name = "math389";
  src = null;
  buildInputs = [
    alt-ergo # required for frama-c
    clang-tools # for clang-format
    framac # formal verification :O
    gcc6
    gnumake42
  ];
}
