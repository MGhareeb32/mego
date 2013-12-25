#include "Mesh.h"

namespace game {

Mesh::Mesh(std::string uid,
           std::vector<glm::vec3> v,
           std::vector<glm::vec3> vt,
           std::vector<glm::vec3> vn,
           std::vector<GLuint> idx, GLint draw_mode) : Resource(uid) {

    draw_mode_ = draw_mode;
    vertex_count_ = idx.size();

    //
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    buffers_ = 0;

    // vertex position to buffer
    glGenBuffers(1, &vbo_[buffers_]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[buffers_]);
    glBufferData(GL_ARRAY_BUFFER,
                 v.size() * sizeof(glm::vec3), &v[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(buffers_);
    glVertexAttribPointer(buffers_, 3, GL_FLOAT, GL_FALSE, 0, 0);

    buffers_++;

    // vertex normal to buffer
    glGenBuffers(1, &vbo_[buffers_]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[buffers_]);
    glBufferData(GL_ARRAY_BUFFER,
                 vn.size() * sizeof(glm::vec3), &vn[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(buffers_);
    glVertexAttribPointer(buffers_, 3, GL_FLOAT, GL_FALSE, 0, 0);

    buffers_++;

    // vertex texture to buffer
    glGenBuffers(1, &vbo_[buffers_]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[buffers_]);
    glBufferData(GL_ARRAY_BUFFER,
                 vt.size() * sizeof(glm::vec3), &vt[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(buffers_);
    glVertexAttribPointer(buffers_, 3, GL_FLOAT, GL_FALSE, 0, 0);

    buffers_++;

    // indices
    glGenBuffers(1, &vbo_[buffers_]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[buffers_]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 idx.size() * sizeof(GLuint), &idx[0], GL_STATIC_DRAW);

    buffers_++;

    glBindVertexArray(0);

    // collision faces
    for (std::size_t i = 0; i < idx.size(); i += 3) {
        glm::vec3 pnt[3];
        for (int j = 0; j < 3; ++j)
            pnt[j] = v[idx[i + j]];
        faces_.push_back(new Face(3, pnt));
    }
}

Mesh::~Mesh() {
    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // delete buffers
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(buffers_, &vbo_[0]);
    // disable attrs
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Mesh::render(glm::mat4 transformation) {
    game::setUniformModelMatrix(transformation);

    glBindVertexArray(vao_);
    glDrawElements(draw_mode_, vertex_count_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

}
