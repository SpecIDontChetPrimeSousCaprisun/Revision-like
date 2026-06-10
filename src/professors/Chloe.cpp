#include "Chloe.h"

Chloe::Chloe() : Professor(5.0f, "French", "Chloe", "textures/Wallpaper.jpeg") {}

bool Chloe::registered = [] {
    Professor::getRegistry().push_back([] {
        return new Chloe();
    });
    return true;
}();
