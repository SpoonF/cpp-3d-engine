#pragma once

// #include "engine/scene.h"

const unsigned int CHUNK_WIDTH = 128; 
const unsigned int CHUNK_HEIGHT = 12; 
// const unsigned int CHUNK_COUNT = 16; 

class Chunk {
public:
    std::vector<Entity*> entities;
    glm::vec3 location;
};

class World {
    std::vector<Chunk*> chunks;

    World(std::vector<Chunk*> &chunks) {
        this->chunks = chunks;
    };
public:
    static World *generate(const unsigned int size);


    glm::vec3 getWorldCenter();

    std::vector<Chunk*> getChunks();
    // Chunk* getChunk(glm::vec3 position);

};