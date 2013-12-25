#include "ObjLoader.h"

namespace game {
namespace ObjLoader {

class vertex_data : public std::pair<GLuint, std::pair<GLuint, GLuint> > {
    typedef std::pair<GLuint, std::pair<GLuint, GLuint> > int_trio;
    typedef std::pair<GLuint, GLuint> int_pair;
public:
    vertex_data(GLuint v, GLuint vt, GLuint vn)
        : int_trio(v, int_pair(vt, vn)) {}
    GLuint v() { return first; }
    GLuint vt() { return second.first; }
    GLuint vn() { return second.second; }
};

std::ostream &operator<<(std::ostream &out, const vertex_data &d) {
    return out << "(" << d.first
               << "," << d.second.first
               << "," << d.second.second  << ")";
}

vertex_data read_vertex_data
    (std::fstream &in, std::size_t v, std::size_t vt, std::size_t vn) {

    // v/vt/vn
    char d;
    GLuint a = 0, b = 0, c = 0;
    // coord
    if (v > 1)
        in >> a;
    // vt
    in >> d;
    if (vt > 1)
        in >> b;
    // normal
    in >> d;
    if (vn > 1)
        in >> c;
    return vertex_data(a, b, c);
}

game::Mesh *load(std::string fname, std::string dir) {
    std::fstream in;
    in.open(fname.c_str(), std::fstream::in);
    //
    std::vector<glm::vec3> file_v, file_vt, file_vn;
    file_v.push_back(glm::vec3(0));
    file_vt.push_back(glm::vec3(0));
    file_vn.push_back(glm::vec3(0));
    //
    std::vector<glm::vec3> v, vt, vn;
    std::vector<GLuint> idx;
    std::map<vertex_data, GLuint> dataIdx;
    //
    std::string type;
    while (in >> type) {
        if (!type.compare("v")) {
            glm::vec3 v;
            in >> v.x >> v.y >> v.z;
            file_v.push_back(v);
        } else if (!type.compare("vt")) {
            glm::vec3 vt;
            in >> vt.x >> vt.y;
            vt.y = 1 - vt.y;
            vt.z = 0;
            file_vt.push_back(vt);
        } else if (!type.compare("vn")) {
            glm::vec3 vn;
            in >> vn.x >> vn.y >> vn.z;
            file_vn.push_back(vn);
        } else if (!type.compare("mtllib")) {
            in >> type;
        } else if (!type.compare("f")) {
            for (int i = 0; i < 3; ++i) {
                vertex_data d = read_vertex_data
                    (in, file_v.size(), file_vt.size(), file_vn.size());
                // not seen
                if (dataIdx.find(d) == dataIdx.end()) {
                    dataIdx[d] = v.size();
                    v.push_back(glm::vec3(file_v[d.v()]));
                    vt.push_back(glm::vec3(file_vt[d.vt()]));
                    vn.push_back(glm::vec3(file_vn[d.vn()]));
                }
                idx.push_back(dataIdx[d]);
            }
        } else {
            getline(in, type);
        }
    }
    in.close();
    // data
    return new game::Mesh(fname, v, vt, vn, idx, GL_TRIANGLES);
}

}
}
