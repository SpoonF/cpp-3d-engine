#pragma once


#include <glm/gtx/hash.hpp>
#include <unordered_set>

// #include "engine/scene.h"

const unsigned int CHUNK_WIDTH = 16; 
const unsigned int CHUNK_HEIGHT = 32; 

class Chunk {
public:
    std::vector<Entity*> entities;
    glm::vec3 location;

    static glm::vec3& getCoordiante(glm::vec3& coordiante);
};

class World {
    std::vector<Chunk*> chunks;

    World(std::vector<Chunk*> &chunks) {
        this->chunks = chunks;
    };
public:
    static World *generate(const unsigned int size);


    glm::vec3 getWorldCenter();
    void setEntity(Entity* entity);

    std::vector<Chunk*> getChunks();

};