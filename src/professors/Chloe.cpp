#include "Chloe.h"

Chloe::Chloe() : Professor(5.0f, "French", "Chloe", "textures/Wallpaper.jpeg", 
    "Adds 2 to the multiplier for every good answer.\n Removes 4 for every wrong answer.") {}

bool Chloe::registered = [] {
    Professor::getRegistry().push_back([] {
        return new Chloe();
    });
    return true;
}();
