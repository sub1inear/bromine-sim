/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Innovation First 2016, All rights reserved.               */
/*                                                                            */
/*    Module:     v5_api.h                                                    */
/*    Author:     James Pearman                                               */
/*    Created:    8 Nov 2016                                                  */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifndef V5_API_H_
#define V5_API_H_

#include "stdarg.h"
#include "stdbool.h"
#include "stdint.h"

/*-----------------------------------------------------------------------------*/
/** @file    v5_api.h
 * @brief   Header for V5 API
 *
 * @details
 *  VEXos exposes its services to user code through a jumptable: a fixed array
 *  of function pointers at a constant address that survives firmware updates.
 *  The prototypes in this file are the wrappers around those entries. They are
 *  ordinary function calls rather than supervisor calls, so there is no mode
 *  switch and no meaningful call overhead.
 *
 *  Most device functions take a #V5_DeviceT handle obtained from
 *  vexDeviceGetByIndex(). v5_apiuser.h declares a parallel set of wrappers that
 *  take a port index instead, which is usually more convenient.
 *
 *  Two conventions run through the whole API. Device calls do not validate the
 *  handle or the device type, so reading a motor function on a port holding a
 *  sensor returns a meaningless value rather than an error. And device readings
 *  come from a shared memory buffer that VEXos refreshes in the background, so
 *  values only change as fast as that buffer is updated, normally every 10 ms.
 *//*---------------------------------------------------------------------------*/

#include "v5_apitypes.h"
#include "v5_apiuser.h"
#include "v5_color.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*----------------------------------------------------------------------------*/
/*    jumptable public API */
/*----------------------------------------------------------------------------*/
//
// These are the raw jumptable wrapper function prototypes
// Many are used directly, others are further wrapped in a simpler API
// allowing access via V5 port number rather than a device pointer. see
// v5_apiuser.h for these functions
//

/**
 * @brief   Runs one pass of VEXos background processing.
 * @details VEXos schedules its own work cooperatively on the user
 * processor. This call gives it the chance to move data between the user
 *          processor and the device processor, refresh device readings,
 * flush serial output and service USB. A program that never calls it, or
 *          that blocks for a long time without calling it, will see stale
 *          device data and stalled console output.
 *
 *          Runtimes with their own scheduler normally call this from a
 *          background task. It is also the right thing to call inside a
 * tight loop that cannot yield, such as a fault handler that still needs to
 *          get its diagnostics out over the serial link.
 */
void vexBackgroundProcessing(void);

// Console output

/**
 * @brief   Writes a formatted message to the VEXos debug output.
 * @param   fmt printf style format string.
 * @param   ... Arguments consumed by the format string.
 * @return  The number of characters written.
 */
int32_t vexDebug(char const *fmt, ...);

/**
 * @brief   Writes a formatted message to the serial console.
 * @param   fmt printf style format string.
 * @param   ... Arguments consumed by the format string.
 * @return  The number of characters written.
 * @details This is the formatter built into VEXos rather than the one from
 * the C library the program is linked against. Output goes to serial
 *          channel 1, the same channel vexSerialWriteBuffer() uses for
 * stdio.
 */
int32_t vex_printf(char const *fmt, ...);

/**
 * @brief   Formats a string into a caller supplied buffer.
 * @param   out    Destination buffer. Must be large enough for the result
 * and its terminating null.
 * @param   format printf style format string.
 * @param   ...    Arguments consumed by the format string.
 * @return  The number of characters written, not counting the terminator.
 */
int32_t vex_sprintf(char *out, const char *format, ...);

/**
 * @brief   Formats a string into a caller supplied buffer with a length
 * limit.
 * @param   out     Destination buffer.
 * @param   max_len Size of the destination buffer in bytes.
 * @param   format  printf style format string.
 * @param   ...     Arguments consumed by the format string.
 * @return  The number of characters written, not counting the terminator.
 */
int32_t vex_snprintf(char *out, uint32_t max_len, const char *format, ...);

/**
 * @brief   va_list form of vex_sprintf().
 * @param   out    Destination buffer.
 * @param   format printf style format string.
 * @param   args   Argument list, already started with va_start().
 * @return  The number of characters written, not counting the terminator.
 */
int32_t vex_vsprintf(char *out, const char *format, va_list args);

/**
 * @brief   va_list form of vex_snprintf().
 * @param   out     Destination buffer.
 * @param   max_len Size of the destination buffer in bytes.
 * @param   format  printf style format string.
 * @param   args    Argument list, already started with va_start().
 * @return  The number of characters written, not counting the terminator.
 */
int32_t vex_vsnprintf(char *out, uint32_t max_len, const char *format,
                      va_list args);

// system

/**
 * @brief   Milliseconds elapsed since the program started.
 * @return  The elapsed time in milliseconds.
 * @details Wraps after about 49 days. For finer resolution use
 *          vexSystemHighResTimeGet().
 */
uint32_t vexSystemTimeGet(void);

/**
 * @brief   Reads the time of day from the brain's real time clock.
 * @param   pTime Structure filled in with the current time.
 */
void vexGettime(struct time *pTime);

/**
 * @brief   Reads the date from the brain's real time clock.
 * @param   pDate Structure filled in with the current date.
 */
void vexGetdate(struct date *pDate);

/**
 * @brief   Prints a summary of the program's memory layout to the console.
 * @details Intended for debugging. The exact format is not fixed.
 */
void vexSystemMemoryDump(void);

/**
 * @brief   Drives one of the brain's internal digital IO pins.
 * @param   pin   Pin number.
 * @param   value Value to drive.
 * @details Used for board level diagnostics. VEX does not publish the pin
 *          numbering or which pins are safe to touch.
 */
void vexSystemDigitalIO(uint32_t pin, uint32_t value);

/**
 * @brief   Returns the startup options word VEXos recorded for this
 * program.
 * @return  The options word.
 * @details VEX does not publish the meaning of the individual bits.
 */
uint32_t vexSystemStartupOptions(void);

/**
 * @brief   Asks VEXos to end the running program.
 * @details Control returns to the brain's program menu. Anything the
 * program still has buffered, console output in particular, should be
 * flushed before calling this. The call does not necessarily return
 *          immediately, so treat the following code as unreachable.
 */
void vexSystemExitRequest(void);

/**
 * @brief   Microseconds elapsed since the program started.
 * @return  The elapsed time in microseconds.
 */
uint64_t vexSystemHighResTimeGet(void);

/**
 * @brief   Microseconds elapsed since the brain was powered on.
 * @return  The elapsed time in microseconds.
 * @details Unlike vexSystemHighResTimeGet(), this keeps counting across
 *          program starts, so it can be used to tell one run from the next.
 */
uint64_t vexSystemPowerupTimeGet(void);

/**
 * @brief   Returns the load address of the file linked to this program.
 * @return  The address in memory, or 0 if no file is linked.
 * @details VEXos lets a program be uploaded alongside a second binary,
 * which is loaded at a fixed address before the program runs. Runtimes use
 *          this to support hot and cold linking, where the rarely changing
 *          library half is uploaded once and only the user half is
 * re-uploaded.
 */
uint32_t vexSystemLinkAddrGet(void);

/**
 * @brief   Returns the brain's USB connection state.
 * @return  A bit field describing the connection.
 * @details VEX does not publish the meaning of the individual bits.
 */
uint32_t vexSystemUsbStatus(void);

// Generic device

/**
 * @brief   Returns the number of device port indexes this brain exposes.
 * @return  The port count.
 * @details Larger than the 21 smart ports, because VEXos also reports
 * internal ports for the controllers and the battery.
 */
uint32_t vexDevicesGetNumber(void);

/**
 * @brief   Counts how many devices of a given type are connected.
 * @param   type Device type to look for.
 * @return  The number of matching devices.
 */
uint32_t vexDevicesGetNumberByType(V5_DeviceType type);

/**
 * @brief   Returns a handle to the first entry in the device array.
 * @return  Handle to port index 0.
 * @details The remaining ports follow it in order, but using
 *          vexDeviceGetByIndex() is clearer and does not rely on the array
 *          layout.
 */
V5_DeviceT vexDevicesGet(void);

/**
 * @brief   Returns the handle for one device port.
 * @param   index Zero based port index, so 0 for smart port 1.
 * @return  Handle to that port.
 * @details The handle is valid even when nothing is plugged in, and stays
 *          valid if the device is unplugged and replaced. Check the port's
 *          type with vexDeviceGetStatus() before trusting readings from it.
 */
V5_DeviceT vexDeviceGetByIndex(uint32_t index);

/**
 * @brief   Reads the type of every device port into a buffer.
 * @param   buffer Array of at least #V5_MAX_DEVICE_PORTS entries.
 * @return  The number of entries written, or -1 if buffer is null.
 * @details This is the only way to tell what is currently plugged in, and
 * it is cheap enough to call every time a port is used.
 */
int32_t vexDeviceGetStatus(V5_DeviceType *buffer);

/**
 * @brief   Reads a device's internal timestamp.
 * @param   device Device handle.
 * @return  The device's timestamp in milliseconds.
 * @details Timestamps come from the device, not the brain, so comparing
 * them across two devices is not meaningful. Comparing successive readings
 *          from one device tells you whether its data has been refreshed.
 */
int32_t vexDeviceGetTimestamp(V5_DeviceT device);

/**
 * @brief   Reads a device's internal timestamp by port index.
 * @param   index Zero based port index.
 * @return  The device's timestamp in milliseconds.
 */
int32_t vexDeviceGetTimestampByIndex(int32_t index);

/**
 * @brief   Returns the state of the brain's front panel buttons.
 * @return  A value encoding which buttons are held.
 * @details VEX does not publish the bit layout.
 */
uint32_t vexDeviceButtonStateGet(void);

// This is used by the port index functions to map an index to the device
// pointer

/**
 * @brief   Declares a device handle for a port index.
 * @param   device Name of the handle variable to declare.
 * @param   index  Zero based port index.
 * @details Shorthand used by the port indexed wrappers in v5_apiuser.h.
 */
#define VEX_DEVICE_GET(device, index)                                          \
    V5_DeviceT device = vexDeviceGetByIndex(index)

// Controller

/**
 * @brief   Reads one channel of a controller.
 * @param   id    Which controller to read.
 * @param   index Channel to read.
 * @return  The channel value, or 0 if the controller is not connected.
 * @details Analog channels return -127 to 127, button channels return 0 or
 * 1, and the battery channels return a percentage. Because a disconnected
 * controller reads as 0 everywhere, use vexControllerConnectionStatusGet()
 * to tell a centred stick from an absent controller.
 */
int32_t vexControllerGet(V5_ControllerId id, V5_ControllerIndex index);

/**
 * @brief   Reports how a controller is connected.
 * @param   id Which controller to query.
 * @return  The connection status.
 */
V5_ControllerStatus vexControllerConnectionStatusGet(V5_ControllerId id);

/**
 * @brief   Writes text to a controller's LCD, or triggers a rumble pattern.
 * @param   id   Which controller to write to.
 * @param   line One based row, 1 to 3. Row 4 is the rumble channel.
 * @param   col  One based column, 1 to 20.
 * @param   str  Null terminated string, truncated past 31 characters.
 * @return  True if the request was accepted.
 * @details The screen is small and the link is slow, so VEXos rate limits
 * these updates. Writing more often than about once every 50 ms will start
 *          dropping requests, which is what a false return means.
 *
 *          Writing to row 4 does not print anything. The string is read as
 * a rumble pattern instead, built from dots for short pulses, dashes for
 *          long pulses and spaces for pauses.
 */
bool vexControllerTextSet(V5_ControllerId id, uint32_t line, uint32_t col,
                          const char *str);

// LED sensor

/**
 * @brief   Sets an LED module to one of the preset colors.
 * @param   device Device handle.
 * @param   value  Color to show.
 * @details The LED module was never sold and only exists on development
 *          systems.
 */
void vexDeviceLedSet(V5_DeviceT device, V5_DeviceLedColor value);

/**
 * @brief   Sets an LED module to an arbitrary color.
 * @param   device Device handle.
 * @param   color  Color as 0x00RRGGBB.
 */
void vexDeviceLedRgbSet(V5_DeviceT device, uint32_t color);

/**
 * @brief   Reads back an LED module's color as a preset.
 * @param   device Device handle.
 * @return  The current color.
 */
V5_DeviceLedColor vexDeviceLedGet(V5_DeviceT device);

/**
 * @brief   Reads back an LED module's color.
 * @param   device Device handle.
 * @return  The current color as 0x00RRGGBB.
 */
uint32_t vexDeviceLedRgbGet(V5_DeviceT device);

// ADI sensor

/**
 * @brief   Configures one 3-Wire channel.
 * @param   device Handle to the brain's 3-Wire ports or a 3-Wire Expander.
 * @param   port   Channel number, 0 to 7, for the ports labelled A to H.
 * @param   type   What the channel is wired to.
 * @details A channel has to be configured before it is read or written,
 * since the configuration is what decides how vexDeviceAdiValueGet() and
 *          vexDeviceAdiValueSet() interpret it.
 *
 *          Configuring a channel as #kAdiPortTypeQuadEncoder or
 *          #kAdiPortTypeSonar also claims the following channel, and the
 * pair has to start on an even numbered channel. Configuring a channel as
 *          #kAdiPortTypeLegacyGyro starts a calibration that takes roughly
 * a second, during which readings are not usable.
 */
void vexDeviceAdiPortConfigSet(V5_DeviceT device, uint32_t port,
                               V5_AdiPortConfiguration type);

/**
 * @brief   Reads back how a 3-Wire channel is configured.
 * @param   device Device handle.
 * @param   port   Channel number, 0 to 7.
 * @return  The channel's configuration.
 */
V5_AdiPortConfiguration vexDeviceAdiPortConfigGet(V5_DeviceT device,
                                                  uint32_t port);

/**
 * @brief   Writes a value to a 3-Wire channel.
 * @param   device Device handle.
 * @param   port   Channel number, 0 to 7.
 * @param   value  Value to write.
 * @details What the value means depends on the channel's configuration: 0
 * or 1 for a digital output, a PWM command for a legacy motor controller or
 *          servo, and the new count for an encoder being reset.
 */
void vexDeviceAdiValueSet(V5_DeviceT device, uint32_t port, int32_t value);

/**
 * @brief   Reads a 3-Wire channel.
 * @param   device Device handle.
 * @param   port   Channel number, 0 to 7.
 * @return  The channel's current reading.
 * @details What the value means depends on the channel's configuration: 0
 * or 1 for a digital input, 0 to 4095 for an analog input, accumulated
 *          ticks for an encoder, and a distance for a sonar.
 */
int32_t vexDeviceAdiValueGet(V5_DeviceT device, uint32_t port);

// Bumper switch

/**
 * @brief   Reads a smart port bumper switch.
 * @param   device Device handle.
 * @return  Whether the switch is pressed.
 * @details This is the smart port bumper from the development systems, not
 * the 3-Wire bumper, which is read through the ADI functions.
 */
V5_DeviceBumperState vexDeviceBumperGet(V5_DeviceT device);

// Gyro - obsolete, there is currently no dedicated gyro

/**
 * @brief   Restarts calibration on a dedicated gyro.
 * @param   device Device handle.
 * @details Obsolete. There is no dedicated smart port gyro; use the
 * Inertial Sensor functions instead.
 */
void vexDeviceGyroReset(V5_DeviceT device);

/**
 * @brief   Reads a dedicated gyro's accumulated rotation.
 * @param   device Device handle.
 * @return  The rotation in degrees, unbounded.
 * @details Obsolete. See vexDeviceImuHeadingGet().
 */
double vexDeviceGyroHeadingGet(V5_DeviceT device);

/**
 * @brief   Reads a dedicated gyro's heading.
 * @param   device Device handle.
 * @return  The heading in degrees, 0 through 360.
 * @details Obsolete. See vexDeviceImuDegreesGet().
 */
double vexDeviceGyroDegreesGet(V5_DeviceT device);

// Sonar - obsolete, there is currently no dedicated gyro

/**
 * @brief   Reads a dedicated smart port sonar.
 * @param   device Device handle.
 * @return  The measured distance.
 * @details Obsolete. Use the Distance Sensor, or a 3-Wire ultrasonic
 *          configured as #kAdiPortTypeSonar.
 */
int32_t vexDeviceSonarValueGet(V5_DeviceT device);

// Generic sensor - (who knows !)

/**
 * @brief   Reads a device VEXos classifies as a generic sensor.
 * @param   device Device handle.
 * @return  The sensor's reading, or 0 if the device is not a generic
 * sensor.
 * @details This is not a way to read any device with one call. A device
 * only responds here if VEXos reports it as #kDeviceTypeGenericSensor.
 */
int32_t vexDeviceGenericValueGet(V5_DeviceT device);

// Motor

/**
 * @brief   Spins a motor at a target velocity.
 * @param   device   Device handle.
 * @param   velocity Target velocity in RPM at the output shaft.
 * @details Switches the motor into #kMotorControlModeVELOCITY and hands
 * control to its internal velocity loop. The usable range depends on the
 *          cartridge: roughly 100 RPM for 36:1, 200 RPM for 18:1 and 600
 * RPM for 6:1. Setting a velocity of 0 actively holds the motor still
 *          rather than letting it coast.
 */
void vexDeviceMotorVelocitySet(V5_DeviceT device, int32_t velocity);

/**
 * @brief   Changes the velocity limit of a motion already in progress.
 * @param   device   Device handle.
 * @param   velocity New velocity limit in RPM.
 * @details Only affects a profiled move started by
 *          vexDeviceMotorAbsoluteTargetSet() or
 *          vexDeviceMotorRelativeTargetSet(). The target position is left
 *          alone.
 */
void vexDeviceMotorVelocityUpdate(V5_DeviceT device, int32_t velocity);

/**
 * @brief   Drives a motor open loop at a given voltage.
 * @param   device Device handle.
 * @param   value  Voltage in millivolts, -12000 to 12000.
 * @details Bypasses the internal control loops entirely. The motor's actual
 *          speed then depends on its load, which is usually what you want
 * when driving it from a joystick.
 */
void vexDeviceMotorVoltageSet(V5_DeviceT device, int32_t value);

/**
 * @brief   Reads back a motor's commanded velocity.
 * @param   device Device handle.
 * @return  The target velocity in RPM.
 * @details This is the commanded value, not the measured one. For the
 * measured speed use vexDeviceMotorActualVelocityGet().
 */
int32_t vexDeviceMotorVelocityGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's measured velocity.
 * @param   device Device handle.
 * @return  The measured velocity in RPM at the output shaft.
 */
double vexDeviceMotorActualVelocityGet(V5_DeviceT device);

/**
 * @brief   Reads which way a motor is turning.
 * @param   device Device handle.
 * @return  1 when turning forwards, -1 when turning backwards.
 */
int32_t vexDeviceMotorDirectionGet(V5_DeviceT device);

/**
 * @brief   Sets a motor's control mode directly.
 * @param   device Device handle.
 * @param   mode   Control mode to switch to.
 * @details The target setting functions already select the mode they need,
 * so this is mostly useful for stopping a motor in a chosen state.
 */
void vexDeviceMotorModeSet(V5_DeviceT device, V5MotorControlMode mode);

/**
 * @brief   Reads a motor's current control mode.
 * @param   device Device handle.
 * @return  The control mode.
 */
V5MotorControlMode vexDeviceMotorModeGet(V5_DeviceT device);

/**
 * @brief   Sets a motor's raw PWM output.
 * @param   device Device handle.
 * @param   value  PWM command.
 * @details The lowest level way to drive a motor. VEX does not publish the
 *          accepted range, and both VEXcode and PROS drive motors through
 *          voltage or velocity instead.
 */
void vexDeviceMotorPwmSet(V5_DeviceT device, int32_t value);

/**
 * @brief   Reads back a motor's raw PWM output.
 * @param   device Device handle.
 * @return  The current PWM command.
 */
int32_t vexDeviceMotorPwmGet(V5_DeviceT device);

/**
 * @brief   Sets the current at which a motor starts backing off.
 * @param   device Device handle.
 * @param   value  Current limit in milliamps.
 * @details The default is 2500 mA per motor. Lowering it protects a
 * mechanism that can stall; raising it above the default is not useful
 * because the brain enforces its own total budget across all motors.
 */
void vexDeviceMotorCurrentLimitSet(V5_DeviceT device, int32_t value);

/**
 * @brief   Reads back a motor's current limit.
 * @param   device Device handle.
 * @return  The current limit in milliamps.
 */
int32_t vexDeviceMotorCurrentLimitGet(V5_DeviceT device);

/**
 * @brief   Caps the voltage a motor's control loops may command.
 * @param   device Device handle.
 * @param   value  Voltage limit in millivolts.
 * @details Applies to closed loop moves. It does not clamp a voltage passed
 *          straight to vexDeviceMotorVoltageSet().
 */
void vexDeviceMotorVoltageLimitSet(V5_DeviceT device, int32_t value);

/**
 * @brief   Reads back a motor's voltage limit.
 * @param   device Device handle.
 * @return  The voltage limit in millivolts.
 */
int32_t vexDeviceMotorVoltageLimitGet(V5_DeviceT device);

/**
 * @brief   Replaces the gains in a motor's position control loop.
 * @param   device Device handle.
 * @param   pid    Gain set to install.
 * @details VEX has never published the scaling of the terms or the motor's
 *          default values, so tuning these is empirical. Leaving them alone
 * is the usual choice.
 */
void vexDeviceMotorPositionPidSet(V5_DeviceT device, V5_DeviceMotorPid *pid);

/**
 * @brief   Replaces the gains in a motor's velocity control loop.
 * @param   device Device handle.
 * @param   pid    Gain set to install.
 * @details The same caveats as vexDeviceMotorPositionPidSet() apply.
 */
void vexDeviceMotorVelocityPidSet(V5_DeviceT device, V5_DeviceMotorPid *pid);

/**
 * @brief   Reads a motor's current draw.
 * @param   device Device handle.
 * @return  The current in milliamps.
 */
int32_t vexDeviceMotorCurrentGet(V5_DeviceT device);

/**
 * @brief   Reads the voltage a motor is applying.
 * @param   device Device handle.
 * @return  The voltage in millivolts.
 */
int32_t vexDeviceMotorVoltageGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's mechanical power output.
 * @param   device Device handle.
 * @return  The power in watts.
 */
double vexDeviceMotorPowerGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's output torque.
 * @param   device Device handle.
 * @return  The torque in newton meters.
 */
double vexDeviceMotorTorqueGet(V5_DeviceT device);

/**
 * @brief   Reads how efficiently a motor is running.
 * @param   device Device handle.
 * @return  The efficiency as a percentage.
 * @details 100 percent means the motor is turning while drawing no
 * electrical power, which happens when it is spinning freely. A stalled
 * motor reads 0 percent.
 */
double vexDeviceMotorEfficiencyGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's internal temperature.
 * @param   device Device handle.
 * @return  The temperature in degrees Celsius.
 * @details The motor reports in steps of about 5 degrees, so the value
 * moves in jumps rather than smoothly.
 */
double vexDeviceMotorTemperatureGet(V5_DeviceT device);

/**
 * @brief   Reports whether a motor is too hot.
 * @param   device Device handle.
 * @return  True when the motor has started limiting its output because of
 *          temperature.
 */
bool vexDeviceMotorOverTempFlagGet(V5_DeviceT device);

/**
 * @brief   Reports whether a motor is hitting its current limit.
 * @param   device Device handle.
 * @return  True when the motor is limiting its output because of current.
 */
bool vexDeviceMotorCurrentLimitFlagGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's fault bits.
 * @param   device Device handle.
 * @return  A bit field of faults: 0x01 motor over temperature, 0x02
 * h-bridge fault, 0x04 over current, 0x08 h-bridge over current. Zero means
 * no faults.
 */
uint32_t vexDeviceMotorFaultsGet(V5_DeviceT device);

/**
 * @brief   Reports whether a motor is stopped.
 * @param   device Device handle.
 * @return  True when the motor's measured velocity is zero.
 */
bool vexDeviceMotorZeroVelocityFlagGet(V5_DeviceT device);

/**
 * @brief   Reports whether a motor is at its zero position.
 * @param   device Device handle.
 * @return  True when the motor's position reading is zero.
 */
bool vexDeviceMotorZeroPositionFlagGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's status bits.
 * @param   device Device handle.
 * @return  A bit field: 0x01 busy, 0x02 zero velocity, 0x04 zero position.
 *          Zero means none of those apply.
 */
uint32_t vexDeviceMotorFlagsGet(V5_DeviceT device);

/**
 * @brief   Reverses the sense of a motor.
 * @param   device Device handle.
 * @param   value  True to reverse.
 * @details Flips the direction of both the commands sent to the motor and
 * the position and velocity read back from it, so a mechanism mounted
 *          backwards can be treated as if it were not.
 */
void vexDeviceMotorReverseFlagSet(V5_DeviceT device, bool value);

/**
 * @brief   Reads back a motor's reverse flag.
 * @param   device Device handle.
 * @return  True if the motor is reversed.
 */
bool vexDeviceMotorReverseFlagGet(V5_DeviceT device);

/**
 * @brief   Chooses the units a motor reports position in.
 * @param   device Device handle.
 * @param   units  Units to use.
 */
void vexDeviceMotorEncoderUnitsSet(V5_DeviceT device,
                                   V5MotorEncoderUnits units);

/**
 * @brief   Reads back a motor's position units.
 * @param   device Device handle.
 * @return  The units in use.
 */
V5MotorEncoderUnits vexDeviceMotorEncoderUnitsGet(V5_DeviceT device);

/**
 * @brief   Chooses what a motor does when it stops being commanded.
 * @param   device Device handle.
 * @param   mode   Coast, brake or hold.
 */
void vexDeviceMotorBrakeModeSet(V5_DeviceT device, V5MotorBrakeMode mode);

/**
 * @brief   Reads back a motor's brake mode.
 * @param   device Device handle.
 * @return  The brake mode in use.
 */
V5MotorBrakeMode vexDeviceMotorBrakeModeGet(V5_DeviceT device);

/**
 * @brief   Redefines a motor's current position.
 * @param   device   Device handle.
 * @param   position Value the current position should read as, in the
 *                   configured encoder units.
 * @details Shifts the reported position without moving the motor. Absolute
 *          targets set afterwards are measured against the new reference.
 */
void vexDeviceMotorPositionSet(V5_DeviceT device, double position);

/**
 * @brief   Reads a motor's position.
 * @param   device Device handle.
 * @return  The position in the configured encoder units.
 */
double vexDeviceMotorPositionGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's raw encoder count along with its timestamp.
 * @param   device    Device handle.
 * @param   timestamp Filled in with the time the count was captured, in
 *                    milliseconds. May not be null.
 * @return  The raw encoder count, ignoring the configured encoder units.
 * @details Reading the count and its timestamp together lets you difference
 * two samples without guessing how stale either one is.
 */
int32_t vexDeviceMotorPositionRawGet(V5_DeviceT device, uint32_t *timestamp);

/**
 * @brief   Zeroes a motor's position reading.
 * @param   device Device handle.
 * @details Equivalent to vexDeviceMotorPositionSet() with a position of 0.
 */
void vexDeviceMotorPositionReset(V5_DeviceT device);

/**
 * @brief   Reads the position a motor is currently driving towards.
 * @param   device Device handle.
 * @return  The target position in the configured encoder units.
 */
double vexDeviceMotorTargetGet(V5_DeviceT device);

/**
 * @brief   Sends a motor to a position and holds it there.
 * @param   device   Device handle.
 * @param   position Target position in the configured encoder units.
 * @details Switches the motor into #kMotorControlModeSERVO. Unlike the
 * profiled moves, this keeps correcting once the target is reached.
 */
void vexDeviceMotorServoTargetSet(V5_DeviceT device, double position);

/**
 * @brief   Starts a profiled move to an absolute position.
 * @param   device   Device handle.
 * @param   position Target position in the configured encoder units.
 * @param   velocity Velocity limit for the move, in RPM.
 * @details Switches the motor into #kMotorControlModePROFILE. The call
 * returns straight away; poll vexDeviceMotorPositionGet() or the zero
 * velocity flag to find out when the move has finished. The motor stops at
 * the target rather than holding it.
 */
void vexDeviceMotorAbsoluteTargetSet(V5_DeviceT device, double position,
                                     int32_t velocity);

/**
 * @brief   Starts a profiled move relative to the current position.
 * @param   device   Device handle.
 * @param   position Distance to travel in the configured encoder units.
 * @param   velocity Velocity limit for the move, in RPM.
 * @details Behaves like vexDeviceMotorAbsoluteTargetSet(), except the
 * target is worked out from where the motor is when the call is made.
 */
void vexDeviceMotorRelativeTargetSet(V5_DeviceT device, double position,
                                     int32_t velocity);

/**
 * @brief   Tells a motor which gear cartridge is fitted.
 * @param   device Device handle.
 * @param   value  Cartridge ratio.
 * @details VEXos needs this to convert encoder counts to degrees and to
 * know the motor's free speed. Getting it wrong makes every position and
 *          velocity reading wrong by a constant factor.
 */
void vexDeviceMotorGearingSet(V5_DeviceT device, V5MotorGearset value);

/**
 * @brief   Reads back the cartridge a motor is configured for.
 * @param   device Device handle.
 * @return  The configured cartridge ratio.
 */
V5MotorGearset vexDeviceMotorGearingGet(V5_DeviceT device);

/**
 * @brief   Feeds a motor one point of an externally generated motion
 * profile.
 * @param   device   Device handle.
 * @param   position Position setpoint in the configured encoder units.
 * @param   velocity Velocity setpoint in RPM.
 * @details Lets user code generate the trajectory and leave only the low
 * level tracking to the motor. VEX does not publish the expected update
 * rate or how the motor behaves if points stop arriving.
 */
void vexDeviceMotorExternalProfileSet(V5_DeviceT device, double position,
                                      int32_t velocity);

// Vision sensor

/**
 * @brief   Chooses what the Vision Sensor looks for.
 * @param   device Device handle.
 * @param   mode   Detection mode.
 */
void vexDeviceVisionModeSet(V5_DeviceT device, V5VisionMode mode);

/**
 * @brief   Reads back the Vision Sensor's detection mode.
 * @param   device Device handle.
 * @return  The current mode.
 */
V5VisionMode vexDeviceVisionModeGet(V5_DeviceT device);

/**
 * @brief   Counts the objects the Vision Sensor is currently reporting.
 * @param   device Device handle.
 * @return  The number of objects available.
 * @details Call this before vexDeviceVisionObjectGet(), since the count
 * changes from frame to frame. The sensor reports objects largest first.
 */
int32_t vexDeviceVisionObjectCountGet(V5_DeviceT device);

/**
 * @brief   Reads one detected object.
 * @param   device   Device handle.
 * @param   indexObj Zero based object index, below the current object
 * count.
 * @param   pObject  Structure filled in with the object's data.
 * @return  The number of objects copied, so 1 on success and 0 if the index
 * is out of range.
 */
int32_t vexDeviceVisionObjectGet(V5_DeviceT device, uint32_t indexObj,
                                 V5_DeviceVisionObject *pObject);

/**
 * @brief   Writes a color signature to the Vision Sensor.
 * @param   device     Device handle.
 * @param   pSignature Signature to store, with its id set to the target
 * slot.
 * @details Signatures live in volatile memory, so they have to be written
 * again every time the sensor powers up.
 */
void vexDeviceVisionSignatureSet(V5_DeviceT device,
                                 V5_DeviceVisionSignature *pSignature);

/**
 * @brief   Reads a color signature back from the Vision Sensor.
 * @param   device     Device handle.
 * @param   id         Signature slot, 1 to 7.
 * @param   pSignature Structure filled in with the stored signature.
 * @return  True if the signature was read.
 * @details The read completes asynchronously. #VISION_SIG_FLAG_STATUS in
 * the flags field indicates the returned data is valid.
 */
bool vexDeviceVisionSignatureGet(V5_DeviceT device, uint32_t id,
                                 V5_DeviceVisionSignature *pSignature);

/**
 * @brief   Sets the Vision Sensor's camera brightness.
 * @param   device  Device handle.
 * @param   percent Brightness, 0 to 100.
 */
void vexDeviceVisionBrightnessSet(V5_DeviceT device, uint8_t percent);

/**
 * @brief   Reads back the Vision Sensor's camera brightness.
 * @param   device Device handle.
 * @return  The brightness, 0 to 100.
 */
uint8_t vexDeviceVisionBrightnessGet(V5_DeviceT device);

/**
 * @brief   Chooses how the Vision Sensor white balances.
 * @param   device Device handle.
 * @param   mode   White balance mode.
 */
void vexDeviceVisionWhiteBalanceModeSet(V5_DeviceT device, V5VisionWBMode mode);

/**
 * @brief   Reads back the Vision Sensor's white balance mode.
 * @param   device Device handle.
 * @return  The current mode.
 */
V5VisionWBMode vexDeviceVisionWhiteBalanceModeGet(V5_DeviceT device);

/**
 * @brief   Sets the manual white balance reference color.
 * @param   device Device handle.
 * @param   color  Color that should be treated as white.
 * @details Only used while the sensor is in #kVisionWBManual.
 */
void vexDeviceVisionWhiteBalanceSet(V5_DeviceT device,
                                    V5_DeviceVisionRgb color);

/**
 * @brief   Reads back the manual white balance reference color.
 * @param   device Device handle.
 * @return  The reference color.
 */
V5_DeviceVisionRgb vexDeviceVisionWhiteBalanceGet(V5_DeviceT device);

/**
 * @brief   Chooses who drives the Vision Sensor's LED.
 * @param   device Device handle.
 * @param   mode   Automatic or manual.
 */
void vexDeviceVisionLedModeSet(V5_DeviceT device, V5VisionLedMode mode);

/**
 * @brief   Reads back who drives the Vision Sensor's LED.
 * @param   device Device handle.
 * @return  The current mode.
 */
V5VisionLedMode vexDeviceVisionLedModeGet(V5_DeviceT device);

/**
 * @brief   Sets the Vision Sensor's LED brightness.
 * @param   device  Device handle.
 * @param   percent Brightness, 0 to 100.
 * @note    The function name contains a typo that is kept for source
 *          compatibility.
 */
void vexDeviceVisionLedBrigntnessSet(V5_DeviceT device, uint8_t percent);

/**
 * @brief   Reads back the Vision Sensor's LED brightness.
 * @param   device Device handle.
 * @return  The brightness, 0 to 100.
 * @note    The function name contains a typo that is kept for source
 *          compatibility.
 */
uint8_t vexDeviceVisionLedBrigntnessGet(V5_DeviceT device);

/**
 * @brief   Sets the Vision Sensor's LED color.
 * @param   device Device handle.
 * @param   color  Color to show.
 * @details Only used while the LED is in #kVisionLedModeManual.
 */
void vexDeviceVisionLedColorSet(V5_DeviceT device, V5_DeviceVisionRgb color);

/**
 * @brief   Reads back the Vision Sensor's LED color.
 * @param   device Device handle.
 * @return  The LED color.
 */
V5_DeviceVisionRgb vexDeviceVisionLedColorGet(V5_DeviceT device);

/**
 * @brief   Turns the Vision Sensor's onboard access point on or off.
 * @param   device Device handle.
 * @param   mode   On or off.
 * @details With the access point on, a browser can view the camera feed,
 * but the sensor stops reporting objects. Leave it off during a match.
 */
void vexDeviceVisionWifiModeSet(V5_DeviceT device, V5VisionWifiMode mode);

/**
 * @brief   Reads back the Vision Sensor's access point state.
 * @param   device Device handle.
 * @return  The current mode.
 */
V5VisionWifiMode vexDeviceVisionWifiModeGet(V5_DeviceT device);

// IMU

/**
 * @brief   Starts calibrating the Inertial Sensor.
 * @param   device Device handle.
 * @details Returns immediately. Calibration takes roughly two seconds, and
 * the sensor has to be still for all of it or the gyro bias it measures
 *          will be wrong. Watch bit 0 of vexDeviceImuStatusGet() to tell
 * when it has finished, and note that the bit is not set the instant this
 *          call returns, so wait for it to go high before waiting for it to
 * go low again. Readings taken during calibration are meaningless.
 */
void vexDeviceImuReset(V5_DeviceT device);

/**
 * @brief   Reads the Inertial Sensor's accumulated rotation about the z
 * axis.
 * @param   device Device handle.
 * @return  The rotation in degrees, unbounded, clockwise positive.
 * @details Keeps counting past 360, so it can be used directly as the error
 *          term of a turn controller without worrying about wrapping.
 */
double vexDeviceImuHeadingGet(V5_DeviceT device);

/**
 * @brief   Reads the Inertial Sensor's heading.
 * @param   device Device handle.
 * @return  The heading in degrees, 0 through 360, clockwise positive.
 */
double vexDeviceImuDegreesGet(V5_DeviceT device);

/**
 * @brief   Reads the Inertial Sensor's orientation as a quaternion.
 * @param   device Device handle.
 * @param   data   Structure filled in with the quaternion.
 */
void vexDeviceImuQuaternionGet(V5_DeviceT device, V5_DeviceImuQuaternion *data);

/**
 * @brief   Reads the Inertial Sensor's orientation as Euler angles.
 * @param   device Device handle.
 * @param   data   Structure filled in with pitch, roll and yaw in degrees,
 *                 each bounded to -180 through 180.
 */
void vexDeviceImuAttitudeGet(V5_DeviceT device, V5_DeviceImuAttitude *data);

/**
 * @brief   Reads the Inertial Sensor's raw gyroscope rates.
 * @param   device Device handle.
 * @param   data   Structure filled in with x, y and z in degrees per
 * second. The w member is left untouched.
 */
void vexDeviceImuRawGyroGet(V5_DeviceT device, V5_DeviceImuRaw *data);

/**
 * @brief   Reads the Inertial Sensor's raw accelerations.
 * @param   device Device handle.
 * @param   data   Structure filled in with x, y and z in g. The w member is
 *                 left untouched.
 */
void vexDeviceImuRawAccelGet(V5_DeviceT device, V5_DeviceImuRaw *data);

/**
 * @brief   Reads the Inertial Sensor's status bits.
 * @param   device Device handle.
 * @return  A bit field: bit 0 is set while calibrating, bits 1 to 3 hold
 * the detected physical orientation, and bit 4 is set if the sensor
 *          calibrated itself automatically.
 */
uint32_t vexDeviceImuStatusGet(V5_DeviceT device);

/**
 * @brief   Sets the Inertial Sensor's mode word.
 * @param   device Device handle.
 * @param   mode   Bitwise OR of a #V5ImuOrientationMode, a
 *                 #V5ImuQuaternionMode and a #_V5ImuHeadingnMode value.
 */
void vexDeviceImuModeSet(V5_DeviceT device, uint32_t mode);

/**
 * @brief   Reads back the Inertial Sensor's mode word.
 * @param   device Device handle.
 * @return  The mode word.
 */
uint32_t vexDeviceImuModeGet(V5_DeviceT device);

/**
 * @brief   Sets how often the Inertial Sensor produces new readings.
 * @param   device Device handle.
 * @param   rate   Interval in milliseconds, a multiple of 5 and at least 5.
 *                 The default is 10.
 * @details VEXos still only copies readings into its shared buffer every 10
 * ms, so a shorter interval does not let you poll faster. What it does buy
 *          you is fresher data whenever you do poll.
 */
void vexDeviceImuDataRateSet(V5_DeviceT device, uint32_t rate);

// Rangefinder/Lidar - actual API to be determined

/**
 * @brief   Reads a rangefinder or lidar device.
 * @param   device Device handle.
 * @return  The measured distance.
 * @details This hardware was never sold. The original header notes the API
 * is still to be determined.
 */
int32_t vexDeviceRangeValueGet(V5_DeviceT device);

// Absolute encoder

/**
 * @brief   Resets the Rotation Sensor's position.
 * @param   device Device handle.
 * @details This does not zero the position. It sets the position to the
 *          sensor's current absolute angle, which is somewhere in 0 through
 *          360 degrees. To actually zero it, call
 *          vexDeviceAbsEncPositionSet() with 0.
 */
void vexDeviceAbsEncReset(V5_DeviceT device);

/**
 * @brief   Redefines the Rotation Sensor's position.
 * @param   device   Device handle.
 * @param   position Value the current position should read as, in
 *                   centidegrees.
 */
void vexDeviceAbsEncPositionSet(V5_DeviceT device, int32_t position);

/**
 * @brief   Reads the Rotation Sensor's accumulated position.
 * @param   device Device handle.
 * @return  The position in centidegrees, so 36000 per turn. Unbounded, and
 *          signed according to the direction of travel.
 */
int32_t vexDeviceAbsEncPositionGet(V5_DeviceT device);

/**
 * @brief   Reads the Rotation Sensor's speed.
 * @param   device Device handle.
 * @return  The speed in centidegrees per second.
 */
int32_t vexDeviceAbsEncVelocityGet(V5_DeviceT device);

/**
 * @brief   Reads the Rotation Sensor's absolute angle.
 * @param   device Device handle.
 * @return  The angle in centidegrees, 0 through 35999.
 * @details Unlike the position, this wraps at a full turn and survives a
 * power cycle, because the sensor measures the shaft's absolute angle.
 */
int32_t vexDeviceAbsEncAngleGet(V5_DeviceT device);

/**
 * @brief   Reverses the Rotation Sensor's direction.
 * @param   device Device handle.
 * @param   value  True to reverse.
 */
void vexDeviceAbsEncReverseFlagSet(V5_DeviceT device, bool value);

/**
 * @brief   Reads back the Rotation Sensor's reverse flag.
 * @param   device Device handle.
 * @return  True if the sensor is reversed.
 */
bool vexDeviceAbsEncReverseFlagGet(V5_DeviceT device);

/**
 * @brief   Reads the Rotation Sensor's status bits.
 * @param   device Device handle.
 * @return  The status word. It reads 0 until the sensor has finished
 * starting up, which is the usual way to wait for it to become ready. VEX
 * does not publish the meaning of the individual bits.
 */
uint32_t vexDeviceAbsEncStatusGet(V5_DeviceT device);

/**
 * @brief   Sets how often the Rotation Sensor produces new readings.
 * @param   device Device handle.
 * @param   rate   Interval in milliseconds, a multiple of 5 and at least 5.
 *                 The default is 10.
 */
void vexDeviceAbsEncDataRateSet(V5_DeviceT device, uint32_t rate);

// Optical/color sensor

/**
 * @brief   Reads the hue of the color in front of the Optical Sensor.
 * @param   device Device handle.
 * @return  The hue in degrees, 0 through 360.
 * @details Not updated while gesture detection is enabled.
 */
double vexDeviceOpticalHueGet(V5_DeviceT device);

/**
 * @brief   Reads the saturation of the color in front of the Optical
 * Sensor.
 * @param   device Device handle.
 * @return  The saturation, 0 through 1.
 * @details Not updated while gesture detection is enabled.
 */
double vexDeviceOpticalSatGet(V5_DeviceT device);

/**
 * @brief   Reads how much light reaches the Optical Sensor.
 * @param   device Device handle.
 * @return  The brightness, 0 through 1.
 * @details Not updated while gesture detection is enabled.
 */
double vexDeviceOpticalBrightnessGet(V5_DeviceT device);

/**
 * @brief   Reads the Optical Sensor's proximity reading.
 * @param   device Device handle.
 * @return  The proximity, 0 through 255, larger meaning closer.
 * @details This is a measure of reflected infrared light, not a distance,
 * so a dark object reads as further away than a light one at the same
 *          range. Not updated while gesture detection is enabled.
 */
int32_t vexDeviceOpticalProximityGet(V5_DeviceT device);

/**
 * @brief   Reads the Optical Sensor's processed color.
 * @param   device Device handle.
 * @param   data   Structure filled in with the color and brightness.
 */
void vexDeviceOpticalRgbGet(V5_DeviceT device, V5_DeviceOpticalRgb *data);

/**
 * @brief   Sets the brightness of the Optical Sensor's white LED.
 * @param   device Device handle.
 * @param   value  Brightness, 0 to 100.
 * @details The LED lights whatever the sensor is looking at, so colors read
 *          much more consistently with it on.
 */
void vexDeviceOpticalLedPwmSet(V5_DeviceT device, int32_t value);

/**
 * @brief   Reads back the Optical Sensor's LED brightness.
 * @param   device Device handle.
 * @return  The brightness, 0 to 100.
 */
int32_t vexDeviceOpticalLedPwmGet(V5_DeviceT device);

/**
 * @brief   Reads the Optical Sensor's status bits.
 * @param   device Device handle.
 * @return  The status word. VEX does not publish the meaning of the
 * individual bits.
 */
uint32_t vexDeviceOpticalStatusGet(V5_DeviceT device);

/**
 * @brief   Reads the Optical Sensor's unprocessed channel counts.
 * @param   device Device handle.
 * @param   data   Structure filled in with the clear, red, green and blue
 *                 counts.
 * @details The magnitudes depend on the integration time and the available
 *          light, so these are most useful as ratios.
 */
void vexDeviceOpticalRawGet(V5_DeviceT device, V5_DeviceOpticalRaw *data);

/**
 * @brief   Sets the Optical Sensor's operating mode.
 * @param   device Device handle.
 * @param   mode   Mode value.
 * @details VEX does not publish the mode values. Gesture detection, the one
 *          mode switch user code normally needs, has its own calls in
 *          vexDeviceOpticalGestureEnable() and
 *          vexDeviceOpticalGestureDisable().
 */
void vexDeviceOpticalModeSet(V5_DeviceT device, uint32_t mode);

/**
 * @brief   Reads back the Optical Sensor's operating mode.
 * @param   device Device handle.
 * @return  The mode value.
 */
uint32_t vexDeviceOpticalModeGet(V5_DeviceT device);

/**
 * @brief   Reads the most recent gesture from the Optical Sensor.
 * @param   device Device handle.
 * @param   pData  Structure filled in with the raw gesture data, or null if
 *                 only the direction is wanted.
 * @return  The recognised direction: 0 none, 1 up, 2 down, 3 right, 4 left.
 * @details Only meaningful while gesture detection is enabled. A gesture is
 *          cleared about 500 ms after it is recognised, so it has to be
 * polled reasonably often to be caught.
 */
uint32_t vexDeviceOpticalGestureGet(V5_DeviceT, V5_DeviceOpticalGesture *pData);

/**
 * @brief   Enables gesture detection on the Optical Sensor.
 * @param   device Device handle.
 * @details The hue, saturation, brightness and proximity readings stop
 *          updating while gesture detection is on.
 */
void vexDeviceOpticalGestureEnable(V5_DeviceT);

/**
 * @brief   Disables gesture detection on the Optical Sensor.
 * @param   device Device handle.
 */
void vexDeviceOpticalGestureDisable(V5_DeviceT);

/**
 * @brief   Sets the proximity threshold gesture detection triggers at.
 * @param   device Device handle.
 * @param   value  Threshold, on the same 0 to 255 scale as
 *                 vexDeviceOpticalProximityGet().
 * @return  VEX does not document the return value.
 */
int32_t vexDeviceOpticalProximityThreshold(V5_DeviceT device, int32_t value);

/**
 * @brief   Sets how long the Optical Sensor integrates light for.
 * @param   device Device handle.
 * @param   timeMs Integration time in milliseconds, 3 through 712.
 * @details This is the trade between update rate and accuracy. A short
 *          integration time gives fresher readings but collects less light,
 * so colors become noisier.
 */
void vexDeviceOpticalIntegrationTimeSet(V5_DeviceT device, double timeMs);

/**
 * @brief   Reads back the Optical Sensor's integration time.
 * @param   device Device handle.
 * @return  The integration time in milliseconds.
 */
double vexDeviceOpticalIntegrationTimeGet(V5_DeviceT device);

// Electro magnet

/**
 * @brief   Energises the Workcell electromagnet.
 * @param   device Device handle.
 * @param   value  Power level. Positive attracts, negative repels.
 * @param   time   How long to stay energised, in milliseconds.
 * @details The magnet cannot be left on indefinitely without overheating,
 *          which is why the duration is part of the command rather than
 *          something the caller has to time.
 */
void vexDeviceMagnetPowerSet(V5_DeviceT device, int32_t value, int32_t time);

/**
 * @brief   Reads back the electromagnet's power level.
 * @param   device Device handle.
 * @return  The power level.
 */
int32_t vexDeviceMagnetPowerGet(V5_DeviceT device);

/**
 * @brief   Runs the electromagnet's pickup sequence.
 * @param   device   Device handle.
 * @param   duration How long to energise for.
 */
void vexDeviceMagnetPickup(V5_DeviceT device, V5_DeviceMagnetDuration duration);

/**
 * @brief   Runs the electromagnet's drop sequence.
 * @param   device   Device handle.
 * @param   duration How long to energise for.
 */
void vexDeviceMagnetDrop(V5_DeviceT device, V5_DeviceMagnetDuration duration);

/**
 * @brief   Reads the electromagnet's temperature.
 * @param   device Device handle.
 * @return  The temperature in degrees Celsius.
 */
double vexDeviceMagnetTemperatureGet(V5_DeviceT device);

/**
 * @brief   Reads the electromagnet's current draw.
 * @param   device Device handle.
 * @return  The current in amps.
 */
double vexDeviceMagnetCurrentGet(V5_DeviceT device);

/**
 * @brief   Reads the electromagnet's status bits.
 * @param   device Device handle.
 * @return  The status word. VEX does not publish the meaning of the
 * individual bits.
 */
uint32_t vexDeviceMagnetStatusGet(V5_DeviceT device);

// Distance

/**
 * @brief   Reads the distance to the nearest object.
 * @param   device Device handle.
 * @return  The distance in millimetres.
 * @details The sensor is specified from 20 mm to 2000 mm, accurate to about
 *          15 mm below 200 mm and about 5 percent above it. Check
 *          vexDeviceDistanceStatusGet() before trusting the reading, since
 * the sensor reports a value even while it is starting up.
 */
uint32_t vexDeviceDistanceDistanceGet(V5_DeviceT device);

/**
 * @brief   Reads how much the sensor trusts its distance reading.
 * @param   device Device handle.
 * @return  The confidence, 0 through 63, larger being better.
 * @details Only meaningful beyond 200 mm. Closer than that the sensor
 * returns a fixed value of 10.
 */
uint32_t vexDeviceDistanceConfidenceGet(V5_DeviceT device);

/**
 * @brief   Reads the apparent size of the detected object.
 * @param   device Device handle.
 * @return  A unitless size, roughly 0 through 400.
 * @details Larger and more reflective objects read bigger, so this cannot
 * be turned into a physical size without knowing the object's material and
 * its range.
 */
int32_t vexDeviceDistanceObjectSizeGet(V5_DeviceT device);

/**
 * @brief   Reads how fast the detected object is approaching or receding.
 * @param   device Device handle.
 * @return  The velocity in meters per second.
 * @details Derived from the change in distance over time, so it is only as
 * good as the distance reading it comes from.
 */
double vexDeviceDistanceObjectVelocityGet(V5_DeviceT device);

/**
 * @brief   Reads the Distance Sensor's status bits.
 * @param   device Device handle.
 * @return  The status word. It reads 0 while the sensor is still
 * initialising. VEX does not publish the meaning of the individual bits;
 * PROS treats 0x82 and 0x86 as the working states.
 */
uint32_t vexDeviceDistanceStatusGet(V5_DeviceT device);

// Gps

/**
 * @brief   Starts calibrating the GPS Sensor's onboard inertial sensor.
 * @param   device Device handle.
 * @details Only the inertial part needs calibrating. Position comes from
 * the camera reading the field perimeter pattern, so it is absolute and
 *          does not drift.
 */
void vexDeviceGpsReset(V5_DeviceT device);

/**
 * @brief   Reads the GPS Sensor's accumulated rotation about the z axis.
 * @param   device Device handle.
 * @return  The rotation in degrees, unbounded.
 */
double vexDeviceGpsHeadingGet(V5_DeviceT device);

/**
 * @brief   Reads the GPS Sensor's heading.
 * @param   device Device handle.
 * @return  The heading in degrees, 0 through 360, with 0 pointing at the
 * far side of the field and increasing clockwise.
 */
double vexDeviceGpsDegreesGet(V5_DeviceT device);

/**
 * @brief   Reads the GPS Sensor's orientation as a quaternion.
 * @param   device Device handle.
 * @param   data   Structure filled in with the quaternion.
 */
void vexDeviceGpsQuaternionGet(V5_DeviceT device, V5_DeviceGpsQuaternion *data);

/**
 * @brief   Reads the GPS Sensor's position and orientation together.
 * @param   device Device handle.
 * @param   data   Structure filled in with position in meters and angles in
 *                 degrees.
 * @param   bRaw   True to skip the corrections VEXos normally applies.
 * @details This is the main GPS read. Position is relative to the centre of
 * the field and already accounts for the mounting offset set with
 *          vexDeviceGpsOriginSet().
 */
void vexDeviceGpsAttitudeGet(V5_DeviceT device, V5_DeviceGpsAttitude *data,
                             bool bRaw);

/**
 * @brief   Reads the GPS Sensor's raw gyroscope rates.
 * @param   device Device handle.
 * @param   data   Structure filled in with x, y and z in degrees per
 * second.
 */
void vexDeviceGpsRawGyroGet(V5_DeviceT device, V5_DeviceGpsRaw *data);

/**
 * @brief   Reads the GPS Sensor's raw accelerations.
 * @param   device Device handle.
 * @param   data   Structure filled in with x, y and z in g.
 */
void vexDeviceGpsRawAccelGet(V5_DeviceT device, V5_DeviceGpsRaw *data);

/**
 * @brief   Reads the GPS Sensor's status bits.
 * @param   device Device handle.
 * @return  The status word. VEX does not publish the meaning of the
 * individual bits.
 */
uint32_t vexDeviceGpsStatusGet(V5_DeviceT device);

/**
 * @brief   Sets the GPS Sensor's mode word.
 * @param   device Device handle.
 * @param   mode   Mode value.
 * @details VEX does not publish the mode values.
 */
void vexDeviceGpsModeSet(V5_DeviceT device, uint32_t mode);

/**
 * @brief   Reads back the GPS Sensor's mode word.
 * @param   device Device handle.
 * @return  The mode word.
 */
uint32_t vexDeviceGpsModeGet(V5_DeviceT device);

/**
 * @brief   Sets how often the GPS Sensor's inertial sensor updates.
 * @param   device Device handle.
 * @param   rate   Interval in milliseconds, a multiple of 5 and at least 5.
 * @details Only affects the inertial sensor, not the camera based position.
 */
void vexDeviceGpsDataRateSet(V5_DeviceT device, uint32_t rate);

/**
 * @brief   Tells the GPS Sensor where it sits on the robot.
 * @param   device Device handle.
 * @param   ox     X offset from the robot's centre of rotation, in meters.
 * @param   oy     Y offset from the robot's centre of rotation, in meters.
 * @details With this set, the reported position is the robot's centre of
 *          rotation rather than the sensor's own position, so it stops
 * swinging around as the robot turns.
 */
void vexDeviceGpsOriginSet(V5_DeviceT device, double ox, double oy);

/**
 * @brief   Reads back the GPS Sensor's mounting offset.
 * @param   device Device handle.
 * @param   ox     Filled in with the X offset in meters.
 * @param   oy     Filled in with the Y offset in meters.
 */
void vexDeviceGpsOriginGet(V5_DeviceT device, double *ox, double *oy);

/**
 * @brief   Sets the GPS Sensor's rotation value.
 * @param   device Device handle.
 * @param   value  Rotation in degrees.
 * @details Neither PROS nor the public VEXcode headers call this, and VEX
 * does not document whether it seeds the accumulated rotation reading or
 *          applies a fixed mounting rotation.
 */
void vexDeviceGpsRotationSet(V5_DeviceT device, double value);

/**
 * @brief   Reads back the GPS Sensor's rotation value.
 * @param   device Device handle.
 * @return  The rotation in degrees.
 */
double vexDeviceGpsRotationGet(V5_DeviceT device);

/**
 * @brief   Tells the GPS Sensor where the robot starts on the field.
 * @param   device           Device handle.
 * @param   initial_x        Starting X position in meters, measured from
 * the centre of the field.
 * @param   initial_y        Starting Y position in meters.
 * @param   initial_rotation Starting heading in degrees, 0 through 360.
 * @details Seeding a known starting pose gives the sensor something to fall
 *          back on before it has a clear view of the field perimeter.
 */
void vexDeviceGpsInitialPositionSet(V5_DeviceT device, double initial_x,
                                    double initial_y, double initial_rotation);

/**
 * @brief   Reads how uncertain the GPS Sensor's position is.
 * @param   device Device handle.
 * @return  The root mean squared position error in meters.
 * @details Grows when the camera cannot see enough of the field perimeter,
 * so it is a useful gate on whether to trust the position at all.
 */
double vexDeviceGpsErrorGet(V5_DeviceT device);

// Generic serial port comms to any device

/**
 * @brief   Switches a smart port into generic serial mode.
 * @param   device  Device handle.
 * @param   options Option flags. VEX does not publish their meaning; PROS
 *                  passes 0.
 * @details Call this before anything else on the port. Until it is called,
 *          VEXos does not report the port as #kDeviceTypeGenericSerial, so
 * code that checks the port type first will never get to enable it.
 */
void vexDeviceGenericSerialEnable(V5_DeviceT device, int32_t options);

/**
 * @brief   Sets the baud rate of a generic serial port.
 * @param   device   Device handle.
 * @param   baudrate Baud rate in bits per second.
 */
void vexDeviceGenericSerialBaudrate(V5_DeviceT device, int32_t baudrate);

/**
 * @brief   Queues one byte for transmission.
 * @param   device Device handle.
 * @param   c      Byte to send.
 * @return  The number of bytes queued, or -1 on error.
 */
int32_t vexDeviceGenericSerialWriteChar(V5_DeviceT device, uint8_t c);

/**
 * @brief   Reports how much room is left in the transmit buffer.
 * @param   device Device handle.
 * @return  The number of free bytes.
 */
int32_t vexDeviceGenericSerialWriteFree(V5_DeviceT device);

/**
 * @brief   Queues a buffer for transmission.
 * @param   device Device handle.
 * @param   buffer Bytes to send.
 * @param   length Number of bytes to send.
 * @return  The number of bytes queued, or -1 on error.
 * @details May queue fewer bytes than asked for if the transmit buffer
 * fills up, so check the return value rather than assuming it all went.
 */
int32_t vexDeviceGenericSerialTransmit(V5_DeviceT device, uint8_t *buffer,
                                       int32_t length);

/**
 * @brief   Removes one byte from the receive buffer.
 * @param   device Device handle.
 * @return  The byte, or -1 if the buffer is empty.
 */
int32_t vexDeviceGenericSerialReadChar(V5_DeviceT device);

/**
 * @brief   Looks at the next received byte without removing it.
 * @param   device Device handle.
 * @return  The byte, or -1 if the buffer is empty.
 */
int32_t vexDeviceGenericSerialPeekChar(V5_DeviceT device);

/**
 * @brief   Reports how many bytes are waiting to be read.
 * @param   device Device handle.
 * @return  The number of buffered bytes.
 */
int32_t vexDeviceGenericSerialReceiveAvail(V5_DeviceT device);

/**
 * @brief   Reads buffered bytes into a caller supplied buffer.
 * @param   device Device handle.
 * @param   buffer Destination buffer.
 * @param   length Maximum number of bytes to read.
 * @return  The number of bytes actually read.
 */
int32_t vexDeviceGenericSerialReceive(V5_DeviceT device, uint8_t *buffer,
                                      int32_t length);

/**
 * @brief   Discards the contents of a generic serial port's buffers.
 * @param   device Device handle.
 */
void vexDeviceGenericSerialFlush(V5_DeviceT device);

// Display/graphics

/*----------------------------------------------------------------------------*/
/** @brief      Brain display
 *
 * @details
 *  The framebuffer is #SYSTEM_DISPLAY_WIDTH by #SYSTEM_DISPLAY_HEIGHT
 * pixels, with (0, 0) at the top left and (479, 271) at the bottom right.
 * VEXos draws the program header across the top 32 rows, so drawing there
 * works but gets painted over. The area left for user code is y = 32
 * through y = 271.
 *
 *  Colors are packed as 0x00RRGGBB, the same encoding v5_color.h uses.
 * Drawing calls take their color from the current foreground, and the
 * "Clear" variants take it from the current background, so erasing a shape
 * is the same call with a different source of color.
 *
 *  Rectangle coordinates are inclusive at both ends, so a rectangle covers
 *  (1 + x2 - x1) by (1 + y2 - y1) pixels. Lines and circles are drawn with
 *  Bresenham's algorithms and are not antialiased.
 */
/*----------------------------------------------------------------------------*/

/**
 * @brief   Sets the color used by drawing operations.
 * @param   col Color as 0x00RRGGBB. Defaults to white.
 */
void vexDisplayForegroundColor(uint32_t col);

/**
 * @brief   Sets the color used by erasing operations.
 * @param   col Color as 0x00RRGGBB. Defaults to black.
 * @details Also used as the fill behind opaque text and as the color newly
 *          exposed pixels take when the screen scrolls.
 */
void vexDisplayBackgroundColor(uint32_t col);

/**
 * @brief   Reads back the current foreground color.
 * @return  The color as 0x00RRGGBB.
 */
uint32_t vexDisplayForegroundColorGet(void);

/**
 * @brief   Reads back the current background color.
 * @return  The color as 0x00RRGGBB.
 */
uint32_t vexDisplayBackgroundColorGet(void);

/**
 * @brief   Fills the whole framebuffer with the background color.
 */
void vexDisplayErase(void);

/**
 * @brief   Scrolls the screen vertically.
 * @param   nStartLine Topmost y coordinate of the region to scroll.
 * Everything from there to the bottom of the screen moves.
 * @param   nLines     How far to move, in pixels. Positive moves up,
 * negative moves down.
 * @details Pixels pushed out of the region are discarded, and the space
 * left behind is filled with the background color.
 */
void vexDisplayScroll(int32_t nStartLine, int32_t nLines);

/**
 * @brief   Scrolls a rectangular region vertically.
 * @param   x1     Left edge of the region.
 * @param   y1     Top edge of the region.
 * @param   x2     Right edge of the region.
 * @param   y2     Bottom edge of the region.
 * @param   nLines How far to move, in pixels. Positive moves up, negative
 * moves down.
 * @details Behaves like vexDisplayScroll() but leaves everything outside
 * the rectangle alone.
 * @note    On VEXos 1.1.4 this clears one row too many, leaving the bottom
 * row of the region filled with the background color.
 */
void vexDisplayScrollRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                          int32_t nLines);

/**
 * @brief   Copies a pixel buffer onto the screen.
 * @param   x1        Left edge of the destination rectangle.
 * @param   y1        Top edge of the destination rectangle.
 * @param   x2        Right edge of the destination rectangle.
 * @param   y2        Bottom edge of the destination rectangle.
 * @param   pSrc      Source buffer, one 0x00RRGGBB pixel per element.
 * @param   srcStride Number of pixels per row in the source buffer.
 * @details The stride is separate from the rectangle width so a
 * sub-rectangle of a larger image can be drawn without copying it out
 * first. This is how a decoded #v5_image is put on screen.
 */
void vexDisplayCopyRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                        uint32_t *pSrc, int32_t srcStride);

/**
 * @brief   Sets one pixel to the foreground color.
 * @param   x X coordinate.
 * @param   y Y coordinate.
 */
void vexDisplayPixelSet(uint32_t x, uint32_t y);

/**
 * @brief   Sets one pixel to the background color.
 * @param   x X coordinate.
 * @param   y Y coordinate.
 */
void vexDisplayPixelClear(uint32_t x, uint32_t y);

/**
 * @brief   Draws a line in the foreground color.
 * @param   x1 X coordinate of the first endpoint.
 * @param   y1 Y coordinate of the first endpoint.
 * @param   x2 X coordinate of the second endpoint.
 * @param   y2 Y coordinate of the second endpoint.
 */
void vexDisplayLineDraw(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/**
 * @brief   Draws a line in the background color.
 * @param   x1 X coordinate of the first endpoint.
 * @param   y1 Y coordinate of the first endpoint.
 * @param   x2 X coordinate of the second endpoint.
 * @param   y2 Y coordinate of the second endpoint.
 */
void vexDisplayLineClear(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/**
 * @brief   Outlines a rectangle in the foreground color.
 * @param   x1 Left edge.
 * @param   y1 Top edge.
 * @param   x2 Right edge.
 * @param   y2 Bottom edge.
 */
void vexDisplayRectDraw(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/**
 * @brief   Outlines a rectangle in the background color.
 * @param   x1 Left edge.
 * @param   y1 Top edge.
 * @param   x2 Right edge.
 * @param   y2 Bottom edge.
 */
void vexDisplayRectClear(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/**
 * @brief   Fills a rectangle with the foreground color.
 * @param   x1 Left edge.
 * @param   y1 Top edge.
 * @param   x2 Right edge.
 * @param   y2 Bottom edge.
 */
void vexDisplayRectFill(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/**
 * @brief   Outlines a circle in the foreground color.
 * @param   xc     X coordinate of the centre.
 * @param   yc     Y coordinate of the centre.
 * @param   radius Radius in pixels.
 */
void vexDisplayCircleDraw(int32_t xc, int32_t yc, int32_t radius);

/**
 * @brief   Outlines a circle in the background color.
 * @param   xc     X coordinate of the centre.
 * @param   yc     Y coordinate of the centre.
 * @param   radius Radius in pixels.
 */
void vexDisplayCircleClear(int32_t xc, int32_t yc, int32_t radius);

/**
 * @brief   Fills a circle with the foreground color.
 * @param   xc     X coordinate of the centre.
 * @param   yc     Y coordinate of the centre.
 * @param   radius Radius in pixels.
 */
void vexDisplayCircleFill(int32_t xc, int32_t yc, int32_t radius);

/**
 * @brief   Draws formatted text at a pixel position.
 * @param   xpos    X coordinate of the text's top left corner.
 * @param   ypos    Y coordinate of the text's top left corner.
 * @param   bOpaque Non-zero to fill the text's background with the
 * background color, 0 to draw only the glyphs.
 * @param   format  printf style format string.
 * @param   ...     Arguments consumed by the format string.
 * @details Uses the size set by vexDisplayTextSize() and the font set by
 *          vexDisplayFontNamedSet().
 */
void vexDisplayPrintf(int32_t xpos, int32_t ypos, uint32_t bOpaque,
                      const char *format, ...);

/**
 * @brief   Draws formatted text on a text row.
 * @param   nLineNumber Zero based row. Row n sits at y = 34 + 20 * n.
 * @param   format      printf style format string.
 * @param   ...         Arguments consumed by the format string.
 * @details Draws opaque text at the left edge of the screen.
 */
void vexDisplayString(const int32_t nLineNumber, const char *format, ...);

/**
 * @brief   Draws formatted text at a pixel position.
 * @param   xpos   X coordinate of the text's top left corner.
 * @param   ypos   Y coordinate of the text's top left corner.
 * @param   format printf style format string.
 * @param   ...    Arguments consumed by the format string.
 */
void vexDisplayStringAt(int32_t xpos, int32_t ypos, const char *format, ...);

/**
 * @brief   Draws formatted text on a text row at the large size.
 * @param   nLineNumber Zero based row.
 * @param   format      printf style format string.
 * @param   ...         Arguments consumed by the format string.
 * @details The large size is two thirds of the font's natural size.
 */
void vexDisplayBigString(const int32_t nLineNumber, const char *format, ...);

/**
 * @brief   Draws formatted text at a pixel position at the large size.
 * @param   xpos   X coordinate of the text's top left corner.
 * @param   ypos   Y coordinate of the text's top left corner.
 * @param   format printf style format string.
 * @param   ...    Arguments consumed by the format string.
 */
void vexDisplayBigStringAt(int32_t xpos, int32_t ypos, const char *format, ...);

/**
 * @brief   Draws formatted text at a pixel position at the small size.
 * @param   xpos   X coordinate of the text's top left corner.
 * @param   ypos   Y coordinate of the text's top left corner.
 * @param   format printf style format string.
 * @param   ...    Arguments consumed by the format string.
 * @details The small size is about a quarter of the font's natural size.
 */
void vexDisplaySmallStringAt(int32_t xpos, int32_t ypos, const char *format,
                             ...);

/**
 * @brief   Draws formatted text centred on a text row.
 * @param   nLineNumber Zero based row.
 * @param   format      printf style format string.
 * @param   ...         Arguments consumed by the format string.
 */
void vexDisplayCenteredString(const int32_t nLineNumber, const char *format,
                              ...);

/**
 * @brief   Draws formatted text centred on a text row at the large size.
 * @param   nLineNumber Zero based row.
 * @param   format      printf style format string.
 * @param   ...         Arguments consumed by the format string.
 */
void vexDisplayBigCenteredString(const int32_t nLineNumber, const char *format,
                                 ...);

// Not really for user code but we need these for some wrapper functions

/**
 * @brief   va_list form of vexDisplayPrintf().
 * @param   xpos    X coordinate of the text's top left corner.
 * @param   ypos    Y coordinate of the text's top left corner.
 * @param   bOpaque Non-zero to fill the text's background.
 * @param   format  printf style format string.
 * @param   args    Argument list, already started with va_start().
 * @details Provided so libraries can put their own variadic wrapper on top.
 */
void vexDisplayVPrintf(int32_t xpos, int32_t ypos, uint32_t bOpaque,
                       const char *format, va_list args);

/**
 * @brief   va_list form of vexDisplayString().
 * @param   nLineNumber Zero based row.
 * @param   format      printf style format string.
 * @param   args        Argument list, already started with va_start().
 */
void vexDisplayVString(const int32_t nLineNumber, const char *format,
                       va_list args);

/**
 * @brief   va_list form of vexDisplayStringAt().
 * @param   xpos   X coordinate of the text's top left corner.
 * @param   ypos   Y coordinate of the text's top left corner.
 * @param   format printf style format string.
 * @param   args   Argument list, already started with va_start().
 */
void vexDisplayVStringAt(int32_t xpos, int32_t ypos, const char *format,
                         va_list args);

/**
 * @brief   va_list form of vexDisplayBigString().
 * @param   nLineNumber Zero based row.
 * @param   format      printf style format string.
 * @param   args        Argument list, already started with va_start().
 */
void vexDisplayVBigString(const int32_t nLineNumber, const char *format,
                          va_list args);

/**
 * @brief   va_list form of vexDisplayBigStringAt().
 * @param   xpos   X coordinate of the text's top left corner.
 * @param   ypos   Y coordinate of the text's top left corner.
 * @param   format printf style format string.
 * @param   args   Argument list, already started with va_start().
 */
void vexDisplayVBigStringAt(int32_t xpos, int32_t ypos, const char *format,
                            va_list args);

/**
 * @brief   va_list form of vexDisplaySmallStringAt().
 * @param   xpos   X coordinate of the text's top left corner.
 * @param   ypos   Y coordinate of the text's top left corner.
 * @param   format printf style format string.
 * @param   args   Argument list, already started with va_start().
 */
void vexDisplayVSmallStringAt(int32_t xpos, int32_t ypos, const char *format,
                              va_list args);

/**
 * @brief   va_list form of vexDisplayCenteredString().
 * @param   nLineNumber Zero based row.
 * @param   format      printf style format string.
 * @param   args        Argument list, already started with va_start().
 */
void vexDisplayVCenteredString(const int32_t nLineNumber, const char *format,
                               va_list args);

/**
 * @brief   va_list form of vexDisplayBigCenteredString().
 * @param   nLineNumber Zero based row.
 * @param   format      printf style format string.
 * @param   args        Argument list, already started with va_start().
 */
void vexDisplayVBigCenteredString(const int32_t nLineNumber, const char *format,
                                  va_list args);

/**
 * @brief   Sets the text size as a fraction of the font's natural size.
 * @param   n Numerator.
 * @param   d Denominator.
 * @details Passing 1 and 3 gives the default size, 2 and 3 gives the large
 *          size, and 1 and 2 gives something in between. Affects the
 * coordinate based text calls; the big, small and line based variants set
 * their own size.
 */
void vexDisplayTextSize(uint32_t n, uint32_t d);

/**
 * @brief   Selects the font used for text.
 * @param   pFontName Font name, either "monospace" or "proportional".
 * @details The default is monospace, which keeps columns lined up when
 * values change width from frame to frame.
 */
void vexDisplayFontNamedSet(const char *pFontName);

/**
 * @brief   Measures how wide a string would be if drawn.
 * @param   pString Null terminated string to measure.
 * @return  The width in pixels.
 * @details Uses the size and font from the last text drawing call, so draw
 *          something at the intended size first if it matters.
 */
int32_t vexDisplayStringWidthGet(const char *pString);

/**
 * @brief   Measures how tall a string would be if drawn.
 * @param   pString Null terminated string to measure.
 * @return  The height in pixels.
 */
int32_t vexDisplayStringHeightGet(const char *pString);

/**
 * @brief   Presents the current frame and enables double buffering.
 * @param   bVsyncWait    True to block until the display is ready to
 * refresh.
 * @param   bRunScheduler True to let VEXos run its background tasks while
 *                        waiting.
 * @return  VEX does not document the return value.
 * @details The display starts out in immediate mode, where every draw goes
 *          straight to the panel. That tears badly when a program redraws
 * in a loop. The first call here switches to double buffering; from then on
 *          nothing appears until this is called again, so it has to be
 * called once per frame. Use vexDisplayDoubleBufferDisable() to go back.
 */
bool vexDisplayRender(bool bVsyncWait, bool bRunScheduler);

/**
 * @brief   Returns the display to immediate mode.
 * @details Undoes vexDisplayRender(). Drawing calls take effect straight
 * away again.
 */
void vexDisplayDoubleBufferDisable(void);

/**
 * @brief   Restricts drawing to a rectangle.
 * @param   x1 Left edge.
 * @param   y1 Top edge.
 * @param   x2 Right edge.
 * @param   y2 Bottom edge.
 * @details Anything outside the rectangle is discarded rather than drawn.
 * Useful for keeping one part of the screen from overwriting another.
 */
void vexDisplayClipRegionSet(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/**
 * @brief   Removes the clip region.
 * @details Drawing can reach the whole screen again.
 */
void vexDisplayClipRegionClear();

/**
 * @brief   Decodes a bitmap image into a pixel buffer.
 * @param   ibuf The BMP file's bytes.
 * @param   oBuf Image structure whose data member already points at a
 * buffer of at least maxw * maxh pixels. Filled in with the decoded size
 *               and pixels.
 * @param   maxw Width capacity of that buffer, in pixels.
 * @param   maxh Height capacity of that buffer, in pixels.
 * @return  1 on success, 0 on failure.
 * @details The decoded pixels use the display's 0x00RRGGBB layout, so they
 * can go straight to vexDisplayCopyRect().
 */
uint32_t vexImageBmpRead(const uint8_t *ibuf, v5_image *oBuf, uint32_t maxw,
                         uint32_t maxh);

/**
 * @brief   Decodes a PNG image into a pixel buffer.
 * @param   ibuf    The PNG file's bytes.
 * @param   oBuf    Image structure whose data member already points at a
 * buffer of at least maxw * maxh pixels.
 * @param   maxw    Width capacity of that buffer, in pixels.
 * @param   maxh    Height capacity of that buffer, in pixels.
 * @param   ibuflen Length of the input buffer in bytes.
 * @return  1 on success, 0 on failure.
 * @details Unlike the BMP decoder, this one is told the input length,
 * because a PNG's size cannot be worked out from its header alone.
 */
uint32_t vexImagePngRead(const uint8_t *ibuf, v5_image *oBuf, uint32_t maxw,
                         uint32_t maxh, uint32_t ibuflen);

// special use only ! Talk to James.

/**
 * @brief   Retrieves a pointer to the VEXos scratch memory region.
 * @param   ptr Filled in with a pointer to the region.
 * @return  VEX does not document the return value.
 * @details Scratch memory is a block VEXos sets aside that survives a
 * program exiting, which makes it a way to pass data between runs. The
 *          original header marks this as special use only.
 */
int32_t vexScratchMemoryPtr(void **ptr);

/**
 * @brief   Takes the lock guarding scratch memory.
 * @return  True if the lock was acquired.
 */
bool vexScratchMemoryLock(void);

/**
 * @brief   Releases the lock guarding scratch memory.
 */
void vexScratchMemoryUnlock(void);

// SD card

/**
 * @brief   Mounts the microSD card.
 * @return  #FR_OK on success, otherwise the reason the mount failed.
 * @details Has to succeed before any other file call will work. The card
 * must be formatted FAT32.
 */
FRESULT vexFileMountSD(void);

/**
 * @brief   Lists the files in a directory.
 * @param   path   Directory to list.
 * @param   buffer Destination buffer for the listing.
 * @param   len    Size of the destination buffer in bytes.
 * @return  #FR_OK on success, otherwise the reason the listing failed.
 * @details File names are written separated by newlines. Subdirectories are
 * not included.
 */
FRESULT vexFileDirectoryGet(const char *path, char *buffer, uint32_t len);

/**
 * @brief   Opens a file for reading.
 * @param   filename Path to the file.
 * @param   mode     Ignored. Pass an empty string.
 * @return  A file handle, or null if the file could not be opened.
 * @details Despite the mode parameter this only opens for reading. Use
 *          vexFileOpenWrite() or vexFileOpenCreate() to write. VEXos allows
 *          only a small number of files open at once, so close them
 * promptly.
 */
FIL *vexFileOpen(const char *filename, const char *mode);

/**
 * @brief   Opens a file for writing, keeping what is already there.
 * @param   filename Path to the file.
 * @return  A file handle, or null if the file could not be opened.
 * @details Writes are appended to the existing contents.
 */
FIL *vexFileOpenWrite(const char *filename);

/**
 * @brief   Creates a file for writing, discarding what is already there.
 * @param   filename Path to the file.
 * @return  A file handle, or null if the file could not be created.
 */
FIL *vexFileOpenCreate(const char *filename);

/**
 * @brief   Closes a file.
 * @param   fdp File handle.
 * @details Flushes anything still buffered and releases the handle.
 */
void vexFileClose(FIL *fdp);

/**
 * @brief   Reads from a file.
 * @param   buf    Destination buffer.
 * @param   size   Size of one item in bytes.
 * @param   nItems Number of items to read.
 * @param   fdp    File handle.
 * @return  The number of bytes read.
 */
int32_t vexFileRead(char *buf, uint32_t size, uint32_t nItems, FIL *fdp);

/**
 * @brief   Writes to a file.
 * @param   buf    Source buffer.
 * @param   size   Size of one item in bytes.
 * @param   nItems Number of items to write.
 * @param   fdp    File handle.
 * @return  The number of bytes written.
 * @details Data may sit in a buffer until the file is synced or closed.
 */
int32_t vexFileWrite(char *buf, uint32_t size, uint32_t nItems, FIL *fdp);

/**
 * @brief   Reports a file's size.
 * @param   fdp File handle.
 * @return  The size in bytes.
 */
int32_t vexFileSize(FIL *fdp);

/**
 * @brief   Moves a file's read and write position.
 * @param   fdp    File handle.
 * @param   offset Offset in bytes.
 * @param   whence Where the offset is measured from: #FS_SEEK_SET,
 *                 #FS_SEEK_CUR or #FS_SEEK_END.
 * @return  #FR_OK on success, otherwise the reason the seek failed.
 */
FRESULT vexFileSeek(FIL *fdp, uint32_t offset, int32_t whence);

/**
 * @brief   Reports whether a drive is present.
 * @param   drive Drive number. The microSD slot is drive 0.
 * @return  True if the drive is installed.
 */
bool vexFileDriveStatus(uint32_t drive);

/**
 * @brief   Reports a file's current position.
 * @param   fdp File handle.
 * @return  The position in bytes from the start of the file.
 */
int32_t vexFileTell(FIL *fdp);

/**
 * @brief   Flushes buffered writes to the card.
 * @param   fdp File handle.
 * @details Makes the data safe against the brain losing power without
 * having to close and reopen the file.
 */
void vexFileSync(FIL *fdp);

/**
 * @brief   Reports what exists at a path.
 * @param   filename Path to check.
 * @return  A bit field: #FS_FILE_EXIST if the path exists, #FS_FILE_DIR if
 * it is a directory.
 */
uint32_t vexFileStatus(const char *filename);

// CDC

/**
 * @brief   Queues one byte on a serial channel.
 * @param   channel Channel number. Channel 1 is the stdio console.
 * @param   c       Byte to send.
 * @return  The number of bytes queued, or -1 on error.
 */
int32_t vexSerialWriteChar(uint32_t channel, uint8_t c);

/**
 * @brief   Queues a buffer on a serial channel.
 * @param   channel  Channel number. Channel 1 is the stdio console.
 * @param   data     Bytes to send.
 * @param   data_len Number of bytes to send.
 * @return  The number of bytes queued, or -1 on error.
 * @details The output buffer holds 2048 bytes, so a larger write is
 * truncated. Check the return value and retry the remainder after VEXos has
 * had a chance to drain the buffer.
 */
int32_t vexSerialWriteBuffer(uint32_t channel, uint8_t *data,
                             uint32_t data_len);

/**
 * @brief   Removes one byte from a serial channel's input buffer.
 * @param   channel Channel number. Channel 1 is the stdio console.
 * @return  The byte, or -1 if nothing is available.
 */
int32_t vexSerialReadChar(uint32_t channel);

/**
 * @brief   Looks at the next byte on a serial channel without removing it.
 * @param   channel Channel number. Channel 1 is the stdio console.
 * @return  The byte, or -1 if nothing is available.
 */
int32_t vexSerialPeekChar(uint32_t channel);

/**
 * @brief   Reports how much room is left in a serial channel's output
 * buffer.
 * @param   channel Channel number. Channel 1 is the stdio console.
 * @return  The number of free bytes, out of 2048.
 * @details A full 2048 means everything queued has been sent, which is the
 *          usual way to wait for output to drain before exiting.
 */
int32_t vexSerialWriteFree(uint32_t channel);

// rtos hooks

/*----------------------------------------------------------------------------*/
/** @brief      RTOS and interrupt hooks
 *
 * @details
 *  These exist so a runtime can bring its own scheduler instead of using
 * the one inside VEXos. They are part of a runtime's port layer, not
 * something ordinary robot code should touch. Calling them from user code
 * will destabilise whichever RTOS is already running.
 */
/*----------------------------------------------------------------------------*/

/**
 * @brief   Stops the system tick timer.
 * @details Called before a runtime installs its own tick handler.
 */
void vexSystemTimerStop();

/**
 * @brief   Acknowledges the system tick interrupt.
 * @details Called from the tick handler so the timer can fire again.
 */
void vexSystemTimerClearInterrupt();

/**
 * @brief   Points the system tick timer at a replacement handler.
 * @param   priority Interrupt priority for the tick.
 * @param   handler  Function to call on every tick.
 * @return  Non-zero on success.
 */
int32_t vexSystemTimerReinitForRtos(uint32_t priority,
                                    void (*handler)(void *data));

/**
 * @brief   Dispatches an interrupt to VEXos.
 * @param   ulICCIAR Value read from the interrupt controller's acknowledge
 *                   register.
 * @details A runtime's own IRQ handler passes on anything it does not
 * claim, so VEXos can still service its own peripherals.
 */
void vexSystemApplicationIRQHandler(uint32_t ulICCIAR);

/**
 * @brief   Reinitialises the watchdog for a replacement RTOS.
 * @return  Non-zero on success.
 */
int32_t vexSystemWatchdogReinitRtos(void);

/**
 * @brief   Reads the watchdog counter.
 * @return  The counter value.
 */
uint32_t vexSystemWatchdogGet(void);

// Hooks into the interrupt system, some of these will be used by FreeRTOS
// the others are just placeholders for now

/**
 * @brief   VEXos boot entry point.
 * @details Called from the reset vector before anything else runs.
 */
void vexSystemBoot(void);

/** @brief Default handler for the undefined instruction exception. */
void vexSystemUndefinedException(void);

/** @brief Default handler for fast interrupts. */
void vexSystemFIQInterrupt(void);

/**
 * @brief   Default handler for normal interrupts.
 * @note    The name contains a typo that is kept for source compatibility.
 */
void vexSystemIQRQnterrupt(void);

/** @brief Default handler for the software interrupt. */
void vexSystemSWInterrupt(void);

/**
 * @brief   Default handler for the data abort exception.
 * @details Raised by a bad data access, most often a null or dangling
 * pointer.
 */
void vexSystemDataAbortInterrupt(void);

/**
 * @brief   Default handler for the prefetch abort exception.
 * @details Raised when the processor cannot fetch an instruction, usually
 * after a jump through a corrupt function pointer.
 */
void vexSystemPrefetchAbortInterrupt(void);

// touch

/**
 * @brief   Installs a callback for touchscreen events.
 * @param   callback Function called with the event kind and the x and y
 *                   coordinates where it happened. Pass null to remove it.
 * @details The callback runs in VEXos context, so keep it short and do not
 *          block in it.
 */
void vexTouchUserCallbackSet(void (*callback)(V5_TouchEvent, int32_t, int32_t));

/**
 * @brief   Reads the current touchscreen state.
 * @param   status Structure filled in with the last event, its position and
 * the running press and release counts.
 * @return  True if the state was read.
 * @details Polling alternative to vexTouchUserCallbackSet(). Comparing the
 *          counts between polls catches presses that started and ended
 * between them.
 */
bool vexTouchDataGet(V5_TouchStatus *status);

// system utility

/**
 * @brief   Returns the VEXos version.
 * @return  The version packed into one word, one byte per component from
 * most to least significant: major, minor, build and beta.
 * @details Compare it as an integer to gate on a feature, as PROS does when
 * it checks for the newer controller text behaviour.
 */
uint32_t vexSystemVersion(void);

/**
 * @brief   Returns the version of the standard library inside VEXos.
 * @return  The version, packed the same way as vexSystemVersion().
 */
uint32_t vexStdlibVersion(void);

// get SDK version

/**
 * @brief   Returns the SDK version.
 * @return  The version, packed the same way as vexSystemVersion().
 */
uint32_t vexSdkVersion(void);

// duplication of prototypes in v5_util.h for use by user code

/**
 * @brief   Returns the standard library version this program was linked
 * against.
 * @return  The version, packed the same way as vexSystemVersion().
 * @details Compare with vexStdlibVersion() to spot a program built against
 * a newer library than the brain is running.
 */
uint32_t vexStdlibVersionLinked(void);

/**
 * @brief   Checks the linked standard library against the one in VEXos.
 * @return  True if the two are compatible.
 */
bool vexStdlibVersionVerify(void);

// comp status

/**
 * @brief   Reads the competition control state.
 * @return  A bit field built from the V5_COMP_BIT_* values.
 * @details The one thing every competition program polls. Note that
 *          #V5_COMP_BIT_EBL is set when the robot is disabled, not enabled,
 * and that #V5_COMP_BIT_MODE only means anything while the robot is
 *          enabled.
 */
uint32_t vexCompetitionStatus(void);

/**
 * @brief   Drives the competition control state from user code.
 * @param   data Bit field using the same layout as vexCompetitionStatus().
 * @details Lets a program simulate a competition switch so autonomous and
 *          driver control transitions can be exercised without a field. VEX
 *          does not document it.
 */
void vexCompetitionControl(uint32_t data);

// battery

/**
 * @brief   Reads the battery voltage.
 * @return  The voltage in millivolts. A healthy pack sits near 12800.
 */
int32_t vexBatteryVoltageGet(void);

/**
 * @brief   Reads the current the battery is supplying.
 * @return  The current in milliamps. The pack can deliver up to about
 * 20000.
 */
int32_t vexBatteryCurrentGet(void);

/**
 * @brief   Reads the battery temperature.
 * @return  The temperature in degrees Celsius.
 */
double vexBatteryTemperatureGet(void);

/**
 * @brief   Reads how much charge the battery has left.
 * @return  The remaining capacity as a percentage, 0 through 100.
 */
double vexBatteryCapacityGet(void);

#ifdef __cplusplus
}
#endif
#endif /* V5_API_H_*/
