#pragma once

#define _HAS_EXCEPTIONS 0
#define SPDLOG_NO_EXCEPTIONS

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <array>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

#include <SDL.h>
#include <SDL_mixer.h>
#include "Logger/Logger.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4x4 = glm::mat4x4;
using Color = glm::u8vec4;
