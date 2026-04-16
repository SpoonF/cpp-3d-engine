#include "engine/object/block.h"
#include "memory"

Block::Block(const glm::vec3& position) : 
    Object("./assets/models/cube.obj", "./assets/textures/ground.bmp", position, ObjectType::BLOCK) { 
        this->shader = std::make_shared<Shader>("./shaders/block.frag","./shaders/block.vert");
    }

std::shared_ptr<Block> Block::create(const glm::vec3& position) {
    std::shared_ptr<Block> block = std::make_shared<Block>(position);
    // instances[block->id] = block;
    return block;
}
