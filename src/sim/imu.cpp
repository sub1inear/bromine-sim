#include "imu.hpp"
#include "units.hpp"
#include <box2d/box2d.h>
#include <numbers>

namespace bromine
{

Imu::Imu(const json &j, Robot &robot)
    : Device(j, robot), port(expectJsonPort(j)), sdk{}, previousHeading(0.0f)
{
    // Measured on https://www.vexrobotics.com/276-4855.html.
    rect.width = inputDistToPhysicsDist(0.866142f);
    rect.height = inputDistToPhysicsDist(1.551181);
}

void Imu::preUpdate() {}

void Imu::postUpdate()
{
    PhysicsRect worldRect = getWorldRect();

    float currentHeading = b2Rot_GetAngle(worldRect.pose.q);
    float deltaHeading = previousHeading - currentHeading;

    constexpr float twoPi = 2.0f * std::numbers::pi_v<float>;

    // If for example previousHeading is 3.13 rad and currentHeading is
    // -3.13 rad, just subtracting it will be -3.13 rad - 3.13 rad = -6.26
    // rad (off by 2π so add 2π). The same goes for the other way around.
    if (deltaHeading > std::numbers::pi_v<float>)
    {
        deltaHeading -= twoPi;
    }
    if (deltaHeading < -std::numbers::pi_v<float>)
    {
        deltaHeading += twoPi;
    }

    sdk.heading += deltaHeading;
    sdk.rotation = std::fmod(sdk.heading, twoPi);
    if (sdk.rotation < 0.0f)
    {
        sdk.rotation += twoPi;
    }

    previousHeading = currentHeading;
}

void Imu::draw(Renderer &renderer) const { renderer.drawImu(*this); }

} // namespace bromine
