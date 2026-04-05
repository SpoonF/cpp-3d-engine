#pragma once

// #include "engine/scene.h"

const unsigned int CHUNK_WIDTH = 16; 
const unsigned int CHUNK_HEIGHT = 6; 
// const unsigned int CHUNK_COUNT = 16; 

class Chunk {
public:
    std::vector<Entity*> entities{};
    glm::vec2 location{};
};

class World {
    std::vector<Chunk*> chunks{};
public:
    World() = default;
    void generate(unsigned int size);


    glm::vec3 getWorldCenter();

    std::vector<Chunk*> getChunks();
    Chunk* getChunk(glm::vec2 position);

};