#include "robot.hpp"
#include "constants.hpp"
#include "motor.hpp"
#include "utils.hpp"
#include "wheel.hpp"
#include <algorithm>
#include <fstream>
#include <print>

namespace cobalt
{

constexpr float linearForce = 1;       // Newtons
constexpr float rotationalForce = 0.1; // Newtons

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

    InputRect inputRect = {0.0f, 0.0f, 0.0f, expectJsonKey<float>(j, "width"),
                           expectJsonKey<float>(j, "height")};
    float mass = inputMassToPhysicsMass(expectJsonKey<float>(j, "mass"));

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    rect = inputRectToPhysicsIdRect(inputRect, worldId, bodyDef, shapeDef);

    std::vector<json> vec = expectJsonKey<std::vector<json>>(j, "devices");

    for (const auto &elem : vec)
    {
        std::string_view type = expectJsonKey<std::string_view>(elem, "type");
        if (type == "motor")
        {
            devices.push_back(std::make_unique<Motor>(elem));
        }
        else if (type == "wheel")
        {
            devices.push_back(std::make_unique<Wheel>(elem));
        }
        else
        {
            error("unknown device type `{}`", type);
        }
    }
}

void Robot::update(float dt)
{
    b2Vec2 force = {0.0f, 0.0f};
    float theta = 0.0f;

    if (IsKeyDown(KEY_LEFT))
    {
        theta += rotationalForce;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        theta -= rotationalForce;
    }
    if (IsKeyDown(KEY_UP))
    {
        b2Rot rotation = b2Body_GetRotation(rect.bodyId);
        force.x += rotation.c * linearForce;
        force.y += rotation.s * linearForce;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        b2Rot rotation = b2Body_GetRotation(rect.bodyId);
        force.x -= rotation.c * linearForce;
        force.y -= rotation.s * linearForce;
    }

    b2Body_ApplyForceToCenter(rect.bodyId, force, true);
    b2Body_ApplyTorque(rect.bodyId, theta, true);

    for (const auto &device : devices)
    {
        device->update(dt);
    }
}
} // namespace cobalt
