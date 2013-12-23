#ifndef INTERNAL_H_
#define INTERNAL_H_

#include "OpenGL.h"

namespace game {

extern std::map<GLint, GLboolean> mouse_down_, mouse_click_;
extern glm::vec2 mouse_pos_, mouse_pos_prev_;
extern std::map<GLint, GLboolean> key_down_, key_press_,
                                  key_release_, key_special_;
extern GLint key_mod_;
extern GLint global_time_;

extern void setUniformModelMatrix(glm::mat4 f);
extern void setUniformViewMatrix(glm::mat4 f);
extern void setUniformProjMatrix(glm::mat4 m);

class Material;
extern void mtlSet(Material* mtl);

extern void sceneColorSet(glm::vec3 color);

extern void fogSet(glm::vec4 color, GLfloat mag);

extern void setUniformBlendColor(glm::vec4 c, glm::vec4 b);

}

#endif
