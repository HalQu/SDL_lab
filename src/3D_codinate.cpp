#include "3D_codinate.h"
#include "Camera.h"
#include <cmath>

Point2D Point3D::To2D(const Camera& camera) {
    // 转换到相对摄像机坐标系
    Point3D relativePos(
        (x - camera.getPosition().getX()) * camera.get_TxG().x +
        (y - camera.getPosition().getY()) * camera.get_TxG().y +
        (z - camera.getPosition().getZ()) * camera.get_TxG().z,
        (x - camera.getPosition().getX()) * camera.get_Up().x +
        (y - camera.getPosition().getY()) * camera.get_Up().y +
        (z - camera.getPosition().getZ()) * camera.get_Up().z,
        (x - camera.getPosition().getX()) * camera.get_Direction().x +
        (y - camera.getPosition().getY()) * camera.get_Direction().y +
        (z - camera.getPosition().getZ()) * camera.get_Direction().z
    );

    

    // 透视投影计算
    float nearClip = camera.getNearClip();
    float farClip = camera.getFarClip();

    if(relativePos.z <= nearClip|| relativePos.z >= farClip) {
        return Point2D(592778,592778); // 返回592778
    }

    float aspect = camera.getAspectRatio();
    float Half_H = tan(camera.getFOV()) * nearClip;
    float Half_W = Half_H * aspect;

    // 计算投影后的2D坐标
    Point2D projectedPoint;
    projectedPoint.x = (relativePos.x * nearClip) / (Half_W * relativePos.z) * SCREEN_WIDTH/2 + SCREEN_WIDTH/2;
    projectedPoint.y = (relativePos.y * nearClip) / (Half_H * relativePos.z) * SCREEN_HEIGHT/2 + SCREEN_HEIGHT/2;

    return projectedPoint;
}


