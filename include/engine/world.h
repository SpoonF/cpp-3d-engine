#pragma once


#include <glm/gtx/hash.hpp>
#include <unordered_set>
#include <map>

#include "object.h"


const unsigned int CHUNK_WIDTH = 16; 
const unsigned int CHUNK_HEIGHT = 100; 


struct Quad {
    glm::vec3 pos;
    int width;
    int height;
    Object object;

    Quad(glm::vec3 pos, int width, int height, const Object& object) : pos(pos), width(width), height(height), object(object) {};
};

class Chunk {
private:
    std::vector<std::shared_ptr<Object>> objects;
public:
    glm::vec3 position;

    Chunk() {
        objects.resize(CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH, nullptr);
    }

    void setLocal(const glm::vec3& position, std::shared_ptr<Object> object) {
        if(position.x >= 0 && position.x < CHUNK_WIDTH && 
            position.y >= 0 && position.y < CHUNK_HEIGHT && 
            position.z >= 0 && position.z < CHUNK_WIDTH) {
            objects[position.x + CHUNK_WIDTH * (position.y + CHUNK_HEIGHT * position.z)] = object;
        }
    }

    std::shared_ptr<Object> getLocal(const glm::vec3& position) const {
        if(position.x < 0 || position.x >= CHUNK_WIDTH || 
            position.y < 0 || position.y >= CHUNK_HEIGHT || 
            position.z < 0 || position.z >= CHUNK_WIDTH) {
            
        }
        return objects[position.x + CHUNK_WIDTH * (position.y + CHUNK_HEIGHT * position.z)];
    }

    std::shared_ptr<Object> getLocal(int x, int y, int z) const {
        if(x < 0 || x >= CHUNK_WIDTH || 
            y < 0 || y >= CHUNK_HEIGHT || 
            z < 0 || z >= CHUNK_WIDTH) {
        }
        return objects[x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z)];
    }

    std::shared_ptr<Object> getGlobal(const glm::vec3& position) const {
        return this->getLocal(position - this->position);
    }

    std::vector<std::shared_ptr<Object>> getObjects() const {
        return this->objects;
    };

    // std::vector<Quad> greedy_mesh_face(int direction) const;
    ~Chunk() {
        printf("Chunk is dead");
    }
};

class World {
    std::vector<std::shared_ptr<Chunk>> chunks;
public:
    World(std::vector<std::shared_ptr<Chunk>> chunks);
    static std::shared_ptr<World> generate(const unsigned int size);


    glm::vec3 getWorldCenter();
    void setObject(std::shared_ptr<Object> object);

    std::vector<std::shared_ptr<Chunk>> getChunks();

};
