#include "TextureLoader.h"

namespace game {
namespace TextureLoader {

Resource* load(std::string file, std::string dir) {
    int w, h;
    unsigned char* image
       = SOIL_load_image(file.c_str(), &w, &h, 0, SOIL_LOAD_RGB);
    Texture *out = new Texture(file, w, h, image);
    SOIL_free_image_data(image);
    return out;
}

}
}
