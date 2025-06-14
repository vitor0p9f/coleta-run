#ifndef SPRITE_MANAGER_HPP
#define SPRITE_MANAGER_HPP

#include <GL/freeglut.h>
#include <string>
#include <unordered_map>
#include "stb_image.h"

class SpriteManager {
  public:
    void load(const std::string& name, const std::string& filename) {
        GLuint texID;
        int width, height, channels;
        unsigned char* image = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if (!image) {
            printf("Erro ao carregar sprite %s (%s)\n", name.c_str(), filename.c_str());
            return;
        }

        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(image);

        textures[name] = texID;
    }

    void draw(const std::string& name, int x, int y, int w, int h) const {
        auto it = textures.find(name);
        if (it == textures.end()) return;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, it->second);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
            glTexCoord2f(0, 1); glVertex2f(x, y);
            glTexCoord2f(1, 1); glVertex2f(x + w, y);
            glTexCoord2f(1, 0); glVertex2f(x + w, y + h);
            glTexCoord2f(0, 0); glVertex2f(x, y + h);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    void unload() {
        for (const auto& pair : textures) {
            glDeleteTextures(1, &pair.second);
        }
        textures.clear();
    }

    ~SpriteManager() {
        unload();
    }

private:
    std::unordered_map<std::string, GLuint> textures;
};

#endif
