#pragma once

#include "../object.h"

class Block: virtual public Object {
public:
    Block(const glm::vec3& position);
    static std::shared_ptr<Shader> sharedShader;
    static std::shared_ptr<Block> create(const glm::vec3& position);
};