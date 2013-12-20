#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_

#include <fstream>
#include <string>

#include "../OpenGL.h"
#include "Texture.h"
#include "../../../include/soil/SOIL.h"

namespace game {
namespace TextureLoader {

Resource* load(std::string file, std::string dir);

}
}

#endif
