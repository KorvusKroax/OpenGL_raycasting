#include "camera.h"

Camera::Camera(Point pos, float height, float heading, float pitch, float fov)
{
    this->pos = pos;
    this->height = height;
    this->heading = heading;
    this->pitch = pitch;
    this->fov = fov;
}

Camera::~Camera() {}

void Camera::update(OpenGL *openGL, Mouse *mouse)
{
    if (glfwGetKey(openGL->window, GLFW_KEY_W) == GLFW_PRESS) {
        pos.x += sin(this->heading) * moveSpeed * openGL->deltaTime;
        pos.y += cos(this->heading) * moveSpeed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_S) == GLFW_PRESS) {
        pos.x -= sin(this->heading) * moveSpeed * openGL->deltaTime;
        pos.y -= cos(this->heading) * moveSpeed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_A) == GLFW_PRESS) {
        pos.x -= cos(this->heading) * moveSpeed * openGL->deltaTime;
        pos.y += sin(this->heading) * moveSpeed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_D) == GLFW_PRESS) {
        pos.x += cos(this->heading) * moveSpeed * openGL->deltaTime;
        pos.y -= sin(this->heading) * moveSpeed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_Q) == GLFW_PRESS) {
        height -= moveSpeed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_E) == GLFW_PRESS) {
        height += moveSpeed * openGL->deltaTime;
    }

    if (glfwGetKey(openGL->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        heading -= turnSpeed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        heading += turnSpeed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_UP) == GLFW_PRESS) {
        pitch -= pitchSpeed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        pitch += pitchSpeed * openGL->deltaTime;
    }

    heading += mouse->xDelta * turnSpeed * mouse->mouseSensivity * openGL->deltaTime;
    pitch += mouse->yDelta * pitchSpeed * mouse->mouseSensivity * openGL->deltaTime;

    if (pitch > maxPitch) pitch = maxPitch;
    if (pitch < -maxPitch) pitch = -maxPitch;
}

void Camera::render(Canvas *canvas, World *world)
{
    Sector *currentSector = nullptr;
    for (Sector sector : world->sectors) {
        if (pointInSector(world, &sector)) {
            currentSector = &sector;
            break;
        }
    }
    if (currentSector == nullptr) return;

    float rayAngle = -fov * .5f;
    float rayAngle_step = fov / canvas->width;
    float nearClip = (canvas->width >> 1) / tan(fov * .5f);
    for (int col = 0; col < canvas->width; col++) {
        render_column(canvas, col, world, currentSector, rayAngle, nearClip, 0, canvas->height);
        rayAngle += rayAngle_step;
    }
}

void Camera::render_column(Canvas *canvas, int col, World *world, Sector *sector, float rayAngle, float nearClip, int portalSlice_bottom, int portalSlice_top)
{
    if (portalSlice_bottom > canvas->height - 1 || portalSlice_top < 0) return;

    Wall *currentWall = nullptr;
    Point intersection;
    float intersectionDistance_sqr = -1;

    for (int i = 0; i < sector->wallCount; i++) {
        Wall *wall = &world->walls[sector->walls[i]];

        if (backfaceCulling(&world->points[wall->start], &world->points[wall->end])) continue;

        Point start = (world->points[wall->start] - this->pos).rotate(this->heading);
        Point end = (world->points[wall->end] - this->pos).rotate(this->heading);

        Point ip;
        float id;
        if (rayIntersection(&start, &end, rayAngle, &ip)) {
            id = ip.x * ip.x + ip.y * ip.y;
            if (intersectionDistance_sqr == -1 || id < intersectionDistance_sqr) {
                currentWall = wall;
                intersection = ip;
                intersectionDistance_sqr = id;
            }
        }
    }

    if (currentWall != nullptr) {

        float horizontLine = (canvas->height >> 1) + this->pitch;

        float intersectionDistance = sqrt(intersectionDistance_sqr);
        float wallSlice_distance = intersectionDistance * cos(rayAngle);

        float wallSlice_bottom = horizontLine - (this->height - sector->bottom) / wallSlice_distance * nearClip;
        float wallSlice_top = horizontLine + (sector->top - this->height) / wallSlice_distance * nearClip;

        if (currentWall->portalTo == -1) {
            render_wallSlice(canvas, col, world, currentWall, sector->top - sector->bottom, wallSlice_bottom, wallSlice_top, portalSlice_bottom, portalSlice_top, intersection);
        } else {
            Sector *otherSector = &world->sectors[currentWall->portalTo];

            float portal_bottom = wallSlice_bottom;
            float bottomWall_height = otherSector->bottom - sector->bottom;
            if (bottomWall_height > 0) {
                portal_bottom = horizontLine - (this->height - otherSector->bottom) / wallSlice_distance * nearClip;
                render_wallSlice(canvas, col, world, currentWall, bottomWall_height, wallSlice_bottom, portal_bottom, portalSlice_bottom, portalSlice_top, intersection);
            }
            portal_bottom = portal_bottom > portalSlice_bottom ? portal_bottom : portalSlice_bottom;

            float portal_top = wallSlice_top;
            float topWall_height = sector->top - otherSector->top;
            if (topWall_height > 0) {
                portal_top = horizontLine + (otherSector->top - this->height) / wallSlice_distance * nearClip;
                render_wallSlice(canvas, col, world, currentWall, topWall_height, portal_top, wallSlice_top, portalSlice_bottom, portalSlice_top, intersection);
            }
            portal_top = portal_top < portalSlice_top ? portal_top : portalSlice_top;

            render_column(canvas, col, world, otherSector, rayAngle, nearClip, (int)portal_bottom, (int)portal_top);
        }

        render_floor(canvas, col, world, sector, rayAngle, nearClip, portalSlice_bottom, portalSlice_top, wallSlice_bottom);

        render_ceiling(canvas, col, world, sector, rayAngle, nearClip, portalSlice_bottom, portalSlice_top, wallSlice_top);
    }
}

void Camera::render_wallSlice(Canvas *canvas, int col, World *world, Wall *wall, float wall_height, float wallSlice_bottom, float wallSlice_top, int portalSlice_bottom, int portalSlice_top, Point intersection)
{
    int wallSlice_clipped_bottom = wallSlice_bottom > portalSlice_bottom ? wallSlice_bottom : portalSlice_bottom;
    int wallSlice_clipped_top = wallSlice_top < portalSlice_top ? wallSlice_top : portalSlice_top;

    Point t_pos = (world->points[wall->start] - pos).rotate(heading);
    Point t_diff = intersection - t_pos;

    float t_step = wall_height / (wallSlice_top - wallSlice_bottom);
    Point t_slice = Point(
        fmod(sqrt(t_diff.x * t_diff.x + t_diff.y * t_diff.y), world->textures[wall->texture].width),
        wallSlice_bottom < wallSlice_clipped_bottom ?
            (portalSlice_bottom - wallSlice_bottom) * t_step : 0
    );

    for (int row = wallSlice_clipped_bottom; row <= wallSlice_clipped_top; row++) {
        Color color = world->textures[wall->texture].pixels[
            (int)t_slice.x + (int)fmod(t_slice.y + t_step * (row - wallSlice_clipped_bottom), world->textures[wall->texture].height) * world->textures[wall->texture].width
        ];
        canvas->setPixel(col, row, color);
    }
}

void Camera::render_floor(Canvas *canvas, int col, World *world, Sector *sector, float rayAngle, float nearClip, int portalSlice_bottom, int portalSlice_top, float wallSlice_bottom)
{
    wallSlice_bottom--;

    if (wallSlice_bottom < 0) return;

    int portalSlice_clipped_bottom = portalSlice_bottom > 0 ? portalSlice_bottom : 0;
    int wallSlice_clipped_bottom = wallSlice_bottom < portalSlice_top ? wallSlice_bottom : portalSlice_top;

    float horizontLine = (canvas->height >> 1) + this->pitch;

    for (int row = portalSlice_clipped_bottom; row <= wallSlice_clipped_bottom; row++) {
        float t_dist = (this->height - sector->bottom) / (row - horizontLine) * nearClip / cos(rayAngle);
        Point t_pos = Point(
            nmod(pos.x - t_dist * sin(heading + rayAngle), world->textures[sector->floorTexture].width),
            nmod(pos.y - t_dist * cos(heading + rayAngle), world->textures[sector->floorTexture].height)
        );

        Color color = world->textures[sector->floorTexture].pixels[
            (int)t_pos.x + (int)t_pos.y * world->textures[sector->floorTexture].width
        ];
        canvas->setPixel(col, row, color);
    }
}

void Camera::render_ceiling(Canvas *canvas, int col, World *world, Sector *sector, float rayAngle, float nearClip, int portalSlice_bottom, int portalSlice_top, float wallSlice_top)
{
    wallSlice_top++;

    if (wallSlice_top > canvas->height) return;

    int portalSlice_clipped_top = portalSlice_top < canvas->height ? portalSlice_top : canvas->height;
    int wallSlice_clipped_top = wallSlice_top > portalSlice_bottom ? wallSlice_top : portalSlice_bottom;

    float horizontLine = (canvas->height >> 1) + this->pitch;

    for (int row = wallSlice_clipped_top; row <= portalSlice_clipped_top; row++) {
        float t_dist = ((sector->top - sector->bottom) - (this->height - sector->bottom)) / (horizontLine - row) * nearClip / cos(rayAngle);
        Point t_pos = Point(
            nmod(pos.x - t_dist * sin(heading + rayAngle), world->textures[sector->ceilingTexture].width),
            nmod(pos.y - t_dist * cos(heading + rayAngle), world->textures[sector->ceilingTexture].height)
        );

        Color color = world->textures[sector->ceilingTexture].pixels[
            (int)t_pos.x + (int)t_pos.y * world->textures[sector->ceilingTexture].width
        ];
        canvas->setPixel(col, row, color);
    }
}



bool Camera::pointInSector(World *world, Sector *sector)
{
    double minX = world->points[world->walls[sector->walls[0]].start].x;
    double minY = world->points[world->walls[sector->walls[0]].start].y;
    double maxX = world->points[world->walls[sector->walls[0]].start].x;
    double maxY = world->points[world->walls[sector->walls[0]].start].y;
    for (int i = 1; i < sector->wallCount; i++) {
        if (minX > world->points[world->walls[sector->walls[i]].start].x) minX = world->points[world->walls[sector->walls[i]].start].x;
        if (minY > world->points[world->walls[sector->walls[i]].start].y) minY = world->points[world->walls[sector->walls[i]].start].y;
        if (maxX < world->points[world->walls[sector->walls[i]].start].x) maxX = world->points[world->walls[sector->walls[i]].start].x;
        if (maxY < world->points[world->walls[sector->walls[i]].start].y) maxY = world->points[world->walls[sector->walls[i]].start].y;
    }
    if (this->pos.x < minX || this->pos.x > maxX || this->pos.y < minY || this->pos.y > maxY) return false;

    // https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
    bool result = false;
    for (int i = 0; i < sector->wallCount; i++) {
        Point wall_start = world->points[world->walls[sector->walls[i]].start];
        Point wall_end = world->points[world->walls[sector->walls[i]].end];
        Point diff = wall_end - wall_start;
        if (((wall_start.y > this->pos.y) != (wall_end.y > this->pos.y)) &&
            (diff.x * (this->pos.y - wall_start.y) / diff.y + wall_start.x) > this->pos.x) {
                result = !result;
        }
    }

    return result;
}

bool Camera::backfaceCulling(Point *start, Point *end)
{
    float x1 = start->x - this->pos.x;
    float y1 = start->y - this->pos.y;
    float x2 = end->x - this->pos.x;
    float y2 = end->y - this->pos.y;

    return (y2 - y1) * x1 - (x2 - x1) * y1 >= 0;
}

// inline float cross(float x1, float y1, float x2, float y2, float px, float py)
// {
//     return (x2 - x1) * (py - y1) - (y2 - y1) * (px - x1);
//     // result > 0: p is left
//     // result = 0: p is on line
//     // result < 0: p is right
// }

bool Camera::rayIntersection(Point *start, Point *end, float rayAngle, Point *intersection)
{
    Point diff = *end - *start;
    Point rayDir = Point(sin(rayAngle), cos(rayAngle));

    float den = diff.x * rayDir.y - diff.y * rayDir.x;
    if (den == 0) return false;

    float t = (start->x * rayDir.y - start->y * rayDir.x) / -den;
    if (0 > t || t > 1) return false;

    float u = (diff.x * start->y - diff.y * start->x) / den;
    if (0 > u) return false;

    *intersection = diff * t + *start;

    return true;
}

// bool getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
// {
//     float s1_x = p1_x - p0_x;
//     float s1_y = p1_y - p0_y;
//     float s2_x = p3_x - p2_x;
//     float s2_y = p3_y - p2_y;

//     float den = (-s2_x * s1_y + s1_x * s2_y)
//     float s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / den;
//     float t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / den;

//     if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
//     {
//         *i_x = p0_x + (t * s1_x);
//         *i_y = p0_y + (t * s1_y);
//         return true;
//     }
//     return false;
// }

inline double Camera::nmod(double value, double mod) // value can goes under 0
{
    return std::fmod(std::fmod(value, mod) + mod, mod);
}
