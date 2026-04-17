#include "engine/object/block.h"
#include "memory"

Block::Block(const glm::vec3& position) : 
    Object("./assets/models/cube.obj", "./assets/textures/ground.bmp", position, ObjectType::BLOCK) {
        printf("Create Block");
        this->shader = Shader::getInstance(
            "./shaders/block.vert",
            "./shaders/block.frag",
            ObjectType::BLOCK);
    }

std::shared_ptr<Block> Block::create(const glm::vec3& position) {
    std::shared_ptr<Block> block = std::make_shared<Block>(position);
    // instances[block->id] = block;
    return block;
}
