#include "camera.h"

Camera::Camera(Point pos, float height, float heading, float pitch, float fov)
{
    this->pos = pos;
    this->height = height;
    this->heading = heading;
    this->pitch = pitch;
    this->fov = fov;

    this->move_speed = 50;
    this->max_pitch = 200;
    this->pitch_speed = 200;
}

Camera::~Camera() { }

void Camera::update(OpenGL* openGL, Mouse* mouse)
{
    if (glfwGetKey(openGL->window, GLFW_KEY_W) == GLFW_PRESS) {
        pos.x += sin(this->heading) * move_speed * openGL->deltaTime;
        pos.y += cos(this->heading) * move_speed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_S) == GLFW_PRESS) {
        pos.x -= sin(this->heading) * move_speed * openGL->deltaTime;
        pos.y -= cos(this->heading) * move_speed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_A) == GLFW_PRESS) {
        pos.x -= cos(this->heading) * move_speed * openGL->deltaTime;
        pos.y += sin(this->heading) * move_speed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_D) == GLFW_PRESS) {
        pos.x += cos(this->heading) * move_speed * openGL->deltaTime;
        pos.y -= sin(this->heading) * move_speed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_Q) == GLFW_PRESS) {
        height -= move_speed * openGL->deltaTime;
    }
    if (glfwGetKey(openGL->window, GLFW_KEY_E) == GLFW_PRESS) {
        height += move_speed * openGL->deltaTime;
    }

    heading += mouse->xDelta * mouse->mouseSensivity * openGL->deltaTime;

    pitch += mouse->yDelta * mouse->mouseSensivity * openGL->deltaTime * pitch_speed;
    if (pitch < -max_pitch) pitch = -max_pitch;
    if (pitch > max_pitch) pitch = max_pitch;
}

void Camera::render(Canvas* canvas, World* world)
{
    float near_clip = (canvas->width >> 1) / tan(fov * .5f);

    Sector* current_sector = nullptr;
    for (Sector sector : world->sectors) {
        if (sector.pointInSector(pos)) {
            current_sector = &sector;
            break;
        }
    }

    if (current_sector == nullptr) return;

    float ray_dir_angle = -fov * .5f;
    float ray_dir_angle_step = fov / canvas->width;
    for (int col = 0; col < canvas->width; col++) {
        render_column(canvas, col, world, current_sector, ray_dir_angle, near_clip, 0, canvas->height);
        ray_dir_angle += ray_dir_angle_step;
    }
}

void Camera::render_column(Canvas* canvas, int col, World* world, Sector* sector, float ray_dir_angle, float near_clip, float portal_slice_bottom, float portal_slice_top)
{
    if (portal_slice_bottom > canvas->height - 1 || portal_slice_top < 0) return;

    Wall* current_wall = nullptr;
    Point intersection;
    float intersection_distance_sqr = std::numeric_limits<float>::max();
    for (int i = 0; i < sector->wall_count; i++) {
        Wall* wall = sector->walls[i];

        if (wall->backface_culling(pos)) continue;

        Point start = (*wall->start).subtract(pos).rotate(this->heading);
        Point end = (*wall->end).subtract(pos).rotate(this->heading);

        Point ip;
        float id;
        if (ray_intersection(&start, &end, ray_dir_angle, &ip)) {
            id = ip.x * ip.x + ip.y * ip.y;
            if (id < intersection_distance_sqr) {
                current_wall = wall;
                intersection = ip;
                intersection_distance_sqr = id;
            }
        }
    }

    if (current_wall != nullptr) {
        float horizont_line = (canvas->height >> 1) + pitch;

        float intersection_distance = sqrt(intersection_distance_sqr);
        float wall_slice_distance = intersection_distance * cos(ray_dir_angle);

        float wall_slice_bottom = horizont_line - (height - sector->bottom) / wall_slice_distance * near_clip;
        float wall_slice_top = horizont_line + (sector->top - height) / wall_slice_distance * near_clip;

        if (current_wall->portal_to == -1) {
            render_wall_slice(canvas, col, current_wall, sector->top - sector->bottom, wall_slice_bottom, wall_slice_top, portal_slice_bottom, portal_slice_top, &intersection);//, intersection_distance);
        } else {
            Sector* other_sector = &world->sectors[current_wall->portal_to];
            float portal_bottom = wall_slice_bottom;
            float bottom_wall_height = other_sector->bottom - sector->bottom;
            if (bottom_wall_height > 0) {
                portal_bottom = horizont_line - (height - other_sector->bottom) / wall_slice_distance * near_clip;
                render_wall_slice(canvas, col, current_wall, bottom_wall_height, wall_slice_bottom, portal_bottom, portal_slice_bottom, portal_slice_top, &intersection);//, intersection_distance);
            }
            portal_bottom = portal_bottom > portal_slice_bottom ? portal_bottom : portal_slice_bottom;

            float portal_top = wall_slice_top;
            float top_wall_height = sector->top - other_sector->top;
            if (top_wall_height > 0) {
                portal_top = horizont_line + (other_sector->top - height) / wall_slice_distance * near_clip;
                render_wall_slice(canvas, col, current_wall, top_wall_height, portal_top, wall_slice_top, portal_slice_bottom, portal_slice_top, &intersection);//, intersection_distance);
            }
            portal_top = portal_top < portal_slice_top ? portal_top : portal_slice_top;

            render_column(canvas, col, world, other_sector, ray_dir_angle, near_clip, portal_bottom, portal_top);
        }

        render_floor(canvas, col, sector, ray_dir_angle, near_clip, portal_slice_bottom, portal_slice_top, wall_slice_bottom);

        render_ceiling(canvas, col, sector, ray_dir_angle, near_clip, portal_slice_bottom, portal_slice_top, wall_slice_top);
    }
}

void Camera::render_wall_slice(Canvas* canvas, int col, Wall* wall, float wall_height, float wall_slice_bottom, float wall_slice_top, float portal_bottom, float portal_top, Point* intersection)//, float intersection_distance)
{
    float wall_slice_clipped_bottom = wall_slice_bottom > portal_bottom ? wall_slice_bottom : portal_bottom;
    float wall_slice_clipped_top = wall_slice_top < portal_top ? wall_slice_top : portal_top;

    Point t_pos = (*wall->start).subtract(pos).rotate(heading);
    Point t_diff = (*intersection).subtract(t_pos);

    float t_step =  wall_height / (wall_slice_top - wall_slice_bottom);
    Point t_slice = Point(
        fmod(sqrt(t_diff.x * t_diff.x + t_diff.y * t_diff.y), wall->texture->width),
        wall_slice_bottom < wall_slice_clipped_bottom ? (portal_bottom - wall_slice_bottom) * t_step : 0
    );

    // float multiplier = 255.0f;
    // float intensity = .25f/ray_intersection_distance * multiplier;

    for (int row = 0; row <= (int)wall_slice_clipped_top - (int)wall_slice_clipped_bottom; row++) {

        int index = (int)t_slice.x + (int)fmod(t_slice.y + t_step * row, wall->texture->height) * wall->texture->width;

        Color color = wall->texture->pixels[index];
        // float r = color.getRed() * intensity;
        // float g = color.getGreen() * intensity;
        // float b = color.getBlue() * intensity;
        // color = Color(r < 255 ? r : 255, g < 255 ? g : 255, b < 255 ? b : 255);

        canvas->setPixel(col, wall_slice_clipped_bottom + row, color);
    }
}

void Camera::render_floor(Canvas* canvas, int col, Sector* sector, float ray_dir_angle, float near_clip, float portal_slice_bottom, float portal_slice_top, float wall_slice_bottom)
{
    wall_slice_bottom--;

    if (wall_slice_bottom < 0) return;

    float portal_slice_clipped_bottom = portal_slice_bottom > 0 ? portal_slice_bottom : 0;
    float wall_slice_clipped_bottom = wall_slice_bottom < portal_slice_top ? wall_slice_bottom : portal_slice_top;

    float horizont_line = (canvas->height >> 1) + pitch;

    for (int row = portal_slice_clipped_bottom; row < wall_slice_clipped_bottom; row++) {

        float texture_dist = (height - sector->bottom) / (row - horizont_line) * near_clip / cos(ray_dir_angle);

        float texture_pos_x = nmod(pos.x - texture_dist * sin(heading + ray_dir_angle), sector->floor_texture->width);
        float texture_pos_y = nmod(pos.y - texture_dist * cos(heading + ray_dir_angle), sector->floor_texture->height);

        int color_index = (int)texture_pos_x + (int)texture_pos_y * sector->floor_texture->width;
        Color color = sector->floor_texture->pixels[color_index];
        canvas->setPixel(col, row, color);
    }
}

void Camera::render_ceiling(Canvas* canvas, int col, Sector* sector, float ray_dir_angle, float near_clip, float portal_slice_bottom, float portal_slice_top, float wall_slice_top)
{
    wall_slice_top++;

    if (wall_slice_top > canvas->height) return;

    float portal_slice_clipped_top = portal_slice_top < canvas->height ? portal_slice_top : canvas->height;
    float wall_slice_clipped_top = wall_slice_top > portal_slice_bottom ? wall_slice_top : portal_slice_bottom;

    float horizont_line = (canvas->height >> 1) + pitch;

    for (int row = wall_slice_clipped_top; row < portal_slice_clipped_top; row++) {

        float texture_dist = ((sector->top - sector->bottom) - (height - sector->bottom)) / (horizont_line - row) * near_clip / cos(ray_dir_angle);

        float texture_pos_x = nmod(pos.x - texture_dist * sin(heading + ray_dir_angle), sector->ceiling_texture->width);
        float texture_pos_y = nmod(pos.y - texture_dist * cos(heading + ray_dir_angle), sector->ceiling_texture->height);

        int color_index = (int)texture_pos_x + (int)texture_pos_y * sector->ceiling_texture->width;
        Color color = sector->ceiling_texture->pixels[color_index];
        canvas->setPixel(col, row, color);
    }
}





bool Camera::ray_intersection(Point* start, Point* end, float ray_dir_angle, Point* intersection)
{
    Point diff = (*end).subtract(*start);
    Point ray_dir = Point(sin(ray_dir_angle), cos(ray_dir_angle));

    float den = diff.x * ray_dir.y - diff.y * ray_dir.x;
    if (den == 0) return false;

    float t = (start->x * ray_dir.y - start->y * ray_dir.x) / -den;
    if (0 > t || t > 1) return false;

    float u = (diff.x * start->y - diff.y * start->x) / den;
    if (0 > u) return false;

    *intersection = diff.multiply(t).add(*start);

    return true;
}

// value can goes under 0
inline double Camera::nmod(double value, double mod)
{
    return std::fmod(std::fmod(value, mod) + mod, mod);
}
