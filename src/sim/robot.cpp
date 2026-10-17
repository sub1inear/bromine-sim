#include "robot.hpp"
#include "categories.hpp"
#include "constants.hpp"
#include "distance.hpp"
#include "imu.hpp"
#include "motor.hpp"
#include "rotation.hpp"
#include "utils.hpp"
#include "wheel.hpp"
#include <algorithm>
#include <fstream>
#include <print>

namespace bromine
{

Robot::Robot(b2WorldId worldId) : wheelCount(0)
{
    std::ifstream file("assets/robot.json");
    json j;
    try
    {
        j = json::parse(file, nullptr, true, true);
    }
    catch (const json::parse_error &e)
    {
        error("{}", e.what());
    }

    uint64_t version = expectJsonKey<uint64_t>(j, "version");

    if (version != 1)
    {
        error("JSON version mismatch: `{}` vs. `{}`", version, 1);
    }

    InputDist height = expectJsonKey<float>(j, "height");

    InputRect inputRect = {
        -70.0f + height / 2.0f, // Turned 90 degrees so relative to height.
        0.0f,
        90.0f,
        expectJsonKey<float>(j, "width"),
        height,
        expectJsonKey<float>(j, "mass"),
    };

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.filter.categoryBits = std::to_underlying(Categories::Player);
    rect = inputRectToPhysicsIdRect(inputRect, worldId, bodyDef, shapeDef);

    std::vector<json> vec = expectJsonKey<std::vector<json>>(j, "devices");

    for (const auto &elem : vec)
    {
        std::string_view type = expectJsonKey<std::string_view>(elem, "type");
        if (type == "motor")
        {
            devices.push_back(std::make_unique<Motor>(elem, *this));
        }
        else if (type == "wheel")
        {
            devices.push_back(std::make_unique<Wheel>(elem, *this));
        }
        else if (type == "imu")
        {
            devices.push_back(std::make_unique<Imu>(elem, *this));
        }
        else if (type == "distance")
        {
            devices.push_back(std::make_unique<Distance>(elem, *this));
        }
        else if (type == "rotation")
        {
            devices.push_back(std::make_unique<Rotation>(elem, *this));
        }
        else
        {
            error("unknown device type `{}`", type);
        }
    }

    // Link motors/rotations to their corresponding wheels.
    // This action must be done following the creation of all devices,
    // so the wheels have been created.
    // Count the wheels as well.
    for (auto &device : devices)
    {
        // TODO: Merge the linking into an init() function.
        Device *ptr = device.get();
        if (auto motor = dynamic_cast<Motor *>(ptr))
        {
            motor->link();
        }
        else if (auto rotation = dynamic_cast<Rotation *>(ptr))
        {
            rotation->link();
        }
        else if (dynamic_cast<Wheel *>(ptr))
        {
            ++wheelCount;
        }
    }
}

void Robot::preUpdate()
{
    for (auto &device : devices)
    {
        device->preUpdate();
    }
}

void Robot::postUpdate()
{
    for (auto &device : devices)
    {
        device->postUpdate();
    }
}

} // namespace bromine
