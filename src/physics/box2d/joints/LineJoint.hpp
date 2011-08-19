
/*
    Copyright 2011 Aevum Software aevum @ aevumlab.com

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    @author Victor Vicente de Carvalho victor.carvalho@aevumlab.com
    @author Ozires Bortolon de Faria ozires@aevumlab.com
*/

#ifndef GDX_CPP_PHYSICS_BOX2D_JOINTS_LINEJOINT_HPP_
#define GDX_CPP_PHYSICS_BOX2D_JOINTS_LINEJOINT_HPP_

namespace gdx_cpp {
namespace physics {
namespace box2d {
namespace joints {

class LineJoint: public gdx_cpp::physics::box2d::Joint {
public:
    float getJointTranslation ();
    float getJointSpeed ();
    bool isLimitEnabled ();
    void enableLimit (bool flag);
    float getLowerLimit ();
    float getUpperLimit ();
    void setLimits (float lower,float upper);
    bool isMotorEnabled ();
    void enableMotor (bool flag);
    void setMotorSpeed (float speed);
    float getMotorSpeed ();
    void setMaxMotorForce (float force);
    float getMaxMotorForce ();
    float getMotorForce ();

protected:


private:

};

} // namespace gdx_cpp
} // namespace physics
} // namespace box2d
} // namespace joints

#endif // GDX_CPP_PHYSICS_BOX2D_JOINTS_LINEJOINT_HPP_
