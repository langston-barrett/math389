# See README.md for explanation.
with import <nixpkgs> {};
  rWrapper.override {
  packages = with rPackages; [
    knitr
    ggplot2
  ];
}
