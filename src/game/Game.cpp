#include "Game.h"

// OUTPUT VECTORS

std::ostream &operator<<(std::ostream &out, const glm::vec2 &vec) {
    out << std::fixed << "(" << vec.x << " " << vec.y << ")";
    return out;
}

std::ostream &operator<<(std::ostream &out, const glm::vec3 &vec) {
    out << std::fixed << "(" << vec.x << " " << vec.y << " " << vec.z << ")";
    return out;
}

std::ostream &operator<<(std::ostream &out, const glm::vec4 &vec) {
    out << std::fixed << "(" << vec.x << " " << vec.y << " " << vec.z << " "
            << vec.w << ")";
    return out;
}

namespace game {

GLint unifrom_show_backface_;

GLint unifrom_model_matrix_, unifrom_view_matrix_, unifrom_proj_matrix_;

GLint unifrom_scene_color_, unifrom_scene_fog_mag_, unifrom_scene_fog_color_;

std::vector<Light*> lights;
GLint unifrom_lights_[NUM_LIGHTS];

GLint unifrom_mtl_ka_, unifrom_mtl_kd_, unifrom_mtl_ks_;
GLint unifrom_mtl_ns_, unifrom_mtl_tr_;
GLint unifrom_texture_flag_;

GLint unifrom_blend_color_, unifrom_blend_factor_ , unifrom_texture;

std::map<GLint, GLboolean> mouse_down_, mouse_click_;
glm::vec2 mouse_pos_, mouse_pos_prev_;
GLboolean mouse_lock_;
std::map<GLint, GLboolean> key_down_, key_press_, key_release_, key_special_;
GLint key_mod_;
GLint global_time_;

game::Entity* scene_;
game::Camera* camera_;

// SCREEN

void mouseLock(GLboolean l) {
    mouse_lock_ = l;
}

glm::vec2 screen_size_;

glm::vec2 screen_size() {
    return screen_size_;
}

glm::vec2 screen_scale(glm::vec2 p) {
    p *= glm::vec2(1.f / screen_size_.x, -1.f / screen_size_.y) * 2.f;
    p += glm::vec2(-1, 1);
    return p;
}

// OPENGL

void setUniformShowBackface(GLboolean b) {
    glUniform1i(unifrom_show_backface_, b);
}

void setUniformModelMatrix(glm::mat4 m) {
    glUniformMatrix4fv(unifrom_model_matrix_, 1, GL_FALSE, &m[0][0]);
}

void setUniformViewMatrix(glm::mat4 m) {
    glUniformMatrix4fv(unifrom_view_matrix_, 1, GL_FALSE, &m[0][0]);
}

void setUniformProjMatrix(glm::mat4 m) {
    glUniformMatrix4fv(unifrom_proj_matrix_, 1, GL_FALSE, &m[0][0]);
}

void fogSet(glm::vec4 color, GLfloat mag) {
    glClearColor(color.x, color.y, color.z, 1.f);
    glUniform1f(unifrom_scene_fog_mag_, mag);
    glUniform3fv(unifrom_scene_fog_color_, 1, &color[0]);
}

void setUniformBlendColor(glm::vec4 c, glm::vec4 b) {
    glUniform4f(unifrom_blend_color_, c.x, c.y, c.z, c.w);
    glUniform4f(unifrom_blend_factor_, b.x, b.y, b.z, b.w);
}

// MATERIAL

void mtlSet(Material* mtl) {
    glUniform3f(unifrom_mtl_ka_, mtl->ka().x, mtl->ka().y, mtl->ka().z);
    glUniform3f(unifrom_mtl_kd_, mtl->kd().x, mtl->kd().y, mtl->kd().z);
    glUniform3f(unifrom_mtl_ks_, mtl->ks().x, mtl->ks().y, mtl->ks().z);
    glUniform1f(unifrom_mtl_ns_, mtl->ns());
    glUniform1f(unifrom_mtl_tr_, mtl->tr());
    // set texture
    if (mtl->texture()) {
        glUniform1i(unifrom_texture_flag_, 1);
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mtl->texture()->textureID());
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(unifrom_texture, 0);
    } else {
        glUniform1i(unifrom_texture_flag_, 0);
    }
}

// LIGHT

void lightSet(int idx, Light* l) {
    glm::mat4 a = glm::mat4(l->color());
    a[3] = glm::vec4(l->o(), 1);
    glUniformMatrix4fv(unifrom_lights_[idx], 1, GL_FALSE, &a[0][0]);
}

// SCENE

void sceneSet(game::Entity *scene, GLboolean destructOld) {
    if (destructOld && scene_)
        scene_->~Entity();
    scene_ = scene;
}

Entity* sceneGet() {
    return scene_;
}

void sceneColorSet(glm::vec3 color) {
    glUniform3f(unifrom_scene_color_, color.x, color.y, color.z);
}

// CAMERA

void cameraSet(game::Camera *camera, GLboolean destructOld) {
    if (destructOld && camera_)
        camera_->~Camera();
    camera_ = camera;
}

Camera* cameraGet() {
    return camera_;
}

// INIT

void init() {
    // shader program
    GLuint program = Angel::InitShader("glsl/vshader.glsl",
                                       "glsl/fshader.glsl");
    glUseProgram(program);
    // uniform bool backface_show;
    unifrom_show_backface_ = glGetUniformLocation(program, "show_backface");
    // uniform mat4 model, view, proj;
    unifrom_model_matrix_ = glGetUniformLocation(program, "model");
    unifrom_view_matrix_ = glGetUniformLocation(program, "view");
    unifrom_proj_matrix_ = glGetUniformLocation(program, "proj");
    // uniform float scene_fog_mag;
    // uniform vec3 scene_fog_color, scene_color;
    unifrom_scene_fog_mag_ = glGetUniformLocation(program, "scene_fog_mag");
    unifrom_scene_fog_color_ = glGetUniformLocation(program, "scene_fog_color");
    unifrom_scene_color_ = glGetUniformLocation(program, "scene_color");
    // uniform vec3 lights[NUM_LIGHTS][4];
    for (int i = 0; i < NUM_LIGHTS; ++i) {
        std::stringstream ss;
        ss << "lights[" << i << "]";
        unifrom_lights_[i] = glGetUniformLocation(program, ss.str().c_str());
        Light *l = new Light(glm::mat3(0));
        lightSet(i, l);
        l->~Light();
    }
    // uniform vec3 ka, kd, ks;
    // uniform float ns, tr;
    // uniform bool texture_flag;
    unifrom_mtl_ka_ = glGetUniformLocation(program, "ka");
    unifrom_mtl_kd_ = glGetUniformLocation(program, "kd");
    unifrom_mtl_ks_ = glGetUniformLocation(program, "ks");
    unifrom_mtl_ns_ = glGetUniformLocation(program, "ns");
    unifrom_mtl_tr_ = glGetUniformLocation(program, "tr");
    unifrom_texture_flag_ = glGetUniformLocation(program, "texture_flag");
    // uniform glm::vec3 blend_color;
    unifrom_blend_color_ = glGetUniformLocation(program, "blend_color");
    // uniform glm::vec3 blend_factor;
    unifrom_blend_factor_ = glGetUniformLocation(program, "blend_factor");
    // uniform sampler2D myTextureSampler;
    unifrom_texture = glGetUniformLocation(program, "myTextureSampler");

    std::cout << "uniforms loaded" << std::endl;

    // controls
    mouse_down_.clear();
    mouse_click_.clear();
    key_down_.clear();
    key_press_.clear();
    key_release_.clear();
    key_special_.clear();
    key_mod_ = 0;
    global_time_ = 0;

    glClearDepth(1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setUniformModelMatrix(glm::mat4(1));
    setUniformViewMatrix(glm::mat4(1));
    setUniformProjMatrix(glm::mat4(1));
    mtlSet(new Material("default"));
    sceneColorSet(glm::vec3(0.1f, 0.1f, 0.1f));
    fogSet(glm::vec4(0.1f, 0.1f, 0.1f, 1.f), 1.f);
    mouseLock(GL_FALSE);
}

// INPUT

void keyPress(unsigned char key, GLint x, GLint y) {
    key_press_[key] = 1;
    key_down_[key] = 1;
    key_mod_ = glutGetModifiers();
}

void keyRelease(unsigned char key, GLint x, GLint y) {
    key_down_[key] = 0;
    key_release_[key] = 1;
    key_mod_ = glutGetModifiers();
}

void keySpecial(int key, GLint x, GLint y) {
    key_special_[key] = 1;
    key_mod_ = glutGetModifiers();
}

void mouseMove(GLint x, GLint y) {
    mouse_pos_ = screen_scale(glm::vec2(x, y));

    key_mod_ = glutGetModifiers();
}

void mouseButton(GLint mouseBtn, GLint isRelease, GLint x, GLint y) {
    mouse_pos_ = screen_scale(glm::vec2(x, y));

    mouse_down_[mouseBtn] = !isRelease;
    mouse_click_[mouseBtn] = isRelease;
    key_mod_ = glutGetModifiers();
}

// DISPLAY

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    game::setUniformBlendColor(glm::vec4(1, 1, 1, 1),
                               glm::vec4(0.f, 0.f, 0.f, 0.f));

    if (camera_)
        setUniformViewMatrix(camera_->getViewMatrix()),
        setUniformProjMatrix(camera_->getProjectionMatrix());
    for (std::size_t i = 0; i < lights.size(); ++i)
        lightSet(i, lights[i]);
    if (scene_)
        scene_->render();

    glutSwapBuffers();
}

// UPDATE

void update() {
    if (scene_)
        scene_->update();
}

// MAIN LOOP

void runMainLoop(GLint f) {
    // update
    update();
    if (mouse_lock_)
        mouse_pos_prev_ = glm::vec2(0, 0);
    else
        mouse_pos_prev_ = mouse_pos_;
    // draw
    glutPostRedisplay();
    // alt + f4
    if ((key_mod_ & GLUT_ACTIVE_ALT) &&  key_special_[GLUT_KEY_F4])
        exit(EXIT_SUCCESS);
    // input
    if (mouse_lock_)
        glutWarpPointer(screen_size().x / 2, screen_size().y / 2);
    mouse_click_.clear();
    key_press_.clear();
    key_release_.clear();
    key_special_.clear();
    // time
    global_time_++;

    glutTimerFunc(1000 / f, runMainLoop, f);
}

// INIT

void init(int argc, char **argv, GLint w, GLint h, std::string t) {
    std::cout.precision(4);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    screen_size_ = glm::vec2(w, h);

    glutCreateWindow(t.c_str());
    glewInit();

    init();
}

void start(GLint f) {
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyRelease);
    glutSpecialFunc(keySpecial);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);

    glutTimerFunc(1000.0 / f, runMainLoop, f);
    glutMainLoop();
}

}
;
