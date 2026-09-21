/**
 * @file    v5_api.cpp
 * @brief   Stubs for the V5 jumptable API declared in v5_api.h.
 */

#include "v5_api.h"
#include "device.hpp"
#include "motor.hpp"
#include "robot.hpp"
#include "sim.hpp"
#include "utils.hpp"
#include <array>
#include <cassert>

struct _V5_Device
{
    bromine::Device *ptr;
    V5_DeviceType type;
};

// Port table: 0-indexed, nullptr for unplugged.
// 0-20 are the physical smart ports and 21-31 are the internal ports.
static std::array<_V5_Device, V5_MAX_DEVICE_PORTS> portTable;

static size_t portToIdx(uint8_t port)
{
    assert(port != 0);
    return static_cast<size_t>(port - 1);
}

static size_t idxToPort(size_t idx)
{
    assert(idx < V5_MAX_DEVICE_PORTS);
    return idx + 1;
}

void vexBackgroundProcessing(void) {}

// Console output

int32_t vexDebug(char const *fmt, ...) { return 0; }

int32_t vex_printf(char const *fmt, ...) { return 0; }

int32_t vex_sprintf(char *out, const char *format, ...) { return 0; }

int32_t vex_snprintf(char *out, uint32_t max_len, const char *format, ...)
{
    return 0;
}

int32_t vex_vsprintf(char *out, const char *format, va_list args) { return 0; }

int32_t vex_vsnprintf(char *out, uint32_t max_len, const char *format,
                      va_list args)
{
    return 0;
}

// system

uint32_t vexSystemTimeGet(void) { return 0; }

void vexGettime(struct time *pTime)
{
    if (pTime != nullptr)
    {
        *pTime = {};
    }
}

void vexGetdate(struct date *pDate)
{
    if (pDate != nullptr)
    {
        *pDate = {};
    }
}

void vexSystemMemoryDump(void) {}

void vexSystemDigitalIO(uint32_t pin, uint32_t value) {}

uint32_t vexSystemStartupOptions(void) { return 0; }

void vexSystemExitRequest(void) {}

uint64_t vexSystemHighResTimeGet(void) { return 0; }

uint64_t vexSystemPowerupTimeGet(void) { return 0; }

uint32_t vexSystemLinkAddrGet(void) { return 0; }

uint32_t vexSystemUsbStatus(void) { return 0; }

// Generic device

uint32_t vexDevicesGetNumber(void)
{
    uint32_t number = 0;
    for (const auto &device : portTable)
    {
        if (device.ptr != nullptr)
        {
            assert(device.type != kDeviceTypeNoSensor);
            number++;
        }
        else
        {
            assert(device.type == kDeviceTypeNoSensor);
        }
    }
    return 0;
}

uint32_t vexDevicesGetNumberByType(V5_DeviceType type)
{
    uint32_t number = 0;
    for (const auto &device : portTable)
    {
        if (device.ptr != nullptr && device.type == type)
        {
            number++;
        }
    }
    return number;
}

V5_DeviceT vexDevicesGet(void) { return nullptr; }

V5_DeviceT vexDeviceGetByIndex(uint32_t index)
{
    assert(index < V5_MAX_DEVICE_PORTS);
    return &portTable[index];
}

int32_t vexDeviceGetStatus(V5_DeviceType *buffer)
{
    assert(buffer != nullptr);
    for (uint32_t i = 0; i < V5_MAX_DEVICE_PORTS; i++)
    {
        buffer[i] = portTable[i].type;
    }
    return V5_MAX_DEVICE_PORTS;
}

int32_t vexDeviceGetTimestamp(V5_DeviceT device) { return 0; }

int32_t vexDeviceGetTimestampByIndex(int32_t index) { return 0; }

uint32_t vexDeviceButtonStateGet(void) { return 0; }

// Controller

int32_t vexControllerGet(V5_ControllerId id, V5_ControllerIndex index)
{
    return 0;
}

V5_ControllerStatus vexControllerConnectionStatusGet(V5_ControllerId id)
{
    return kV5ControllerOffline;
}

bool vexControllerTextSet(V5_ControllerId id, uint32_t line, uint32_t col,
                          const char *str)
{
    return false;
}

// LED sensor

void vexDeviceLedSet(V5_DeviceT device, V5_DeviceLedColor value) {}

void vexDeviceLedRgbSet(V5_DeviceT device, uint32_t color) {}

V5_DeviceLedColor vexDeviceLedGet(V5_DeviceT device) { return kLedColorBlack; }

uint32_t vexDeviceLedRgbGet(V5_DeviceT device) { return 0; }

// ADI sensor

void vexDeviceAdiPortConfigSet(V5_DeviceT device, uint32_t port,
                               V5_AdiPortConfiguration type)
{
}

V5_AdiPortConfiguration vexDeviceAdiPortConfigGet(V5_DeviceT device,
                                                  uint32_t port)
{
    return kAdiPortTypeUndefined;
}

void vexDeviceAdiValueSet(V5_DeviceT device, uint32_t port, int32_t value) {}

int32_t vexDeviceAdiValueGet(V5_DeviceT device, uint32_t port) { return 0; }

int32_t vexDeviceAdiAddrLedSet(V5_DeviceT device, uint32_t port,
                               uint32_t *pData, uint32_t nOffset,
                               uint32_t nLength, uint32_t options)
{
    return 0;
}

// Bumper switch

V5_DeviceBumperState vexDeviceBumperGet(V5_DeviceT device)
{
    return kBumperReleased;
}

// Gyro - obsolete, there is currently no dedicated gyro

void vexDeviceGyroReset(V5_DeviceT device) {}

double vexDeviceGyroHeadingGet(V5_DeviceT device) { return 0.0; }

double vexDeviceGyroDegreesGet(V5_DeviceT device) { return 0.0; }

// Sonar - obsolete, there is currently no dedicated gyro

int32_t vexDeviceSonarValueGet(V5_DeviceT device) { return 0; }

// Generic sensor - (who knows !)

int32_t vexDeviceGenericValueGet(V5_DeviceT device) { return 0; }

// Motor

void vexDeviceMotorVelocitySet(V5_DeviceT device, int32_t velocity) {}

void vexDeviceMotorVelocityUpdate(V5_DeviceT device, int32_t velocity) {}

void vexDeviceMotorVoltageSet(V5_DeviceT device, int32_t value)
{
    if (bromine::Motor *motor = dynamic_cast<bromine::Motor *>(device->ptr))
    {
        motor->setVoltage(value);
    }
}

int32_t vexDeviceMotorVelocityGet(V5_DeviceT device) { return 0; }

double vexDeviceMotorActualVelocityGet(V5_DeviceT device) { return 0.0; }

int32_t vexDeviceMotorDirectionGet(V5_DeviceT device) { return 0; }

void vexDeviceMotorModeSet(V5_DeviceT device, V5MotorControlMode mode) {}

V5MotorControlMode vexDeviceMotorModeGet(V5_DeviceT device)
{
    return kMotorControlModeOFF;
}

void vexDeviceMotorPwmSet(V5_DeviceT device, int32_t value) {}

int32_t vexDeviceMotorPwmGet(V5_DeviceT device) { return 0; }

void vexDeviceMotorCurrentLimitSet(V5_DeviceT device, int32_t value) {}

int32_t vexDeviceMotorCurrentLimitGet(V5_DeviceT device) { return 0; }

void vexDeviceMotorVoltageLimitSet(V5_DeviceT device, int32_t value) {}

int32_t vexDeviceMotorVoltageLimitGet(V5_DeviceT device) { return 0; }

void vexDeviceMotorPositionPidSet(V5_DeviceT device, V5_DeviceMotorPid *pid) {}

void vexDeviceMotorVelocityPidSet(V5_DeviceT device, V5_DeviceMotorPid *pid) {}

int32_t vexDeviceMotorCurrentGet(V5_DeviceT device) { return 0; }

int32_t vexDeviceMotorVoltageGet(V5_DeviceT device)
{
    if (bromine::Motor *motor = dynamic_cast<bromine::Motor *>(device->ptr))
    {
        return motor->getVoltage();
    }
    return 0;
}

double vexDeviceMotorPowerGet(V5_DeviceT device) { return 0.0; }

double vexDeviceMotorTorqueGet(V5_DeviceT device) { return 0.0; }

double vexDeviceMotorEfficiencyGet(V5_DeviceT device) { return 0.0; }

double vexDeviceMotorTemperatureGet(V5_DeviceT device) { return 0.0; }

bool vexDeviceMotorOverTempFlagGet(V5_DeviceT device) { return false; }

bool vexDeviceMotorCurrentLimitFlagGet(V5_DeviceT device) { return false; }

uint32_t vexDeviceMotorFaultsGet(V5_DeviceT device) { return 0; }

bool vexDeviceMotorZeroVelocityFlagGet(V5_DeviceT device) { return false; }

bool vexDeviceMotorZeroPositionFlagGet(V5_DeviceT device) { return false; }

uint32_t vexDeviceMotorFlagsGet(V5_DeviceT device) { return 0; }

void vexDeviceMotorReverseFlagSet(V5_DeviceT device, bool value) {}

bool vexDeviceMotorReverseFlagGet(V5_DeviceT device) { return false; }

void vexDeviceMotorEncoderUnitsSet(V5_DeviceT device, V5MotorEncoderUnits units)
{
}

V5MotorEncoderUnits vexDeviceMotorEncoderUnitsGet(V5_DeviceT device)
{
    return kMotorEncoderDegrees;
}

void vexDeviceMotorBrakeModeSet(V5_DeviceT device, V5MotorBrakeMode mode) {}

V5MotorBrakeMode vexDeviceMotorBrakeModeGet(V5_DeviceT device)
{
    return kV5MotorBrakeModeCoast;
}

void vexDeviceMotorPositionSet(V5_DeviceT device, double position) {}

double vexDeviceMotorPositionGet(V5_DeviceT device) { return 0.0; }

int32_t vexDeviceMotorPositionRawGet(V5_DeviceT device, uint32_t *timestamp)
{
    if (timestamp != nullptr)
    {
        *timestamp = 0;
    }
    return 0;
}

void vexDeviceMotorPositionReset(V5_DeviceT device) {}

double vexDeviceMotorTargetGet(V5_DeviceT device) { return 0.0; }

void vexDeviceMotorServoTargetSet(V5_DeviceT device, double position) {}

void vexDeviceMotorAbsoluteTargetSet(V5_DeviceT device, double position,
                                     int32_t velocity)
{
}

void vexDeviceMotorRelativeTargetSet(V5_DeviceT device, double position,
                                     int32_t velocity)
{
}

void vexDeviceMotorGearingSet(V5_DeviceT device, V5MotorGearset value) {}

V5MotorGearset vexDeviceMotorGearingGet(V5_DeviceT device)
{
    return kMotorGearSet_18;
}

int32_t vexDeviceMotorTypeGet(V5_DeviceT device) { return 0; }

void vexDeviceMotorExternalProfileSet(V5_DeviceT device, double position,
                                      int32_t velocity)
{
}

// Vision sensor

void vexDeviceVisionModeSet(V5_DeviceT device, V5VisionMode mode) {}

V5VisionMode vexDeviceVisionModeGet(V5_DeviceT device)
{
    return kVisionModeNormal;
}

int32_t vexDeviceVisionObjectCountGet(V5_DeviceT device) { return 0; }

int32_t vexDeviceVisionObjectGet(V5_DeviceT device, uint32_t indexObj,
                                 V5_DeviceVisionObject *pObject)
{
    if (pObject != nullptr)
    {
        *pObject = {};
    }
    return 0;
}

void vexDeviceVisionSignatureSet(V5_DeviceT device,
                                 V5_DeviceVisionSignature *pSignature)
{
}

bool vexDeviceVisionSignatureGet(V5_DeviceT device, uint32_t id,
                                 V5_DeviceVisionSignature *pSignature)
{
    if (pSignature != nullptr)
    {
        *pSignature = {};
    }
    return false;
}

void vexDeviceVisionBrightnessSet(V5_DeviceT device, uint8_t percent) {}

uint8_t vexDeviceVisionBrightnessGet(V5_DeviceT device) { return 0; }

void vexDeviceVisionWhiteBalanceModeSet(V5_DeviceT device, V5VisionWBMode mode)
{
}

V5VisionWBMode vexDeviceVisionWhiteBalanceModeGet(V5_DeviceT device)
{
    return kVisionWBNormal;
}

void vexDeviceVisionWhiteBalanceSet(V5_DeviceT device, V5_DeviceVisionRgb color)
{
}

V5_DeviceVisionRgb vexDeviceVisionWhiteBalanceGet(V5_DeviceT device)
{
    return {};
}

void vexDeviceVisionLedModeSet(V5_DeviceT device, V5VisionLedMode mode) {}

V5VisionLedMode vexDeviceVisionLedModeGet(V5_DeviceT device)
{
    return kVisionLedModeAuto;
}

void vexDeviceVisionLedBrigntnessSet(V5_DeviceT device, uint8_t percent) {}

uint8_t vexDeviceVisionLedBrigntnessGet(V5_DeviceT device) { return 0; }

void vexDeviceVisionLedColorSet(V5_DeviceT device, V5_DeviceVisionRgb color) {}

V5_DeviceVisionRgb vexDeviceVisionLedColorGet(V5_DeviceT device) { return {}; }

void vexDeviceVisionWifiModeSet(V5_DeviceT device, V5VisionWifiMode mode) {}

V5VisionWifiMode vexDeviceVisionWifiModeGet(V5_DeviceT device)
{
    return kVisionWifiModeOff;
}

// AI Vision sensor

void vexDeviceAiVisionReset(V5_DeviceT device) {}

void vexDeviceAiVisionEnableSet(V5_DeviceT device, uint8_t bits,
                                uint8_t bitmask)
{
}

uint8_t vexDeviceAiVisionEnableGet(V5_DeviceT device) { return 0; }

void vexDeviceAiVisionModeSet(V5_DeviceT device, uint32_t mode) {}

uint32_t vexDeviceAiVisionStatusGet(V5_DeviceT device) { return 0; }

int32_t vexDeviceAiVisionObjectCountGet(V5_DeviceT device) { return 0; }

int32_t vexDeviceAiVisionObjectGet(V5_DeviceT device, uint32_t indexObj,
                                   V5_DeviceAiVisionObject *pObject)
{
    if (pObject != nullptr)
    {
        *pObject = {};
    }
    return 0;
}

void vexDeviceAiVisionColorSet(V5_DeviceT device,
                               V5_DeviceAiVisionColor *pColor)
{
}

bool vexDeviceAiVisionColorGet(V5_DeviceT device, uint32_t id,
                               V5_DeviceAiVisionColor *pColor)
{
    if (pColor != nullptr)
    {
        *pColor = {};
    }
    return false;
}

void vexDeviceAiVisionCodeSet(V5_DeviceT device, V5_DeviceAiVisionCode *pCode)
{
}

bool vexDeviceAiVisionCodeGet(V5_DeviceT device, uint32_t id,
                              V5_DeviceAiVisionCode *pCode)
{
    if (pCode != nullptr)
    {
        *pCode = {};
    }
    return false;
}

// Only the terminator is written, rather than the whole 20 byte buffer the
// caller is asked for, so the name reads as an empty string without assuming
// more of the caller's memory than one byte.
int32_t vexDeviceAiVisionClassNameGet(V5_DeviceT device, int32_t id,
                                      uint8_t *pName)
{
    if (pName != nullptr)
    {
        pName[0] = '\0';
    }
    return 0;
}

double vexDeviceAiVisionTemperatureGet(V5_DeviceT device) { return 0.0; }

// IMU

void vexDeviceImuReset(V5_DeviceT device) {}

double vexDeviceImuHeadingGet(V5_DeviceT device) { return 0.0; }

double vexDeviceImuDegreesGet(V5_DeviceT device) { return 0.0; }

void vexDeviceImuQuaternionGet(V5_DeviceT device, V5_DeviceImuQuaternion *data)
{
    if (data != nullptr)
    {
        *data = {};
    }
}

void vexDeviceImuAttitudeGet(V5_DeviceT device, V5_DeviceImuAttitude *data)
{
    if (data != nullptr)
    {
        *data = {};
    }
}

void vexDeviceImuRawGyroGet(V5_DeviceT device, V5_DeviceImuRaw *data)
{
    if (data != nullptr)
    {
        *data = {};
    }
}

void vexDeviceImuRawAccelGet(V5_DeviceT device, V5_DeviceImuRaw *data)
{
    if (data != nullptr)
    {
        *data = {};
    }
}

uint32_t vexDeviceImuStatusGet(V5_DeviceT device) { return 0; }

void vexDeviceImuModeSet(V5_DeviceT device, uint32_t mode) {}

uint32_t vexDeviceImuModeGet(V5_DeviceT device) { return 0; }

void vexDeviceImuDataRateSet(V5_DeviceT device, uint32_t rate) {}

// Rangefinder/Lidar - actual API to be determined

int32_t vexDeviceRangeValueGet(V5_DeviceT device) { return 0; }

// Absolute encoder

void vexDeviceAbsEncReset(V5_DeviceT device) {}

void vexDeviceAbsEncPositionSet(V5_DeviceT device, int32_t position) {}

int32_t vexDeviceAbsEncPositionGet(V5_DeviceT device) { return 0; }

int32_t vexDeviceAbsEncVelocityGet(V5_DeviceT device) { return 0; }

int32_t vexDeviceAbsEncAngleGet(V5_DeviceT device) { return 0; }

void vexDeviceAbsEncReverseFlagSet(V5_DeviceT device, bool value) {}

bool vexDeviceAbsEncReverseFlagGet(V5_DeviceT device) { return false; }

uint32_t vexDeviceAbsEncStatusGet(V5_DeviceT device) { return 0; }

void vexDeviceAbsEncDataRateSet(V5_DeviceT device, uint32_t rate) {}

// Optical/color sensor

double vexDeviceOpticalHueGet(V5_DeviceT device) { return 0.0; }

double vexDeviceOpticalSatGet(V5_DeviceT device) { return 0.0; }

double vexDeviceOpticalBrightnessGet(V5_DeviceT device) { return 0.0; }

int32_t vexDeviceOpticalProximityGet(V5_DeviceT device) { return 0; }

void vexDeviceOpticalRgbGet(V5_DeviceT device, V5_DeviceOpticalRgb *data)
{
    if (data != nullptr)
    {
        *data = {};
    }
}

void vexDeviceOpticalLedPwmSet(V5_DeviceT device, int32_t value) {}

int32_t vexDeviceOpticalLedPwmGet(V5_DeviceT device) { return 0; }

uint32_t vexDeviceOpticalStatusGet(V5_DeviceT device) { return 0; }

void vexDeviceOpticalRawGet(V5_DeviceT device, V5_DeviceOpticalRaw *data)
{
    if (data != nullptr)
    {
        *data = {};
    }
}

void vexDeviceOpticalModeSet(V5_DeviceT device, uint32_t mode) {}

uint32_t vexDeviceOpticalModeGet(V5_DeviceT device) { return 0; }

uint32_t vexDeviceOpticalGestureGet(V5_DeviceT device,
                                    V5_DeviceOpticalGesture *pData)
{
    if (pData != nullptr)
    {
        *pData = {};
    }
    return 0;
}

void vexDeviceOpticalGestureEnable(V5_DeviceT device) {}

void vexDeviceOpticalGestureDisable(V5_DeviceT device) {}

int32_t vexDeviceOpticalProximityThreshold(V5_DeviceT device, int32_t value)
{
    return 0;
}

void vexDeviceOpticalIntegrationTimeSet(V5_DeviceT device, double timeMs) {}

double vexDeviceOpticalIntegrationTimeGet(V5_DeviceT device) { return 0.0; }

// Electro magnet

void vexDeviceMagnetPowerSet(V5_DeviceT device, int32_t value, int32_t time) {}

int32_t vexDeviceMagnetPowerGet(V5_DeviceT device) { return 0; }

void vexDeviceMagnetPickup(V5_DeviceT device, V5_DeviceMagnetDuration duration)
{
}

void vexDeviceMagnetDrop(V5_DeviceT device, V5_DeviceMagnetDuration duration) {}

double vexDeviceMagnetTemperatureGet(V5_DeviceT device) { return 0.0; }

double vexDeviceMagnetCurrentGet(V5_DeviceT device) { return 0.0; }

uint32_t vexDeviceMagnetStatusGet(V5_DeviceT device) { return 0; }

// Distance

uint32_t vexDeviceDistanceDistanceGet(V5_DeviceT device) { return 0; }

uint32_t vexDeviceDistanceConfidenceGet(V5_DeviceT device) { return 0; }

int32_t vexDeviceDistanceObjectSizeGet(V5_DeviceT device) { return 0; }

double vexDeviceDistanceObjectVelocityGet(V5_DeviceT device) { return 0.0; }

uint32_t vexDeviceDistanceStatusGet(V5_DeviceT device) { return 0; }

// Gps

void vexDeviceGpsReset(V5_DeviceT device) {}

double vexDeviceGpsHeadingGet(V5_DeviceT device) { return 0.0; }

double vexDeviceGpsDegreesGet(V5_DeviceT device) { return 0.0; }

void vexDeviceGpsQuaternionGet(V5_DeviceT device, V5_DeviceGpsQuaternion *data)
{
    if (data != nullptr)
    {
        *data = {};
    }
}

void vexDeviceGpsAttitudeGet(V5_DeviceT device, V5_DeviceGpsAttitude *data,
                             bool bRaw)
{
    if (data != nullptr)
    {
        *data = {};
    }
}

void vexDeviceGpsRawGyroGet(V5_DeviceT device, V5_DeviceGpsRaw *data)
{
    if (data != nullptr)
    {
        *data = {};
    }
}

void vexDeviceGpsRawAccelGet(V5_DeviceT device, V5_DeviceGpsRaw *data)
{
    if (data != nullptr)
    {
        *data = {};
    }
}

uint32_t vexDeviceGpsStatusGet(V5_DeviceT device) { return 0; }

void vexDeviceGpsModeSet(V5_DeviceT device, uint32_t mode) {}

uint32_t vexDeviceGpsModeGet(V5_DeviceT device) { return 0; }

void vexDeviceGpsDataRateSet(V5_DeviceT device, uint32_t rate) {}

void vexDeviceGpsOriginSet(V5_DeviceT device, double ox, double oy) {}

void vexDeviceGpsOriginGet(V5_DeviceT device, double *ox, double *oy)
{
    if (ox != nullptr)
    {
        *ox = 0.0;
    }
    if (oy != nullptr)
    {
        *oy = 0.0;
    }
}

void vexDeviceGpsRotationSet(V5_DeviceT device, double value) {}

double vexDeviceGpsRotationGet(V5_DeviceT device) { return 0.0; }

void vexDeviceGpsInitialPositionSet(V5_DeviceT device, double initial_x,
                                    double initial_y, double initial_rotation)
{
}

double vexDeviceGpsErrorGet(V5_DeviceT device) { return 0.0; }

// Generic serial port comms to any device

void vexDeviceGenericSerialEnable(V5_DeviceT device, int32_t options) {}

void vexDeviceGenericSerialBaudrate(V5_DeviceT device, int32_t baudrate) {}

int32_t vexDeviceGenericSerialWriteChar(V5_DeviceT device, uint8_t c)
{
    return 0;
}

int32_t vexDeviceGenericSerialWriteFree(V5_DeviceT device) { return 0; }

int32_t vexDeviceGenericSerialTransmit(V5_DeviceT device, uint8_t *buffer,
                                       int32_t length)
{
    return 0;
}

int32_t vexDeviceGenericSerialReadChar(V5_DeviceT device) { return 0; }

int32_t vexDeviceGenericSerialPeekChar(V5_DeviceT device) { return 0; }

int32_t vexDeviceGenericSerialReceiveAvail(V5_DeviceT device) { return 0; }

int32_t vexDeviceGenericSerialReceive(V5_DeviceT device, uint8_t *buffer,
                                      int32_t length)
{
    return 0;
}

void vexDeviceGenericSerialFlush(V5_DeviceT device) {}

// Radio link between two brains

void vexDeviceGenericRadioConnection(V5_DeviceT device, char *link_id,
                                     int32_t type, bool ov)
{
}

bool vexDeviceGenericRadioLinkStatus(V5_DeviceT device) { return false; }

uint32_t vexDeviceGenericRadioWriteFree(V5_DeviceT device) { return 0; }

uint32_t vexDeviceGenericRadioTransmit(V5_DeviceT device, uint8_t *buffer,
                                       uint32_t length)
{
    return 0;
}

uint32_t vexDeviceGenericRadioReceiveAvail(V5_DeviceT device) { return 0; }

uint32_t vexDeviceGenericRadioReceive(V5_DeviceT device, uint8_t *buffer,
                                      uint32_t length)
{
    return 0;
}

// Display/graphics

void vexDisplayForegroundColor(uint32_t col) {}

void vexDisplayBackgroundColor(uint32_t col) {}

uint32_t vexDisplayForegroundColorGet(void) { return 0; }

uint32_t vexDisplayBackgroundColorGet(void) { return 0; }

void vexDisplayErase(void) {}

void vexDisplayScroll(int32_t nStartLine, int32_t nLines) {}

void vexDisplayScrollRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                          int32_t nLines)
{
}

void vexDisplayCopyRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                        uint32_t *pSrc, int32_t srcStride)
{
}

void vexDisplayPixelSet(uint32_t x, uint32_t y) {}

void vexDisplayPixelClear(uint32_t x, uint32_t y) {}

void vexDisplayLineDraw(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {}

void vexDisplayLineClear(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {}

void vexDisplayRectDraw(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {}

void vexDisplayRectClear(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {}

void vexDisplayRectFill(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {}

void vexDisplayCircleDraw(int32_t xc, int32_t yc, int32_t radius) {}

void vexDisplayCircleClear(int32_t xc, int32_t yc, int32_t radius) {}

void vexDisplayCircleFill(int32_t xc, int32_t yc, int32_t radius) {}

void vexDisplayPrintf(int32_t xpos, int32_t ypos, uint32_t bOpaque,
                      const char *format, ...)
{
}

void vexDisplayString(const int32_t nLineNumber, const char *format, ...) {}

void vexDisplayStringAt(int32_t xpos, int32_t ypos, const char *format, ...) {}

void vexDisplayBigString(const int32_t nLineNumber, const char *format, ...) {}

void vexDisplayBigStringAt(int32_t xpos, int32_t ypos, const char *format, ...)
{
}

void vexDisplaySmallStringAt(int32_t xpos, int32_t ypos, const char *format,
                             ...)
{
}

void vexDisplayCenteredString(const int32_t nLineNumber, const char *format,
                              ...)
{
}

void vexDisplayBigCenteredString(const int32_t nLineNumber, const char *format,
                                 ...)
{
}

// Not really for user code but we need these for some wrapper functions

void vexDisplayVPrintf(int32_t xpos, int32_t ypos, uint32_t bOpaque,
                       const char *format, va_list args)
{
}

void vexDisplayVString(const int32_t nLineNumber, const char *format,
                       va_list args)
{
}

void vexDisplayVStringAt(int32_t xpos, int32_t ypos, const char *format,
                         va_list args)
{
}

void vexDisplayVBigString(const int32_t nLineNumber, const char *format,
                          va_list args)
{
}

void vexDisplayVBigStringAt(int32_t xpos, int32_t ypos, const char *format,
                            va_list args)
{
}

void vexDisplayVSmallStringAt(int32_t xpos, int32_t ypos, const char *format,
                              va_list args)
{
}

void vexDisplayVCenteredString(const int32_t nLineNumber, const char *format,
                               va_list args)
{
}

void vexDisplayVBigCenteredString(const int32_t nLineNumber, const char *format,
                                  va_list args)
{
}

void vexDisplayTextSize(uint32_t n, uint32_t d) {}

void vexDisplayFontNamedSet(const char *pFontName) {}

int32_t vexDisplayStringWidthGet(const char *pString) { return 0; }

int32_t vexDisplayStringHeightGet(const char *pString) { return 0; }

bool vexDisplayRender(bool bVsyncWait, bool bRunScheduler) { return false; }

void vexDisplayDoubleBufferDisable(void) {}

void vexDisplayClipRegionSet(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {}

void vexDisplayClipRegionClear() {}

uint32_t vexImageBmpRead(const uint8_t *ibuf, v5_image *oBuf, uint32_t maxw,
                         uint32_t maxh)
{
    return 0;
}

uint32_t vexImagePngRead(const uint8_t *ibuf, v5_image *oBuf, uint32_t maxw,
                         uint32_t maxh, uint32_t ibuflen)
{
    return 0;
}

// special use only ! Talk to James.

int32_t vexScratchMemoryPtr(void **ptr)
{
    if (ptr != nullptr)
    {
        *ptr = nullptr;
    }
    return 0;
}

bool vexScratchMemoryLock(void) { return false; }

void vexScratchMemoryUnlock(void) {}

// SD card

FRESULT vexFileMountSD(void) { return FR_OK; }

FRESULT vexFileDirectoryGet(const char *path, char *buffer, uint32_t len)
{
    if (buffer != nullptr && len > 0)
    {
        buffer[0] = '\0';
    }
    return FR_OK;
}

FIL *vexFileOpen(const char *filename, const char *mode) { return nullptr; }

FIL *vexFileOpenWrite(const char *filename) { return nullptr; }

FIL *vexFileOpenCreate(const char *filename) { return nullptr; }

void vexFileClose(FIL *fdp) {}

int32_t vexFileRead(char *buf, uint32_t size, uint32_t nItems, FIL *fdp)
{
    return 0;
}

int32_t vexFileWrite(char *buf, uint32_t size, uint32_t nItems, FIL *fdp)
{
    return 0;
}

int32_t vexFileSize(FIL *fdp) { return 0; }

FRESULT vexFileSeek(FIL *fdp, uint32_t offset, int32_t whence) { return FR_OK; }

bool vexFileDriveStatus(uint32_t drive) { return false; }

int32_t vexFileTell(FIL *fdp) { return 0; }

void vexFileSync(FIL *fdp) {}

uint32_t vexFileStatus(const char *filename) { return 0; }

// CDC

int32_t vexSerialWriteChar(uint32_t channel, uint8_t c) { return 0; }

int32_t vexSerialWriteBuffer(uint32_t channel, uint8_t *data, uint32_t data_len)
{
    return 0;
}

int32_t vexSerialReadChar(uint32_t channel) { return 0; }

int32_t vexSerialPeekChar(uint32_t channel) { return 0; }

int32_t vexSerialWriteFree(uint32_t channel) { return 0; }

// rtos hooks

void vexSystemTimerStop() {}

void vexSystemTimerClearInterrupt() {}

int32_t vexSystemTimerReinitForRtos(uint32_t priority,
                                    void (*handler)(void *data))
{
    return 0;
}

void vexSystemApplicationIRQHandler(uint32_t ulICCIAR) {}

int32_t vexSystemWatchdogReinitRtos(void) { return 0; }

uint32_t vexSystemWatchdogGet(void) { return 0; }

// Hooks into the interrupt system, some of these will be used by FreeRTOS
// the others are just placeholders for now

void vexSystemBoot(void)
{
    // Convert device vector to LUT.
    bromine::Sim &sim = bromine::getSim();

    // First 21 ports can actually be devices, so scan the simulator's device
    // list for their ports. Could be faster but for 32 devices it doesn't
    // matter.
    for (size_t i = 0; i < 21; i++)
    {
        struct _V5_Device elem = {nullptr, kDeviceTypeNoSensor};
        for (auto &device : sim.robot.devices)
        {
            uint8_t port;
            V5_DeviceType type;

            if (bromine::Motor *motor =
                    dynamic_cast<bromine::Motor *>(device.get()))
            {
                port = motor->port;
                type = kDeviceTypeMotorSensor;
            }
            else
            {
                continue;
            }

            if (i == portToIdx(port))
            {
                if (elem.ptr != nullptr)
                {
                    // PROS doesn't do anything about duplicate ports, so we
                    // shouldn't either, but it's best to at least warn about
                    // it.
                    bromine::warning("multiple devices on port {}",
                                     idxToPort(i));
                }
                else
                {
                    elem.ptr = device.get();
                    elem.type = type;
                }
            }
        }
        portTable[i] = elem;
    }

    // Rest of the ports are reserved internally for the radio, controller, etc.
    // Clear them for now.
    for (size_t i = 21; i < portTable.size(); i++)
    {
        portTable[i] = {nullptr, kDeviceTypeNoSensor};
    }
}

void vexSystemUndefinedException(void) {}

void vexSystemFIQInterrupt(void) {}

void vexSystemIQRQnterrupt(void) {}

void vexSystemSWInterrupt(void) {}

void vexSystemDataAbortInterrupt(void) {}

void vexSystemPrefetchAbortInterrupt(void) {}

// touch

void vexTouchUserCallbackSet(void (*callback)(V5_TouchEvent, int32_t, int32_t))
{
}

bool vexTouchDataGet(V5_TouchStatus *status)
{
    if (status != nullptr)
    {
        *status = {};
    }
    return false;
}

// system utility

uint32_t vexSystemVersion(void) { return 0; }

uint32_t vexStdlibVersion(void) { return 0; }

// get SDK version

uint32_t vexSdkVersion(void) { return 0; }

// duplication of prototypes in v5_util.h for use by user code

uint32_t vexStdlibVersionLinked(void) { return 0; }

bool vexStdlibVersionVerify(void) { return false; }

// comp status

uint32_t vexCompetitionStatus(void) { return 0; }

void vexCompetitionControl(uint32_t data) {}

// battery

int32_t vexBatteryVoltageGet(void) { return 0; }

int32_t vexBatteryCurrentGet(void) { return 0; }

double vexBatteryTemperatureGet(void) { return 0.0; }

double vexBatteryCapacityGet(void) { return 0.0; }
