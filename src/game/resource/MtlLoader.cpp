#include "MtlLoader.h"

using namespace std;
namespace game {
namespace MtlLoader {

unsigned char* raedimage(const char * imagepath) {
    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(imagepath, "rb");
    if (!file) {
        printf(
                "%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n",
                imagepath);
        getchar();
        return 0;
    }

    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if (fread(header, 1, 54, file) != 54) {
        printf("Not a correct BMP file\n");
        return 0;
    }
    // A BMP files always begins with "BM"
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file\n");
        return 0;
    }
    // Make sure this is a 24bpp file
    if (*(int*) &(header[0x1E]) != 0) {
        printf("Not a correct BMP file\n");
        return 0;
    }
    if (*(int*) &(header[0x1C]) != 24) {
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read the information about the image
    dataPos = *(int*) &(header[0x0A]);
    imageSize = *(int*) &(header[0x22]);
    width = *(int*) &(header[0x12]);
    height = *(int*) &(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0)
        imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0)
        dataPos = 54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    // Everything is in memory now, the file wan be closed
    fclose(file);
    return data;
}

Material *load(std::string fname, std::string dir) {
    std::fstream in;
    in.open(fname.c_str(), std::fstream::in);
    //
    GLfloat ns, ni, tr, lum;
    glm::vec3 ka, kd, ks;
    std::string type, textureFile = dir;
    while (in >> type) {
        if (!type.compare("Ns")) {
            in >> ns;
        } else if (!type.compare("Ka")) {
            in >> ka.x >> ka.y >> ka.z;
        } else if (!type.compare("Kd")) {
            in >> kd.x >> kd.y >> kd.z;
        } else if (!type.compare("Ks")) {
            in >> ks.x >> ks.y >> ks.z;
        } else if (!type.compare("Ni")) {
            in >> ni;
        } else if (!type.compare("Tr") || !type.compare("d")) {
            in >> tr;
        } else if (!type.compare("illum")) {
            in >> lum;
        } else if (!type.compare("map_Kd")) {
            string tmp;
            in >> tmp;
            textureFile.append(tmp.c_str());
        } else {
            getline(in, type);
        }
    }
    in.close();

    return new Material(fname, ka, kd, ks, ns, tr);
}

}
}
