#pragma once

#include "config.h"
#include <cmath>

class Camera; // 前向声明 Camera 类

class Point2D {
public:
    float x, y;

    Point2D() : x(0), y(0) {}
    Point2D(float x_, float y_) : x(x_), y(y_) {}
};

// 方向类：同时保存笛卡尔(x,y,z)与球坐标(theta,phi)
// theta 取值约在 [-PI, PI)，phi 在 [-PI/2, PI/2]
class direction3D {
public:
    // 笛卡尔坐标（建议保持为单位向量）
    float x = 0.0f;
    float y = 0.0f;
    float z = 1.0f;

    // 球坐标
    // theta: 水平角，atan2(x,z) 形式（0 指向 +z）；phi: 垂直角，asin(y)
    float theta = 0.0f;
    float phi = 0.0f;

    direction3D() { updateSpherical(); }

    // 从笛卡尔构造（会自动归一化并更新球坐标）
    direction3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {
        normalize();
        updateSpherical();
    }

    // 工厂函数：从球坐标构造 (theta, phi)（弧度）
    static direction3D fromSpherical(float theta_, float phi_) {
        direction3D d;
        d.theta = wrapToPiF(theta_);
        d.phi = phi_;
        d.updateCartesian();
        d.normalize();
        d.updateSpherical();
        return d;
    }

    // 设置为笛卡尔坐标（会归一化并更新球坐标）
    void setCartesian(float nx, float ny, float nz) {
        x = nx; y = ny; z = nz;
        normalize();
        updateSpherical();
    }

    // 设置为球坐标（theta, phi）（会更新笛卡尔并归一化）
    void setSpherical(float ntheta, float nphi) {
        theta = wrapToPiF(ntheta);
        phi = nphi;
        updateCartesian();
        normalize();
        updateSpherical(); // 保证 theta/phi 在规范范围
    }

    // 将当前球坐标转换为笛卡尔
    void updateCartesian() {
        float cosPhi = std::cos(phi);
        x = cosPhi * std::sinf(theta);
        y = std::sinf(phi);
        z = cosPhi * std::cosf(theta);
    }

    // 将当前笛卡尔转换为球坐标；会先保证笛卡尔为单位向量
    void updateSpherical() {
        // 先归一化以保证 asin/atan2 合理
        float len = std::sqrt(x*x + y*y + z*z);
        if (len <= 1e-8f) { // 避免零向量
            x = 0.0f; y = 0.0f; z = 1.0f; len = 1.0f;
        }
        x /= len; y /= len; z /= len;

        // phi = asin(y) 在 [-pi/2, pi/2]
        phi = std::asinf(std::fmaxf(-1.0f, std::fminf(1.0f, y)));
        // theta = atan2(x, z) 使得 theta=0 指向 +z，theta 增大向 +x 旋转
        theta = std::atan2f(x, z);
        theta = wrapToPiF(theta);
    }

    // 仅做单位化（不改变 theta/phi）
    void normalize() {
        float len = std::sqrt(x*x + y*y + z*z);
        if (len <= 1e-8f) {
            x = 0.0f; y = 0.0f; z = 1.0f;
        } else {
            x /= len; y /= len; z /= len;
        }
    }

    // 小工具：返回一个绕 Y 轴旋转后的新方向（角度以弧度）
    direction3D rotatedAroundY(float angleRad) const {
        float c = std::cos(angleRad), s = std::sin(angleRad);
        // 旋转矩阵作用于 (x,z) 平面
        float nx = x * c - z * s;
        float nz = x * s + z * c;
        direction3D d(nx, y, nz);
        return d;
    }
};

class Point3D {
public:
    Point3D() : x(0), y(0), z(0) {}
    Point3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }

    void setX(float x_) {x=x_;}
    void setY(float y_) {y=y_;}
    void setZ(float z_) {z=z_;}

    Point2D To2D(const Camera& camera); // 声明 To2D 函数
    Point2D To2D_forback(const Camera& camera);

private:
    float x, y, z;
};


