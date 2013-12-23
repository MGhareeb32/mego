#ifndef GAME_H_
#define GAME_H_

#include "OpenGL.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include "resource/Mesh.h"
#include "MeshEntity.h"
#include "resource/ResMgr.h"


std::ostream &operator<<(std::ostream &out, const glm::vec2 &vec);
std::ostream &operator<<(std::ostream &out, const glm::vec3 &vec);
std::ostream &operator<<(std::ostream &out, const glm::vec4 &vec);

namespace game {

void mouseLock(GLboolean l);

glm::vec2 screen_size();
glm::vec2 screen_scale(glm::vec2 p);

void sceneSet(Entity *scene, GLboolean destructOld = true);
Entity* sceneGet();

void cameraSet(Camera* camera, GLboolean destructOld = true);
Camera* cameraGet();

extern void mtlSet(Material* mtl);

extern void sceneColorSet(glm::vec3 color);
static const GLint NUM_LIGHTS = 32;
extern std::vector<Light*> lights;

extern void fogSet(glm::vec4 color, GLfloat mag);

void init(int argc, char **argv, GLint w, GLint h, std::string t);
void start(GLint f);
}

#endif
