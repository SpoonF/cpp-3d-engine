#pragma once


#include <glm/gtx/hash.hpp>
#include <unordered_set>

// #include "engine/scene.h"

const unsigned int CHUNK_WIDTH = 16; 
const unsigned int CHUNK_HEIGHT = 100; 


struct Quad {
    glm::vec3 pos;
    int width;
    int height;
    std::shared_ptr<Object3D> object;

    Quad(glm::vec3 pos, int width, int height, std::shared_ptr<Object3D> object) : pos(pos), width(width), height(height), object(object) {};
};

class Chunk {
private:
    std::vector<std::shared_ptr<Entity>> grid;
public:
    std::map<std::shared_ptr<Object3D>, std::vector<glm::vec3>> objects;
    glm::vec3 location;

    Chunk() {
        grid.resize(CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH, nullptr);
    }

    void setLocal(glm::vec3 position, std::shared_ptr<Entity> entity) {
        if(position.x >= 0 && position.x < CHUNK_WIDTH && 
            position.y >= 0 && position.y < CHUNK_HEIGHT && 
            position.z >= 0 && position.z < CHUNK_WIDTH) {
            grid[position.x + CHUNK_WIDTH * (position.y + CHUNK_HEIGHT * position.z)] = entity;

            objects[entity->getObject()].push_back(entity->getPosition());
        }
    }

    std::shared_ptr<Entity> getLocal(glm::vec3 position) const {
        if(position.x < 0 || position.x >= CHUNK_WIDTH || 
            position.y < 0 || position.y >= CHUNK_HEIGHT || 
            position.z < 0 || position.z >= CHUNK_WIDTH) {
            return nullptr;
        }
        return grid[position.x + CHUNK_WIDTH * (position.y + CHUNK_HEIGHT * position.z)];
    }

    std::shared_ptr<Entity> getLocal(int x, int y, int z) const {
        if(x < 0 || x >= CHUNK_WIDTH || 
            y < 0 || y >= CHUNK_HEIGHT || 
            z < 0 || z >= CHUNK_WIDTH) {
            return nullptr;
        }
        return grid[x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z)];
    }

    std::shared_ptr<Entity> getGlobal(glm::vec3 position) {
        position = glm::mod(position, this->location);
        return this->getLocal(position);
    }

    std::vector<std::shared_ptr<Entity>> getEntities() const {
        return this->grid;
    };

    std::vector<Quad> greedy_mesh_face(int direction) const;
};

class World {
    std::vector<std::shared_ptr<Chunk>> chunks;
public:
    World(std::vector<std::shared_ptr<Chunk>>& chunks);
    static std::shared_ptr<World> generate(const unsigned int size);


    glm::vec3 getWorldCenter();
    void setEntity(std::shared_ptr<Entity> entity);

    std::vector<std::shared_ptr<Chunk>> getChunks();

};
