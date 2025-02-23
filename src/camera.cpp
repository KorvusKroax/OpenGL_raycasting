#include "camera.h"

Camera::Camera(Point pos, float height, float heading, float pitch, float fov)
{
    this->pos = pos;
    this->height = height;
    this->heading = heading;
    this->pitch = pitch;
    this->fov = fov;
}

Camera::~Camera() { }

void Camera::update(OpenGL* openGL, Mouse* mouse)
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

    heading += mouse->xDelta * mouse->mouseSensivity * openGL->deltaTime;

    pitch += mouse->yDelta * mouse->mouseSensivity * openGL->deltaTime * pitchSpeed;
    if (pitch < -maxPitch) pitch = -maxPitch;
    if (pitch > maxPitch) pitch = maxPitch;
}

void Camera::render(Canvas* canvas, World* world)
{
    float nearClip = (canvas->width >> 1) / tan(fov * .5f);

    Sector* currentSector = nullptr;
    for (Sector sector : world->sectors) {
        if (sector.pointInSector(this->pos)) {
            currentSector = &sector;
            break;
        }
    }

    if (currentSector == nullptr) return;

    float rayAngle = -fov * .5f;
    float rayAngle_step = fov / canvas->width;
    for (int col = 0; col < canvas->width; col++) {
        render_column(canvas, col, world, currentSector, rayAngle, nearClip, 0, canvas->height);
        rayAngle += rayAngle_step;
    }
}

void Camera::render_column(Canvas* canvas, int col, World* world, Sector* sector, float rayAngle, float nearClip, float portalSlice_bottom, float portalSlice_top)
{
    if (portalSlice_bottom > canvas->height - 1 || portalSlice_top < 0) return;

    Wall* currentWall = nullptr;
    Point intersection;
    float intersectionDistance_sqr = std::numeric_limits<float>::max();

    for (int i = 0; i < sector->wallCount; i++) {
        Wall* wall = sector->walls[i];

        if (wall->backfaceCulling(this->pos)) continue;

        Point start = (*wall->start - this->pos).rotate(this->heading);
        Point end = (*wall->end - this->pos).rotate(this->heading);

        Point ip;
        float id;
        if (rayIntersection(&start, &end, rayAngle, &ip)) {
            id = ip.x * ip.x + ip.y * ip.y;
            if (id < intersectionDistance_sqr) {
                currentWall = wall;
                intersection = ip;
                intersectionDistance_sqr = id;
            }
        }
    }

    if (currentWall != nullptr) {
        float horizontLine = (canvas->height >> 1) + pitch;

        float intersectionDistance = sqrt(intersectionDistance_sqr);
        float wallSlice_distance = intersectionDistance * cos(rayAngle);

        float wallSlice_bottom = horizontLine - (height - sector->bottom) / wallSlice_distance * nearClip;
        float wallSlice_top = horizontLine + (sector->top - height) / wallSlice_distance * nearClip;

        if (currentWall->portalTo == -1) {
            render_wallSlice(canvas, col, currentWall, sector->top - sector->bottom, wallSlice_bottom, wallSlice_top, portalSlice_bottom, portalSlice_top, &intersection);//, intersectionDistance);
        } else {
            Sector* otherSector = &world->sectors[currentWall->portalTo];
            float portal_bottom = wallSlice_bottom;
            float bottomWall_height = otherSector->bottom - sector->bottom;
            if (bottomWall_height > 0) {
                portal_bottom = horizontLine - (height - otherSector->bottom) / wallSlice_distance * nearClip;
                render_wallSlice(canvas, col, currentWall, bottomWall_height, wallSlice_bottom, portal_bottom, portalSlice_bottom, portalSlice_top, &intersection);//, intersectionDistance);
            }
            portal_bottom = portal_bottom > portalSlice_bottom ? portal_bottom : portalSlice_bottom;

            float portal_top = wallSlice_top;
            float topWall_height = sector->top - otherSector->top;
            if (topWall_height > 0) {
                portal_top = horizontLine + (otherSector->top - height) / wallSlice_distance * nearClip;
                render_wallSlice(canvas, col, currentWall, topWall_height, portal_top, wallSlice_top, portalSlice_bottom, portalSlice_top, &intersection);//, intersectionDistance);
            }
            portal_top = portal_top < portalSlice_top ? portal_top : portalSlice_top;

            render_column(canvas, col, world, otherSector, rayAngle, nearClip, portal_bottom, portal_top);
        }

        render_floor(canvas, col, sector, rayAngle, nearClip, portalSlice_bottom, portalSlice_top, wallSlice_bottom);

        render_ceiling(canvas, col, sector, rayAngle, nearClip, portalSlice_bottom, portalSlice_top, wallSlice_top);
    }
}

void Camera::render_wallSlice(Canvas* canvas, int col, Wall* wall, float wall_height, float wallSlice_bottom, float wallSlice_top, float portal_bottom, float portal_top, Point* intersection)//, float intersectionDistance)
{
    int wallSlice_clipped_bottom = wallSlice_bottom > portal_bottom ? wallSlice_bottom : portal_bottom;
    int wallSlice_clipped_top = wallSlice_top < portal_top ? wallSlice_top : portal_top;

    Point t_pos = (*wall->start - pos).rotate(heading);
    Point t_diff = (*intersection - t_pos);

    float t_step =  wall_height / (wallSlice_top - wallSlice_bottom);
    Point t_slice = Point(
        fmod(sqrt(t_diff.x * t_diff.x + t_diff.y * t_diff.y), wall->texture->width),
        wallSlice_bottom < wallSlice_clipped_bottom ? (portal_bottom - wallSlice_bottom) * t_step : 0
    );

    for (int row = wallSlice_clipped_bottom; row <= wallSlice_clipped_top; row++) {

        int index = (int)t_slice.x + (int)fmod(t_slice.y + t_step * (row - wallSlice_clipped_bottom), wall->texture->height) * wall->texture->width;

        Color color = wall->texture->pixels[index];

        canvas->setPixel(col, row, color);
    }
}

void Camera::render_floor(Canvas* canvas, int col, Sector* sector, float rayAngle, float nearClip, float portalSlice_bottom, float portalSlice_top, float wallSlice_bottom)
{
    wallSlice_bottom--;

    if (wallSlice_bottom < 0) return;

    int portalSlice_clipped_bottom = portalSlice_bottom > 0 ? portalSlice_bottom : 0;
    int wallSlice_clipped_bottom = wallSlice_bottom < portalSlice_top ? wallSlice_bottom : portalSlice_top;

    float horizontLine = (canvas->height >> 1) + pitch;

    for (int row = portalSlice_clipped_bottom; row <= wallSlice_clipped_bottom; row++) {
        float t_dist = (height - sector->bottom) / (row - horizontLine) * nearClip / cos(rayAngle);
        Point t_pos = Point(
            nmod(pos.x - t_dist * sin(heading + rayAngle), sector->floorTexture->width),
            nmod(pos.y - t_dist * cos(heading + rayAngle), sector->floorTexture->height)
        );

        int color_index = (int)t_pos.x + (int)t_pos.y * sector->floorTexture->width;
        Color color = sector->floorTexture->pixels[color_index];
        canvas->setPixel(col, row, color);
    }
}

void Camera::render_ceiling(Canvas* canvas, int col, Sector* sector, float rayAngle, float nearClip, float portalSlice_bottom, float portalSlice_top, float wallSlice_top)
{
    wallSlice_top++;

    if (wallSlice_top > canvas->height) return;

    int portalSlice_clipped_top = portalSlice_top < canvas->height ? portalSlice_top : canvas->height;
    int wallSlice_clipped_top = wallSlice_top > portalSlice_bottom ? wallSlice_top : portalSlice_bottom;

    float horizontLine = (canvas->height >> 1) + pitch;

    for (int row = wallSlice_clipped_top; row <= portalSlice_clipped_top; row++) {
        float t_dist = ((sector->top - sector->bottom) - (height - sector->bottom)) / (horizontLine - row) * nearClip / cos(rayAngle);
        Point t_pos = Point(
            nmod(pos.x - t_dist * sin(heading + rayAngle), sector->ceilingTexture->width),
            nmod(pos.y - t_dist * cos(heading + rayAngle), sector->ceilingTexture->height)
        );

        int color_index = (int)t_pos.x + (int)t_pos.y * sector->ceilingTexture->width;
        Color color = sector->ceilingTexture->pixels[color_index];
        canvas->setPixel(col, row, color);
    }
}

bool Camera::rayIntersection(Point* start, Point* end, float rayAngle, Point* intersection)
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

inline double Camera::nmod(double value, double mod) // value can goes under 0
{
    return std::fmod(std::fmod(value, mod) + mod, mod);
}
