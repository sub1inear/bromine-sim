#include "motor.hpp"
#include "constants.hpp"
#include "robot.hpp"
#include "utils.hpp"
#include "wheel.hpp"
#include <algorithm>
#include <cassert>
#include <numbers>

namespace bromine
{

constexpr float maxVoltage = 12000;
constexpr float minVoltage = -maxVoltage;

static float gearsetToRatio(V5MotorGearset gearset)
{
    switch (gearset)
    {
    case kMotorGearSet_06:
        return 6.0f;
    case kMotorGearSet_18:
        return 18.0f;
    case kMotorGearSet_36:
        return 36.0f;
    default:
        error("unknown motor gearset `{}`", (int)gearset);
    }
}

Motor::Motor(const json &j, Robot &robot)
    : Device(j, robot), id(expectJsonKey<uint32_t>(j, "id")), sdk{}
{
    port = expectJsonKey<uint8_t>(j, "port");
    if (port < 1 || port > 21)
    {
        error("motor port must be in range [1, 21], got `{}`", port);
    }

    std::string_view roleStr = expectJsonKey<std::string_view>(j, "role");
    if (roleStr == "input")
    {
        role = Role::Input;
    }
    else if (roleStr == "output")
    {
        role = Role::Output;
    }
    else if (roleStr == "drive")
    {
        role = Role::Drive;
    }
    else
    {
        error("unknown motor role `{}`, expected one of: `input`, `output`, "
              "`drive`",
              roleStr);
    }

    std::string_view powerStr = expectJsonKey<std::string_view>(j, "power");
    if (powerStr == "full")
    {
        power = Power::Full;
        rect.width = inputDistToPhysicsDist(2.26f);
        rect.height = inputDistToPhysicsDist(2.82f);

        std::string_view gearsetStr =
            expectJsonKey<std::string_view>(j, "gearset");

        if (gearsetStr == "red")
        {
            gearset = kMotorGearSet_36;
        }
        else if (gearsetStr == "green")
        {
            gearset = kMotorGearSet_18;
        }
        else if (gearsetStr == "blue")
        {
            gearset = kMotorGearSet_06;
        }
        else
        {
            error(
                "unknown motor gearset `{}`, expected one of: `red`, `green`, "
                "`blue`",
                gearsetStr);
        }
    }
    else if (powerStr == "half")
    {
        power = Power::Half;
        rect.width = inputDistToPhysicsDist(2.25f);
        rect.height = inputDistToPhysicsDist(2.5f);
        if (j.find("gear") != j.end())
        {
            error("5.5W motors (half-watts) do not support gearing");
        }
        // Internally, 5.5W motors are geared to 18:1 (200 RPM):
        gearset = kMotorGearSet_18;
    }
    else
    {
        error("unknown motor power `{}`, expected one of: `full`, `half`",
              powerStr);
    }
}

void Motor::update()
{
    // Load voltage once to prevent it changing while it's applied.
    float loadedVoltage = sdk.voltage;
    loadedVoltage = std::clamp(loadedVoltage, minVoltage, maxVoltage);

    float ratio = gearsetToRatio(gearset);
    // For every wheel, apply force.
    for (size_t i = 0; i < wheels.size(); i++)
    {
        Wheel *wheel = wheels[i];
        // Get the wheel's rect in world space.
        PhysicsRect rect = wheel->getWorldRect();

        // Find the rotated forward vector.
        PhysicsPoint forward = b2RotateVector(rect.pose.q, {0.0f, 1.0f});

        // Get the actual velocity of the wheel.
        PhysicsPoint wheelVelocity =
            b2Body_GetWorldPointVelocity(robot.rect.bodyId, rect.pose.p);
        // Isolate the forward component of the velocity and make it a
        // scalar.
        float speed = b2Dot(wheelVelocity, forward);

        PhysicsDist wheelRadius = rect.height / 2.0f;
        PhysicsDist wheelCircumference =
            2.0f * std::numbers::pi_v<float> * wheelRadius;

        if (i == 0)
        {
            // Calculate the change in position: the total amount travelled,
            // speed * dt, divided by the wheel circumference to get wheel's
            // rotations and multiplied by the wheel's ratio to reverse its
            // gearing. NOTE: The motor's cartridge/ratio does not apply here.
            // Must only do this once to avoid counting the same change multiple
            // times.
            float deltaPosition =
                speed * fixedDt / wheelCircumference * wheel->ratio;
            sdk.position += deltaPosition;
        }

        // The wheel RPM is the motor's base RPM (3600 RPM) divided by the
        // motor's gear ratio and the wheel's gear ratio.
        float wheelRpm = 3600 / ratio / wheel->ratio;

        // Convert revolutions per minute to per second.
        float revolutionsPerSecond = wheelRpm / 60.0f;

        // The max speed in m/s is the revolutions per second times the
        // wheel circumference in meters.
        float maxSpeed = revolutionsPerSecond * wheelCircumference;

        // Calculate stall torque by scaling the published stall torque for
        // 11W/5.5W at 100RPM to the actual RPM.
        float stallTorque = power == Power::Full ? 2.1f : 0.5f;
        stallTorque *= 100.0f / wheelRpm;

        // Torque = force * distance, so divide by distance to get the true
        // force.
        float stallForce = stallTorque / wheelRadius;

        // Calculate target speed as a proportion of the maximum speed.
        float targetSpeed = (loadedVoltage / maxVoltage) * maxSpeed;

        // Simulate motor-speed curve; voltage sets the speed and back-EMF
        // reduces it.
        float force = stallForce * (targetSpeed - speed) / maxSpeed;

        // Clamp the force to prevent instantly switching directions.
        force = std::clamp(force, -stallForce, stallForce);

        // Split the motor's force across every linked wheel.
        // NOTE: This must be done following clamping by stallForce,
        // otherwise stallForce must be divided by the number of wheels as
        // well.
        force /= wheels.size();

        // Make the force point in the forward direction.
        PhysicsPoint forceVec = b2MulSV(force, forward);

        // Apply the force.
        b2Body_ApplyForce(robot.rect.bodyId, forceVec, rect.pose.p, true);
    }
}

void Motor::draw(Renderer &renderer) const { renderer.drawMotor(*this); }

void Motor::link()
{
    wheels.clear();
    for (auto &device : robot.devices)
    {
        auto wheel = dynamic_cast<Wheel *>(device.get());
        if (wheel != nullptr && wheel->id == id)
        {
            wheels.push_back(wheel);
        }
    }
    if (wheels.empty())
    {
        warning("motor on port `{}` has no connected wheels", port);
    }
}

} // namespace bromine
