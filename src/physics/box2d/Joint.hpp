
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

#ifndef GDX_CPP_PHYSICS_BOX2D_JOINT_HPP_
#define GDX_CPP_PHYSICS_BOX2D_JOINT_HPP_

namespace gdx_cpp {
namespace physics {
namespace box2d {

class Joint {
public:
    gdx_cpp::physics::box2d::JointDef::JointType& getType ();
    Body& getBodyA ();
    Body& getBodyB ();
    gdx_cpp::math::Vector2& getAnchorA ();
    gdx_cpp::math::Vector2& getAnchorB ();
    gdx_cpp::math::Vector2& getReactionForce (float inv_dt);
    float getReactionTorque (float inv_dt);
    bool isActive ();

protected:


private:
    World world;
};

} // namespace gdx_cpp
} // namespace physics
} // namespace box2d

#endif // GDX_CPP_PHYSICS_BOX2D_JOINT_HPP_
