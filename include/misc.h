#pragma once

namespace Misc
{
    const double PI = 3.141592653589793238463;

    inline double rad2deg(double angle) { return angle / PI * 180.0; }

    inline double deg2rad(double angle) { return angle / 180.0 * PI; }

    inline double nmod(double value, double mod)
    {
        return std::fmod(std::fmod(value, mod) + mod, mod);
        // value can goes under 0
    }

    inline float cross(float x1, float y1, float x2, float y2, float px, float py)
    {
        return (x2 - x1) * (py - y1) - (y2 - y1) * (px - x1);
        // result > 0: p is left
        // result = 0: p is on line
        // result < 0: p is right
    }

    inline bool pointInTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
    {
        return
            cross(x1, y1, x2, y2, x, y) <= 0 &&
            cross(x2, y2, x3, y3, x, y) <= 0 &&
            cross(x3, y3, x1, y1, x, y) <= 0;
    }

/*
    bool getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
    {
        float s1_x = p1_x - p0_x;
        float s1_y = p1_y - p0_y;
        float s2_x = p3_x - p2_x;
        float s2_y = p3_y - p2_y;

        float den = (-s2_x * s1_y + s1_x * s2_y)
        float s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / den;
        float t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / den;

        if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
        {
            *i_x = p0_x + (t * s1_x);
            *i_y = p0_y + (t * s1_y);
            return true;
        }
        return false;
    }
*/

    bool lineIntersectTriangle(float lx1, float ly1, float lx2, float ly2, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3)
    {
        bool t1 = cross(lx1, ly1, lx2, ly2, tx1, ty1) >= 0;
        bool t2 = cross(lx1, ly1, lx2, ly2, tx2, ty2) >= 0;
        bool t3 = cross(lx1, ly1, lx2, ly2, tx3, ty3) >= 0;
        if (t1 == t2 && t2 == t3) return false;

        bool l1 = cross(tx1, ty1, tx2, ty2, lx1, ly1) >= 0;
        bool l2 = cross(tx1, ty1, tx2, ty2, lx2, ly2) >= 0;
        t3 = cross(tx1, ty1, tx2, ty2, tx3, ty3) >= 0;
        if (l1 == l2 && l2 != t3) return false;

        l1 = cross(tx2, ty2, tx3, ty3, lx1, ly1) >= 0;
        l2 = cross(tx2, ty2, tx3, ty3, lx2, ly2) >= 0;
        t1 = cross(tx2, ty2, tx3, ty3, tx1, ty1) >= 0;
        if (l1 == l2 && l2 != t1) return false;

        l1 = cross(tx3, ty3, tx1, ty1, lx1, ly1) >= 0;
        l2 = cross(tx3, ty3, tx1, ty1, lx2, ly2) >= 0;
        t2 = cross(tx3, ty3, tx1, ty1, tx2, ty2) >= 0;
        if (l1 == l2 && l2 != t2) return false;

        return true;
    }

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

    bool rayIntersection(float line_x1, float line_y1, float line_x2, float line_y2, float ray_dir_angle, float* ix, float* iy) // ray starts from (0, 0)
    {
        float line_dx = line_x2 - line_x1;
        float line_dy = line_y2 - line_y1;

        float ray_dir_x = sin(ray_dir_angle);
        float ray_dir_y = cos(ray_dir_angle);

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

    bool pointInPolygon(int polygon_point_count, float* polygon_x, float* polygon_y, float x, float y)
    {
        double minX = polygon_x[0];
        double maxX = polygon_x[0];
        double minY = polygon_y[0];
        double maxY = polygon_y[0];
        for (int i = 1 ; i < polygon_point_count; i++) {
            if (minX > polygon_x[i]) minX = polygon_x[i];
            if (maxX < polygon_x[i]) maxX = polygon_x[i];
            if (minY > polygon_y[i]) minY = polygon_y[i];
            if (maxY < polygon_y[i]) maxY = polygon_y[i];
        }
        if (x < minX || x > maxX || y < minY || y > maxY) return false;

        // https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
        bool result = false;
        for (int i = 0, j = polygon_point_count - 1; i < polygon_point_count; j = i++) {
            if ( ((polygon_y[i] > y) != (polygon_y[j] > y)) &&
                 (x < (polygon_x[j] - polygon_x[i]) * (y - polygon_y[i]) / (polygon_y[j] - polygon_y[i]) + polygon_x[i])
            ) {
                result = !result;
            }
        }

        return result;
    }
}
