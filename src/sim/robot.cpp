#include "robot.hpp"
#include "constants.hpp"
#include "motor.hpp"
#include "utils.hpp"
#include "wheel.hpp"
#include <algorithm>
#include <fstream>
#include <print>

namespace bromine
{

constexpr float throttleCoeff = 1;
constexpr float turnCoeff = 0.1;

Robot::Robot(b2WorldId worldId)
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

    InputRect inputRect = {
        0.0f,
        0.0f,
        0.0f,
        expectJsonKey<float>(j, "width"),
        expectJsonKey<float>(j, "height"),
        inputMassToPhysicsMass(expectJsonKey<float>(j, "mass")),
    };

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.linearDamping = 0.9f;
    bodyDef.angularDamping = 0.9f;
    bodyDef.type = b2_dynamicBody;
    b2ShapeDef shapeDef = b2DefaultShapeDef();
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
        else
        {
            error("unknown device type `{}`", type);
        }
    }
}

void Robot::update(float dt)
{
    for (auto &device : devices)
    {
        device->update(dt);
        if (Motor *motor = dynamic_cast<Motor *>(device.get()))
        {
            motor->setVoltage(14000);
        }
    }
}

} // namespace bromine
