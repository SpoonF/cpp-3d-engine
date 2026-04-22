#pragma once


#include <glm/gtx/hash.hpp>
#include <unordered_set>
#include <map>

#include "engine/object/block.h"

// class Block;


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
    std::vector<Block*> blocks;
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
    void setLocal(const glm::vec3& position, Block *block) {
        if(position.x >= 0 && position.x < CHUNK_WIDTH && position.y >= 0 && position.y < CHUNK_HEIGHT && position.z >= 0 && position.z < CHUNK_WIDTH) {

            this->setObject(block);
            if(positions.count(block->getType())) {
                positions[block->getType()].push_back(block->getPosition());
            } else {
                positions[block->getType()] = {block->getPosition()};
            }
            
        }
    }

    std::unordered_map<ObjectType, std::vector<glm::vec3>> getPositions() const {
        return this->positions;
    };

    void setObject(Block *object) {
        this->blocks.push_back(object);
    };
    std::vector<Block*> getBlocks() {
        return this->blocks;
    }
    void deletBlock(Block *object) {
        std::erase(this->positions[object->getType()], object->getPosition());
        std::erase(this->blocks, object);
        delete object;
    }
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
    void setObject(Block* object);
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

    Block* getBlock(const glm::vec3& position) {
        Chunk* chunk = this->getChunk(position);
        for (auto block : chunk->getBlocks())
        {
            if(block->check(position)) {
                return block;
            }
        }

        return nullptr;
    }

    void deleteBlock(Block* object) {
        Chunk* chunk = this->getChunk(object->getPosition());
        for (auto block : chunk->getBlocks())
        {
            if(block == object) {
                chunk->deletBlock(block);
            }
        }
    }


    std::vector<std::shared_ptr<Chunk>> getChunks();

protected:
    void setSize(const unsigned int size);

};
