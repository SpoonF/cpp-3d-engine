#include "engine/world.h"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <FastNoise/FastNoise.h>

#include "engine/entity/block.h"


// std::vector<std::vector<int>> genNoise(const unsigned int size) {

//     std::vector<std::vector<int>> map = std::vector(size * CHUNK_WIDTH, std::vector(size * CHUNK_WIDTH, 0));



//     for (size_t x = 0; x < size * CHUNK_WIDTH; x++)
//     {
//         for (size_t z = 0; z < size * CHUNK_WIDTH; z++)
//         {
//             map[x][z] = (int)noise->GenSingle2D(x, z, 324344);
//             printf(" %i ", map[x ][z]);
            
//         }
//         printf("\n");
//     }
//     // exit(0);

//     return map;
// }


World *World::generate(const unsigned int size)
{
    auto simplex = FastNoise::New<FastNoise::Simplex>();
    auto fractal = FastNoise::New<FastNoise::FractalFBm>();
    fractal->SetSource( simplex );
    fractal->SetOctaveCount( 5 );

    

    // Generate a 16x16x16 grid of 3D noise
    std::vector<float> noise( CHUNK_WIDTH * size * CHUNK_WIDTH * size  );
    fractal->GenUniformGrid2D( noise.data(), 0, 0, CHUNK_WIDTH * size, CHUNK_WIDTH * size, 1, 1, 1337 );

    std::vector<Chunk*> chunks; 

    int index = 0;

    for (size_t x = 0; x < size; x++)
    {
        for (size_t z = 0; z < size; z++)
        {
            Chunk *chunk = new Chunk();
            for (int c_x = 0; c_x < CHUNK_WIDTH; c_x++)
            {
                for (int c_z = 0; c_z < CHUNK_WIDTH; c_z++)
                {
                    int height = (int)(fractal->GenSingle2D(c_x + 1 * x + 1, c_z + 1 * z + 1, 1337) * 10) + 12;
                    for (int c_y = 0; c_y < height; c_y++)
                    {
                        chunk->entities.push_back(new Block(glm::vec3(c_x * 2, std::max(c_y * 2 , 0), c_z * 2)));
                        // printf("%f \n",);
                    }
                }
            }
            chunk->location = glm::vec3(x, 0, z);
            
            
            chunks.push_back(chunk);
        }
    }

    return new World(chunks);
    
}
glm::vec3 World::getWorldCenter()
{
    return glm::vec3();
}
std::vector<Chunk*> World::getChunks()
{
    return this->chunks;
}
// Chunk* World::getChunk(glm::vec2 position) {
//     return this->chunks->at(0);
// }