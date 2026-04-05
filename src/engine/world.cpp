#include "engine/world.h"

// #include "engine/entity/block.h"

void World::generate(unsigned int size)
{
    for (size_t x = 0; x < size; x++)
    {
        for (size_t z = 0; z < size; z++)
        {
            Chunk *chunk = new Chunk();
            for (size_t c_x = 0; c_x < CHUNK_WIDTH; c_x++)
            {
                for (size_t c_z = 0; c_z < CHUNK_WIDTH; c_z++)
                {
                    for (size_t c_y = 0; c_y < CHUNK_HEIGHT; c_y++)
                    {
                        // chunk->entities.push_back(new Block(glm::vec3(c_x * 2, c_y * 2, c_y * 2)));
                        // chunk
                    }
                }
            }
            chunk->location = glm::vec2(x, z);
            
            
            this->chunks.push_back(chunk);
        }
    }
    
}
glm::vec3 World::getWorldCenter()
{
    return glm::vec3();
}
std::vector<Chunk *> World::getChunks()
{
    return this->chunks;
}
Chunk* World::getChunk(glm::vec2 position) {
    return this->chunks[0];
}
// void World::addEntity(Entity *entity)
// {
//     // entity
// }