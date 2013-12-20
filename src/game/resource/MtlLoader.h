#ifndef MTLLOADER_H_
#define MTLLOADER_H_

#include <fstream>
#include <string>

#include "../OpenGL.h"
#include "Material.h"
#include "libexternal/SOIL.h"

namespace game {
namespace MtlLoader {

Material *load(std::string fname, std::string dir);

}
}

#endif
