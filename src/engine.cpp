#define GLM_ENABLE_EXPERIMENTAL
#include <cassert>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string.h>
#include <queue>
#include <functional>
#include <map>
#include <algorithm>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>



#include "config.h"

#include "utils/imageBMP.cpp"
#include "utils/model.cpp"

#include "engine/camera.cpp"
#include "engine/shader.cpp"
#include "engine/object.cpp"
#include "engine/object/block.cpp"
#include "engine/world.cpp"
#include "engine/frustum.cpp"
#include "engine/collision.cpp"
#include "engine/collision_box.cpp"
#include "engine/collision_sphere.cpp"
#include "engine/scene.cpp"





