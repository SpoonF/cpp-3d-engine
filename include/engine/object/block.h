#pragma once

#include "../object.h"

class Block: virtual public Object {
public:
    Block(const glm::vec3& position);
    static std::map<uint, std::shared_ptr<Object>> instances;
    static std::shared_ptr<Block> create(const glm::vec3& position);
};