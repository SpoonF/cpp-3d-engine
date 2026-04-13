#include "engine/world.h"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <FastNoise/FastNoise.h>

#include "engine/entity/block.h"

World *World::generate(const unsigned int size)
{
    auto simplex = FastNoise::New<FastNoise::Simplex>();
    auto fractal = FastNoise::New<FastNoise::FractalFBm>();
    fractal->SetSource( simplex );
    fractal->SetOctaveCount( 5 );

    unsigned int world_width = CHUNK_WIDTH * size;

    // Generate a 16x16x16 grid of 3D noise
    std::vector<float> noise( world_width * world_width  );
    fractal->GenUniformGrid2D( noise.data(), 0, 0, world_width, world_width, 1, 1, 1337 );

    std::vector<Chunk*> chunks; 

    for (size_t x = 0; x < size; x++)
    {
        for (size_t z = 0; z < size; z++)
        {
            Chunk *chunk = new Chunk();
            chunk->location = glm::vec3(x, 0, z);
            chunks.push_back(chunk);
        }
    }
    World *world = new World (chunks);

    for (int x = 0; x < world_width; x++)
    {
        for (int z = 0; z < world_width; z++)
        {
            int height = (int)(fractal->GenSingle2D(x, z, 1337) * 10) + CHUNK_HEIGHT;
            for (int y = 0; y < height; y++)
            {
                world->setEntity(new Block(glm::vec3(x * 2, std::max(y * 2 , 0), z * 2)));
            }
        }
    }

    return world;
    
}


glm::vec3 World::getWorldCenter()
{
    return glm::vec3();
}
void World::setEntity(Entity* entity)
{
    glm::vec3 pos = entity->getPosition();
    for (auto &chunk : this->chunks)
    {
        if(chunk->location == glm::vec3 { (int)(pos.x / CHUNK_WIDTH / 2), 0, (int)(pos.z / CHUNK_WIDTH / 2)}) {
            chunk->entities.push_back(entity);
            break;
        }
    }
    
    
}
std::vector<Chunk*> World::getChunks()
{
    return this->chunks;
}
// Chunk* World::getChunk(glm::vec2 position) {
//     return this->chunks->at(0);
// }