#pragma once

namespace Misc
{
    const double PI = 3.141592653589793238463;

    bool lineIntersection(float line_x1, float line_y1, float line_x2, float line_y2, float ray_x1, float ray_y1, float ray_x2, float ray_y2, float* ix, float* iy)
    {
        float den = (line_x1 - line_x2) * (ray_y1 - ray_y2) - (line_y1 - line_y2) * (ray_x1 - ray_x2);
        if (den == 0) return false;

        float t = ((line_x1 - ray_x1) * (ray_y1 - ray_y2) - (line_y1 - ray_y1) * (ray_x1 - ray_x2)) / den;
        if (0 > t || t > 1) return false;

        float u = ((line_x1 - line_x2) * (line_y1 - ray_y1) - (line_y1 - line_y2) * (line_x1 - ray_x1)) / -den;
        if (0 > u || u > 1) return false;

        *ix = line_x1 + (line_x2 - line_x1) * t;
        *iy = line_y1 + (line_y2 - line_y1) * t;
        return true;
    }

    bool rayIntersection(float line_x1, float line_y1, float line_x2, float line_y2, float ray_dir_x, float ray_dir_y, float* ix, float* iy) // ray starts from (0, 0)
    {
        float line_dx = line_x2 - line_x1;
        float line_dy = line_y2 - line_y1;

        float den = line_dx * ray_dir_y - line_dy * ray_dir_x;
        if (den == 0) return false;

        float t = (line_x1 * ray_dir_y - line_y1 * ray_dir_x) / -den;
        if (0 > t || t > 1) return false;

        float u = (line_dx * line_y1 - line_dy * line_x1) / den;
        if (0 > u) return false;

        *ix = line_x1 + line_dx * t;
        *iy = line_y1 + line_dy * t;
        return true;
    }
}
