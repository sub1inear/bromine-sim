/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Innovation First 2016, All rights reserved.               */
/*                                                                            */
/*    Module:     v5_apiuser.h                                                */
/*    Author:     James Pearman                                               */
/*    Created:    8 Nov 2016                                                  */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifndef V5_APIUSER_H_
#define V5_APIUSER_H_

#include "stdbool.h"
#include "stdint.h"

/*----------------------------------------------------------------------------*/
/** @file    v5_apiuser.h
 * @brief   Header for V5 API device wrapper functions
 *
 * @details
 *  Every function here is a thin wrapper around the matching vexDevice call in
 *  v5_api.h. The only difference is the first argument: these take a port index
 *  instead of a #V5_DeviceT handle, and look the handle up with
 *  vexDeviceGetByIndex() on every call.
 *
 *  The index is zero based, so smart port 1 is index 0 and smart port 21 is
 *  index 20. The ports printed on the brain are numbered from 1, which is why
 *  PROS passes port - 1 into every one of these calls.
 *
 *  Like the underlying calls, none of these check that the port holds the kind
 *  of device being asked about, and what VEXos returns for a mismatched device
 *  is not documented. Check the port's type with vexDeviceGetStatus() first.
 *
 *  Behaviour, units and value ranges are documented once on the vexDevice
 *  functions in v5_api.h; each wrapper points at its counterpart with @see.
 *//*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief   Blocks the calling task for a period of time.
 * @param   timems How long to wait, in milliseconds.
 * @details Uses whatever scheduler VEXos is running. A runtime with its own
 *          scheduler, such as PROS, has its own delay that should be used
 *          instead so the wait is visible to that scheduler.
 */
void vexDelay(uint32_t timems);

/**
 * @brief   Sets an LED module to one of the preset colors.
 * @param   index Zero based smart port index.
 * @param   value Color to show.
 * @see     vexDeviceLedSet
 */
void vexLedSet(uint32_t index, V5_DeviceLedColor value);

/**
 * @brief   Sets an LED module to an arbitrary color.
 * @param   index Zero based smart port index.
 * @param   color Color as 0x00RRGGBB.
 * @see     vexDeviceLedRgbSet
 */
void vexLedRgbSet(uint32_t index, uint32_t color);

/**
 * @brief   Reads back an LED module's color as a preset.
 * @param   index Zero based smart port index.
 * @return  The current color.
 * @see     vexDeviceLedGet
 */
V5_DeviceLedColor vexLedGet(uint32_t index);

/**
 * @brief   Reads back an LED module's color.
 * @param   index Zero based smart port index.
 * @return  The current color as 0x00RRGGBB.
 * @see     vexDeviceLedRgbGet
 */
uint32_t vexLedRgbGet(uint32_t index);

/**
 * @brief   Configures one 3-Wire channel.
 * @param   index Zero based smart port index of the brain's 3-Wire ports or
 * a 3-Wire Expander.
 * @param   port  Channel number, 0 to 7, for the ports labelled A to H.
 * @param   type  What the channel is wired to.
 * @see     vexDeviceAdiPortConfigSet
 */
void vexAdiPortConfigSet(uint32_t index, uint32_t port,
                         V5_AdiPortConfiguration type);

/**
 * @brief   Reads back how a 3-Wire channel is configured.
 * @param   index Zero based smart port index.
 * @param   port  Channel number, 0 to 7.
 * @return  The channel's configuration.
 * @see     vexDeviceAdiPortConfigGet
 */
V5_AdiPortConfiguration vexAdiPortConfigGet(uint32_t index, uint32_t port);

/**
 * @brief   Writes a value to a 3-Wire channel.
 * @param   index Zero based smart port index.
 * @param   port  Channel number, 0 to 7.
 * @param   value Value to write, interpreted according to the channel's
 *                configuration.
 * @see     vexDeviceAdiValueSet
 */
void vexAdiValueSet(uint32_t index, uint32_t port, int32_t value);

/**
 * @brief   Reads a 3-Wire channel.
 * @param   index Zero based smart port index.
 * @param   port  Channel number, 0 to 7.
 * @return  The channel's reading, interpreted according to its
 * configuration.
 * @see     vexDeviceAdiValueGet
 */
int32_t vexAdiValueGet(uint32_t index, uint32_t port);

/**
 * @brief   Writes a run of colors to an addressable LED strip.
 * @param   index   Zero based smart port index.
 * @param   port    Channel number, 0 to 7.
 * @param   pData   Colors to write, one 0x00RRGGBB word per LED.
 * @param   nOffset Index of the first LED to write.
 * @param   nLength Number of LEDs to write.
 * @param   options Option flags. VEX does not publish their meaning.
 * @return  The number of LEDs written.
 * @see     vexDeviceAdiAddrLedSet
 */
int32_t vexAdiAddrLedSet(uint32_t index, uint32_t port, uint32_t *pData,
                         uint32_t nOffset, uint32_t nLength, uint32_t options);

/**
 * @brief   Reads a smart port bumper switch.
 * @param   index Zero based smart port index.
 * @return  Whether the switch is pressed.
 * @see     vexDeviceBumperGet
 */
V5_DeviceBumperState vexBumperGet(uint32_t index);

/**
 * @brief   Restarts calibration on a dedicated gyro.
 * @param   index Zero based smart port index.
 * @see     vexDeviceGyroReset
 */
void vexGyroReset(uint32_t index);

/**
 * @brief   Reads a dedicated gyro's accumulated rotation.
 * @param   index Zero based smart port index.
 * @return  The rotation in degrees, unbounded.
 * @see     vexDeviceGyroHeadingGet
 */
double vexGyroHeadingGet(uint32_t index);

/**
 * @brief   Reads a dedicated gyro's heading.
 * @param   index Zero based smart port index.
 * @return  The heading in degrees, 0 through 360.
 * @see     vexDeviceGyroDegreesGet
 */
double vexGyroDegreesGet(uint32_t index);

/**
 * @brief   Reads a dedicated smart port sonar.
 * @param   index Zero based smart port index.
 * @return  The measured distance.
 * @see     vexDeviceSonarValueGet
 */
int32_t vexSonarValueGet(uint32_t index);

/**
 * @brief   Reads a device VEXos classifies as a generic sensor.
 * @param   index Zero based smart port index.
 * @return  The sensor's reading, or 0 if the device is not a generic
 * sensor.
 * @see     vexDeviceGenericValueGet
 */
int32_t vexGenericValueGet(uint32_t index);

/**
 * @brief   Spins a motor at a target velocity.
 * @param   index    Zero based smart port index.
 * @param   velocity Target velocity in RPM at the output shaft.
 * @see     vexDeviceMotorVelocitySet
 */
void vexMotorVelocitySet(uint32_t index, int32_t velocity);

/**
 * @brief   Changes the velocity limit of a motion already in progress.
 * @param   index    Zero based smart port index.
 * @param   velocity New velocity limit in RPM.
 * @see     vexDeviceMotorVelocityUpdate
 */
void vexMotorVelocityUpdate(uint32_t index, int32_t velocity);

/**
 * @brief   Drives a motor open loop at a given voltage.
 * @param   index Zero based smart port index.
 * @param   value Voltage in millivolts, -12000 to 12000.
 * @see     vexDeviceMotorVoltageSet
 */
void vexMotorVoltageSet(uint32_t index, int32_t value);

/**
 * @brief   Reads back a motor's commanded velocity.
 * @param   index Zero based smart port index.
 * @return  The target velocity in RPM.
 * @see     vexDeviceMotorVelocityGet
 */
int32_t vexMotorVelocityGet(uint32_t index);

/**
 * @brief   Reads which way a motor is turning.
 * @param   index Zero based smart port index.
 * @return  1 when turning forwards, -1 when turning backwards.
 * @see     vexDeviceMotorDirectionGet
 */
int32_t vexMotorDirectionGet(uint32_t index);

/**
 * @brief   Reads a motor's measured velocity.
 * @param   index Zero based smart port index.
 * @return  The measured velocity in RPM at the output shaft.
 * @see     vexDeviceMotorActualVelocityGet
 */
double vexMotorActualVelocityGet(uint32_t index);

/**
 * @brief   Sets a motor's control mode directly.
 * @param   index Zero based smart port index.
 * @param   mode  Control mode to switch to.
 * @see     vexDeviceMotorModeSet
 */
void vexMotorModeSet(uint32_t index, V5MotorControlMode mode);

/**
 * @brief   Reads a motor's current control mode.
 * @param   index Zero based smart port index.
 * @return  The control mode.
 * @see     vexDeviceMotorModeGet
 */
V5MotorControlMode vexMotorModeGet(uint32_t index);

/**
 * @brief   Sets a motor's raw PWM output.
 * @param   index Zero based smart port index.
 * @param   value PWM command.
 * @see     vexDeviceMotorPwmSet
 */
void vexMotorPwmSet(uint32_t index, int32_t value);

/**
 * @brief   Reads back a motor's raw PWM output.
 * @param   index Zero based smart port index.
 * @return  The current PWM command.
 * @see     vexDeviceMotorPwmGet
 */
int32_t vexMotorPwmGet(uint32_t index);

/**
 * @brief   Sets the current at which a motor starts backing off.
 * @param   index Zero based smart port index.
 * @param   value Current limit in milliamps.
 * @see     vexDeviceMotorCurrentLimitSet
 */
void vexMotorCurrentLimitSet(uint32_t index, int32_t value);

/**
 * @brief   Reads back a motor's current limit.
 * @param   index Zero based smart port index.
 * @return  The current limit in milliamps.
 * @see     vexDeviceMotorCurrentLimitGet
 */
int32_t vexMotorCurrentLimitGet(uint32_t index);

/**
 * @brief   Caps the voltage a motor's control loops may command.
 * @param   index Zero based smart port index.
 * @param   value Voltage limit in millivolts.
 * @see     vexDeviceMotorVoltageLimitSet
 */
void vexMotorVoltageLimitSet(uint32_t index, int32_t value);

/**
 * @brief   Reads back a motor's voltage limit.
 * @param   index Zero based smart port index.
 * @return  The voltage limit in millivolts.
 * @see     vexDeviceMotorVoltageLimitGet
 */
int32_t vexMotorVoltageLimitGet(uint32_t index);

/**
 * @brief   Replaces the gains in a motor's position control loop.
 * @param   index Zero based smart port index.
 * @param   pid   Gain set to install.
 * @see     vexDeviceMotorPositionPidSet
 */
void vexMotorPositionPidSet(uint32_t index, V5_DeviceMotorPid *pid);

/**
 * @brief   Replaces the gains in a motor's velocity control loop.
 * @param   index Zero based smart port index.
 * @param   pid   Gain set to install.
 * @see     vexDeviceMotorVelocityPidSet
 */
void vexMotorVelocityPidSet(uint32_t index, V5_DeviceMotorPid *pid);

/**
 * @brief   Reads a motor's current draw.
 * @param   index Zero based smart port index.
 * @return  The current in milliamps.
 * @see     vexDeviceMotorCurrentGet
 */
int32_t vexMotorCurrentGet(uint32_t index);

/**
 * @brief   Reads the voltage a motor is applying.
 * @param   index Zero based smart port index.
 * @return  The voltage in millivolts.
 * @see     vexDeviceMotorVoltageGet
 */
int32_t vexMotorVoltageGet(uint32_t index);

/**
 * @brief   Reads a motor's mechanical power output.
 * @param   index Zero based smart port index.
 * @return  The power in watts.
 * @see     vexDeviceMotorPowerGet
 */
double vexMotorPowerGet(uint32_t index);

/**
 * @brief   Reads a motor's output torque.
 * @param   index Zero based smart port index.
 * @return  The torque in newton meters.
 * @see     vexDeviceMotorTorqueGet
 */
double vexMotorTorqueGet(uint32_t index);

/**
 * @brief   Reads how efficiently a motor is running.
 * @param   index Zero based smart port index.
 * @return  The efficiency as a percentage.
 * @see     vexDeviceMotorEfficiencyGet
 */
double vexMotorEfficiencyGet(uint32_t index);

/**
 * @brief   Reads a motor's internal temperature.
 * @param   index Zero based smart port index.
 * @return  The temperature in degrees Celsius.
 * @see     vexDeviceMotorTemperatureGet
 */
double vexMotorTemperatureGet(uint32_t index);

/**
 * @brief   Reports whether a motor is too hot.
 * @param   index Zero based smart port index.
 * @return  True when the motor is limiting its output because of
 * temperature.
 * @see     vexDeviceMotorOverTempFlagGet
 */
bool vexMotorOverTempFlagGet(uint32_t index);

/**
 * @brief   Reports whether a motor is hitting its current limit.
 * @param   index Zero based smart port index.
 * @return  True when the motor is limiting its output because of current.
 * @see     vexDeviceMotorCurrentLimitFlagGet
 */
bool vexMotorCurrentLimitFlagGet(uint32_t index);

/**
 * @brief   Reads a motor's fault bits.
 * @param   index Zero based smart port index.
 * @return  A bit field of faults, or 0 if there are none.
 * @see     vexDeviceMotorFaultsGet
 */
uint32_t vexMotorFaultsGet(uint32_t index);

/**
 * @brief   Reports whether a motor is stopped.
 * @param   index Zero based smart port index.
 * @return  True when the motor's measured velocity is zero.
 * @see     vexDeviceMotorZeroVelocityFlagGet
 */
bool vexMotorZeroVelocityFlagGet(uint32_t index);

/**
 * @brief   Reports whether a motor is at its zero position.
 * @param   index Zero based smart port index.
 * @return  True when the motor's position reading is zero.
 * @see     vexDeviceMotorZeroPositionFlagGet
 */
bool vexMotorZeroPositionFlagGet(uint32_t index);

/**
 * @brief   Reads a motor's status bits.
 * @param   index Zero based smart port index.
 * @return  A bit field of status flags.
 * @see     vexDeviceMotorFlagsGet
 */
uint32_t vexMotorFlagsGet(uint32_t index);

/**
 * @brief   Reverses the sense of a motor.
 * @param   index Zero based smart port index.
 * @param   value True to reverse.
 * @see     vexDeviceMotorReverseFlagSet
 */
void vexMotorReverseFlagSet(uint32_t index, bool value);

/**
 * @brief   Reads back a motor's reverse flag.
 * @param   index Zero based smart port index.
 * @return  True if the motor is reversed.
 * @see     vexDeviceMotorReverseFlagGet
 */
bool vexMotorReverseFlagGet(uint32_t index);

/**
 * @brief   Chooses the units a motor reports position in.
 * @param   index Zero based smart port index.
 * @param   units Units to use.
 * @see     vexDeviceMotorEncoderUnitsSet
 */
void vexMotorEncoderUnitsSet(uint32_t index, V5MotorEncoderUnits units);

/**
 * @brief   Reads back a motor's position units.
 * @param   index Zero based smart port index.
 * @return  The units in use.
 * @see     vexDeviceMotorEncoderUnitsGet
 */
V5MotorEncoderUnits vexMotorEncoderUnitsGet(uint32_t index);

/**
 * @brief   Chooses what a motor does when it stops being commanded.
 * @param   index Zero based smart port index.
 * @param   mode  Coast, brake or hold.
 * @see     vexDeviceMotorBrakeModeSet
 */
void vexMotorBrakeModeSet(uint32_t index, V5MotorBrakeMode mode);

/**
 * @brief   Reads back a motor's brake mode.
 * @param   index Zero based smart port index.
 * @return  The brake mode in use.
 * @see     vexDeviceMotorBrakeModeGet
 */
V5MotorBrakeMode vexMotorBrakeModeGet(uint32_t index);

/**
 * @brief   Redefines a motor's current position.
 * @param   index    Zero based smart port index.
 * @param   position Value the current position should read as, in the
 *                   configured encoder units.
 * @see     vexDeviceMotorPositionSet
 */
void vexMotorPositionSet(uint32_t index, double position);

/**
 * @brief   Reads a motor's position.
 * @param   index Zero based smart port index.
 * @return  The position in the configured encoder units.
 * @see     vexDeviceMotorPositionGet
 */
double vexMotorPositionGet(uint32_t index);

/**
 * @brief   Reads a motor's raw encoder count along with its timestamp.
 * @param   index     Zero based smart port index.
 * @param   timestamp Filled in with the time the count was captured, in
 *                    milliseconds.
 * @return  The raw encoder count.
 * @see     vexDeviceMotorPositionRawGet
 */
int32_t vexMotorPositionRawGet(uint32_t index, uint32_t *timestamp);

/**
 * @brief   Zeroes a motor's position reading.
 * @param   index Zero based smart port index.
 * @see     vexDeviceMotorPositionReset
 */
void vexMotorPositionReset(uint32_t index);

/**
 * @brief   Reads the position a motor is currently driving towards.
 * @param   index Zero based smart port index.
 * @return  The target position in the configured encoder units.
 * @see     vexDeviceMotorTargetGet
 */
double vexMotorTargetGet(uint32_t index);

/**
 * @brief   Sends a motor to a position and holds it there.
 * @param   index    Zero based smart port index.
 * @param   position Target position in the configured encoder units.
 * @see     vexDeviceMotorServoTargetSet
 */
void vexMotorServoTargetSet(uint32_t index, double position);

/**
 * @brief   Starts a profiled move to an absolute position.
 * @param   index    Zero based smart port index.
 * @param   position Target position in the configured encoder units.
 * @param   velocity Velocity limit for the move, in RPM.
 * @see     vexDeviceMotorAbsoluteTargetSet
 */
void vexMotorAbsoluteTargetSet(uint32_t index, double position,
                               int32_t velocity);

/**
 * @brief   Starts a profiled move relative to the current position.
 * @param   index    Zero based smart port index.
 * @param   position Distance to travel in the configured encoder units.
 * @param   velocity Velocity limit for the move, in RPM.
 * @see     vexDeviceMotorRelativeTargetSet
 */
void vexMotorRelativeTargetSet(uint32_t index, double position,
                               int32_t velocity);

/**
 * @brief   Tells a motor which gear cartridge is fitted.
 * @param   index Zero based smart port index.
 * @param   value Cartridge ratio.
 * @see     vexDeviceMotorGearingSet
 */
void vexMotorGearingSet(uint32_t index, V5MotorGearset value);

/**
 * @brief   Reads back the cartridge a motor is configured for.
 * @param   index Zero based smart port index.
 * @return  The configured cartridge ratio.
 * @see     vexDeviceMotorGearingGet
 */
V5MotorGearset vexMotorGearingGet(uint32_t index);

/**
 * @brief   Reads which kind of smart motor is on a port.
 * @param   index Zero based smart port index.
 * @return  0 for the 11W V5 motor and 1 for the 5.5W EXP motor.
 * @see     vexDeviceMotorTypeGet
 */
int32_t vexMotorTypeGet(uint32_t index);

/**
 * @brief   Feeds a motor one point of an externally generated motion
 * profile.
 * @param   index    Zero based smart port index.
 * @param   position Position setpoint in the configured encoder units.
 * @param   velocity Velocity setpoint in RPM.
 * @see     vexDeviceMotorExternalProfileSet
 */
void vexMotorExternalProfileSet(uint32_t index, double position,
                                int32_t velocity);

/**
 * @brief   Chooses what the Vision Sensor looks for.
 * @param   index Zero based smart port index.
 * @param   mode  Detection mode.
 * @see     vexDeviceVisionModeSet
 */
void vexVisionModeSet(uint32_t index, V5VisionMode mode);

/**
 * @brief   Reads back the Vision Sensor's detection mode.
 * @param   index Zero based smart port index.
 * @return  The current mode.
 * @see     vexDeviceVisionModeGet
 */
V5VisionMode vexVisionModeGet(uint32_t index);

/**
 * @brief   Counts the objects the Vision Sensor is currently reporting.
 * @param   index Zero based smart port index.
 * @return  The number of objects available.
 * @see     vexDeviceVisionObjectCountGet
 */
int32_t vexVisionObjectCountGet(uint32_t index);

/**
 * @brief   Reads one detected object.
 * @param   index    Zero based smart port index.
 * @param   indexObj Zero based object index.
 * @param   pObject  Structure filled in with the object's data.
 * @return  The number of objects copied.
 * @see     vexDeviceVisionObjectGet
 */
int32_t vexVisionObjectGet(uint32_t index, uint32_t indexObj,
                           V5_DeviceVisionObject *pObject);

/**
 * @brief   Writes a color signature to the Vision Sensor.
 * @param   index      Zero based smart port index.
 * @param   pSignature Signature to store, with its id set to the target
 * slot.
 * @see     vexDeviceVisionSignatureSet
 */
void vexVisionSignatureSet(uint32_t index,
                           V5_DeviceVisionSignature *pSignature);

/**
 * @brief   Reads a color signature back from the Vision Sensor.
 * @param   index      Zero based smart port index.
 * @param   id         Signature slot, 1 to 7.
 * @param   pSignature Structure filled in with the stored signature.
 * @return  True if the signature was read.
 * @see     vexDeviceVisionSignatureGet
 */
bool vexVisionSignatureGet(uint32_t index, uint32_t id,
                           V5_DeviceVisionSignature *pSignature);

/**
 * @brief   Sets the Vision Sensor's camera brightness.
 * @param   index   Zero based smart port index.
 * @param   percent Brightness, 0 to 100.
 * @see     vexDeviceVisionBrightnessSet
 */
void vexVisionBrightnessSet(uint32_t index, uint8_t percent);

/**
 * @brief   Reads back the Vision Sensor's camera brightness.
 * @param   index Zero based smart port index.
 * @return  The brightness, 0 to 100.
 * @see     vexDeviceVisionBrightnessGet
 */
uint8_t vexVisionBrightnessGet(uint32_t index);

/**
 * @brief   Chooses how the Vision Sensor white balances.
 * @param   index Zero based smart port index.
 * @param   mode  White balance mode.
 * @see     vexDeviceVisionWhiteBalanceModeSet
 */
void vexVisionWhiteBalanceModeSet(uint32_t index, V5VisionWBMode mode);

/**
 * @brief   Reads back the Vision Sensor's white balance mode.
 * @param   index Zero based smart port index.
 * @return  The current mode.
 * @see     vexDeviceVisionWhiteBalanceModeGet
 */
V5VisionWBMode vexVisionWhiteBalanceModeGet(uint32_t index);

/**
 * @brief   Sets the manual white balance reference color.
 * @param   index Zero based smart port index.
 * @param   color Color that should be treated as white.
 * @see     vexDeviceVisionWhiteBalanceSet
 */
void vexVisionWhiteBalanceSet(uint32_t index, V5_DeviceVisionRgb color);

/**
 * @brief   Reads back the manual white balance reference color.
 * @param   index Zero based smart port index.
 * @return  The reference color.
 * @see     vexDeviceVisionWhiteBalanceGet
 */
V5_DeviceVisionRgb vexVisionWhiteBalanceGet(uint32_t index);

/**
 * @brief   Chooses who drives the Vision Sensor's LED.
 * @param   index Zero based smart port index.
 * @param   mode  Automatic or manual.
 * @see     vexDeviceVisionLedModeSet
 */
void vexVisionLedModeSet(uint32_t index, V5VisionLedMode mode);

/**
 * @brief   Reads back who drives the Vision Sensor's LED.
 * @param   index Zero based smart port index.
 * @return  The current mode.
 * @see     vexDeviceVisionLedModeGet
 */
V5VisionLedMode vexVisionLedModeGet(uint32_t index);

/**
 * @brief   Sets the Vision Sensor's LED brightness.
 * @param   index   Zero based smart port index.
 * @param   percent Brightness, 0 to 100.
 * @note    The function name contains a typo that is kept for source
 *          compatibility.
 * @see     vexDeviceVisionLedBrigntnessSet
 */
void vexVisionLedBrigntnessSet(uint32_t index, uint8_t percent);

/**
 * @brief   Reads back the Vision Sensor's LED brightness.
 * @param   index Zero based smart port index.
 * @return  The brightness, 0 to 100.
 * @note    The function name contains a typo that is kept for source
 *          compatibility.
 * @see     vexDeviceVisionLedBrigntnessGet
 */
uint8_t vexVisionLedBrigntnessGet(uint32_t index);

/**
 * @brief   Sets the Vision Sensor's LED color.
 * @param   index Zero based smart port index.
 * @param   color Color to show.
 * @see     vexDeviceVisionLedColorSet
 */
void vexVisionLedColorSet(uint32_t index, V5_DeviceVisionRgb color);

/**
 * @brief   Reads back the Vision Sensor's LED color.
 * @param   index Zero based smart port index.
 * @return  The LED color.
 * @see     vexDeviceVisionLedColorGet
 */
V5_DeviceVisionRgb vexVisionLedColorGet(uint32_t index);

/**
 * @brief   Turns the Vision Sensor's onboard access point on or off.
 * @param   index Zero based smart port index.
 * @param   mode  On or off.
 * @see     vexDeviceVisionWifiModeSet
 */
void vexVisionWifiModeSet(uint32_t index, V5VisionWifiMode mode);

/**
 * @brief   Reads back the Vision Sensor's access point state.
 * @param   index Zero based smart port index.
 * @return  The current mode.
 * @see     vexDeviceVisionWifiModeGet
 */
V5VisionWifiMode vexVisionWifiModeGet(uint32_t index);

/**
 * @brief   Returns the AI Vision Sensor to its power on state.
 * @param   index Zero based smart port index.
 * @see     vexDeviceAiVisionReset
 */
void vexAiVisionReset(uint32_t index);

/**
 * @brief   Turns individual kinds of detection on or off.
 * @param   index   Zero based smart port index.
 * @param   bits    New state for the kinds selected by bitmask.
 * @param   bitmask Which kinds of detection the call applies to.
 * @see     vexDeviceAiVisionEnableSet
 */
void vexAiVisionEnableSet(uint32_t index, uint8_t bits, uint8_t bitmask);

/**
 * @brief   Reads back which kinds of detection are enabled.
 * @param   index Zero based smart port index.
 * @return  A bitfield of the enabled kinds.
 * @see     vexDeviceAiVisionEnableGet
 */
uint8_t vexAiVisionEnableGet(uint32_t index);

/**
 * @brief   Sets the AI Vision Sensor's mode word.
 * @param   index Zero based smart port index.
 * @param   mode  Packed mode word.
 * @see     vexDeviceAiVisionModeSet
 */
void vexAiVisionModeSet(uint32_t index, uint32_t mode);

/**
 * @brief   Reads the AI Vision Sensor's status word.
 * @param   index Zero based smart port index.
 * @return  The packed status word.
 * @see     vexDeviceAiVisionStatusGet
 */
uint32_t vexAiVisionStatusGet(uint32_t index);

/**
 * @brief   Counts the objects the AI Vision Sensor is currently reporting.
 * @param   index Zero based smart port index.
 * @return  The number of objects available.
 * @see     vexDeviceAiVisionObjectCountGet
 */
int32_t vexAiVisionObjectCountGet(uint32_t index);

/**
 * @brief   Reads one detected object.
 * @param   index    Zero based smart port index.
 * @param   indexObj Zero based object index.
 * @param   pObject  Structure filled in with the object's data.
 * @return  The number of objects copied.
 * @see     vexDeviceAiVisionObjectGet
 */
int32_t vexAiVisionObjectGet(uint32_t index, uint32_t indexObj,
                             V5_DeviceAiVisionObject *pObject);

/**
 * @brief   Writes a color descriptor to the AI Vision Sensor.
 * @param   index  Zero based smart port index.
 * @param   pColor Descriptor to store, with its id set to the target slot.
 * @see     vexDeviceAiVisionColorSet
 */
void vexAiVisionColorSet(uint32_t index, V5_DeviceAiVisionColor *pColor);

/**
 * @brief   Reads a color descriptor back from the AI Vision Sensor.
 * @param   index  Zero based smart port index.
 * @param   id     Descriptor slot, 1 to 7.
 * @param   pColor Structure filled in with the stored descriptor.
 * @return  True if the descriptor was read.
 * @see     vexDeviceAiVisionColorGet
 */
bool vexAiVisionColorGet(uint32_t index, uint32_t id,
                         V5_DeviceAiVisionColor *pColor);

/**
 * @brief   Writes a code descriptor to the AI Vision Sensor.
 * @param   index Zero based smart port index.
 * @param   pCode Descriptor to store, with its id set to the target slot.
 * @see     vexDeviceAiVisionCodeSet
 */
void vexAiVisionCodeSet(uint32_t index, V5_DeviceAiVisionCode *pCode);

/**
 * @brief   Reads a code descriptor back from the AI Vision Sensor.
 * @param   index Zero based smart port index.
 * @param   id    Descriptor slot, 1 to 5.
 * @param   pCode Structure filled in with the stored descriptor.
 * @return  True if the descriptor was read.
 * @see     vexDeviceAiVisionCodeGet
 */
bool vexAiVisionCodeGet(uint32_t index, uint32_t id,
                        V5_DeviceAiVisionCode *pCode);

/**
 * @brief   Reads the name of one of the AI model's classes.
 * @param   index Zero based smart port index.
 * @param   id    Class index, or -1 to ask how many classes there are.
 * @param   pName Buffer of at least 20 bytes for the name.
 * @return  The number of classes when id is -1, otherwise unspecified.
 * @see     vexDeviceAiVisionClassNameGet
 */
int32_t vexAiVisionClassNameGet(uint32_t index, int32_t id, uint8_t *pName);

/**
 * @brief   Reads the AI Vision Sensor's internal temperature.
 * @param   index Zero based smart port index.
 * @return  The temperature in degrees Celsius.
 * @see     vexDeviceAiVisionTemperatureGet
 */
double vexAiVisionTemperatureGet(uint32_t index);

/**
 * @brief   Starts calibrating the Inertial Sensor.
 * @param   index Zero based smart port index.
 * @see     vexDeviceImuReset
 */
void vexImuReset(uint32_t index);

/**
 * @brief   Reads the Inertial Sensor's accumulated rotation about the z
 * axis.
 * @param   index Zero based smart port index.
 * @return  The rotation in degrees, unbounded, clockwise positive.
 * @see     vexDeviceImuHeadingGet
 */
double vexImuHeadingGet(uint32_t index);

/**
 * @brief   Reads the Inertial Sensor's heading.
 * @param   index Zero based smart port index.
 * @return  The heading in degrees, 0 through 360, clockwise positive.
 * @see     vexDeviceImuDegreesGet
 */
double vexImuDegreesGet(uint32_t index);

/**
 * @brief   Reads the Inertial Sensor's orientation as a quaternion.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with the quaternion.
 * @see     vexDeviceImuQuaternionGet
 */
void vexImuQuaternionGet(uint32_t index, V5_DeviceImuQuaternion *data);

/**
 * @brief   Reads the Inertial Sensor's orientation as Euler angles.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with pitch, roll and yaw in degrees.
 * @see     vexDeviceImuAttitudeGet
 */
void vexImuAttitudeGet(uint32_t index, V5_DeviceImuAttitude *data);

/**
 * @brief   Reads the Inertial Sensor's raw gyroscope rates.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with x, y and z in degrees per second.
 * @see     vexDeviceImuRawGyroGet
 */
void vexImuRawGyroGet(uint32_t index, V5_DeviceImuRaw *data);

/**
 * @brief   Reads the Inertial Sensor's raw accelerations.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with x, y and z in g.
 * @see     vexDeviceImuRawAccelGet
 */
void vexImuRawAccelGet(uint32_t index, V5_DeviceImuRaw *data);

/**
 * @brief   Reads the Inertial Sensor's status bits.
 * @param   index Zero based smart port index.
 * @return  The status word.
 * @see     vexDeviceImuStatusGet
 */
uint32_t vexImuStatusGet(uint32_t index);

/**
 * @brief   Sets the Inertial Sensor's mode word.
 * @param   index Zero based smart port index.
 * @param   mode  Bitwise OR of the IMU mode values.
 * @see     vexDeviceImuModeSet
 */
void vexImuModeSet(uint32_t index, uint32_t mode);

/**
 * @brief   Reads back the Inertial Sensor's mode word.
 * @param   index Zero based smart port index.
 * @return  The mode word.
 * @see     vexDeviceImuModeGet
 */
uint32_t vexImuModeGet(uint32_t index);

/**
 * @brief   Sets how often the Inertial Sensor produces new readings.
 * @param   index Zero based smart port index.
 * @param   rate  Interval in milliseconds, a multiple of 5 and at least 5.
 * @see     vexDeviceImuDataRateSet
 */
void vexImuDataRateSet(uint32_t index, uint32_t rate);

/**
 * @brief   Reads a rangefinder or lidar device.
 * @param   index Zero based smart port index.
 * @return  The measured distance.
 * @see     vexDeviceRangeValueGet
 */
int32_t vexRangeValueGet(uint32_t index);

/**
 * @brief   Resets the Rotation Sensor's position to its absolute angle.
 * @param   index Zero based smart port index.
 * @see     vexDeviceAbsEncReset
 */
void vexAbsEncReset(uint32_t index);

/**
 * @brief   Redefines the Rotation Sensor's position.
 * @param   index    Zero based smart port index.
 * @param   position Value the current position should read as, in
 *                   centidegrees.
 * @see     vexDeviceAbsEncPositionSet
 */
void vexAbsEncPositionSet(uint32_t index, int32_t position);

/**
 * @brief   Reads the Rotation Sensor's accumulated position.
 * @param   index Zero based smart port index.
 * @return  The position in centidegrees, unbounded.
 * @see     vexDeviceAbsEncPositionGet
 */
int32_t vexAbsEncPositionGet(uint32_t index);

/**
 * @brief   Reads the Rotation Sensor's speed.
 * @param   index Zero based smart port index.
 * @return  The speed in centidegrees per second.
 * @see     vexDeviceAbsEncVelocityGet
 */
int32_t vexAbsEncVelocityGet(uint32_t index);

/**
 * @brief   Reads the Rotation Sensor's absolute angle.
 * @param   index Zero based smart port index.
 * @return  The angle in centidegrees, 0 through 35999.
 * @see     vexDeviceAbsEncAngleGet
 */
int32_t vexAbsEncAngleGet(uint32_t index);

/**
 * @brief   Reverses the Rotation Sensor's direction.
 * @param   index Zero based smart port index.
 * @param   value True to reverse.
 * @see     vexDeviceAbsEncReverseFlagSet
 */
void vexAbsEncReverseFlagSet(uint32_t index, bool value);

/**
 * @brief   Reads back the Rotation Sensor's reverse flag.
 * @param   index Zero based smart port index.
 * @return  True if the sensor is reversed.
 * @see     vexDeviceAbsEncReverseFlagGet
 */
bool vexAbsEncReverseFlagGet(uint32_t index);

/**
 * @brief   Reads the Rotation Sensor's status bits.
 * @param   index Zero based smart port index.
 * @return  The status word, 0 until the sensor has finished starting up.
 * @see     vexDeviceAbsEncStatusGet
 */
uint32_t vexAbsEncStatusGet(uint32_t index);

/**
 * @brief   Sets how often the Rotation Sensor produces new readings.
 * @param   index Zero based smart port index.
 * @param   rate  Interval in milliseconds, a multiple of 5 and at least 5.
 * @see     vexDeviceAbsEncDataRateSet
 */
void vexAbsEncDataRateSet(uint32_t index, uint32_t rate);

/**
 * @brief   Reads the hue of the color in front of the Optical Sensor.
 * @param   index Zero based smart port index.
 * @return  The hue in degrees, 0 through 360.
 * @see     vexDeviceOpticalHueGet
 */
double vexOpticalHueGet(uint32_t index);

/**
 * @brief   Reads the saturation of the color in front of the Optical
 * Sensor.
 * @param   index Zero based smart port index.
 * @return  The saturation, 0 through 1.
 * @see     vexDeviceOpticalSatGet
 */
double vexOpticalSatGet(uint32_t index);

/**
 * @brief   Reads how much light reaches the Optical Sensor.
 * @param   index Zero based smart port index.
 * @return  The brightness, 0 through 1.
 * @see     vexDeviceOpticalBrightnessGet
 */
double vexOpticalBrightnessGet(uint32_t index);

/**
 * @brief   Reads the Optical Sensor's proximity reading.
 * @param   index Zero based smart port index.
 * @return  The proximity, 0 through 255, larger meaning closer.
 * @see     vexDeviceOpticalProximityGet
 */
int32_t vexOpticalProximityGet(uint32_t index);

/**
 * @brief   Reads the Optical Sensor's processed color.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with the color and brightness.
 * @see     vexDeviceOpticalRgbGet
 */
void vexOpticalRgbGet(uint32_t index, V5_DeviceOpticalRgb *data);

/**
 * @brief   Sets the brightness of the Optical Sensor's white LED.
 * @param   index Zero based smart port index.
 * @param   value Brightness, 0 to 100.
 * @see     vexDeviceOpticalLedPwmSet
 */
void vexOpticalLedPwmSet(uint32_t index, int32_t value);

/**
 * @brief   Reads back the Optical Sensor's LED brightness.
 * @param   index Zero based smart port index.
 * @return  The brightness, 0 to 100.
 * @see     vexDeviceOpticalLedPwmGet
 */
int32_t vexOpticalLedPwmGet(uint32_t index);

/**
 * @brief   Reads the Optical Sensor's status bits.
 * @param   index Zero based smart port index.
 * @return  The status word.
 * @see     vexDeviceOpticalStatusGet
 */
uint32_t vexOpticalStatusGet(uint32_t index);

/**
 * @brief   Reads the Optical Sensor's unprocessed channel counts.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with the clear, red, green and blue
 *                counts.
 * @see     vexDeviceOpticalRawGet
 */
void vexOpticalRawGet(uint32_t index, V5_DeviceOpticalRaw *data);

/**
 * @brief   Sets the Optical Sensor's operating mode.
 * @param   index Zero based smart port index.
 * @param   mode  Mode value.
 * @see     vexDeviceOpticalModeSet
 */
void vexOpticalModeSet(uint32_t index, uint32_t mode);

/**
 * @brief   Reads back the Optical Sensor's operating mode.
 * @param   index Zero based smart port index.
 * @return  The mode value.
 * @see     vexDeviceOpticalModeGet
 */
uint32_t vexOpticalModeGet(uint32_t index);

/**
 * @brief   Reads the most recent gesture from the Optical Sensor.
 * @param   index Zero based smart port index.
 * @param   pData Structure filled in with the raw gesture data, or null if
 * only the direction is wanted.
 * @return  The recognised direction: 0 none, 1 up, 2 down, 3 right, 4 left.
 * @see     vexDeviceOpticalGestureGet
 */
uint32_t vexOpticalGestureGet(uint32_t index, V5_DeviceOpticalGesture *pData);

/**
 * @brief   Enables gesture detection on the Optical Sensor.
 * @param   index Zero based smart port index.
 * @see     vexDeviceOpticalGestureEnable
 */
void vexOpticalGestureEnable(uint32_t index);

/**
 * @brief   Disables gesture detection on the Optical Sensor.
 * @param   index Zero based smart port index.
 * @see     vexDeviceOpticalGestureDisable
 */
void vexOpticalGestureDisable(uint32_t index);

/**
 * @brief   Sets the proximity threshold gesture detection triggers at.
 * @param   index Zero based smart port index.
 * @param   value Threshold, on the same 0 to 255 scale as the proximity
 *                reading.
 * @return  VEX does not document the return value.
 * @see     vexDeviceOpticalProximityThreshold
 */
int32_t vexOpticalProximityThreshold(uint32_t index, int32_t value);

/**
 * @brief   Sets how long the Optical Sensor integrates light for.
 * @param   index  Zero based smart port index.
 * @param   timems Integration time in milliseconds, 3 through 712.
 * @see     vexDeviceOpticalIntegrationTimeSet
 */
void vexOpticalIntegrationTimeSet(uint32_t index, double timems);

/**
 * @brief   Reads back the Optical Sensor's integration time.
 * @param   index Zero based smart port index.
 * @return  The integration time in milliseconds.
 * @see     vexDeviceOpticalIntegrationTimeGet
 */
double vexOpticalIntegrationTimeGet(uint32_t index);

/**
 * @brief   Energises the Workcell electromagnet.
 * @param   index Zero based smart port index.
 * @param   value Power level. Positive attracts, negative repels.
 * @param   time  How long to stay energised, in milliseconds.
 * @see     vexDeviceMagnetPowerSet
 */
void vexMagnetPowerSet(uint32_t index, int32_t value, int32_t time);

/**
 * @brief   Reads back the electromagnet's power level.
 * @param   index Zero based smart port index.
 * @return  The power level.
 * @see     vexDeviceMagnetPowerGet
 */
int32_t vexMagnetPowerGet(uint32_t index);

/**
 * @brief   Runs the electromagnet's pickup sequence.
 * @param   index    Zero based smart port index.
 * @param   duration How long to energise for.
 * @see     vexDeviceMagnetPickup
 */
void vexMagnetPickup(uint32_t index, V5_DeviceMagnetDuration duration);

/**
 * @brief   Runs the electromagnet's drop sequence.
 * @param   index    Zero based smart port index.
 * @param   duration How long to energise for.
 * @see     vexDeviceMagnetDrop
 */
void vexMagnetDrop(uint32_t index, V5_DeviceMagnetDuration duration);

/**
 * @brief   Reads the electromagnet's temperature.
 * @param   index Zero based smart port index.
 * @return  The temperature in degrees Celsius.
 * @see     vexDeviceMagnetTemperatureGet
 */
double vexMagnetTemperatureGet(uint32_t index);

/**
 * @brief   Reads the electromagnet's current draw.
 * @param   index Zero based smart port index.
 * @return  The current in amps.
 * @see     vexDeviceMagnetCurrentGet
 */
double vexMagnetCurrentGet(uint32_t index);

/**
 * @brief   Reads the electromagnet's status bits.
 * @param   index Zero based smart port index.
 * @return  The status word.
 * @see     vexDeviceMagnetStatusGet
 */
uint32_t vexMagnetStatusGet(uint32_t index);

/**
 * @brief   Reads the distance to the nearest object.
 * @param   index Zero based smart port index.
 * @return  The distance in millimetres.
 * @see     vexDeviceDistanceDistanceGet
 */
uint32_t vexDistanceDistanceGet(uint32_t index);

/**
 * @brief   Reads how much the sensor trusts its distance reading.
 * @param   index Zero based smart port index.
 * @return  The confidence, 0 through 63.
 * @see     vexDeviceDistanceConfidenceGet
 */
uint32_t vexDistanceConfidenceGet(uint32_t index);

/**
 * @brief   Reads the apparent size of the detected object.
 * @param   index Zero based smart port index.
 * @return  A unitless size, roughly 0 through 400.
 * @see     vexDeviceDistanceObjectSizeGet
 */
int32_t vexDistanceObjectSizeGet(uint32_t index);

/**
 * @brief   Reads how fast the detected object is approaching or receding.
 * @param   index Zero based smart port index.
 * @return  The velocity in meters per second.
 * @see     vexDeviceDistanceObjectVelocityGet
 */
double vexDistanceObjectVelocityGet(uint32_t index);

/**
 * @brief   Reads the Distance Sensor's status bits.
 * @param   index Zero based smart port index.
 * @return  The status word, 0 while the sensor is still initialising.
 * @see     vexDeviceDistanceStatusGet
 */
uint32_t vexDistanceStatusGet(uint32_t index);

/**
 * @brief   Starts calibrating the GPS Sensor's onboard inertial sensor.
 * @param   index Zero based smart port index.
 * @see     vexDeviceGpsReset
 */
void vexGpsReset(uint32_t index);

/**
 * @brief   Reads the GPS Sensor's accumulated rotation about the z axis.
 * @param   index Zero based smart port index.
 * @return  The rotation in degrees, unbounded.
 * @see     vexDeviceGpsHeadingGet
 */
double vexGpsHeadingGet(uint32_t index);

/**
 * @brief   Reads the GPS Sensor's heading.
 * @param   index Zero based smart port index.
 * @return  The heading in degrees, 0 through 360.
 * @see     vexDeviceGpsDegreesGet
 */
double vexGpsDegreesGet(uint32_t index);

/**
 * @brief   Reads the GPS Sensor's orientation as a quaternion.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with the quaternion.
 * @see     vexDeviceGpsQuaternionGet
 */
void vexGpsQuaternionGet(uint32_t index, V5_DeviceGpsQuaternion *data);

/**
 * @brief   Reads the GPS Sensor's position and orientation together.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with position in meters and angles in
 *                degrees.
 * @param   bRaw  True to skip the corrections VEXos normally applies.
 * @see     vexDeviceGpsAttitudeGet
 */
void vexGpsAttitudeGet(uint32_t index, V5_DeviceGpsAttitude *data, bool bRaw);

/**
 * @brief   Reads the GPS Sensor's raw gyroscope rates.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with x, y and z in degrees per second.
 * @see     vexDeviceGpsRawGyroGet
 */
void vexGpsRawGyroGet(uint32_t index, V5_DeviceGpsRaw *data);

/**
 * @brief   Reads the GPS Sensor's raw accelerations.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with x, y and z in g.
 * @see     vexDeviceGpsRawAccelGet
 */
void vexGpsRawAccelGet(uint32_t index, V5_DeviceGpsRaw *data);

/**
 * @brief   Reads the GPS Sensor's status bits.
 * @param   index Zero based smart port index.
 * @return  The status word.
 * @see     vexDeviceGpsStatusGet
 */
uint32_t vexGpsStatusGet(uint32_t index);

/**
 * @brief   Sets the GPS Sensor's mode word.
 * @param   index Zero based smart port index.
 * @param   mode  Mode value.
 * @see     vexDeviceGpsModeSet
 */
void vexGpsModeSet(uint32_t index, uint32_t mode);

/**
 * @brief   Reads back the GPS Sensor's mode word.
 * @param   index Zero based smart port index.
 * @return  The mode word.
 * @see     vexDeviceGpsModeGet
 */
uint32_t vexGpsModeGet(uint32_t index);

/**
 * @brief   Sets how often the GPS Sensor's inertial sensor updates.
 * @param   index Zero based smart port index.
 * @param   rate  Interval in milliseconds, a multiple of 5 and at least 5.
 * @see     vexDeviceGpsDataRateSet
 */
void vexGpsDataRateSet(uint32_t index, uint32_t rate);

/**
 * @brief   Tells the GPS Sensor where it sits on the robot.
 * @param   index Zero based smart port index.
 * @param   ox    X offset from the robot's centre of rotation, in meters.
 * @param   oy    Y offset from the robot's centre of rotation, in meters.
 * @see     vexDeviceGpsOriginSet
 */
void vexGpsOriginSet(uint32_t index, double ox, double oy);

/**
 * @brief   Reads back the GPS Sensor's mounting offset.
 * @param   index Zero based smart port index.
 * @param   ox    Filled in with the X offset in meters.
 * @param   oy    Filled in with the Y offset in meters.
 * @see     vexDeviceGpsOriginGet
 */
void vexGpsOriginGet(uint32_t index, double *ox, double *oy);

/**
 * @brief   Sets the GPS Sensor's rotation value.
 * @param   index Zero based smart port index.
 * @param   value Rotation in degrees.
 * @see     vexDeviceGpsRotationSet
 */
void vexGpsRotationSet(uint32_t index, double value);

/**
 * @brief   Reads back the GPS Sensor's rotation value.
 * @param   index Zero based smart port index.
 * @return  The rotation in degrees.
 * @see     vexDeviceGpsRotationGet
 */
double vexGpsRotationGet(uint32_t index);

/**
 * @brief   Tells the GPS Sensor where the robot starts on the field.
 * @param   index            Zero based smart port index.
 * @param   initial_x        Starting X position in meters.
 * @param   initial_y        Starting Y position in meters.
 * @param   initial_rotation Starting heading in degrees.
 * @see     vexDeviceGpsInitialPositionSet
 */
void vexGpsInitialPositionSet(uint32_t index, double initial_x,
                              double initial_y, double initial_rotation);

/**
 * @brief   Reads how uncertain the GPS Sensor's position is.
 * @param   index Zero based smart port index.
 * @return  The root mean squared position error in meters.
 * @see     vexDeviceGpsErrorGet
 */
double vexGpsErrorGet(uint32_t index);

// Generic serial port comms to any device

/**
 * @brief   Switches a smart port into generic serial mode.
 * @param   index   Zero based smart port index.
 * @param   options Option flags. VEX does not publish their meaning.
 * @see     vexDeviceGenericSerialEnable
 */
void vexGenericSerialEnable(uint32_t index, int32_t options);

/**
 * @brief   Sets the baud rate of a generic serial port.
 * @param   index    Zero based smart port index.
 * @param   baudrate Baud rate in bits per second.
 * @see     vexDeviceGenericSerialBaudrate
 */
void vexGenericSerialBaudrate(uint32_t index, int32_t baudrate);

/**
 * @brief   Queues one byte for transmission.
 * @param   index Zero based smart port index.
 * @param   c     Byte to send.
 * @return  The number of bytes queued, or -1 on error.
 * @see     vexDeviceGenericSerialWriteChar
 */
int32_t vexGenericSerialWriteChar(uint32_t index, uint8_t c);

/**
 * @brief   Reports how much room is left in the transmit buffer.
 * @param   index Zero based smart port index.
 * @return  The number of free bytes.
 * @see     vexDeviceGenericSerialWriteFree
 */
int32_t vexGenericSerialWriteFree(uint32_t index);

/**
 * @brief   Queues a buffer for transmission.
 * @param   index  Zero based smart port index.
 * @param   buffer Bytes to send.
 * @param   length Number of bytes to send.
 * @return  The number of bytes queued, or -1 on error.
 * @see     vexDeviceGenericSerialTransmit
 */
int32_t vexGenericSerialTransmit(uint32_t index, uint8_t *buffer,
                                 int32_t length);

/**
 * @brief   Removes one byte from the receive buffer.
 * @param   index Zero based smart port index.
 * @return  The byte, or -1 if the buffer is empty.
 * @see     vexDeviceGenericSerialReadChar
 */
int32_t vexGenericSerialReadChar(uint32_t index);

/**
 * @brief   Looks at the next received byte without removing it.
 * @param   index Zero based smart port index.
 * @return  The byte, or -1 if the buffer is empty.
 * @see     vexDeviceGenericSerialPeekChar
 */
int32_t vexGenericSerialPeekChar(uint32_t index);

/**
 * @brief   Reports how many bytes are waiting to be read.
 * @param   index Zero based smart port index.
 * @return  The number of buffered bytes.
 * @see     vexDeviceGenericSerialReceiveAvail
 */
int32_t vexGenericSerialReceiveAvail(uint32_t index);

/**
 * @brief   Reads buffered bytes into a caller supplied buffer.
 * @param   index  Zero based smart port index.
 * @param   buffer Destination buffer.
 * @param   length Maximum number of bytes to read.
 * @return  The number of bytes actually read.
 * @see     vexDeviceGenericSerialReceive
 */
int32_t vexGenericSerialReceive(uint32_t index, uint8_t *buffer,
                                int32_t length);

/**
 * @brief   Discards the contents of a generic serial port's buffers.
 * @param   index Zero based smart port index.
 * @see     vexDeviceGenericSerialFlush
 */
void vexGenericSerialFlush(uint32_t index);

// Radio link between two brains

/**
 * @brief   Joins a VEXnet radio to a named link.
 * @param   index   Zero based smart port index.
 * @param   link_id Name shared by the two brains that are to be paired.
 * @param   type    0 for the receiving end and 1 for the transmitting end.
 * @param   ov      Whether to override an existing link on the radio.
 * @see     vexDeviceGenericRadioConnection
 */
void vexGenericRadioConnection(uint32_t index, char *link_id, int32_t type,
                               bool ov);

/**
 * @brief   Reports whether a radio link is up.
 * @param   index Zero based smart port index.
 * @return  True once the two radios have paired.
 * @see     vexDeviceGenericRadioLinkStatus
 */
bool vexGenericRadioLinkStatus(uint32_t index);

/**
 * @brief   Reports how much room is left in the link's transmit buffer.
 * @param   index Zero based smart port index.
 * @return  The number of free bytes.
 * @see     vexDeviceGenericRadioWriteFree
 */
uint32_t vexGenericRadioWriteFree(uint32_t index);

/**
 * @brief   Queues a buffer for transmission over the link.
 * @param   index  Zero based smart port index.
 * @param   buffer Bytes to send.
 * @param   length Number of bytes to send.
 * @return  The number of bytes queued.
 * @see     vexDeviceGenericRadioTransmit
 */
uint32_t vexGenericRadioTransmit(uint32_t index, uint8_t *buffer,
                                 uint32_t length);

/**
 * @brief   Reports how many bytes the link has waiting to be read.
 * @param   index Zero based smart port index.
 * @return  The number of buffered bytes.
 * @see     vexDeviceGenericRadioReceiveAvail
 */
uint32_t vexGenericRadioReceiveAvail(uint32_t index);

/**
 * @brief   Reads buffered bytes from the link into a caller supplied buffer.
 * @param   index  Zero based smart port index.
 * @param   buffer Destination buffer.
 * @param   length Maximum number of bytes to read.
 * @return  The number of bytes actually read.
 * @see     vexDeviceGenericRadioReceive
 */
uint32_t vexGenericRadioReceive(uint32_t index, uint8_t *buffer,
                                uint32_t length);

#ifdef __cplusplus
}
#endif
#endif /* V5_APIUSER_H_ */
