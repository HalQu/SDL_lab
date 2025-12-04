#include "3D_codinate.h"
#include "Camera.h"
#include <cmath>

Point2D Point3D::To2D(const Camera& camera) {
    auto camPos = camera.getPosition();
    auto forward = camera.get_Direction();
    auto up = camera.get_Up();
    auto right = camera.get_TxG();

    float dx = x - camPos.getX();
    float dy = y - camPos.getY();
    float dz = z - camPos.getZ();

    float cz = dx*forward.x + dy*forward.y + dz*forward.z;
    float cx = dx*right.x   + dy*right.y   + dz*right.z;
    float cy = dx*up.x      + dy*up.y      + dz*up.z;

    float nearC = camera.getNearClip();
    float farC  = camera.getFarClip();
    // 点在相机前方并在裁剪范围内才可见（cz>near_clip 且 cz<far_clip）
    if (cz <= nearC || cz >= farC) return Point2D(-1e6f, -1e6f);

    // 使用垂直 FOV 计算 fy，然后 fx = fy / aspect（不要乘）
    float fovY = camera.getFOV(); // 垂直 FOV（弧度）
    float fy = 1.0f / std::tan(fovY * 0.5f);
    float fx = fy / camera.getAspectRatio(); // 正确计算 fx

    float ndc_x = (cx * fx) / cz;
    float ndc_y = (cy * fy) / cz;

    // 将标准化设备坐标映射到屏幕（NDC 默认 -1..1）
    float sx = (ndc_x * 0.5f + 0.5f) * SCREEN_WIDTH;
    float sy = (-ndc_y * 0.5f + 0.5f) * SCREEN_HEIGHT; // 翻转 Y

    return Point2D(sx, sy);
}
Point2D Point3D::To2D_forback(const Camera& camera) {
    auto camPos = camera.getPosition();
    auto forward = camera.get_Direction();
    auto up = camera.get_Up();
    auto right = camera.get_TxG();

    float nearC = camera.getNearClip();
    float farC  = camera.getFarClip();

    

    float dx = x - camPos.getX();
    float dy = y - camPos.getY();
    float dz = z - camPos.getZ();

    float cz = dx*forward.x + dy*forward.y + dz*forward.z;
    float cx = dx*right.x   + dy*right.y   + dz*right.z;
    float cy = dx*up.x      + dy*up.y      + dz*up.z;


    // 使用垂直 FOV 计算 fy，然后 fx = fy / aspect（不要乘）
    float fovY = camera.getFOV(); // 垂直 FOV（弧度）
    float fy = 1.0f / std::tan(fovY * 0.5f);
    float fx = fy / camera.getAspectRatio(); // 正确计算 fx

    float ndc_x = (cx * fx) / cz;
    float ndc_y = (cy * fy) / cz;

    // 将标准化设备坐标映射到屏幕（NDC 默认 -1..1）
    float sx = (ndc_x * 0.5f + 0.5f) * SCREEN_WIDTH;
    float sy = (-ndc_y * 0.5f + 0.5f) * SCREEN_HEIGHT; // 翻转 Y

    return Point2D(sx, sy);
}




