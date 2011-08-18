/*
    Copyright 2011 <copyright holder> <email>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#include "Matrix3.hpp"
#include "MathUtils.hpp"

#include <string>
#include <sstream>

using namespace gdx_cpp::math;

#define DEGREE_TO_RAD = (float)PI / 180

Matrix3::Matrix3()
{
    idt();
}

Matrix3::Matrix3(const Matrix3& other)
{
}

Matrix3::~Matrix3()
{
}

Matrix3& Matrix3::operator=(const Matrix3& other)
{
    return *this;
}

bool Matrix3::operator==(const Matrix3& other) const
{
}

Matrix3& Matrix3::idt() {
    this->vals[0] = 1;
    this->vals[1] = 0;
    this->vals[2] = 0;

    this->vals[3] = 0;
    this->vals[4] = 1;
    this->vals[5] = 0;

    this->vals[6] = 0;
    this->vals[7] = 0;
    this->vals[8] = 1;

    return *this;
}

Matrix3& Matrix3::mul(const Matrix3& m) {
    float v00 = vals[0] * m.vals[0] + vals[3] * m.vals[1] + vals[6] * m.vals[2];
    float v01 = vals[0] * m.vals[3] + vals[3] * m.vals[4] + vals[6] * m.vals[5];
    float v02 = vals[0] * m.vals[6] + vals[3] * m.vals[7] + vals[6] * m.vals[8];

    float v10 = vals[1] * m.vals[0] + vals[4] * m.vals[1] + vals[7] * m.vals[2];
    float v11 = vals[1] * m.vals[3] + vals[4] * m.vals[4] + vals[7] * m.vals[5];
    float v12 = vals[1] * m.vals[6] + vals[4] * m.vals[7] + vals[7] * m.vals[8];

    float v20 = vals[2] * m.vals[0] + vals[5] * m.vals[1] + vals[8] * m.vals[2];
    float v21 = vals[2] * m.vals[3] + vals[5] * m.vals[4] + vals[8] * m.vals[5];
    float v22 = vals[2] * m.vals[6] + vals[5] * m.vals[7] + vals[8] * m.vals[8];

    vals[0] = v00;
    vals[1] = v10;
    vals[2] = v20;
    vals[3] = v01;
    vals[4] = v11;
    vals[5] = v21;
    vals[6] = v02;
    vals[7] = v12;
    vals[8] = v22;

    return *this;
}

Matrix3& Matrix3::setToRotation(float angle) {
    angle = DEGREE_TO_RAD * angle;
    float cos = (float)stlp_std::cos(angle);
    float sin = (float)stlp_std::sin(angle);

    this->vals[0] = cos;
    this->vals[1] = sin;
    this->vals[2] = 0;

    this->vals[3] = -sin;
    this->vals[4] = cos;
    this->vals[5] = 0;

    this->vals[6] = 0;
    this->vals[7] = 0;
    this->vals[8] = 1;

    return this;
}

Matrix3& Matrix3::setToTranslation(float x, float y) {
    this->vals[0] = 1;
    this->vals[1] = 0;
    this->vals[2] = 0;

    this->vals[3] = 0;
    this->vals[4] = 1;
    this->vals[5] = 0;

    this->vals[6] = x;
    this->vals[7] = y;
    this->vals[8] = 1;

    return *this;
}

Matrix3& Matrix3::setToScaling(float sx, float sy) {
    this->vals[0] = sx;
    this->vals[1] = 0;
    this->vals[2] = 0;

    this->vals[3] = 0;
    this->vals[4] = sy;
    this->vals[5] = 0;

    this->vals[6] = 0;
    this->vals[7] = 0;
    this->vals[8] = 1;

    return *this;
}

stlport::string Matrix3::toString() {
    stlport::stringstream ss;
    ss << "[" << vals[0] << "|" << vals[3] << "|" << vals[6] << "]\n" << "[" << vals[1] << "|" << vals[4] << "|" << vals[7] << "]\n" << "["
    << vals[2] << "|" << vals[5] << "|" << vals[8] << "]";
    return ss.str();
}

float Matrix3::det() {
    return vals[0] * vals[4] * vals[8] + vals[3] * vals[7] * vals[2] + vals[6] * vals[1] * vals[5] - vals[0] * vals[7]
           * vals[5] - vals[3] * vals[1] * vals[8] - vals[6] * vals[4] * vals[2];
}

Matrix3& Matrix3::inv() {
    float det = det();
    if (det == 0) throw new GdxRuntimeException("Can't invert a singular matrix");

    float inv_det = 1.0f / det;
    float tmp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    tmp[0] = vals[4] * vals[8] - vals[5] * vals[7];
    tmp[1] = vals[2] * vals[7] - vals[1] * vals[8];
    tmp[2] = vals[1] * vals[5] - vals[2] * vals[4];
    tmp[3] = vals[5] * vals[6] - vals[3] * vals[8];
    tmp[4] = vals[0] * vals[8] - vals[2] * vals[6];
    tmp[5] = vals[2] * vals[3] - vals[0] * vals[5];
    tmp[6] = vals[3] * vals[7] - vals[4] * vals[6];
    tmp[7] = vals[1] * vals[6] - vals[0] * vals[7];
    tmp[8] = vals[0] * vals[4] - vals[1] * vals[3];

    vals[0] = inv_det * tmp[0];
    vals[1] = inv_det * tmp[1];
    vals[2] = inv_det * tmp[2];
    vals[3] = inv_det * tmp[3];
    vals[4] = inv_det * tmp[4];
    vals[5] = inv_det * tmp[5];
    vals[6] = inv_det * tmp[6];
    vals[7] = inv_det * tmp[7];
    vals[8] = inv_det * tmp[8];

    return *this;
}

Matrix3& Matrix3::set(const Matrix3& mat) {
    vals[0] = mat.vals[0];
    vals[1] = mat.vals[1];
    vals[2] = mat.vals[2];
    vals[3] = mat.vals[3];
    vals[4] = mat.vals[4];
    vals[5] = mat.vals[5];
    vals[6] = mat.vals[6];
    vals[7] = mat.vals[7];
    vals[8] = mat.vals[8];
    return *this;
}

Matrix3& Matrix3::trn(const Vector3& vector) {
    vals[6] += vector.x;
    vals[7] += vector.y;
    return *this;
}

Matrix3& Matrix3::trn(float x, float y) {
    vals[6] += x;
    vals[7] += y;
    return *this;
}

float* Matrix3::getValues() {
    return vals;
}






