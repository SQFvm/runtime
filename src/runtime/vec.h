#pragma once

namespace sqf::runtime
{
    struct vec3
    {
        float x;
        float y;
        float z;

        vec3() : x(0), y(0), z(0) {}
        vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        vec3(float x, float y) : x(x), y(y), z(0) {}
    };
}