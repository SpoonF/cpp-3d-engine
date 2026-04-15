#include "engine/world.h"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <FastNoise/FastNoise.h>

#include "engine/entity/block.h"
#include <cstring>

World::World(std::vector<std::shared_ptr<Chunk>>& chunks) {
    this->chunks = chunks;
}

std::shared_ptr<World> World::generate(const unsigned int size)
{
    auto simplex = FastNoise::New<FastNoise::Simplex>();
    auto fractal = FastNoise::New<FastNoise::FractalFBm>();
    fractal->SetSource( simplex );
    fractal->SetOctaveCount( 5 );

    unsigned int world_width = CHUNK_WIDTH * size;

    // Generate a 16x16x16 grid of 3D noise
    std::vector<float> noise( world_width * world_width  );
    fractal->GenUniformGrid2D( noise.data(), 0, 0, world_width, world_width, 1, 1, 1337 );

    std::vector<std::shared_ptr<Chunk>> chunks; 

    for (size_t x = 0; x < size; x++)
    {
        for (size_t z = 0; z < size; z++)
        {
            std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();
            chunk->location = glm::vec3(x, 0, z);
            chunks.push_back(chunk);
        }
    }

    std::shared_ptr<World> world = std::make_shared<World>(chunks);

    for (int x = 0; x < world_width; x++)
    {
        for (int z = 0; z < world_width; z++)
        {
            int height = (int)(fractal->GenSingle2D(x, z, 1337) * 16) + 16;
            for (int y = 0; y < height; y++)
            {
                world->setEntity(std::make_shared<Block>(glm::vec3(x * 2, std::max(y * 2 , 0), z * 2)));
            }
        }
    }

    return world;
}


glm::vec3 World::getWorldCenter()
{
    return glm::vec3();
}
void World::setEntity(std::shared_ptr<Entity> entity)
{
    glm::vec3 pos = entity->getPosition();
    for (auto &chunk : this->chunks)
    {
        if(chunk->location == glm::vec3 { (int)(pos.x / CHUNK_WIDTH / 2), 0, (int)(pos.z / CHUNK_WIDTH / 2)}) {
            chunk->setLocal(glm::vec3(
                (pos.x - (chunk->location.x * CHUNK_WIDTH * 2)) / 2,
                pos.y,
                (pos.z - (chunk->location.z * CHUNK_WIDTH * 2)) / 2
            ), entity);
            
            break;
        }
    }
}
std::vector<std::shared_ptr<Chunk>> World::getChunks()
{
    return this->chunks;
}



std::vector<Quad> Chunk::greedy_mesh_face(int direction) const {
    std::vector<Quad> quads;
    int dimX = CHUNK_WIDTH;
    int dimY = CHUNK_HEIGHT;
    int dimZ = CHUNK_WIDTH;


    std::vector<uint8_t> mask(dimX * dimZ);

    // int startY = (direction == 1) ? dimY - 1 : 0;
    // int endY = (direction == 1) ? -1 : dimY;
    // int stepY = (direction == 1) ? -1 : 1;


    const std::shared_ptr<Entity>* data = this->grid.data();

    const int strideY = dimX;
    const int strideZ = dimX * dimY;

    for (int y = 0; y < dimY; ++y)
    {
        memset(mask.data(), 0, dimX * dimZ * sizeof(uint8_t));

        int nextY = y + direction;
        if(nextY < 0 || nextY >= dimY) continue;

        for (int z = 0; z < dimZ; ++z) {
            for (int x = 0; x < dimX; ++x)
            {
                int maskIds = x + z * dimX;
                if(mask[maskIds]) continue;

                int currentIdx = x + y * strideY + z * strideZ;
                int nextIdx = currentIdx + strideY;

                auto currentBlock = data[currentIdx];
                auto nextBlock = data[nextIdx];

                if (currentBlock != nullptr && nextBlock == nullptr)
                {
                    std::shared_ptr<Object3D> object = currentBlock->getObject();

                    int width = 1;

                    while (x + width < dimX)
                    {
                        auto neighborBlock = this->getLocal(x + width, y, z);
                        auto neighborNextBlock = this->getLocal(x + width, nextY, z);
                        auto neighborMaskIds = (x + width) + z * dimX;

                        if(!mask[neighborMaskIds] &&
                            (neighborBlock != nullptr && neighborBlock->getObject() == object) &&
                            neighborNextBlock == nullptr) {
                            width++;
                        } else {
                            break;
                        }
                    }

                    int height = 1;
                    bool canExtendHeight = true;

                    while (z + height < dimZ && canExtendHeight) {
                        for (int dx = 0; dx < width; ++dx)
                        {
                            int checkX = x + dx;
                            int checkZ = z + height;

                            auto checkBlock = this->getLocal(glm::vec3(checkX, y, checkZ));
                            auto checkNextBlock = this->getLocal(glm::vec3(checkX, nextY, checkZ));
                            auto checkMaskIdx = checkX + checkZ * dimX;

                            if(mask[checkMaskIdx] ||
                                (checkBlock != nullptr && checkBlock->getObject() != object) ||
                                checkNextBlock != nullptr) {
                                canExtendHeight = false;
                                break;
                            }
                        }
                        if(canExtendHeight) {
                            height++;
                        }
                        
                    }

                    for (int dz = 0; dz < height; ++dz)
                    {
                        for (int dx = 0; dx < width; ++dx)
                        {

                            mask[(x+dx) + (z+dz) * dimX] = 1;
                        }
                        
                    }

                    glm::vec3 quadPos;
                    if(direction == 1) {
                        quadPos = glm::vec3(x, y + 1, z);
                    } else {
                        quadPos = glm::vec3(x, y, z);
                    }
                    quadPos = glm::vec3(quadPos.x * 2 + (this->location.x * CHUNK_WIDTH * 2), quadPos.y * 2, quadPos.z * 2 + (this->location.z * CHUNK_WIDTH * 2));
                    quads.emplace_back(quadPos, width * 2, height * 2, object);
                }
                
            }
            
        }
        
        
    }

    return quads;
    
}