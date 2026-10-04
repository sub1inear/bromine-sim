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
 *  Apart from vexDelay(), every function here is a thin wrapper around the
 *  matching vexDevice call in v5_api.h. The only difference is the first
 *  argument: these take a port index instead of a #V5_DeviceT handle. In
 *  libv5rt each one calls vexDeviceGetByIndex() on every call and passes the
 *  result on to its counterpart.
 *
 *  The index is zero based, so smart port 1 is index 0. PROS passes
 *  port - 1 when it calls these.
 *
 *  Some wrappers check the handle for null and return early, and some pass it
 *  straight on; each wrapper's details say which. None of them checks that
 *  the port holds the kind of device being asked about, and VEX does not
 *  document what VEXos returns for a mismatched device. PROS and vexide check
 *  the port's type themselves before calling into VEXos.
 *
 *  Behaviour, units and value ranges are documented on the vexDevice
 *  functions in v5_api.h, and the text here is copied from there. See
 *  docs/dev/sdk-doc-evidence.md for the sources.
 *//*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief   Blocks the calling task for a period of time.
 * @param   timems How long to wait, in milliseconds.
 * @details libv5rt implements this as a tail call to vexTaskSleep(), which
 *          these headers do not declare. Neither PROS nor vexide calls it.
 */
void vexDelay(uint32_t timems);

/**
 * @brief   Sets an LED module to one of the preset colors.
 * @param   index Zero based smart port index.
 * @param   value Color to show.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceLedSet
 */
void vexLedSet(uint32_t index, V5_DeviceLedColor value);

/**
 * @brief   Sets an LED module to an arbitrary color.
 * @param   index Zero based smart port index.
 * @param   color Color. The encoding is not documented; the #V5_DeviceLedColor
 *                presets are 0xRRGGBB values.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceLedRgbSet
 */
void vexLedRgbSet(uint32_t index, uint32_t color);

/**
 * @brief   Reads back an LED module's color as a preset.
 * @param   index Zero based smart port index.
 * @return  The current color.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceLedGet
 */
V5_DeviceLedColor vexLedGet(uint32_t index);

/**
 * @brief   Reads back an LED module's color.
 * @param   index Zero based smart port index.
 * @return  The current color. The encoding is not documented.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceLedRgbGet
 */
uint32_t vexLedRgbGet(uint32_t index);

/**
 * @brief   Configures one 3-Wire channel.
 * @param   index Zero based smart port index.
 * @param   port  Channel number, 0 to 7, for the ports labelled A to H.
 * @param   type  What the channel is wired to.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceAdiPortConfigSet
 */
void vexAdiPortConfigSet(uint32_t index, uint32_t port,
                         V5_AdiPortConfiguration type);

/**
 * @brief   Reads back how a 3-Wire channel is configured.
 * @param   index Zero based smart port index.
 * @param   port  Channel number, 0 to 7.
 * @return  The channel's configuration.
 * @details Returns 255 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceAdiPortConfigGet
 */
V5_AdiPortConfiguration vexAdiPortConfigGet(uint32_t index, uint32_t port);

/**
 * @brief   Writes a value to a 3-Wire channel.
 * @param   index Zero based smart port index.
 * @param   port  Channel number, 0 to 7.
 * @param   value Value to write.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceAdiValueSet
 */
void vexAdiValueSet(uint32_t index, uint32_t port, int32_t value);

/**
 * @brief   Reads a 3-Wire channel.
 * @param   index Zero based smart port index.
 * @param   port  Channel number, 0 to 7.
 * @return  The channel's current reading.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceAdiValueGet
 */
int32_t vexAdiValueGet(uint32_t index, uint32_t port);

/**
 * @brief   Writes a run of colors to an addressable LED strip.
 * @param   index   Zero based smart port index.
 * @param   port    Channel number, 0 to 7, for the ports labelled A to H.
 * @param   pData   Colors to write, one word per LED.
 * @param   nOffset Offset into the strip. PROS always passes 0; vexide passes
 *                  its own offset through.
 * @param   nLength Number of LEDs to write. PROS and vexide both cap strips at
 *                  64.
 * @param   options Option flags. VEX does not publish their meaning; PROS and
 *                  vexide pass 0.
 * @return  Not documented by VEX. PROS passes the value through unchanged.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceAdiAddrLedSet
 */
int32_t vexAdiAddrLedSet(uint32_t index, uint32_t port, uint32_t *pData,
                         uint32_t nOffset, uint32_t nLength, uint32_t options);

/**
 * @brief   Reads a smart port bumper switch.
 * @param   index Zero based smart port index.
 * @return  Whether the switch is pressed.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceBumperGet
 */
V5_DeviceBumperState vexBumperGet(uint32_t index);

/**
 * @brief   Resets a dedicated smart port gyro.
 * @param   index Zero based smart port index.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGyroReset
 */
void vexGyroReset(uint32_t index);

/**
 * @brief   Reads a dedicated smart port gyro's heading.
 * @param   index Zero based smart port index.
 * @return  Not documented.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGyroHeadingGet
 */
double vexGyroHeadingGet(uint32_t index);

/**
 * @brief   Reads a dedicated smart port gyro's angle.
 * @param   index Zero based smart port index.
 * @return  Not documented.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGyroDegreesGet
 */
double vexGyroDegreesGet(uint32_t index);

/**
 * @brief   Reads a dedicated smart port sonar.
 * @param   index Zero based smart port index.
 * @return  Not documented.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceSonarValueGet
 */
int32_t vexSonarValueGet(uint32_t index);

/**
 * @brief   Reads a device VEXos classifies as a generic sensor.
 * @param   index Zero based smart port index.
 * @return  The sensor's reading, or 0 if VEXos does not consider the device a
 *          generic sensor.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGenericValueGet
 */
int32_t vexGenericValueGet(uint32_t index);

/**
 * @brief   Spins a motor at a target velocity.
 * @param   index    Zero based smart port index.
 * @param   velocity Target velocity in RPM. PROS documents the range as +-100,
 *                   +-200 or +-600 depending on the gear cartridge.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorVelocitySet
 */
void vexMotorVelocitySet(uint32_t index, int32_t velocity);

/**
 * @brief   Changes the velocity of a profiled move already in progress.
 * @param   index    Zero based smart port index.
 * @param   velocity New velocity in RPM.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorVelocityUpdate
 */
void vexMotorVelocityUpdate(uint32_t index, int32_t velocity);

/**
 * @brief   Drives a motor at a given voltage.
 * @param   index Zero based smart port index.
 * @param   value Voltage in millivolts, -12000 to 12000. vexide caps the 5.5W
 *                EXP motor at 8000.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorVoltageSet
 */
void vexMotorVoltageSet(uint32_t index, int32_t value);

/**
 * @brief   Reads back a motor's commanded velocity.
 * @param   index Zero based smart port index.
 * @return  The velocity last commanded, in RPM.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorVelocityGet
 */
int32_t vexMotorVelocityGet(uint32_t index);

/**
 * @brief   Reads which way a motor is turning.
 * @param   index Zero based smart port index.
 * @return  1 when moving in the positive direction, -1 when moving in the
 *          negative direction.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorDirectionGet
 */
int32_t vexMotorDirectionGet(uint32_t index);

/**
 * @brief   Reads a motor's measured velocity.
 * @param   index Zero based smart port index.
 * @return  The measured velocity in RPM.
 * @details Returns 0.0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorActualVelocityGet
 */
double vexMotorActualVelocityGet(uint32_t index);

/**
 * @brief   Sets a motor's control mode directly.
 * @param   index Zero based smart port index.
 * @param   mode  Control mode to switch to.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorModeSet
 */
void vexMotorModeSet(uint32_t index, V5MotorControlMode mode);

/**
 * @brief   Reads a motor's current control mode.
 * @param   index Zero based smart port index.
 * @return  The control mode.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorModeGet
 */
V5MotorControlMode vexMotorModeGet(uint32_t index);

/**
 * @brief   Sets a motor's raw PWM output.
 * @param   index Zero based smart port index.
 * @param   value PWM command. The accepted range is not documented.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorPwmSet
 */
void vexMotorPwmSet(uint32_t index, int32_t value);

/**
 * @brief   Reads back a motor's raw PWM output.
 * @param   index Zero based smart port index.
 * @return  The current PWM command.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorPwmGet
 */
int32_t vexMotorPwmGet(uint32_t index);

/**
 * @brief   Sets a motor's current limit.
 * @param   index Zero based smart port index.
 * @param   value Current limit in milliamps.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorCurrentLimitSet
 */
void vexMotorCurrentLimitSet(uint32_t index, int32_t value);

/**
 * @brief   Reads back a motor's current limit.
 * @param   index Zero based smart port index.
 * @return  The current limit in milliamps.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorCurrentLimitGet
 */
int32_t vexMotorCurrentLimitGet(uint32_t index);

/**
 * @brief   Sets a motor's voltage limit.
 * @param   index Zero based smart port index.
 * @param   value Voltage limit. vexide passes millivolts; PROS documents its
 *                wrapper's argument as volts but passes it through unchanged,
 *                so the sources disagree.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorVoltageLimitSet
 */
void vexMotorVoltageLimitSet(uint32_t index, int32_t value);

/**
 * @brief   Reads back a motor's voltage limit.
 * @param   index Zero based smart port index.
 * @return  The voltage limit, in the same unit as
 *          vexDeviceMotorVoltageLimitSet(). 0 by default, meaning no limit.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorVoltageLimitGet
 */
int32_t vexMotorVoltageLimitGet(uint32_t index);

/**
 * @brief   Replaces the gains in a motor's position control loop.
 * @param   index Zero based smart port index.
 * @param   pid   Gain set to install. The gains are 4.4 fixed point, so 0x20
 *                means 2.0.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorPositionPidSet
 */
void vexMotorPositionPidSet(uint32_t index, V5_DeviceMotorPid *pid);

/**
 * @brief   Replaces the gains in a motor's velocity control loop.
 * @param   index Zero based smart port index.
 * @param   pid   Gain set to install, in the same format as for
 *                vexDeviceMotorPositionPidSet().
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorVelocityPidSet
 */
void vexMotorVelocityPidSet(uint32_t index, V5_DeviceMotorPid *pid);

/**
 * @brief   Reads a motor's current draw.
 * @param   index Zero based smart port index.
 * @return  The current in milliamps.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorCurrentGet
 */
int32_t vexMotorCurrentGet(uint32_t index);

/**
 * @brief   Reads the voltage delivered to a motor.
 * @param   index Zero based smart port index.
 * @return  The voltage in millivolts.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorVoltageGet
 */
int32_t vexMotorVoltageGet(uint32_t index);

/**
 * @brief   Reads the power a motor draws.
 * @param   index Zero based smart port index.
 * @return  The power in watts.
 * @details Returns 0.0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorPowerGet
 */
double vexMotorPowerGet(uint32_t index);

/**
 * @brief   Reads a motor's output torque.
 * @param   index Zero based smart port index.
 * @return  The torque in newton metres.
 * @details Returns 0.0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorTorqueGet
 */
double vexMotorTorqueGet(uint32_t index);

/**
 * @brief   Reads how efficiently a motor is running.
 * @param   index Zero based smart port index.
 * @return  The efficiency as a percentage.
 * @details Returns 0.0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorEfficiencyGet
 */
double vexMotorEfficiencyGet(uint32_t index);

/**
 * @brief   Reads a motor's internal temperature.
 * @param   index Zero based smart port index.
 * @return  The temperature in degrees Celsius.
 * @details Returns 0.0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorTemperatureGet
 */
double vexMotorTemperatureGet(uint32_t index);

/**
 * @brief   Reports whether a motor's temperature is above its limit.
 * @param   index Zero based smart port index.
 * @return  True when the temperature limit is exceeded.
 * @details Returns false without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorOverTempFlagGet
 */
bool vexMotorOverTempFlagGet(uint32_t index);

/**
 * @brief   Reports whether a motor is drawing more than its current limit.
 * @param   index Zero based smart port index.
 * @return  True when the current limit is exceeded.
 * @details Returns false without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorCurrentLimitFlagGet
 */
bool vexMotorCurrentLimitFlagGet(uint32_t index);

/**
 * @brief   Reads a motor's fault bits.
 * @param   index Zero based smart port index.
 * @return  A bit field of faults: 0x01 over temperature, 0x02 h-bridge fault,
 *          0x04 over current, 0x08 h-bridge over current. Zero means no faults.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorFaultsGet
 */
uint32_t vexMotorFaultsGet(uint32_t index);

/**
 * @brief   Reads a motor's zero velocity flag.
 * @param   index Zero based smart port index.
 * @return  The flag.
 * @details Returns false without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorZeroVelocityFlagGet
 */
bool vexMotorZeroVelocityFlagGet(uint32_t index);

/**
 * @brief   Reads a motor's zero position flag.
 * @param   index Zero based smart port index.
 * @return  The flag.
 * @details Returns false without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorZeroPositionFlagGet
 */
bool vexMotorZeroPositionFlagGet(uint32_t index);

/**
 * @brief   Reads a motor's status bits.
 * @param   index Zero based smart port index.
 * @return  A bit field: 0x01 busy, meaning the brain cannot currently
 *          communicate with the motor, 0x02 zero velocity and 0x04 zero
 *          position. Zero means none of those apply.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorFlagsGet
 */
uint32_t vexMotorFlagsGet(uint32_t index);

/**
 * @brief   Reverses the sense of a motor.
 * @param   index Zero based smart port index.
 * @param   value True to reverse.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorReverseFlagSet
 */
void vexMotorReverseFlagSet(uint32_t index, bool value);

/**
 * @brief   Reads back a motor's reverse flag.
 * @param   index Zero based smart port index.
 * @return  True if the motor is reversed.
 * @details Returns false without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorReverseFlagGet
 */
bool vexMotorReverseFlagGet(uint32_t index);

/**
 * @brief   Chooses the units a motor reports position in.
 * @param   index Zero based smart port index.
 * @param   units Degrees, rotations or encoder counts.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorEncoderUnitsSet
 */
void vexMotorEncoderUnitsSet(uint32_t index, V5MotorEncoderUnits units);

/**
 * @brief   Reads back a motor's position units.
 * @param   index Zero based smart port index.
 * @return  The units in use.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorEncoderUnitsGet
 */
V5MotorEncoderUnits vexMotorEncoderUnitsGet(uint32_t index);

/**
 * @brief   Chooses what a motor does when it is stopped.
 * @param   index Zero based smart port index.
 * @param   mode  Coast, brake or hold.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorBrakeModeSet
 */
void vexMotorBrakeModeSet(uint32_t index, V5MotorBrakeMode mode);

/**
 * @brief   Reads back a motor's brake mode.
 * @param   index Zero based smart port index.
 * @return  The brake mode in use.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorBrakeModeGet
 */
V5MotorBrakeMode vexMotorBrakeModeGet(uint32_t index);

/**
 * @brief   Redefines a motor's current position.
 * @param   index    Zero based smart port index.
 * @param   position Value the current position should read as, in the
 *                   configured encoder units.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorPositionSet
 */
void vexMotorPositionSet(uint32_t index, double position);

/**
 * @brief   Reads a motor's position.
 * @param   index Zero based smart port index.
 * @return  The position in the configured encoder units.
 * @details Returns 0.0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorPositionGet
 */
double vexMotorPositionGet(uint32_t index);

/**
 * @brief   Reads a motor's raw encoder count along with its timestamp.
 * @param   index     Zero based smart port index.
 * @param   timestamp Filled in with the time the count was read, in
 *                    milliseconds. May be null.
 * @return  The raw encoder count.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorPositionRawGet
 */
int32_t vexMotorPositionRawGet(uint32_t index, uint32_t *timestamp);

/**
 * @brief   Zeroes a motor's position reading.
 * @param   index Zero based smart port index.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorPositionReset
 */
void vexMotorPositionReset(uint32_t index);

/**
 * @brief   Reads a motor's target position.
 * @param   index Zero based smart port index.
 * @return  The target position set by the user, in the configured encoder
 *          units.
 * @details Returns 0.0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorTargetGet
 */
double vexMotorTargetGet(uint32_t index);

/**
 * @brief   Sets a servo style position target.
 * @param   index    Zero based smart port index.
 * @param   position Target position in the configured encoder units.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorServoTargetSet
 */
void vexMotorServoTargetSet(uint32_t index, double position);

/**
 * @brief   Starts a profiled move to an absolute position.
 * @param   index    Zero based smart port index.
 * @param   position Target position in the configured encoder units.
 * @param   velocity Velocity for the move, in RPM.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorAbsoluteTargetSet
 */
void vexMotorAbsoluteTargetSet(uint32_t index, double position,
                               int32_t velocity);

/**
 * @brief   Starts a profiled move relative to the current position.
 * @param   index    Zero based smart port index.
 * @param   position Distance to travel in the configured encoder units.
 * @param   velocity Velocity for the move, in RPM.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorRelativeTargetSet
 */
void vexMotorRelativeTargetSet(uint32_t index, double position,
                               int32_t velocity);

/**
 * @brief   Tells a motor which gear cartridge is fitted.
 * @param   index Zero based smart port index.
 * @param   value Cartridge ratio.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorGearingSet
 */
void vexMotorGearingSet(uint32_t index, V5MotorGearset value);

/**
 * @brief   Reads back the cartridge a motor is configured for.
 * @param   index Zero based smart port index.
 * @return  The configured cartridge ratio.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorGearingGet
 */
V5MotorGearset vexMotorGearingGet(uint32_t index);

/**
 * @brief   Reads which kind of smart motor is on a port.
 * @param   index Zero based smart port index.
 * @return  0 for the 11W V5 motor and 1 for the 5.5W EXP motor.
 * @details Returns -1 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceMotorTypeGet
 */
int32_t vexMotorTypeGet(uint32_t index);

/**
 * @brief   Feeds a motor a point of an externally generated motion profile.
 * @param   index    Zero based smart port index.
 * @param   position Position setpoint.
 * @param   velocity Velocity setpoint.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceMotorExternalProfileSet
 */
void vexMotorExternalProfileSet(uint32_t index, double position,
                                int32_t velocity);

/**
 * @brief   Chooses what the Vision Sensor looks for.
 * @param   index Zero based smart port index.
 * @param   mode  Detection mode.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceVisionModeSet
 */
void vexVisionModeSet(uint32_t index, V5VisionMode mode);

/**
 * @brief   Reads back the Vision Sensor's detection mode.
 * @param   index Zero based smart port index.
 * @return  The current mode.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceVisionModeGet
 */
V5VisionMode vexVisionModeGet(uint32_t index);

/**
 * @brief   Counts the objects the Vision Sensor is currently reporting.
 * @param   index Zero based smart port index.
 * @return  The number of objects available.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceVisionObjectCountGet
 */
int32_t vexVisionObjectCountGet(uint32_t index);

/**
 * @brief   Reads one detected object.
 * @param   index    Zero based smart port index.
 * @param   indexObj Zero based object index, below the current object count. 0
 *                   is the largest object, roughly.
 * @param   pObject  Structure filled in with the object's data.
 * @return  0 if the object could not be read. PROS and vexide both treat 0 as
 *          failure.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceVisionObjectGet
 */
int32_t vexVisionObjectGet(uint32_t index, uint32_t indexObj,
                           V5_DeviceVisionObject *pObject);

/**
 * @brief   Writes a color signature to the Vision Sensor.
 * @param   index      Zero based smart port index.
 * @param   pSignature Signature to store, with its id set to the target slot, 1
 *                     to 7.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceVisionSignatureSet
 */
void vexVisionSignatureSet(uint32_t index,
                           V5_DeviceVisionSignature *pSignature);

/**
 * @brief   Reads a color signature back from the Vision Sensor.
 * @param   index      Zero based smart port index.
 * @param   id         Signature slot, 1 to 7.
 * @param   pSignature Structure filled in with the stored signature.
 * @return  False if no signature could be read.
 * @details Returns false without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceVisionSignatureGet
 */
bool vexVisionSignatureGet(uint32_t index, uint32_t id,
                           V5_DeviceVisionSignature *pSignature);

/**
 * @brief   Sets the Vision Sensor's brightness.
 * @param   index   Zero based smart port index.
 * @param   percent Brightness, 0 to 100.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceVisionBrightnessSet
 */
void vexVisionBrightnessSet(uint32_t index, uint8_t percent);

/**
 * @brief   Reads back the Vision Sensor's brightness.
 * @param   index Zero based smart port index.
 * @return  The brightness, 0 to 100.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceVisionBrightnessGet
 */
uint8_t vexVisionBrightnessGet(uint32_t index);

/**
 * @brief   Chooses how the Vision Sensor white balances.
 * @param   index Zero based smart port index.
 * @param   mode  White balance mode.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceVisionWhiteBalanceModeSet
 */
void vexVisionWhiteBalanceModeSet(uint32_t index, V5VisionWBMode mode);

/**
 * @brief   Reads back the Vision Sensor's white balance mode.
 * @param   index Zero based smart port index.
 * @return  The current mode.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceVisionWhiteBalanceModeGet
 */
V5VisionWBMode vexVisionWhiteBalanceModeGet(uint32_t index);

/**
 * @brief   Sets the manual white balance color.
 * @param   index Zero based smart port index.
 * @param   color White balance color. PROS and vexide set the brightness member
 *                to 255; vexide doubts it has any effect.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceVisionWhiteBalanceSet
 */
void vexVisionWhiteBalanceSet(uint32_t index, V5_DeviceVisionRgb color);

/**
 * @brief   Reads back the manual white balance color.
 * @param   index Zero based smart port index.
 * @return  The white balance color.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceVisionWhiteBalanceGet
 */
V5_DeviceVisionRgb vexVisionWhiteBalanceGet(uint32_t index);

/**
 * @brief   Chooses who drives the Vision Sensor's LED.
 * @param   index Zero based smart port index.
 * @param   mode  Automatic or manual.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceVisionLedModeSet
 */
void vexVisionLedModeSet(uint32_t index, V5VisionLedMode mode);

/**
 * @brief   Reads back who drives the Vision Sensor's LED.
 * @param   index Zero based smart port index.
 * @return  The current mode.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceVisionLedModeGet
 */
V5VisionLedMode vexVisionLedModeGet(uint32_t index);

/**
 * @brief   Sets the Vision Sensor's LED brightness.
 * @param   index   Zero based smart port index.
 * @param   percent Brightness. The scale is not documented.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @note    The misspelt name is the one libv5rt defines.
 * @see     vexDeviceVisionLedBrigntnessSet
 */
void vexVisionLedBrigntnessSet(uint32_t index, uint8_t percent);

/**
 * @brief   Reads back the Vision Sensor's LED brightness.
 * @param   index Zero based smart port index.
 * @return  The brightness. The scale is not documented.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @note    The misspelt name is the one libv5rt defines.
 * @see     vexDeviceVisionLedBrigntnessGet
 */
uint8_t vexVisionLedBrigntnessGet(uint32_t index);

/**
 * @brief   Sets the Vision Sensor's LED color.
 * @param   index Zero based smart port index.
 * @param   color Color to show. The scale of the brightness member is disputed:
 *                PROS sets it to 255, while vexide writes 0 to 100.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceVisionLedColorSet
 */
void vexVisionLedColorSet(uint32_t index, V5_DeviceVisionRgb color);

/**
 * @brief   Reads back the Vision Sensor's LED color.
 * @param   index Zero based smart port index.
 * @return  The LED color.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceVisionLedColorGet
 */
V5_DeviceVisionRgb vexVisionLedColorGet(uint32_t index);

/**
 * @brief   Turns the Vision Sensor's Wi-Fi mode on or off.
 * @param   index Zero based smart port index.
 * @param   mode  On or off.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceVisionWifiModeSet
 */
void vexVisionWifiModeSet(uint32_t index, V5VisionWifiMode mode);

/**
 * @brief   Reads back the Vision Sensor's Wi-Fi mode.
 * @param   index Zero based smart port index.
 * @return  The current mode.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceVisionWifiModeGet
 */
V5VisionWifiMode vexVisionWifiModeGet(uint32_t index);

/**
 * @brief   Resets the AI Vision Sensor to its initial state.
 * @param   index Zero based smart port index.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAiVisionReset
 */
void vexAiVisionReset(uint32_t index);

/**
 * @brief   Turns individual kinds of detection on or off.
 * @param   index   Zero based smart port index.
 * @param   bits    New state for the kinds selected by bitmask.
 * @param   bitmask Which kinds of detection the call applies to.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAiVisionEnableSet
 */
void vexAiVisionEnableSet(uint32_t index, uint8_t bits, uint8_t bitmask);

/**
 * @brief   Reads back which kinds of detection are enabled.
 * @param   index Zero based smart port index.
 * @return  A bit field in the same layout as the bits argument of
 *          vexDeviceAiVisionEnableSet().
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAiVisionEnableGet
 */
uint8_t vexAiVisionEnableGet(uint32_t index);

/**
 * @brief   Writes the AI Vision Sensor's mode word.
 * @param   index Zero based smart port index.
 * @param   mode  Packed mode word.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceAiVisionModeSet
 */
void vexAiVisionModeSet(uint32_t index, uint32_t mode);

/**
 * @brief   Reads the AI Vision Sensor's status word.
 * @param   index Zero based smart port index.
 * @return  The status word. vexide documents it as the mode word shifted right
 *          by 8 bits, so its lowest byte holds the flags byte.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAiVisionStatusGet
 */
uint32_t vexAiVisionStatusGet(uint32_t index);

/**
 * @brief   Counts the objects the AI Vision Sensor is currently reporting.
 * @param   index Zero based smart port index.
 * @return  The number of objects available. vexide documents the maximum as 24.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceAiVisionObjectCountGet
 */
int32_t vexAiVisionObjectCountGet(uint32_t index);

/**
 * @brief   Reads one detected object.
 * @param   index    Zero based smart port index.
 * @param   indexObj Zero based object index, below the current object count.
 * @param   pObject  Structure filled in with the object's data.
 * @return  Not documented. PROS and vexide ignore it.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceAiVisionObjectGet
 */
int32_t vexAiVisionObjectGet(uint32_t index, uint32_t indexObj,
                             V5_DeviceAiVisionObject *pObject);

/**
 * @brief   Writes a color descriptor to the AI Vision Sensor.
 * @param   index  Zero based smart port index.
 * @param   pColor Descriptor to store, with its id set to the target slot, 1 to
 *                 7.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceAiVisionColorSet
 */
void vexAiVisionColorSet(uint32_t index, V5_DeviceAiVisionColor *pColor);

/**
 * @brief   Reads a color descriptor back from the AI Vision Sensor.
 * @param   index  Zero based smart port index.
 * @param   id     Descriptor slot, 1 to 7.
 * @param   pColor Structure filled in with the stored descriptor.
 * @return  True if the descriptor was read. vexide treats false as an empty
 *          slot.
 * @details Returns false without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceAiVisionColorGet
 */
bool vexAiVisionColorGet(uint32_t index, uint32_t id,
                         V5_DeviceAiVisionColor *pColor);

/**
 * @brief   Writes a code descriptor to the AI Vision Sensor.
 * @param   index Zero based smart port index.
 * @param   pCode Descriptor to store, with its id set to the target slot. PROS
 *                documents slots 1 to 5, vexide accepts 1 to 8.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceAiVisionCodeSet
 */
void vexAiVisionCodeSet(uint32_t index, V5_DeviceAiVisionCode *pCode);

/**
 * @brief   Reads a code descriptor back from the AI Vision Sensor.
 * @param   index Zero based smart port index.
 * @param   id    Descriptor slot; see vexDeviceAiVisionCodeSet().
 * @param   pCode Structure filled in with the stored descriptor.
 * @return  True if the descriptor was read. vexide treats false as an empty
 *          slot.
 * @details Returns false without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceAiVisionCodeGet
 */
bool vexAiVisionCodeGet(uint32_t index, uint32_t id,
                        V5_DeviceAiVisionCode *pCode);

/**
 * @brief   Reads the name of one of the AI model's classes.
 * @param   index Zero based smart port index.
 * @param   id    Class index, 0 to 19, or -1 to ask how many classes there are.
 * @param   pName Buffer of at least 20 bytes for the name.
 * @return  The number of classes when id is -1, otherwise undefined.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAiVisionClassNameGet
 */
int32_t vexAiVisionClassNameGet(uint32_t index, int32_t id, uint8_t *pName);

/**
 * @brief   Reads the AI Vision Sensor's temperature.
 * @param   index Zero based smart port index.
 * @return  The temperature in degrees Celsius.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAiVisionTemperatureGet
 */
double vexAiVisionTemperatureGet(uint32_t index);

/**
 * @brief   Starts calibrating the Inertial Sensor.
 * @param   index Zero based smart port index.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuReset
 */
void vexImuReset(uint32_t index);

/**
 * @brief   Reads the Inertial Sensor's accumulated rotation about the z axis.
 * @param   index Zero based smart port index.
 * @return  The rotation in degrees, unbounded, clockwise positive.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuHeadingGet
 */
double vexImuHeadingGet(uint32_t index);

/**
 * @brief   Reads the Inertial Sensor's heading.
 * @param   index Zero based smart port index.
 * @return  The heading in degrees, 0 up to but not including 360, clockwise
 *          positive.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuDegreesGet
 */
double vexImuDegreesGet(uint32_t index);

/**
 * @brief   Reads the Inertial Sensor's orientation as a quaternion.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with the quaternion.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuQuaternionGet
 */
void vexImuQuaternionGet(uint32_t index, V5_DeviceImuQuaternion *data);

/**
 * @brief   Reads the Inertial Sensor's orientation as Euler angles.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with pitch, roll and yaw in degrees, each
 *                bounded to -180 through 180.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuAttitudeGet
 */
void vexImuAttitudeGet(uint32_t index, V5_DeviceImuAttitude *data);

/**
 * @brief   Reads the Inertial Sensor's raw gyroscope rates.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with x, y and z in degrees per second. The
 *                w member is not used.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuRawGyroGet
 */
void vexImuRawGyroGet(uint32_t index, V5_DeviceImuRaw *data);

/**
 * @brief   Reads the Inertial Sensor's raw accelerations.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with x, y and z in g. The w member is not
 *                used.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuRawAccelGet
 */
void vexImuRawAccelGet(uint32_t index, V5_DeviceImuRaw *data);

/**
 * @brief   Reads the Inertial Sensor's status bits.
 * @param   index Zero based smart port index.
 * @return  A bit field: bit 0 is set while calibrating, bits 1 to 3 hold the
 *          physical orientation measured at calibration, and bit 4 is set if
 *          VEXos calibrated the sensor automatically. vexide waits for a
 *          non-zero value at startup, before VEXos has the sensor's first
 *          packet.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuStatusGet
 */
uint32_t vexImuStatusGet(uint32_t index);

/**
 * @brief   Sets the Inertial Sensor's mode word.
 * @param   index Zero based smart port index.
 * @param   mode  Mode word.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuModeSet
 */
void vexImuModeSet(uint32_t index, uint32_t mode);

/**
 * @brief   Reads back the Inertial Sensor's mode word.
 * @param   index Zero based smart port index.
 * @return  The mode word.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuModeGet
 */
uint32_t vexImuModeGet(uint32_t index);

/**
 * @brief   Sets how often the Inertial Sensor produces new readings.
 * @param   index Zero based smart port index.
 * @param   rate  Interval in milliseconds, in steps of 5 and at least 5. The
 *                default is 10.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceImuDataRateSet
 */
void vexImuDataRateSet(uint32_t index, uint32_t rate);

/**
 * @brief   Reads a rangefinder or lidar device.
 * @param   index Zero based smart port index.
 * @return  Not documented.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceRangeValueGet
 */
int32_t vexRangeValueGet(uint32_t index);

/**
 * @brief   Resets the Rotation Sensor's position to its angle.
 * @param   index Zero based smart port index.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAbsEncReset
 */
void vexAbsEncReset(uint32_t index);

/**
 * @brief   Redefines the Rotation Sensor's position.
 * @param   index    Zero based smart port index.
 * @param   position Value the current position should read as, in centidegrees.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAbsEncPositionSet
 */
void vexAbsEncPositionSet(uint32_t index, int32_t position);

/**
 * @brief   Reads the Rotation Sensor's accumulated position.
 * @param   index Zero based smart port index.
 * @return  The position in centidegrees, so 36000 per turn. Unbounded.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAbsEncPositionGet
 */
int32_t vexAbsEncPositionGet(uint32_t index);

/**
 * @brief   Reads the Rotation Sensor's speed.
 * @param   index Zero based smart port index.
 * @return  The speed in centidegrees per second.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAbsEncVelocityGet
 */
int32_t vexAbsEncVelocityGet(uint32_t index);

/**
 * @brief   Reads the Rotation Sensor's absolute angle.
 * @param   index Zero based smart port index.
 * @return  The angle in centidegrees. PROS documents the range as 0 to 36000.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAbsEncAngleGet
 */
int32_t vexAbsEncAngleGet(uint32_t index);

/**
 * @brief   Reverses the Rotation Sensor's direction.
 * @param   index Zero based smart port index.
 * @param   value True to reverse.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAbsEncReverseFlagSet
 */
void vexAbsEncReverseFlagSet(uint32_t index, bool value);

/**
 * @brief   Reads back the Rotation Sensor's reverse flag.
 * @param   index Zero based smart port index.
 * @return  True if the sensor is reversed.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAbsEncReverseFlagGet
 */
bool vexAbsEncReverseFlagGet(uint32_t index);

/**
 * @brief   Reads the Rotation Sensor's status word.
 * @param   index Zero based smart port index.
 * @return  The status word. PROS treats 0 as the sensor not yet being ready.
 *          VEX does not publish the meaning of the individual bits.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAbsEncStatusGet
 */
uint32_t vexAbsEncStatusGet(uint32_t index);

/**
 * @brief   Sets how often the Rotation Sensor produces new readings.
 * @param   index Zero based smart port index.
 * @param   rate  Interval in milliseconds, in steps of 5 and at least 5. The
 *                default is 10.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceAbsEncDataRateSet
 */
void vexAbsEncDataRateSet(uint32_t index, uint32_t rate);

/**
 * @brief   Reads the hue of the color in front of the Optical Sensor.
 * @param   index Zero based smart port index.
 * @return  The hue in degrees, 0 to 359.999.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalHueGet
 */
double vexOpticalHueGet(uint32_t index);

/**
 * @brief   Reads the saturation of the color in front of the Optical Sensor.
 * @param   index Zero based smart port index.
 * @return  The saturation, 0 to 1.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalSatGet
 */
double vexOpticalSatGet(uint32_t index);

/**
 * @brief   Reads the brightness of the color in front of the Optical Sensor.
 * @param   index Zero based smart port index.
 * @return  The brightness, 0 to 1.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalBrightnessGet
 */
double vexOpticalBrightnessGet(uint32_t index);

/**
 * @brief   Reads the Optical Sensor's proximity reading.
 * @param   index Zero based smart port index.
 * @return  The proximity, 0 to 255, larger meaning closer.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalProximityGet
 */
int32_t vexOpticalProximityGet(uint32_t index);

/**
 * @brief   Reads the Optical Sensor's processed color.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with the color and brightness.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalRgbGet
 */
void vexOpticalRgbGet(uint32_t index, V5_DeviceOpticalRgb *data);

/**
 * @brief   Sets the brightness of the Optical Sensor's white LED.
 * @param   index Zero based smart port index.
 * @param   value Brightness, 0 to 100.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalLedPwmSet
 */
void vexOpticalLedPwmSet(uint32_t index, int32_t value);

/**
 * @brief   Reads back the Optical Sensor's LED brightness.
 * @param   index Zero based smart port index.
 * @return  The brightness, 0 to 100.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalLedPwmGet
 */
int32_t vexOpticalLedPwmGet(uint32_t index);

/**
 * @brief   Reads the Optical Sensor's status word.
 * @param   index Zero based smart port index.
 * @return  The status word. VEX does not publish the meaning of the individual
 *          bits.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalStatusGet
 */
uint32_t vexOpticalStatusGet(uint32_t index);

/**
 * @brief   Reads the Optical Sensor's unprocessed channel values.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with the clear, red, green and blue
 *                values.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalRawGet
 */
void vexOpticalRawGet(uint32_t index, V5_DeviceOpticalRaw *data);

/**
 * @brief   Sets the Optical Sensor's operating mode.
 * @param   index Zero based smart port index.
 * @param   mode  Mode value.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalModeSet
 */
void vexOpticalModeSet(uint32_t index, uint32_t mode);

/**
 * @brief   Reads back the Optical Sensor's operating mode.
 * @param   index Zero based smart port index.
 * @return  The mode value.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalModeGet
 */
uint32_t vexOpticalModeGet(uint32_t index);

/**
 * @brief   Reads the most recent gesture from the Optical Sensor.
 * @param   index Zero based smart port index.
 * @param   pData Structure filled in with the raw gesture data, or null if only
 *                the direction is wanted.
 * @return  The recognised direction: 0 for none, 1 up and 2 down. PROS
 *          documents 3 as right and 4 as left; vexide maps them the other way
 *          round.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalGestureGet
 */
uint32_t vexOpticalGestureGet(uint32_t index, V5_DeviceOpticalGesture *pData);

/**
 * @brief   Enables gesture detection on the Optical Sensor.
 * @param   index Zero based smart port index.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalGestureEnable
 */
void vexOpticalGestureEnable(uint32_t index);

/**
 * @brief   Disables gesture detection on the Optical Sensor.
 * @param   index Zero based smart port index.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalGestureDisable
 */
void vexOpticalGestureDisable(uint32_t index);

/**
 * @brief   Sets a proximity threshold.
 * @param   index Zero based smart port index.
 * @param   value Threshold.
 * @return  Not documented.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalProximityThreshold
 */
int32_t vexOpticalProximityThreshold(uint32_t index, int32_t value);

/**
 * @brief   Sets how long the Optical Sensor integrates light for.
 * @param   index  Zero based smart port index.
 * @param   timems Integration time in milliseconds, 3 to 712. The default is
 *                 103.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalIntegrationTimeSet
 */
void vexOpticalIntegrationTimeSet(uint32_t index, double timems);

/**
 * @brief   Reads back the Optical Sensor's integration time.
 * @param   index Zero based smart port index.
 * @return  The integration time in milliseconds.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceOpticalIntegrationTimeGet
 */
double vexOpticalIntegrationTimeGet(uint32_t index);

/**
 * @brief   Energises the Workcell electromagnet for a set time.
 * @param   index Zero based smart port index.
 * @param   value Power level, -100 to 100. Larger values attract more strongly.
 * @param   time  How long to stay energised, in milliseconds. vexide documents
 *                a maximum of 2 seconds.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceMagnetPowerSet
 */
void vexMagnetPowerSet(uint32_t index, int32_t value, int32_t time);

/**
 * @brief   Reads back the electromagnet's power level.
 * @param   index Zero based smart port index.
 * @return  The power level last set, -100 to 100.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceMagnetPowerGet
 */
int32_t vexMagnetPowerGet(uint32_t index);

/**
 * @brief   Runs the electromagnet's pickup sequence.
 * @param   index    Zero based smart port index.
 * @param   duration One of the V5_DeviceMagnetDuration presets.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceMagnetPickup
 */
void vexMagnetPickup(uint32_t index, V5_DeviceMagnetDuration duration);

/**
 * @brief   Runs the electromagnet's drop sequence.
 * @param   index    Zero based smart port index.
 * @param   duration One of the V5_DeviceMagnetDuration presets.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceMagnetDrop
 */
void vexMagnetDrop(uint32_t index, V5_DeviceMagnetDuration duration);

/**
 * @brief   Reads the electromagnet's temperature.
 * @param   index Zero based smart port index.
 * @return  The temperature in degrees Celsius.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceMagnetTemperatureGet
 */
double vexMagnetTemperatureGet(uint32_t index);

/**
 * @brief   Reads the electromagnet's current draw.
 * @param   index Zero based smart port index.
 * @return  The current in milliamps. vexide divides it by 1000 to report amps.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceMagnetCurrentGet
 */
double vexMagnetCurrentGet(uint32_t index);

/**
 * @brief   Reads the electromagnet's status code.
 * @param   index Zero based smart port index.
 * @return  The status code. VEX does not publish its meaning.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceMagnetStatusGet
 */
uint32_t vexMagnetStatusGet(uint32_t index);

/**
 * @brief   Reads the distance to the object in front of the sensor.
 * @param   index Zero based smart port index.
 * @return  The distance in millimetres, or 9999 if no object was found.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceDistanceDistanceGet
 */
uint32_t vexDistanceDistanceGet(uint32_t index);

/**
 * @brief   Reads how much the sensor trusts its distance reading.
 * @param   index Zero based smart port index.
 * @return  The confidence, 0 through 63, larger being better.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceDistanceConfidenceGet
 */
uint32_t vexDistanceConfidenceGet(uint32_t index);

/**
 * @brief   Reads the sensor's guess at the detected object's relative size.
 * @param   index Zero based smart port index.
 * @return  A unitless size, 0 through 400. An 18 by 30 inch grey card reads
 *          about 75 in typical room lighting.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceDistanceObjectSizeGet
 */
int32_t vexDistanceObjectSizeGet(uint32_t index);

/**
 * @brief   Reads the detected object's approach velocity.
 * @param   index Zero based smart port index.
 * @return  The velocity in metres per second.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceDistanceObjectVelocityGet
 */
double vexDistanceObjectVelocityGet(uint32_t index);

/**
 * @brief   Reads the Distance Sensor's status code.
 * @param   index Zero based smart port index.
 * @return  The status code. It reads 0 while the sensor is still initialising.
 *          PROS and vexide treat 0x82 and 0x86 as working; VEX does not publish
 *          what the codes mean.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceDistanceStatusGet
 */
uint32_t vexDistanceStatusGet(uint32_t index);

/**
 * @brief   Resets the GPS Sensor.
 * @param   index Zero based smart port index.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsReset
 */
void vexGpsReset(uint32_t index);

/**
 * @brief   Reads the GPS Sensor's accumulated rotation about the z axis.
 * @param   index Zero based smart port index.
 * @return  The rotation in degrees, unbounded, with clockwise positive.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsHeadingGet
 */
double vexGpsHeadingGet(uint32_t index);

/**
 * @brief   Reads the GPS Sensor's heading.
 * @param   index Zero based smart port index.
 * @return  The heading in degrees, from 0 up to but not including 360, with
 *          clockwise positive.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsDegreesGet
 */
double vexGpsDegreesGet(uint32_t index);

/**
 * @brief   Reads the GPS Sensor's orientation as a quaternion.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with the quaternion.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsQuaternionGet
 */
void vexGpsQuaternionGet(uint32_t index, V5_DeviceGpsQuaternion *data);

/**
 * @brief   Reads the GPS Sensor's position and orientation together.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with position in metres and pitch, roll
 *                and yaw in degrees.
 * @param   bRaw  VEX does not document this flag. PROS and vexide always pass
 *                false.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsAttitudeGet
 */
void vexGpsAttitudeGet(uint32_t index, V5_DeviceGpsAttitude *data, bool bRaw);

/**
 * @brief   Reads the raw gyroscope values of the GPS Sensor's inertial sensor.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with the x, y and z values. VEX does not
 *                document their units.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsRawGyroGet
 */
void vexGpsRawGyroGet(uint32_t index, V5_DeviceGpsRaw *data);

/**
 * @brief   Reads the raw accelerometer values of the GPS Sensor's inertial
 *          sensor.
 * @param   index Zero based smart port index.
 * @param   data  Structure filled in with the x, y and z values. VEX does not
 *                document their units.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsRawAccelGet
 */
void vexGpsRawAccelGet(uint32_t index, V5_DeviceGpsRaw *data);

/**
 * @brief   Reads the GPS Sensor's status code.
 * @param   index Zero based smart port index.
 * @return  The status code. VEX does not publish its meaning.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsStatusGet
 */
uint32_t vexGpsStatusGet(uint32_t index);

/**
 * @brief   Sets the GPS Sensor's mode word.
 * @param   index Zero based smart port index.
 * @param   mode  Mode value.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsModeSet
 */
void vexGpsModeSet(uint32_t index, uint32_t mode);

/**
 * @brief   Reads back the GPS Sensor's mode word.
 * @param   index Zero based smart port index.
 * @return  The mode word.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsModeGet
 */
uint32_t vexGpsModeGet(uint32_t index);

/**
 * @brief   Sets how often the GPS Sensor's inertial sensor updates.
 * @param   index Zero based smart port index.
 * @param   rate  Interval in milliseconds. PROS raises it to at least 5 and
 *                rounds it down to a multiple of 5 before calling this.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsDataRateSet
 */
void vexGpsDataRateSet(uint32_t index, uint32_t rate);

/**
 * @brief   Tells the GPS Sensor where it sits on the robot.
 * @param   index Zero based smart port index.
 * @param   ox    X offset of the sensor from the robot's reference point, such
 *                as its centre of turning, in metres.
 * @param   oy    Y offset of the sensor from the same point, in metres.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsOriginSet
 */
void vexGpsOriginSet(uint32_t index, double ox, double oy);

/**
 * @brief   Reads back the GPS Sensor's mounting offset.
 * @param   index Zero based smart port index.
 * @param   ox    Filled in with the X offset in metres.
 * @param   oy    Filled in with the Y offset in metres.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsOriginGet
 */
void vexGpsOriginGet(uint32_t index, double *ox, double *oy);

/**
 * @brief   Sets the GPS Sensor's rotation value.
 * @param   index Zero based smart port index.
 * @param   value Rotation in degrees.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsRotationSet
 */
void vexGpsRotationSet(uint32_t index, double value);

/**
 * @brief   Reads back the GPS Sensor's rotation value.
 * @param   index Zero based smart port index.
 * @return  The rotation value; see vexDeviceGpsRotationSet().
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsRotationGet
 */
double vexGpsRotationGet(uint32_t index);

/**
 * @brief   Tells the GPS Sensor where the robot starts on the field.
 * @param   index            Zero based smart port index.
 * @param   initial_x        Starting X position in metres, with (0, 0) at the
 *                           centre of the field.
 * @param   initial_y        Starting Y position in metres.
 * @param   initial_rotation Starting heading in degrees, 0 to 360, with 0 being
 *                           north on the field and clockwise positive.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsInitialPositionSet
 */
void vexGpsInitialPositionSet(uint32_t index, double initial_x,
                              double initial_y, double initial_rotation);

/**
 * @brief   Reads how uncertain the GPS Sensor's position is.
 * @param   index Zero based smart port index.
 * @return  The root mean squared position error in metres.
 * @details Passes the handle from vexDeviceGetByIndex() on without checking it
 *          for null.
 * @see     vexDeviceGpsErrorGet
 */
double vexGpsErrorGet(uint32_t index);

// Generic serial port comms to any device

/**
 * @brief   Switches a smart port into generic serial mode.
 * @param   index   Zero based smart port index.
 * @param   options Option flags. VEX does not publish their meaning; PROS and
 *                  vexide pass 0.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceGenericSerialEnable
 */
void vexGenericSerialEnable(uint32_t index, int32_t options);

/**
 * @brief   Sets the baud rate of a generic serial port.
 * @param   index    Zero based smart port index.
 * @param   baudrate Baud rate in bits per second. vexide documents 921600 as
 *                   the maximum for user programs.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceGenericSerialBaudrate
 */
void vexGenericSerialBaudrate(uint32_t index, int32_t baudrate);

/**
 * @brief   Queues one byte for transmission.
 * @param   index Zero based smart port index.
 * @param   c     Byte to send.
 * @return  The number of bytes written, or -1 on error.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericSerialWriteChar
 */
int32_t vexGenericSerialWriteChar(uint32_t index, uint8_t c);

/**
 * @brief   Reports how much room is left in the transmit buffer.
 * @param   index Zero based smart port index.
 * @return  The number of free bytes. vexide treats -1 as an error.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericSerialWriteFree
 */
int32_t vexGenericSerialWriteFree(uint32_t index);

/**
 * @brief   Queues a buffer for transmission.
 * @param   index  Zero based smart port index.
 * @param   buffer Bytes to send.
 * @param   length Maximum number of bytes to send.
 * @return  The number of bytes written, or -1 on error.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericSerialTransmit
 */
int32_t vexGenericSerialTransmit(uint32_t index, uint8_t *buffer,
                                 int32_t length);

/**
 * @brief   Removes one byte from the receive buffer.
 * @param   index Zero based smart port index.
 * @return  The byte, or -1 if the buffer is empty.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericSerialReadChar
 */
int32_t vexGenericSerialReadChar(uint32_t index);

/**
 * @brief   Looks at the next received byte without removing it.
 * @param   index Zero based smart port index.
 * @return  The byte, or -1 if the buffer is empty.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericSerialPeekChar
 */
int32_t vexGenericSerialPeekChar(uint32_t index);

/**
 * @brief   Reports how many bytes are waiting to be read.
 * @param   index Zero based smart port index.
 * @return  The number of buffered bytes. vexide treats -1 as an error.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericSerialReceiveAvail
 */
int32_t vexGenericSerialReceiveAvail(uint32_t index);

/**
 * @brief   Reads buffered bytes into a caller supplied buffer.
 * @param   index  Zero based smart port index.
 * @param   buffer Destination buffer.
 * @param   length Maximum number of bytes to read.
 * @return  The number of bytes read. vexide treats -1 as an error.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericSerialReceive
 */
int32_t vexGenericSerialReceive(uint32_t index, uint8_t *buffer,
                                int32_t length);

/**
 * @brief   Discards the contents of a generic serial port's transmit and
 *          receive buffers.
 * @param   index Zero based smart port index.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceGenericSerialFlush
 */
void vexGenericSerialFlush(uint32_t index);

// Radio link between two brains

/**
 * @brief   Starts a VEXlink connection on a VEXnet radio.
 * @param   index   Zero based smart port index.
 * @param   link_id Name of the link. The two radios to be linked use the same
 *                  name, which should differ from any other link nearby.
 * @param   type    1 for the manager (PROS: transmitter) end, which can send
 *                  about 1040 bytes per second, or 0 for the worker (PROS:
 *                  receiver) end, which can send about 520. A link needs one of
 *                  each. The official VEX header names this bMaster.
 * @param   ov      Whether the link may take over the radio from the
 *                  controller. The official VEX header names this
 *                  bAllowRadioOverride.
 * @details Does nothing if vexDeviceGetByIndex() returns null.
 * @see     vexDeviceGenericRadioConnection
 */
void vexGenericRadioConnection(uint32_t index, char *link_id, int32_t type,
                               bool ov);

/**
 * @brief   Reports whether a radio link is up.
 * @param   index Zero based smart port index.
 * @return  True once the radio is linked with another radio.
 * @details Returns false without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericRadioLinkStatus
 */
bool vexGenericRadioLinkStatus(uint32_t index);

/**
 * @brief   Reports how much room is left in the link's transmit buffer.
 * @param   index Zero based smart port index.
 * @return  The number of free bytes.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericRadioWriteFree
 */
uint32_t vexGenericRadioWriteFree(uint32_t index);

/**
 * @brief   Queues a buffer for transmission over the link.
 * @param   index  Zero based smart port index.
 * @param   buffer Bytes to send.
 * @param   length Number of bytes to send.
 * @return  The number of bytes queued.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericRadioTransmit
 */
uint32_t vexGenericRadioTransmit(uint32_t index, uint8_t *buffer,
                                 uint32_t length);

/**
 * @brief   Reports how many bytes the link has waiting to be read.
 * @param   index Zero based smart port index.
 * @return  The number of buffered bytes.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericRadioReceiveAvail
 */
uint32_t vexGenericRadioReceiveAvail(uint32_t index);

/**
 * @brief   Reads buffered bytes from the link into a caller supplied buffer.
 * @param   index  Zero based smart port index.
 * @param   buffer Destination buffer.
 * @param   length Maximum number of bytes to read.
 * @return  The number of bytes read.
 * @details Returns 0 without calling VEXos if vexDeviceGetByIndex() returns
 *          null.
 * @see     vexDeviceGenericRadioReceive
 */
uint32_t vexGenericRadioReceive(uint32_t index, uint8_t *buffer,
                                uint32_t length);

#ifdef __cplusplus
}
#endif
#endif /* V5_APIUSER_H_ */
