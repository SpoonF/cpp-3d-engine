#pragma once


#include <glm/gtx/hash.hpp>
#include <unordered_set>
#include <map>

// #include "engine/object.h"

class Object;


const unsigned int CHUNK_WIDTH = 16; 
const unsigned int CHUNK_HEIGHT = 100; 


// struct Quad {
//     glm::vec3 pos;
//     int width;
//     int height;
//     std::shared_ptr<Object> object;

//     Quad(glm::vec3 pos, int width, int height, std::shared_ptr<Object> object) : pos(pos), width(width), height(height), object(object) {};
// };

class Chunk {
private:
    // std::vector<int> loc_objects;
    // std::unordered_map<int, std::shared_ptr<Object>> objects;
    std::unordered_map<ObjectType, std::vector<glm::vec3>> positions;
public:
    glm::vec3 position;

    void setLocal(const glm::vec3& position, std::pair<ObjectType, glm::vec3> object) {
        if(position.x >= 0 && position.x < CHUNK_WIDTH && position.y >= 0 && position.y < CHUNK_HEIGHT && position.z >= 0 && position.z < CHUNK_WIDTH) {

            // loc_objects[position.x + CHUNK_WIDTH * (position.y + CHUNK_HEIGHT * position.z)] = object->getId();
            // objects.insert({object->getId(), object});

            if(positions.count(std::get<0>(object))) {
                positions[std::get<0>(object)].push_back(std::get<1>(object));
            } else {
                positions[std::get<0>(object)] = {std::get<1>(object)};
            }
            
        }
    }

    std::unordered_map<ObjectType, std::vector<glm::vec3>> getPositions() const {
        return this->positions;
    };
};

class World {
    std::vector<std::shared_ptr<Chunk>> chunks;
    std::vector<Object*> lights;
    unsigned int size;
public:
    World(std::vector<std::shared_ptr<Chunk>> chunks);
    static std::shared_ptr<World> generate(const unsigned int size);


    glm::vec3 getWorldCenter();
    void setObject(ObjectType type, const glm::vec3& position);
    void addLight(Object* lightObject);
    std::vector<Object*> getLights() {
        return lights;
    }

    Chunk* getChunk(const glm::vec3& position) {
        for (auto &chunk : this->chunks)
        {
            glm::vec3 locPos = position - chunk->position;
            if(locPos.x <= CHUNK_WIDTH * 2 && 
                locPos.x >= 0 &&
                locPos.z <= CHUNK_WIDTH * 2 &&
                locPos.z >= 0) {
                return chunk.get();
            }
        }

        return nullptr;
    }


    std::vector<std::shared_ptr<Chunk>> getChunks();

protected:
    void setSize(const unsigned int size);

};
