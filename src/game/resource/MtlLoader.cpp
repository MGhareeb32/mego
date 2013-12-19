#include "MtlLoader.h"
#include <GL/glew.h>
#include "libextrnal/SOIL.h"
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

Material *load(std::string fname) {
	std::fstream in;
	in.open(fname.c_str(), std::fstream::in);
	//
	GLfloat ns, ni, tr, lum;
	glm::vec3 ka, kd, ks;
	std::string type, txtureFile = "res/mesh/viewer/";
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
			txtureFile.append(tmp.c_str());
		} else {
			getline(in, type);
		}
	}
	in.close();

	cout << "file " << txtureFile << endl;
	// Load textures
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
//	std::cout << height  << " " << width <<  " ... " << std::endl;
	image = SOIL_load_image(txtureFile.c_str(), &width, &height, 0,
			SOIL_LOAD_RGB);
	//	image = raedimage(txtureFile.c_str());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image);
	std::cout << height  << " " << width <<  " ... " << std::endl;
//	for (int i = 0; i < height; i++) {
//		for (int j = 0; j < width; j++)
//			std::cout << image[height * i + j];
//		std::cout << std::endl;
//	}
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return new Material(fname, ka, kd, ks, ns, tr, textureID);
}

}
}
