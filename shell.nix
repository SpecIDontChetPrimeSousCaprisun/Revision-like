{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = with pkgs; [
    gcc
    gnumake
    pkg-config

    glfw
    assimp

    libx11
    libxrandr
    libxi
    libxxf86vm
    libxcursor
  ];
}
