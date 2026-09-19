/**
 * @file    v5_apiuser.cpp
 * @brief   Port indexed wrappers for the V5 API, declared in v5_apiuser.h.
 *
 * @details
 *  Each wrapper looks up the device handle for a zero based port index and
 *  hands the call to its vexDevice counterpart in v5_api.cpp, which is where
 *  the behaviour lives. They are stubs only for as long as the functions they
 *  call are.
 */

#include "v5_api.h"

void vexDelay(uint32_t timems) {}

void vexLedSet(uint32_t index, V5_DeviceLedColor value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceLedSet(device, value);
}

void vexLedRgbSet(uint32_t index, uint32_t color)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceLedRgbSet(device, color);
}

V5_DeviceLedColor vexLedGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceLedGet(device);
}

uint32_t vexLedRgbGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceLedRgbGet(device);
}

void vexAdiPortConfigSet(uint32_t index, uint32_t port,
                         V5_AdiPortConfiguration type)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceAdiPortConfigSet(device, port, type);
}

V5_AdiPortConfiguration vexAdiPortConfigGet(uint32_t index, uint32_t port)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceAdiPortConfigGet(device, port);
}

void vexAdiValueSet(uint32_t index, uint32_t port, int32_t value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceAdiValueSet(device, port, value);
}

int32_t vexAdiValueGet(uint32_t index, uint32_t port)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceAdiValueGet(device, port);
}

V5_DeviceBumperState vexBumperGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceBumperGet(device);
}

void vexGyroReset(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGyroReset(device);
}

double vexGyroHeadingGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGyroHeadingGet(device);
}

double vexGyroDegreesGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGyroDegreesGet(device);
}

int32_t vexSonarValueGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceSonarValueGet(device);
}

int32_t vexGenericValueGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGenericValueGet(device);
}

void vexMotorVelocitySet(uint32_t index, int32_t velocity)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorVelocitySet(device, velocity);
}

void vexMotorVelocityUpdate(uint32_t index, int32_t velocity)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorVelocityUpdate(device, velocity);
}

void vexMotorVoltageSet(uint32_t index, int32_t value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorVoltageSet(device, value);
}

int32_t vexMotorVelocityGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorVelocityGet(device);
}

int32_t vexMotorDirectionGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorDirectionGet(device);
}

double vexMotorActualVelocityGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorActualVelocityGet(device);
}

void vexMotorModeSet(uint32_t index, V5MotorControlMode mode)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorModeSet(device, mode);
}

V5MotorControlMode vexMotorModeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorModeGet(device);
}

void vexMotorPwmSet(uint32_t index, int32_t value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorPwmSet(device, value);
}

int32_t vexMotorPwmGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorPwmGet(device);
}

void vexMotorCurrentLimitSet(uint32_t index, int32_t value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorCurrentLimitSet(device, value);
}

int32_t vexMotorCurrentLimitGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorCurrentLimitGet(device);
}

void vexMotorVoltageLimitSet(uint32_t index, int32_t value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorVoltageLimitSet(device, value);
}

int32_t vexMotorVoltageLimitGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorVoltageLimitGet(device);
}

void vexMotorPositionPidSet(uint32_t index, V5_DeviceMotorPid *pid)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorPositionPidSet(device, pid);
}

void vexMotorVelocityPidSet(uint32_t index, V5_DeviceMotorPid *pid)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorVelocityPidSet(device, pid);
}

int32_t vexMotorCurrentGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorCurrentGet(device);
}

int32_t vexMotorVoltageGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorVoltageGet(device);
}

double vexMotorPowerGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorPowerGet(device);
}

double vexMotorTorqueGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorTorqueGet(device);
}

double vexMotorEfficiencyGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorEfficiencyGet(device);
}

double vexMotorTemperatureGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorTemperatureGet(device);
}

bool vexMotorOverTempFlagGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorOverTempFlagGet(device);
}

bool vexMotorCurrentLimitFlagGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorCurrentLimitFlagGet(device);
}

uint32_t vexMotorFaultsGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorFaultsGet(device);
}

bool vexMotorZeroVelocityFlagGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorZeroVelocityFlagGet(device);
}

bool vexMotorZeroPositionFlagGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorZeroPositionFlagGet(device);
}

uint32_t vexMotorFlagsGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorFlagsGet(device);
}

void vexMotorReverseFlagSet(uint32_t index, bool value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorReverseFlagSet(device, value);
}

bool vexMotorReverseFlagGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorReverseFlagGet(device);
}

void vexMotorEncoderUnitsSet(uint32_t index, V5MotorEncoderUnits units)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorEncoderUnitsSet(device, units);
}

V5MotorEncoderUnits vexMotorEncoderUnitsGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorEncoderUnitsGet(device);
}

void vexMotorBrakeModeSet(uint32_t index, V5MotorBrakeMode mode)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorBrakeModeSet(device, mode);
}

V5MotorBrakeMode vexMotorBrakeModeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorBrakeModeGet(device);
}

void vexMotorPositionSet(uint32_t index, double position)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorPositionSet(device, position);
}

double vexMotorPositionGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorPositionGet(device);
}

int32_t vexMotorPositionRawGet(uint32_t index, uint32_t *timestamp)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorPositionRawGet(device, timestamp);
}

void vexMotorPositionReset(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorPositionReset(device);
}

double vexMotorTargetGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorTargetGet(device);
}

void vexMotorServoTargetSet(uint32_t index, double position)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorServoTargetSet(device, position);
}

void vexMotorAbsoluteTargetSet(uint32_t index, double position,
                               int32_t velocity)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorAbsoluteTargetSet(device, position, velocity);
}

void vexMotorRelativeTargetSet(uint32_t index, double position,
                               int32_t velocity)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorRelativeTargetSet(device, position, velocity);
}

void vexMotorGearingSet(uint32_t index, V5MotorGearset value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorGearingSet(device, value);
}

V5MotorGearset vexMotorGearingGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMotorGearingGet(device);
}

void vexMotorExternalProfileSet(uint32_t index, double position,
                                int32_t velocity)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMotorExternalProfileSet(device, position, velocity);
}

void vexVisionModeSet(uint32_t index, V5VisionMode mode)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceVisionModeSet(device, mode);
}

V5VisionMode vexVisionModeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionModeGet(device);
}

int32_t vexVisionObjectCountGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionObjectCountGet(device);
}

int32_t vexVisionObjectGet(uint32_t index, uint32_t indexObj,
                           V5_DeviceVisionObject *pObject)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionObjectGet(device, indexObj, pObject);
}

void vexVisionSignatureSet(uint32_t index, V5_DeviceVisionSignature *pSignature)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceVisionSignatureSet(device, pSignature);
}

bool vexVisionSignatureGet(uint32_t index, uint32_t id,
                           V5_DeviceVisionSignature *pSignature)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionSignatureGet(device, id, pSignature);
}

void vexVisionBrightnessSet(uint32_t index, uint8_t percent)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceVisionBrightnessSet(device, percent);
}

uint8_t vexVisionBrightnessGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionBrightnessGet(device);
}

void vexVisionWhiteBalanceModeSet(uint32_t index, V5VisionWBMode mode)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceVisionWhiteBalanceModeSet(device, mode);
}

V5VisionWBMode vexVisionWhiteBalanceModeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionWhiteBalanceModeGet(device);
}

void vexVisionWhiteBalanceSet(uint32_t index, V5_DeviceVisionRgb color)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceVisionWhiteBalanceSet(device, color);
}

V5_DeviceVisionRgb vexVisionWhiteBalanceGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionWhiteBalanceGet(device);
}

void vexVisionLedModeSet(uint32_t index, V5VisionLedMode mode)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceVisionLedModeSet(device, mode);
}

V5VisionLedMode vexVisionLedModeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionLedModeGet(device);
}

void vexVisionLedBrigntnessSet(uint32_t index, uint8_t percent)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceVisionLedBrigntnessSet(device, percent);
}

uint8_t vexVisionLedBrigntnessGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionLedBrigntnessGet(device);
}

void vexVisionLedColorSet(uint32_t index, V5_DeviceVisionRgb color)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceVisionLedColorSet(device, color);
}

V5_DeviceVisionRgb vexVisionLedColorGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionLedColorGet(device);
}

void vexVisionWifiModeSet(uint32_t index, V5VisionWifiMode mode)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceVisionWifiModeSet(device, mode);
}

V5VisionWifiMode vexVisionWifiModeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceVisionWifiModeGet(device);
}

void vexImuReset(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceImuReset(device);
}

double vexImuHeadingGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceImuHeadingGet(device);
}

double vexImuDegreesGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceImuDegreesGet(device);
}

void vexImuQuaternionGet(uint32_t index, V5_DeviceImuQuaternion *data)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceImuQuaternionGet(device, data);
}

void vexImuAttitudeGet(uint32_t index, V5_DeviceImuAttitude *data)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceImuAttitudeGet(device, data);
}

void vexImuRawGyroGet(uint32_t index, V5_DeviceImuRaw *data)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceImuRawGyroGet(device, data);
}

void vexImuRawAccelGet(uint32_t index, V5_DeviceImuRaw *data)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceImuRawAccelGet(device, data);
}

uint32_t vexImuStatusGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceImuStatusGet(device);
}

void vexImuModeSet(uint32_t index, uint32_t mode)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceImuModeSet(device, mode);
}

uint32_t vexImuModeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceImuModeGet(device);
}

void vexImuDataRateSet(uint32_t index, uint32_t rate)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceImuDataRateSet(device, rate);
}

int32_t vexRangeValueGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceRangeValueGet(device);
}

void vexAbsEncReset(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceAbsEncReset(device);
}

void vexAbsEncPositionSet(uint32_t index, int32_t position)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceAbsEncPositionSet(device, position);
}

int32_t vexAbsEncPositionGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceAbsEncPositionGet(device);
}

int32_t vexAbsEncVelocityGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceAbsEncVelocityGet(device);
}

int32_t vexAbsEncAngleGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceAbsEncAngleGet(device);
}

void vexAbsEncReverseFlagSet(uint32_t index, bool value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceAbsEncReverseFlagSet(device, value);
}

bool vexAbsEncReverseFlagGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceAbsEncReverseFlagGet(device);
}

uint32_t vexAbsEncStatusGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceAbsEncStatusGet(device);
}

void vexAbsEncDataRateSet(uint32_t index, uint32_t rate)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceAbsEncDataRateSet(device, rate);
}

double vexOpticalHueGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceOpticalHueGet(device);
}

double vexOpticalSatGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceOpticalSatGet(device);
}

double vexOpticalBrightnessGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceOpticalBrightnessGet(device);
}

int32_t vexOpticalProximityGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceOpticalProximityGet(device);
}

void vexOpticalRgbGet(uint32_t index, V5_DeviceOpticalRgb *data)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceOpticalRgbGet(device, data);
}

void vexOpticalLedPwmSet(uint32_t index, int32_t value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceOpticalLedPwmSet(device, value);
}

int32_t vexOpticalLedPwmGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceOpticalLedPwmGet(device);
}

uint32_t vexOpticalStatusGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceOpticalStatusGet(device);
}

void vexOpticalRawGet(uint32_t index, V5_DeviceOpticalRaw *data)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceOpticalRawGet(device, data);
}

void vexOpticalModeSet(uint32_t index, uint32_t mode)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceOpticalModeSet(device, mode);
}

uint32_t vexOpticalModeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceOpticalModeGet(device);
}

uint32_t vexOpticalGestureGet(uint32_t index, V5_DeviceOpticalGesture *pData)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceOpticalGestureGet(device, pData);
}

void vexOpticalGestureEnable(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceOpticalGestureEnable(device);
}

void vexOpticalGestureDisable(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceOpticalGestureDisable(device);
}

int32_t vexOpticalProximityThreshold(uint32_t index, int32_t value)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceOpticalProximityThreshold(device, value);
}

void vexOpticalIntegrationTimeSet(uint32_t index, double timems)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceOpticalIntegrationTimeSet(device, timems);
}

double vexOpticalIntegrationTimeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceOpticalIntegrationTimeGet(device);
}

void vexMagnetPowerSet(uint32_t index, int32_t value, int32_t time)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMagnetPowerSet(device, value, time);
}

int32_t vexMagnetPowerGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMagnetPowerGet(device);
}

void vexMagnetPickup(uint32_t index, V5_DeviceMagnetDuration duration)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMagnetPickup(device, duration);
}

void vexMagnetDrop(uint32_t index, V5_DeviceMagnetDuration duration)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceMagnetDrop(device, duration);
}

double vexMagnetTemperatureGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMagnetTemperatureGet(device);
}

double vexMagnetCurrentGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMagnetCurrentGet(device);
}

uint32_t vexMagnetStatusGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceMagnetStatusGet(device);
}

uint32_t vexDistanceDistanceGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceDistanceDistanceGet(device);
}

uint32_t vexDistanceConfidenceGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceDistanceConfidenceGet(device);
}

int32_t vexDistanceObjectSizeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceDistanceObjectSizeGet(device);
}

double vexDistanceObjectVelocityGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceDistanceObjectVelocityGet(device);
}

uint32_t vexDistanceStatusGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceDistanceStatusGet(device);
}

void vexGpsReset(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsReset(device);
}

double vexGpsHeadingGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGpsHeadingGet(device);
}

double vexGpsDegreesGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGpsDegreesGet(device);
}

void vexGpsQuaternionGet(uint32_t index, V5_DeviceGpsQuaternion *data)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsQuaternionGet(device, data);
}

void vexGpsAttitudeGet(uint32_t index, V5_DeviceGpsAttitude *data, bool bRaw)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsAttitudeGet(device, data, bRaw);
}

void vexGpsRawGyroGet(uint32_t index, V5_DeviceGpsRaw *data)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsRawGyroGet(device, data);
}

void vexGpsRawAccelGet(uint32_t index, V5_DeviceGpsRaw *data)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsRawAccelGet(device, data);
}

uint32_t vexGpsStatusGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGpsStatusGet(device);
}

void vexGpsModeSet(uint32_t index, uint32_t mode)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsModeSet(device, mode);
}

uint32_t vexGpsModeGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGpsModeGet(device);
}

void vexGpsDataRateSet(uint32_t index, uint32_t rate)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsDataRateSet(device, rate);
}

void vexGpsOriginSet(uint32_t index, double ox, double oy)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsOriginSet(device, ox, oy);
}

void vexGpsOriginGet(uint32_t index, double *ox, double *oy)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsOriginGet(device, ox, oy);
}

void vexGpsRotationSet(uint32_t index, double value)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsRotationSet(device, value);
}

double vexGpsRotationGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGpsRotationGet(device);
}

void vexGpsInitialPositionSet(uint32_t index, double initial_x,
                              double initial_y, double initial_rotation)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGpsInitialPositionSet(device, initial_x, initial_y,
                                   initial_rotation);
}

double vexGpsErrorGet(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGpsErrorGet(device);
}

// Generic serial port comms to any device

void vexGenericSerialEnable(uint32_t index, int32_t options)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGenericSerialEnable(device, options);
}

void vexGenericSerialBaudrate(uint32_t index, int32_t baudrate)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGenericSerialBaudrate(device, baudrate);
}

int32_t vexGenericSerialWriteChar(uint32_t index, uint8_t c)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGenericSerialWriteChar(device, c);
}

int32_t vexGenericSerialWriteFree(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGenericSerialWriteFree(device);
}

int32_t vexGenericSerialTransmit(uint32_t index, uint8_t *buffer,
                                 int32_t length)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGenericSerialTransmit(device, buffer, length);
}

int32_t vexGenericSerialReadChar(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGenericSerialReadChar(device);
}

int32_t vexGenericSerialPeekChar(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGenericSerialPeekChar(device);
}

int32_t vexGenericSerialReceiveAvail(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGenericSerialReceiveAvail(device);
}

int32_t vexGenericSerialReceive(uint32_t index, uint8_t *buffer, int32_t length)
{
    VEX_DEVICE_GET(device, index);
    return vexDeviceGenericSerialReceive(device, buffer, length);
}

void vexGenericSerialFlush(uint32_t index)
{
    VEX_DEVICE_GET(device, index);
    vexDeviceGenericSerialFlush(device);
}
