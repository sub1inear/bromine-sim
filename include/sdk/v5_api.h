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
 *  of function pointers at a constant address (0x037fc000 on the V5). The
 *  prototypes in this file are the wrappers around those entries. They are
 *  ordinary function calls rather than supervisor calls.
 *
 *  Most device functions take a #V5_DeviceT handle obtained from
 *  vexDeviceGetByIndex(). v5_apiuser.h declares a parallel set of wrappers that
 *  take a port index instead.
 *
 *  Two things are worth knowing up front. VEX does not document what these
 *  calls do when the port holds a different kind of device from the one being
 *  asked about, so PROS and vexide both check the port's type themselves before
 *  every call. And device readings come from a shared memory buffer that is
 *  refreshed every 10 ms, so values change no faster than that.
 *
 *  The evidence for every statement in this file's documentation is in
 *  docs/dev/sdk-doc-evidence.md.
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
 * @details Gives VEXos time to do its background work. PROS calls it every
 *          2 ms from its system daemon, and depends on it for two things in
 *          particular. The Inertial Sensor's calibration flag is only set while
 *          it runs, which is why PROS's blocking IMU reset releases the port
 *          and delays so the daemon can run, rather than spinning. Console
 *          output only drains while it runs, which is why PROS calls it in a
 *          loop before exiting until the serial buffer is empty.
 *
 *          It is also what to call inside a loop that cannot yield. PROS's
 *          data abort and prefetch abort handlers loop on it so their
 *          diagnostics still get out over the serial link.
 */
void vexBackgroundProcessing(void);

// Console output

/**
 * @brief   Writes a formatted message using VEXos's own printf.
 * @param   fmt printf style format string.
 * @param   ... Arguments consumed by the format string.
 * @return  Not documented by VEX.
 * @details Where the output is routed is not documented, and no public runtime
 *          calls this.
 */
int32_t vexDebug(char const *fmt, ...);

/**
 * @brief   Formats a message using VEXos's own printf.
 * @param   fmt printf style format string.
 * @param   ... Arguments consumed by the format string.
 * @return  Not documented by VEX.
 * @details This is the formatter built into VEXos, reached through the
 *          jumptable, rather than the one from the C library the program is
 *          linked against. VEX does not document where the output goes.
 */
int32_t vex_printf(char const *fmt, ...);

/**
 * @brief   Formats a string into a caller supplied buffer using VEXos's own
 *          formatter.
 * @param   out    Destination buffer.
 * @param   format printf style format string.
 * @param   ...    Arguments consumed by the format string.
 * @return  Not documented by VEX.
 */
int32_t vex_sprintf(char *out, const char *format, ...);

/**
 * @brief   Formats a string into a caller supplied buffer with a length
 *          limit, using VEXos's own formatter.
 * @param   out     Destination buffer.
 * @param   max_len Length limit.
 * @param   format  printf style format string.
 * @param   ...     Arguments consumed by the format string.
 * @return  Not documented by VEX.
 */
int32_t vex_snprintf(char *out, uint32_t max_len, const char *format, ...);

/**
 * @brief   va_list form of vex_sprintf().
 * @param   out    Destination buffer.
 * @param   format printf style format string.
 * @param   args   Argument list, already started with va_start().
 * @return  Not documented by VEX.
 */
int32_t vex_vsprintf(char *out, const char *format, va_list args);

/**
 * @brief   va_list form of vex_snprintf().
 * @param   out     Destination buffer.
 * @param   max_len Length limit.
 * @param   format  printf style format string.
 * @param   args    Argument list, already started with va_start().
 * @return  Not documented by VEX.
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
 * @brief   Reads the time of day.
 * @param   pTime Structure filled in with the current time.
 * @details VEX does not document where the value comes from. Note that
 *          vexide's bindings declare this as returning the structure by value
 *          with no arguments, which is a different calling convention from
 *          this declaration.
 */
void vexGettime(struct time *pTime);

/**
 * @brief   Reads the date.
 * @param   pDate Structure filled in with the current date.
 * @details VEX does not document where the value comes from. vexide's
 *          bindings declare this as returning the structure by value, as with
 *          vexGettime().
 */
void vexGetdate(struct date *pDate);

/**
 * @brief   Undocumented.
 * @details No public runtime calls this, and VEX does not document what it
 *          does.
 */
void vexSystemMemoryDump(void);

/**
 * @brief   Undocumented.
 * @param   pin   Not documented.
 * @param   value Not documented.
 * @details No public runtime calls this, and VEX does not document what it
 *          does.
 */
void vexSystemDigitalIO(uint32_t pin, uint32_t value);

/**
 * @brief   Undocumented.
 * @return  Not documented.
 * @details No public runtime calls this, and VEX does not document what it
 *          returns.
 */
uint32_t vexSystemStartupOptions(void);

/**
 * @brief   Asks VEXos to end the running program.
 * @details Control returns to the brain's program screen. PROS flushes its
 *          output streams and waits for the serial buffer to drain before
 *          calling this. The call does not necessarily return straight away,
 *          and PROS follows it with an endless vexBackgroundProcessing() loop.
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
 * @details Unlike vexSystemHighResTimeGet(), this counts from power on rather
 *          than from the start of the program.
 */
uint64_t vexSystemPowerupTimeGet(void);

/**
 * @brief   Returns the address of the file linked to this program.
 * @return  The address in memory, or 0 if no file is linked.
 * @details VEXos lets a program link another binary package. PROS uses this
 *          to detect whether its hot/cold linked package is present.
 */
uint32_t vexSystemLinkAddrGet(void);

/**
 * @brief   Undocumented.
 * @return  Not documented.
 * @details No public runtime calls this, and VEX does not document what it
 *          returns.
 */
uint32_t vexSystemUsbStatus(void);

// Generic device

/**
 * @brief   Returns the number of device port indexes this brain exposes.
 * @return  The port count. This is 23 as of VEXos 1.1.4.
 * @details Size buffers for vexDeviceGetStatus() with #V5_MAX_DEVICE_PORTS
 *          rather than with this value, as both PROS and vexide do.
 */
uint32_t vexDevicesGetNumber(void);

/**
 * @brief   Counts how many devices of a given type are plugged in.
 * @param   type Device type to look for.
 * @return  The number of matching devices.
 */
uint32_t vexDevicesGetNumberByType(V5_DeviceType type);

/**
 * @brief   Returns a buffer of all devices on the brain.
 * @return  Not documented beyond that. No public runtime calls this, so the
 *          layout of what it returns is unknown.
 */
V5_DeviceT vexDevicesGet(void);

/**
 * @brief   Returns the handle for one device port.
 * @param   index Zero based port index, so 0 for smart port 1.
 * @return  Handle to that port. The port-index wrappers in v5_apiuser.h check
 *          the result for null.
 * @details PROS and vexide both fetch a port's handle once and keep using it.
 *          vexide notes that state written to a motor or rotation sensor
 *          through its handle is kept while the device is unplugged. Check the
 *          port's type with vexDeviceGetStatus() before trusting readings.
 */
V5_DeviceT vexDeviceGetByIndex(uint32_t index);

/**
 * @brief   Reads the type of every device port into a buffer.
 * @param   buffer Array of #V5_MAX_DEVICE_PORTS entries.
 * @return  The number of entries written, or -1 if buffer is null.
 * @details PROS and vexide both call this to refresh their view of the
 *          ports, then compare each entry against what the port is expected
 *          to hold.
 */
int32_t vexDeviceGetStatus(V5_DeviceType *buffer);

/**
 * @brief   Reads the timestamp of a device's most recent data.
 * @param   device Device handle.
 * @return  A timestamp in milliseconds.
 * @details vexide documents this as the time of the last packet the brain
 *          processed from the device, taken from the brain's millisecond
 *          timer, the same clock as vexSystemTimeGet(). The open-source
 *          jumptable bindings instead describe it as recorded by the device's
 *          internal clock. Either way, comparing successive readings from one
 *          device shows whether its data has been refreshed.
 */
int32_t vexDeviceGetTimestamp(V5_DeviceT device);

/**
 * @brief   Port index counterpart to vexDeviceGetTimestamp().
 * @param   index Zero based port index.
 * @return  Not documented.
 * @details Absent from the open-source jumptable bindings, and no public
 *          runtime calls it.
 */
int32_t vexDeviceGetTimestampByIndex(int32_t index);

/**
 * @brief   Undocumented.
 * @return  Not documented.
 * @details No public runtime calls this, and VEX does not document what it
 *          returns.
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
 * @return  The channel value.
 * @details Analog channels return -127 to 127, and 0 if the controller is
 *          not connected. Button channels are non-zero while the button is
 *          held. #BatteryCapacity is a percentage; the unit of #BatteryLevel
 *          is not documented. Use vexControllerConnectionStatusGet() to tell a
 *          centred stick from an absent controller.
 */
int32_t vexControllerGet(V5_ControllerId id, V5_ControllerIndex index);

/**
 * @brief   Reports how a controller is connected.
 * @param   id Which controller to query.
 * @return  The connection status, #kV5ControllerOffline when the controller
 *          is absent.
 */
V5_ControllerStatus vexControllerConnectionStatusGet(V5_ControllerId id);

/**
 * @brief   Writes text to a controller's LCD, or triggers a rumble pattern.
 * @param   id   Which controller to write to.
 * @param   line One based row, 1 to 3. Row 4 is the rumble channel, and row 0
 *               with an empty string clears the screen.
 * @param   col  Column. PROS passes 1 for the first column and vexide's
 *               try_set_text does the same, but vexide's set_text passes 0,
 *               so the sources disagree.
 * @param   str  Null terminated string.
 * @return  True if the request was accepted.
 * @details Controller text updates are slow. PROS documents that updates sent
 *          faster than every 10 ms on a wired connection, or every 50 ms over
 *          VEXnet, will not be applied. PROS and vexide both treat a false
 *          return as a reason to retry.
 *
 *          Writing to row 4 does not print anything. The string is read as a
 *          rumble pattern instead, built from dots for short pulses, dashes
 *          for long pulses and spaces for pauses, up to 8 characters.
 */
bool vexControllerTextSet(V5_ControllerId id, uint32_t line, uint32_t col,
                          const char *str);

// LED sensor

/**
 * @brief   Sets an LED module to one of the preset colors.
 * @param   device Device handle.
 * @param   value  Color to show.
 * @details The LED module is not sold by VEX and only exists as development
 *          hardware. No public runtime calls any of the LED functions.
 */
void vexDeviceLedSet(V5_DeviceT device, V5_DeviceLedColor value);

/**
 * @brief   Sets an LED module to an arbitrary color.
 * @param   device Device handle.
 * @param   color  Color. The encoding is not documented; the
 *                 #V5_DeviceLedColor presets are 0xRRGGBB values.
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
 * @return  The current color. The encoding is not documented.
 */
uint32_t vexDeviceLedRgbGet(V5_DeviceT device);

// ADI sensor

/**
 * @brief   Configures one 3-Wire channel.
 * @param   device Handle to the brain's 3-Wire ports or a 3-Wire Expander.
 *                 PROS reaches the brain's own 3-Wire ports at index 21.
 * @param   port   Channel number, 0 to 7, for the ports labelled A to H.
 * @param   type   What the channel is wired to.
 * @details PROS and vexide configure a channel before using it, and PROS
 *          checks the configuration with vexDeviceAdiPortConfigGet() before
 *          every read or write.
 *
 *          A quadrature encoder or ultrasonic sensor uses two adjacent
 *          channels. PROS and vexide configure only the first of the pair,
 *          which has to be an even channel number (port A, C, E or G).
 *          Configuring a channel as #kAdiPortTypeLegacyGyro starts a
 *          calibration. PROS waits 1300 ms after it, noting the theoretical
 *          time is 1024 ms.
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
 * @details What the value means depends on the channel's configuration. For a
 *          digital output it is the output level. For a legacy motor or servo
 *          it is the command, which PROS clamps to -128 through 127. For an
 *          encoder it sets the count, which is how PROS (writing 0) and vexide
 *          (writing any value) reset it. For a legacy gyro, vexide writes the
 *          calibration duration in milliseconds.
 */
void vexDeviceAdiValueSet(V5_DeviceT device, uint32_t port, int32_t value);

/**
 * @brief   Reads a 3-Wire channel.
 * @param   device Device handle.
 * @param   port   Channel number, 0 to 7.
 * @return  The channel's current reading.
 * @details What the value means depends on the channel's configuration:
 *          non-zero for a high digital input, 0 to 4095 for an analog input
 *          spanning 0 V to 5 V, the accumulated count for an encoder, and the
 *          distance in centimetres for an ultrasonic sensor, or -1 when
 *          nothing is in range. For a legacy motor, PROS subtracts 127 from the
 *          value it reads back.
 */
int32_t vexDeviceAdiValueGet(V5_DeviceT device, uint32_t port);

/**
 * @brief   Writes a run of colors to an addressable LED strip.
 * @param   device  Handle to the brain's 3-Wire ports or a 3-Wire Expander.
 * @param   port    Channel number, 0 to 7, for the ports labelled A to H.
 * @param   pData   Colors to write, one word per LED.
 * @param   nOffset Offset into the strip. PROS always passes 0; vexide passes
 *                  its own offset through.
 * @param   nLength Number of LEDs to write. PROS and vexide both cap strips
 *                  at 64.
 * @param   options Option flags. VEX does not publish their meaning; PROS and
 *                  vexide pass 0.
 * @return  Not documented by VEX. PROS passes the value through unchanged.
 * @details PROS and vexide configure the channel as #kAdiPortTypeDigitalOut
 *          first. The function is absent from the published v5_api.h, so the
 *          PROS kernel declares it itself in devices/vdml_ext_adi.c.
 */
int32_t vexDeviceAdiAddrLedSet(V5_DeviceT device, uint32_t port,
                               uint32_t *pData, uint32_t nOffset,
                               uint32_t nLength, uint32_t options);

// Bumper switch

/**
 * @brief   Reads a smart port bumper switch.
 * @param   device Device handle.
 * @return  Whether the switch is pressed.
 * @details This is for a smart port bumper (#kDeviceTypeBumperSensor). No
 *          public runtime calls it.
 */
V5_DeviceBumperState vexDeviceBumperGet(V5_DeviceT device);

// Gyro - obsolete, there is currently no dedicated gyro

/**
 * @brief   Resets a dedicated smart port gyro.
 * @param   device Device handle.
 * @details Obsolete: VEX's header notes there is no dedicated gyro. No public
 *          runtime calls it.
 */
void vexDeviceGyroReset(V5_DeviceT device);

/**
 * @brief   Reads a dedicated smart port gyro's heading.
 * @param   device Device handle.
 * @return  Not documented.
 * @details Obsolete, as for vexDeviceGyroReset().
 */
double vexDeviceGyroHeadingGet(V5_DeviceT device);

/**
 * @brief   Reads a dedicated smart port gyro's angle.
 * @param   device Device handle.
 * @return  Not documented.
 * @details Obsolete, as for vexDeviceGyroReset().
 */
double vexDeviceGyroDegreesGet(V5_DeviceT device);

// Sonar - obsolete, there is currently no dedicated gyro

/**
 * @brief   Reads a dedicated smart port sonar.
 * @param   device Device handle.
 * @return  Not documented.
 * @details Obsolete: VEX's header marks it so. No public runtime calls it. A
 *          3-Wire ultrasonic is read through vexDeviceAdiValueGet() with the
 *          channel configured as #kAdiPortTypeSonar.
 */
int32_t vexDeviceSonarValueGet(V5_DeviceT device);

// Generic sensor - (who knows !)

/**
 * @brief   Reads a device VEXos classifies as a generic sensor.
 * @param   device Device handle.
 * @return  The sensor's reading, or 0 if VEXos does not consider the device
 *          a generic sensor.
 * @details This is not a way to read any device with one call. A Distance
 *          Sensor, for example, is not a generic sensor and reads 0.
 */
int32_t vexDeviceGenericValueGet(V5_DeviceT device);

// Motor

/**
 * @brief   Spins a motor at a target velocity.
 * @param   device   Device handle.
 * @param   velocity Target velocity in RPM. PROS documents the range as
 *                   +-100, +-200 or +-600 depending on the gear cartridge.
 * @details The velocity is held by the motor's internal PID loop, as opposed
 *          to setting a voltage. A velocity of 0 stops the motor according to
 *          its brake mode, which is how both PROS and vexide brake a motor.
 */
void vexDeviceMotorVelocitySet(V5_DeviceT device, int32_t velocity);

/**
 * @brief   Changes the velocity of a profiled move already in progress.
 * @param   device   Device handle.
 * @param   velocity New velocity in RPM.
 * @details Only affects a profiled move started by
 *          vexDeviceMotorAbsoluteTargetSet() or
 *          vexDeviceMotorRelativeTargetSet(), and has no effect otherwise.
 */
void vexDeviceMotorVelocityUpdate(V5_DeviceT device, int32_t velocity);

/**
 * @brief   Drives a motor at a given voltage.
 * @param   device Device handle.
 * @param   value  Voltage in millivolts, -12000 to 12000. vexide caps the
 *                 5.5W EXP motor at 8000.
 * @details Does not respect the brake mode; the voltage is simply set.
 */
void vexDeviceMotorVoltageSet(V5_DeviceT device, int32_t value);

/**
 * @brief   Reads back a motor's commanded velocity.
 * @param   device Device handle.
 * @return  The velocity last commanded, in RPM.
 * @details This is the commanded value, not the measured one. For the
 *          measured speed use vexDeviceMotorActualVelocityGet().
 */
int32_t vexDeviceMotorVelocityGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's measured velocity.
 * @param   device Device handle.
 * @return  The measured velocity in RPM.
 * @details vexide warns that the motor's estimate may be noisy or inaccurate,
 *          especially at high speeds.
 */
double vexDeviceMotorActualVelocityGet(V5_DeviceT device);

/**
 * @brief   Reads which way a motor is turning.
 * @param   device Device handle.
 * @return  1 when moving in the positive direction, -1 when moving in the
 *          negative direction.
 */
int32_t vexDeviceMotorDirectionGet(V5_DeviceT device);

/**
 * @brief   Sets a motor's control mode directly.
 * @param   device Device handle.
 * @param   mode   Control mode to switch to.
 * @details No public runtime calls this.
 */
void vexDeviceMotorModeSet(V5_DeviceT device, V5MotorControlMode mode);

/**
 * @brief   Reads a motor's current control mode.
 * @param   device Device handle.
 * @return  The control mode.
 * @details No public runtime calls this.
 */
V5MotorControlMode vexDeviceMotorModeGet(V5_DeviceT device);

/**
 * @brief   Sets a motor's raw PWM output.
 * @param   device Device handle.
 * @param   value  PWM command. The accepted range is not documented.
 * @details No public runtime calls this; PROS and vexide drive motors through
 *          voltage, velocity or position instead.
 */
void vexDeviceMotorPwmSet(V5_DeviceT device, int32_t value);

/**
 * @brief   Reads back a motor's raw PWM output.
 * @param   device Device handle.
 * @return  The current PWM command.
 * @details No public runtime calls this.
 */
int32_t vexDeviceMotorPwmGet(V5_DeviceT device);

/**
 * @brief   Sets a motor's current limit.
 * @param   device Device handle.
 * @param   value  Current limit in milliamps.
 * @details PROS documents the default as 2500 mA. vexide notes that with more
 *          than eight 11W motors plugged in, VEXos lowers the default limit of
 *          each motor, taking any limits set here into account.
 */
void vexDeviceMotorCurrentLimitSet(V5_DeviceT device, int32_t value);

/**
 * @brief   Reads back a motor's current limit.
 * @param   device Device handle.
 * @return  The current limit in milliamps.
 */
int32_t vexDeviceMotorCurrentLimitGet(V5_DeviceT device);

/**
 * @brief   Sets a motor's voltage limit.
 * @param   device Device handle.
 * @param   value  Voltage limit. vexide passes millivolts; PROS documents its
 *                 wrapper's argument as volts but passes it through unchanged,
 *                 so the sources disagree.
 * @details PROS documents a limit of 0 as meaning no limit, which is the
 *          default.
 */
void vexDeviceMotorVoltageLimitSet(V5_DeviceT device, int32_t value);

/**
 * @brief   Reads back a motor's voltage limit.
 * @param   device Device handle.
 * @return  The voltage limit, in the same unit as
 *          vexDeviceMotorVoltageLimitSet(). 0 by default, meaning no limit.
 */
int32_t vexDeviceMotorVoltageLimitGet(V5_DeviceT device);

/**
 * @brief   Replaces the gains in a motor's position control loop.
 * @param   device Device handle.
 * @param   pid    Gain set to install. The gains are 4.4 fixed point, so 0x20
 *                 means 2.0.
 * @details VEX has not disclosed the motor's default gains. vexide puts this
 *          behind a feature flag and warns that bad values can permanently
 *          damage a motor.
 */
void vexDeviceMotorPositionPidSet(V5_DeviceT device, V5_DeviceMotorPid *pid);

/**
 * @brief   Replaces the gains in a motor's velocity control loop.
 * @param   device Device handle.
 * @param   pid    Gain set to install, in the same format as for
 *                 vexDeviceMotorPositionPidSet().
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
 * @brief   Reads the voltage delivered to a motor.
 * @param   device Device handle.
 * @return  The voltage in millivolts.
 */
int32_t vexDeviceMotorVoltageGet(V5_DeviceT device);

/**
 * @brief   Reads the power a motor draws.
 * @param   device Device handle.
 * @return  The power in watts.
 */
double vexDeviceMotorPowerGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's output torque.
 * @param   device Device handle.
 * @return  The torque in newton metres.
 */
double vexDeviceMotorTorqueGet(V5_DeviceT device);

/**
 * @brief   Reads how efficiently a motor is running.
 * @param   device Device handle.
 * @return  The efficiency as a percentage.
 * @details 100 percent means the motor is moving while drawing no electrical
 *          power, and 0 percent means it is drawing power but not moving.
 */
double vexDeviceMotorEfficiencyGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's internal temperature.
 * @param   device Device handle.
 * @return  The temperature in degrees Celsius.
 * @details vexide documents the resolution as 5 degrees.
 */
double vexDeviceMotorTemperatureGet(V5_DeviceT device);

/**
 * @brief   Reports whether a motor's temperature is above its limit.
 * @param   device Device handle.
 * @return  True when the temperature limit is exceeded.
 */
bool vexDeviceMotorOverTempFlagGet(V5_DeviceT device);

/**
 * @brief   Reports whether a motor is drawing more than its current limit.
 * @param   device Device handle.
 * @return  True when the current limit is exceeded.
 */
bool vexDeviceMotorCurrentLimitFlagGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's fault bits.
 * @param   device Device handle.
 * @return  A bit field of faults: 0x01 over temperature, 0x02 h-bridge
 *          fault, 0x04 over current, 0x08 h-bridge over current. Zero means
 *          no faults.
 */
uint32_t vexDeviceMotorFaultsGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's zero velocity flag.
 * @param   device Device handle.
 * @return  The flag.
 * @details No public runtime calls this. PROS exposes the zero velocity bit of
 *          vexDeviceMotorFlagsGet() instead.
 */
bool vexDeviceMotorZeroVelocityFlagGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's zero position flag.
 * @param   device Device handle.
 * @return  The flag.
 * @details No public runtime calls this. PROS exposes the zero position bit of
 *          vexDeviceMotorFlagsGet() instead.
 */
bool vexDeviceMotorZeroPositionFlagGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's status bits.
 * @param   device Device handle.
 * @return  A bit field: 0x01 busy, meaning the brain cannot currently
 *          communicate with the motor, 0x02 zero velocity and 0x04 zero
 *          position. Zero means none of those apply.
 */
uint32_t vexDeviceMotorFlagsGet(V5_DeviceT device);

/**
 * @brief   Reverses the sense of a motor.
 * @param   device Device handle.
 * @param   value  True to reverse.
 * @details Not reversed, positive velocity and voltage commands turn the motor
 *          the way the arrow on its back points, and the position increases
 *          that way. Reversed, both go against the arrow.
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
 * @param   units  Degrees, rotations or encoder counts.
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
 * @brief   Chooses what a motor does when it is stopped.
 * @param   device Device handle.
 * @param   mode   Coast, brake or hold.
 * @details vexide describes coast as never braking, brake as regenerative
 *          braking, and hold as exerting force to stay in position. The mode
 *          takes effect when the motor is commanded to a velocity of 0.
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
 * @details Does not move the motor. Absolute targets set afterwards are
 *          measured against the new reference.
 */
void vexDeviceMotorPositionSet(V5_DeviceT device, double position);

/**
 * @brief   Reads a motor's position.
 * @param   device Device handle.
 * @return  The position in the configured encoder units.
 * @details Depends on the configured gear cartridge, so it is wrong if the
 *          cartridge set with vexDeviceMotorGearingSet() does not match the
 *          one fitted.
 */
double vexDeviceMotorPositionGet(V5_DeviceT device);

/**
 * @brief   Reads a motor's raw encoder count along with its timestamp.
 * @param   device    Device handle.
 * @param   timestamp Filled in with the time the count was read, in
 *                    milliseconds. May be null.
 * @return  The raw encoder count.
 * @details vexide documents the count as 4096 ticks per revolution of the
 *          motor before the gear cartridge, unaffected by the configured
 *          cartridge and by vexDeviceMotorPositionSet() or
 *          vexDeviceMotorPositionReset().
 */
int32_t vexDeviceMotorPositionRawGet(V5_DeviceT device, uint32_t *timestamp);

/**
 * @brief   Zeroes a motor's position reading.
 * @param   device Device handle.
 * @details Does not move the motor. Equivalent to vexDeviceMotorPositionSet()
 *          with a position of 0.
 */
void vexDeviceMotorPositionReset(V5_DeviceT device);

/**
 * @brief   Reads a motor's target position.
 * @param   device Device handle.
 * @return  The target position set by the user, in the configured encoder
 *          units.
 */
double vexDeviceMotorTargetGet(V5_DeviceT device);

/**
 * @brief   Sets a servo style position target.
 * @param   device   Device handle.
 * @param   position Target position in the configured encoder units.
 * @details No public runtime calls this, and VEX does not document how it
 *          differs from vexDeviceMotorAbsoluteTargetSet().
 */
void vexDeviceMotorServoTargetSet(V5_DeviceT device, double position);

/**
 * @brief   Starts a profiled move to an absolute position.
 * @param   device   Device handle.
 * @param   position Target position in the configured encoder units.
 * @param   velocity Velocity for the move, in RPM.
 * @details The position is measured from where the motor was at startup or
 *          at the last vexDeviceMotorPositionSet() or
 *          vexDeviceMotorPositionReset(). The call only sets the target and
 *          returns straight away.
 */
void vexDeviceMotorAbsoluteTargetSet(V5_DeviceT device, double position,
                                     int32_t velocity);

/**
 * @brief   Starts a profiled move relative to the current position.
 * @param   device   Device handle.
 * @param   position Distance to travel in the configured encoder units.
 * @param   velocity Velocity for the move, in RPM.
 * @details Behaves like vexDeviceMotorAbsoluteTargetSet(), except the
 *          target is measured from the motor's current position.
 */
void vexDeviceMotorRelativeTargetSet(V5_DeviceT device, double position,
                                     int32_t velocity);

/**
 * @brief   Tells a motor which gear cartridge is fitted.
 * @param   device Device handle.
 * @param   value  Cartridge ratio.
 * @details Position readings depend on it, so getting it wrong makes them
 *          wrong.
 */
void vexDeviceMotorGearingSet(V5_DeviceT device, V5MotorGearset value);

/**
 * @brief   Reads back the cartridge a motor is configured for.
 * @param   device Device handle.
 * @return  The configured cartridge ratio.
 */
V5MotorGearset vexDeviceMotorGearingGet(V5_DeviceT device);

/**
 * @brief   Reads which kind of smart motor is on a port.
 * @param   device Device handle.
 * @return  0 for the 11W V5 motor and 1 for the 5.5W EXP motor.
 * @details PROS returns the value unchanged as motor_type_e_t.
 */
int32_t vexDeviceMotorTypeGet(V5_DeviceT device);

/**
 * @brief   Feeds a motor a point of an externally generated motion profile.
 * @param   device   Device handle.
 * @param   position Position setpoint.
 * @param   velocity Velocity setpoint.
 * @details No public runtime calls this, and VEX does not document it beyond
 *          its name.
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
 * @details Call this before vexDeviceVisionObjectGet(). Objects are roughly
 *          ordered by size: index 0 is the largest, 1 the second largest,
 *          and so on.
 */
int32_t vexDeviceVisionObjectCountGet(V5_DeviceT device);

/**
 * @brief   Reads one detected object.
 * @param   device   Device handle.
 * @param   indexObj Zero based object index, below the current object count.
 *                   0 is the largest object, roughly.
 * @param   pObject  Structure filled in with the object's data.
 * @return  0 if the object could not be read. PROS and vexide both treat 0 as
 *          failure.
 */
int32_t vexDeviceVisionObjectGet(V5_DeviceT device, uint32_t indexObj,
                                 V5_DeviceVisionObject *pObject);

/**
 * @brief   Writes a color signature to the Vision Sensor.
 * @param   device     Device handle.
 * @param   pSignature Signature to store, with its id set to the target slot,
 *                     1 to 7.
 * @details Signatures are stored in the sensor's volatile memory, so they are
 *          lost when it loses power.
 */
void vexDeviceVisionSignatureSet(V5_DeviceT device,
                                 V5_DeviceVisionSignature *pSignature);

/**
 * @brief   Reads a color signature back from the Vision Sensor.
 * @param   device     Device handle.
 * @param   id         Signature slot, 1 to 7.
 * @param   pSignature Structure filled in with the stored signature.
 * @return  False if no signature could be read.
 * @details PROS and vexide also treat the read as failed unless the flags
 *          field is non-zero; PROS documents it as set to 1 when the data is
 *          valid. See #VISION_SIG_FLAG_STATUS.
 */
bool vexDeviceVisionSignatureGet(V5_DeviceT device, uint32_t id,
                                 V5_DeviceVisionSignature *pSignature);

/**
 * @brief   Sets the Vision Sensor's brightness.
 * @param   device  Device handle.
 * @param   percent Brightness, 0 to 100.
 * @details PROS exposes this as an exposure setting from 0 to 150, which it
 *          converts to this scale with a formula it attributes to VEX.
 */
void vexDeviceVisionBrightnessSet(V5_DeviceT device, uint8_t percent);

/**
 * @brief   Reads back the Vision Sensor's brightness.
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
 * @brief   Sets the manual white balance color.
 * @param   device Device handle.
 * @param   color  White balance color. PROS and vexide set the brightness
 *                 member to 255; vexide doubts it has any effect.
 * @details PROS and vexide switch the sensor to #kVisionWBManual before
 *          calling this.
 */
void vexDeviceVisionWhiteBalanceSet(V5_DeviceT device,
                                    V5_DeviceVisionRgb color);

/**
 * @brief   Reads back the manual white balance color.
 * @param   device Device handle.
 * @return  The white balance color.
 */
V5_DeviceVisionRgb vexDeviceVisionWhiteBalanceGet(V5_DeviceT device);

/**
 * @brief   Chooses who drives the Vision Sensor's LED.
 * @param   device Device handle.
 * @param   mode   Automatic or manual.
 * @details In automatic mode the LED shows the color of the most prominent
 *          detected object's signature.
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
 * @param   percent Brightness. The scale is not documented.
 * @details No public runtime calls this. The misspelt name is the one VEX's
 *          header and runtime library use.
 */
void vexDeviceVisionLedBrigntnessSet(V5_DeviceT device, uint8_t percent);

/**
 * @brief   Reads back the Vision Sensor's LED brightness.
 * @param   device Device handle.
 * @return  The brightness. The scale is not documented.
 * @details No public runtime calls this. The misspelt name is the one VEX's
 *          header and runtime library use.
 */
uint8_t vexDeviceVisionLedBrigntnessGet(V5_DeviceT device);

/**
 * @brief   Sets the Vision Sensor's LED color.
 * @param   device Device handle.
 * @param   color  Color to show. The scale of the brightness member is
 *                 disputed: PROS sets it to 255, while vexide writes 0 to 100.
 * @details PROS and vexide switch the LED to #kVisionLedModeManual before
 *          calling this.
 */
void vexDeviceVisionLedColorSet(V5_DeviceT device, V5_DeviceVisionRgb color);

/**
 * @brief   Reads back the Vision Sensor's LED color.
 * @param   device Device handle.
 * @return  The LED color.
 */
V5_DeviceVisionRgb vexDeviceVisionLedColorGet(V5_DeviceT device);

/**
 * @brief   Turns the Vision Sensor's Wi-Fi mode on or off.
 * @param   device Device handle.
 * @param   mode   On or off.
 * @details Wi-Fi mode disables all object detection and starts the sensor's
 *          own Wi-Fi network, which serves the camera feed. vexide documents
 *          that it is turned off automatically under field control.
 */
void vexDeviceVisionWifiModeSet(V5_DeviceT device, V5VisionWifiMode mode);

/**
 * @brief   Reads back the Vision Sensor's Wi-Fi mode.
 * @param   device Device handle.
 * @return  The current mode.
 */
V5VisionWifiMode vexDeviceVisionWifiModeGet(V5_DeviceT device);

// AI Vision sensor

/**
 * @brief   Resets the AI Vision Sensor to its initial state.
 * @param   device Device handle.
 * @details PROS calls this for its reset. vexide instead resets the sensor by
 *          writing the reset flag (bit 30) with vexDeviceAiVisionModeSet().
 *          VEX does not document which settings a reset clears.
 */
void vexDeviceAiVisionReset(V5_DeviceT device);

/**
 * @brief   Turns individual kinds of detection on or off.
 * @param   device  Device handle.
 * @param   bits    New state for the kinds selected by bitmask.
 * @param   bitmask Which kinds of detection the call applies to.
 * @details Only the kinds selected by bitmask change, so one kind can be
 *          turned on or off without disturbing the others. PROS documents the
 *          bit positions as aivision_mode_type_e_t: bit 0 AprilTags, bit 1
 *          colors, bit 2 AI model objects and bit 4 color merging.
 */
void vexDeviceAiVisionEnableSet(V5_DeviceT device, uint8_t bits,
                                uint8_t bitmask);

/**
 * @brief   Reads back which kinds of detection are enabled.
 * @param   device Device handle.
 * @return  A bit field in the same layout as the bits argument of
 *          vexDeviceAiVisionEnableSet().
 */
uint8_t vexDeviceAiVisionEnableGet(V5_DeviceT device);

/**
 * @brief   Writes the AI Vision Sensor's mode word.
 * @param   device Device handle.
 * @param   mode   Packed mode word.
 * @details The word carries several settings, each with a flag bit that marks
 *          it as being written. vexide uses bit 25 for the flags byte in bits
 *          8 to 15 (including the USB overlay), bit 29 for the AprilTag family
 *          in bits 16 to 23, bit 27 to start auto white balance, bit 26 for a
 *          test mode of unknown use, and bit 30 to reset the sensor. PROS uses
 *          the same bits 25, 27 and 29.
 *
 *          Runtimes read the current settings back with
 *          vexDeviceAiVisionStatusGet() before writing. vexide documents the
 *          status word as the mode word shifted right by 8 bits, and shifts it
 *          back left before writing.
 */
void vexDeviceAiVisionModeSet(V5_DeviceT device, uint32_t mode);

/**
 * @brief   Reads the AI Vision Sensor's status word.
 * @param   device Device handle.
 * @return  The status word. vexide documents it as the mode word shifted
 *          right by 8 bits, so its lowest byte holds the flags byte.
 * @see     vexDeviceAiVisionModeSet
 */
uint32_t vexDeviceAiVisionStatusGet(V5_DeviceT device);

/**
 * @brief   Counts the objects the AI Vision Sensor is currently reporting.
 * @param   device Device handle.
 * @return  The number of objects available. vexide documents the maximum as
 *          24.
 * @details Call this before vexDeviceAiVisionObjectGet().
 */
int32_t vexDeviceAiVisionObjectCountGet(V5_DeviceT device);

/**
 * @brief   Reads one detected object.
 * @param   device   Device handle.
 * @param   indexObj Zero based object index, below the current object count.
 * @param   pObject  Structure filled in with the object's data.
 * @return  Not documented. PROS and vexide ignore it.
 * @details The object's type decides which member of the returned union holds
 *          its geometry: the color member for color and code objects, the
 *          model member for AI model objects and the tag member for
 *          AprilTags.
 */
int32_t vexDeviceAiVisionObjectGet(V5_DeviceT device, uint32_t indexObj,
                                   V5_DeviceAiVisionObject *pObject);

/**
 * @brief   Writes a color descriptor to the AI Vision Sensor.
 * @param   device Device handle.
 * @param   pColor Descriptor to store, with its id set to the target slot,
 *                 1 to 7.
 * @details Writing a slot that already holds a descriptor overwrites it. Color
 *          detection has to be enabled separately with
 *          vexDeviceAiVisionEnableSet().
 */
void vexDeviceAiVisionColorSet(V5_DeviceT device,
                               V5_DeviceAiVisionColor *pColor);

/**
 * @brief   Reads a color descriptor back from the AI Vision Sensor.
 * @param   device Device handle.
 * @param   id     Descriptor slot, 1 to 7.
 * @param   pColor Structure filled in with the stored descriptor.
 * @return  True if the descriptor was read. vexide treats false as an empty
 *          slot.
 * @details PROS documents reading a slot that has never been written as
 *          undefined behaviour.
 */
bool vexDeviceAiVisionColorGet(V5_DeviceT device, uint32_t id,
                               V5_DeviceAiVisionColor *pColor);

/**
 * @brief   Writes a code descriptor to the AI Vision Sensor.
 * @param   device Device handle.
 * @param   pCode  Descriptor to store, with its id set to the target slot.
 *                 PROS documents slots 1 to 5, vexide accepts 1 to 8.
 * @details A code merges nearby objects matching its color descriptors into
 *          one object.
 */
void vexDeviceAiVisionCodeSet(V5_DeviceT device, V5_DeviceAiVisionCode *pCode);

/**
 * @brief   Reads a code descriptor back from the AI Vision Sensor.
 * @param   device Device handle.
 * @param   id     Descriptor slot; see vexDeviceAiVisionCodeSet().
 * @param   pCode  Structure filled in with the stored descriptor.
 * @return  True if the descriptor was read. vexide treats false as an empty
 *          slot.
 */
bool vexDeviceAiVisionCodeGet(V5_DeviceT device, uint32_t id,
                              V5_DeviceAiVisionCode *pCode);

/**
 * @brief   Reads the name of one of the AI model's classes.
 * @param   device Device handle.
 * @param   id     Class index, 0 to 19, or -1 to ask how many classes there
 *                 are.
 * @param   pName  Buffer of at least 20 bytes for the name.
 * @return  The number of classes when id is -1, otherwise undefined.
 * @details PROS documents that the sensor may not report class names
 *          correctly for the first several hundred milliseconds after it is
 *          plugged in.
 */
int32_t vexDeviceAiVisionClassNameGet(V5_DeviceT device, int32_t id,
                                      uint8_t *pName);

/**
 * @brief   Reads the AI Vision Sensor's temperature.
 * @param   device Device handle.
 * @return  The temperature in degrees Celsius.
 */
double vexDeviceAiVisionTemperatureGet(V5_DeviceT device);

// IMU

/**
 * @brief   Starts calibrating the Inertial Sensor.
 * @param   device Device handle.
 * @details Returns immediately. PROS documents calibration as taking about two
 *          seconds, and vexide warns that readings are unpredictable if the
 *          sensor moves during it. Watch bit 0 of vexDeviceImuStatusGet() to
 *          tell when it has finished. The bit is not set the instant this call
 *          returns, so both runtimes wait for it to go high (allowing up to
 *          1 s) before waiting for it to go low again (allowing up to 3 s).
 *
 *          VEXos also starts a calibration by itself when the sensor regains
 *          power. vexide notes that calling this while a calibration is
 *          already running has caused bugs, so it waits for that one instead.
 */
void vexDeviceImuReset(V5_DeviceT device);

/**
 * @brief   Reads the Inertial Sensor's accumulated rotation about the z axis.
 * @param   device Device handle.
 * @return  The rotation in degrees, unbounded, clockwise positive.
 */
double vexDeviceImuHeadingGet(V5_DeviceT device);

/**
 * @brief   Reads the Inertial Sensor's heading.
 * @param   device Device handle.
 * @return  The heading in degrees, 0 up to but not including 360, clockwise
 *          positive.
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
 * @param   data   Structure filled in with x, y and z in degrees per second.
 *                 The w member is not used.
 */
void vexDeviceImuRawGyroGet(V5_DeviceT device, V5_DeviceImuRaw *data);

/**
 * @brief   Reads the Inertial Sensor's raw accelerations.
 * @param   device Device handle.
 * @param   data   Structure filled in with x, y and z in g. The w member is
 *                 not used.
 */
void vexDeviceImuRawAccelGet(V5_DeviceT device, V5_DeviceImuRaw *data);

/**
 * @brief   Reads the Inertial Sensor's status bits.
 * @param   device Device handle.
 * @return  A bit field: bit 0 is set while calibrating, bits 1 to 3 hold the
 *          physical orientation measured at calibration, and bit 4 is set if
 *          VEXos calibrated the sensor automatically. vexide waits for a
 *          non-zero value at startup, before VEXos has the sensor's first
 *          packet.
 */
uint32_t vexDeviceImuStatusGet(V5_DeviceT device);

/**
 * @brief   Sets the Inertial Sensor's mode word.
 * @param   device Device handle.
 * @param   mode   Mode word.
 * @details No public runtime calls this. VEX's header defines
 *          #V5ImuOrientationMode, #V5ImuQuaternionMode and #_V5ImuHeadingnMode
 *          values in separate bits, which suggests they are combined with a
 *          bitwise OR, but VEX does not document the word.
 */
void vexDeviceImuModeSet(V5_DeviceT device, uint32_t mode);

/**
 * @brief   Reads back the Inertial Sensor's mode word.
 * @param   device Device handle.
 * @return  The mode word.
 * @details No public runtime calls this.
 */
uint32_t vexDeviceImuModeGet(V5_DeviceT device);

/**
 * @brief   Sets how often the Inertial Sensor produces new readings.
 * @param   device Device handle.
 * @param   rate   Interval in milliseconds, in steps of 5 and at least 5.
 *                 The default is 10.
 * @details Readings are only copied into the shared buffer every 10 ms, so a
 *          shorter interval does not let you poll faster. It does make the
 *          data as recent as possible whenever you do poll.
 */
void vexDeviceImuDataRateSet(V5_DeviceT device, uint32_t rate);

// Rangefinder/Lidar - actual API to be determined

/**
 * @brief   Reads a rangefinder or lidar device.
 * @param   device Device handle.
 * @return  Not documented.
 * @details VEX's header notes the API is still to be determined. No public
 *          runtime calls this.
 */
int32_t vexDeviceRangeValueGet(V5_DeviceT device);

// Absolute encoder

/**
 * @brief   Resets the Rotation Sensor's position to its angle.
 * @param   device Device handle.
 * @details This does not zero the position. It sets the position to the
 *          sensor's current absolute angle. To zero it, call
 *          vexDeviceAbsEncPositionSet() with 0, as PROS and vexide do.
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
 * @return  The position in centidegrees, so 36000 per turn. Unbounded.
 * @details Lost if the sensor loses power.
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
 * @return  The angle in centidegrees. PROS documents the range as 0 to 36000.
 * @details Unlike the position, this wraps at a full turn and is kept across
 *          a power cycle.
 */
int32_t vexDeviceAbsEncAngleGet(V5_DeviceT device);

/**
 * @brief   Reverses the Rotation Sensor's direction.
 * @param   device Device handle.
 * @param   value  True to reverse.
 * @details vexide documents that reversing turns the angle into 36000 minus
 *          the angle, while the position keeps its current value and only
 *          future movement is counted the other way. It also documents a race:
 *          reversing before the sensor has reported its first position makes
 *          the position start at 36000. PROS avoids this by waiting for
 *          vexDeviceAbsEncStatusGet() to become non-zero before setting the
 *          flag.
 */
void vexDeviceAbsEncReverseFlagSet(V5_DeviceT device, bool value);

/**
 * @brief   Reads back the Rotation Sensor's reverse flag.
 * @param   device Device handle.
 * @return  True if the sensor is reversed.
 */
bool vexDeviceAbsEncReverseFlagGet(V5_DeviceT device);

/**
 * @brief   Reads the Rotation Sensor's status word.
 * @param   device Device handle.
 * @return  The status word. PROS treats 0 as the sensor not yet being ready.
 *          VEX does not publish the meaning of the individual bits.
 */
uint32_t vexDeviceAbsEncStatusGet(V5_DeviceT device);

/**
 * @brief   Sets how often the Rotation Sensor produces new readings.
 * @param   device Device handle.
 * @param   rate   Interval in milliseconds, in steps of 5 and at least 5.
 *                 The default is 10.
 * @details The brain still only reads the sensor every 10 ms.
 */
void vexDeviceAbsEncDataRateSet(V5_DeviceT device, uint32_t rate);

// Optical/color sensor

/**
 * @brief   Reads the hue of the color in front of the Optical Sensor.
 * @param   device Device handle.
 * @return  The hue in degrees, 0 to 359.999.
 * @details PROS documents this as unavailable while gesture detection is
 *          enabled. vexide reports that hardware testing shows otherwise.
 */
double vexDeviceOpticalHueGet(V5_DeviceT device);

/**
 * @brief   Reads the saturation of the color in front of the Optical Sensor.
 * @param   device Device handle.
 * @return  The saturation, 0 to 1.
 * @details The same caveat as vexDeviceOpticalHueGet() applies.
 */
double vexDeviceOpticalSatGet(V5_DeviceT device);

/**
 * @brief   Reads the brightness of the color in front of the Optical Sensor.
 * @param   device Device handle.
 * @return  The brightness, 0 to 1.
 * @details The same caveat as vexDeviceOpticalHueGet() applies.
 */
double vexDeviceOpticalBrightnessGet(V5_DeviceT device);

/**
 * @brief   Reads the Optical Sensor's proximity reading.
 * @param   device Device handle.
 * @return  The proximity, 0 to 255, larger meaning closer.
 * @details Based on reflected light intensity, so it depends on the ambient
 *          light and on how reflective the target is. The same caveat as
 *          vexDeviceOpticalHueGet() applies.
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
 */
void vexDeviceOpticalLedPwmSet(V5_DeviceT device, int32_t value);

/**
 * @brief   Reads back the Optical Sensor's LED brightness.
 * @param   device Device handle.
 * @return  The brightness, 0 to 100.
 */
int32_t vexDeviceOpticalLedPwmGet(V5_DeviceT device);

/**
 * @brief   Reads the Optical Sensor's status word.
 * @param   device Device handle.
 * @return  The status word. VEX does not publish the meaning of the
 *          individual bits.
 */
uint32_t vexDeviceOpticalStatusGet(V5_DeviceT device);

/**
 * @brief   Reads the Optical Sensor's unprocessed channel values.
 * @param   device Device handle.
 * @param   data   Structure filled in with the clear, red, green and blue
 *                 values.
 */
void vexDeviceOpticalRawGet(V5_DeviceT device, V5_DeviceOpticalRaw *data);

/**
 * @brief   Sets the Optical Sensor's operating mode.
 * @param   device Device handle.
 * @param   mode   Mode value.
 * @details No public runtime calls this, and VEX does not publish the mode
 *          values. Gesture detection is switched with
 *          vexDeviceOpticalGestureEnable() and
 *          vexDeviceOpticalGestureDisable().
 */
void vexDeviceOpticalModeSet(V5_DeviceT device, uint32_t mode);

/**
 * @brief   Reads back the Optical Sensor's operating mode.
 * @param   device Device handle.
 * @return  The mode value.
 * @details No public runtime calls this.
 */
uint32_t vexDeviceOpticalModeGet(V5_DeviceT device);

/**
 * @brief   Reads the most recent gesture from the Optical Sensor.
 * @param   device Device handle.
 * @param   pData  Structure filled in with the raw gesture data, or null if
 *                 only the direction is wanted.
 * @return  The recognised direction: 0 for none, 1 up and 2 down. PROS
 *          documents 3 as right and 4 as left; vexide maps them the other way
 *          round.
 * @details Only meaningful while gesture detection is enabled. PROS documents
 *          that a gesture is cleared 500 ms after it is recognised.
 */
uint32_t vexDeviceOpticalGestureGet(V5_DeviceT, V5_DeviceOpticalGesture *pData);

/**
 * @brief   Enables gesture detection on the Optical Sensor.
 * @param   device Device handle.
 * @details PROS documents the hue, saturation, brightness and proximity
 *          readings as unavailable while gesture detection is on. vexide
 *          reports that hardware testing shows otherwise.
 */
void vexDeviceOpticalGestureEnable(V5_DeviceT);

/**
 * @brief   Disables gesture detection on the Optical Sensor.
 * @param   device Device handle.
 */
void vexDeviceOpticalGestureDisable(V5_DeviceT);

/**
 * @brief   Sets a proximity threshold.
 * @param   device Device handle.
 * @param   value  Threshold.
 * @return  Not documented.
 * @details No public runtime calls this, and VEX does not document what the
 *          threshold is used for.
 */
int32_t vexDeviceOpticalProximityThreshold(V5_DeviceT device, int32_t value);

/**
 * @brief   Sets how long the Optical Sensor integrates light for.
 * @param   device Device handle.
 * @param   timeMs Integration time in milliseconds, 3 to 712. The default is
 *                 103.
 * @details PROS calls this the update rate. A shorter integration time gives
 *          faster updates but lower accuracy, because less light is
 *          collected.
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
 * @brief   Energises the Workcell electromagnet for a set time.
 * @param   device Device handle.
 * @param   value  Power level, -100 to 100. Larger values attract more
 *                 strongly.
 * @param   time   How long to stay energised, in milliseconds. vexide documents
 *                 a maximum of 2 seconds.
 * @details The duration is part of the command rather than something the
 *          caller times. VEX does not document what negative power does or
 *          what happens when the time elapses.
 */
void vexDeviceMagnetPowerSet(V5_DeviceT device, int32_t value, int32_t time);

/**
 * @brief   Reads back the electromagnet's power level.
 * @param   device Device handle.
 * @return  The power level last set, -100 to 100.
 */
int32_t vexDeviceMagnetPowerGet(V5_DeviceT device);

/**
 * @brief   Runs the electromagnet's pickup sequence.
 * @param   device   Device handle.
 * @param   duration One of the V5_DeviceMagnetDuration presets.
 * @details Neither PROS nor vexide calls this, and VEX does not document what
 *          the sequence does or how long each preset lasts.
 */
void vexDeviceMagnetPickup(V5_DeviceT device, V5_DeviceMagnetDuration duration);

/**
 * @brief   Runs the electromagnet's drop sequence.
 * @param   device   Device handle.
 * @param   duration One of the V5_DeviceMagnetDuration presets.
 * @details Neither PROS nor vexide calls this; see vexDeviceMagnetPickup().
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
 * @return  The current in milliamps. vexide divides it by 1000 to report amps.
 */
double vexDeviceMagnetCurrentGet(V5_DeviceT device);

/**
 * @brief   Reads the electromagnet's status code.
 * @param   device Device handle.
 * @return  The status code. VEX does not publish its meaning.
 */
uint32_t vexDeviceMagnetStatusGet(V5_DeviceT device);

// Distance

/**
 * @brief   Reads the distance to the object in front of the sensor.
 * @param   device Device handle.
 * @return  The distance in millimetres, or 9999 if no object was found.
 * @details vexide documents the sensor's range as 20 mm to 2000 mm, accurate
 *          to about 15 mm below 200 mm and about 5 percent above it. The
 *          sensor takes 10 to 100 ms to start up, so check
 *          vexDeviceDistanceStatusGet() before trusting the reading.
 */
uint32_t vexDeviceDistanceDistanceGet(V5_DeviceT device);

/**
 * @brief   Reads how much the sensor trusts its distance reading.
 * @param   device Device handle.
 * @return  The confidence, 0 through 63, larger being better.
 * @details PROS documents confidence as only available beyond 200 mm, with
 *          a value of 10 returned closer in.
 */
uint32_t vexDeviceDistanceConfidenceGet(V5_DeviceT device);

/**
 * @brief   Reads the sensor's guess at the detected object's relative size.
 * @param   device Device handle.
 * @return  A unitless size, 0 through 400. An 18 by 30 inch grey card reads
 *          about 75 in typical room lighting.
 * @details vexide notes that it is unknown what the sensor actually measures
 *          here.
 */
int32_t vexDeviceDistanceObjectSizeGet(V5_DeviceT device);

/**
 * @brief   Reads the detected object's approach velocity.
 * @param   device Device handle.
 * @return  The velocity in metres per second.
 * @details vexide documents this as calculated by the brain, by
 *          differentiating the distance over time and low-pass filtering the
 *          result.
 */
double vexDeviceDistanceObjectVelocityGet(V5_DeviceT device);

/**
 * @brief   Reads the Distance Sensor's status code.
 * @param   device Device handle.
 * @return  The status code. It reads 0 while the sensor is still
 *          initialising. PROS and vexide treat 0x82 and 0x86 as working;
 *          VEX does not publish what the codes mean.
 */
uint32_t vexDeviceDistanceStatusGet(V5_DeviceT device);

// Gps

/**
 * @brief   Resets the GPS Sensor.
 * @param   device Device handle.
 * @details Neither PROS nor vexide calls this, and VEX does not document what
 *          it resets.
 */
void vexDeviceGpsReset(V5_DeviceT device);

/**
 * @brief   Reads the GPS Sensor's accumulated rotation about the z axis.
 * @param   device Device handle.
 * @return  The rotation in degrees, unbounded, with clockwise positive.
 */
double vexDeviceGpsHeadingGet(V5_DeviceT device);

/**
 * @brief   Reads the GPS Sensor's heading.
 * @param   device Device handle.
 * @return  The heading in degrees, from 0 up to but not including 360, with
 *          clockwise positive.
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
 * @param   data   Structure filled in with position in metres and pitch, roll
 *                 and yaw in degrees.
 * @param   bRaw   VEX does not document this flag. PROS and vexide always pass
 *                 false.
 * @details Position is on the field, with (0, 0) at its centre, and is
 *          reported for the reference point set with vexDeviceGpsOriginSet()
 *          rather than for the sensor itself.
 */
void vexDeviceGpsAttitudeGet(V5_DeviceT device, V5_DeviceGpsAttitude *data,
                             bool bRaw);

/**
 * @brief   Reads the raw gyroscope values of the GPS Sensor's inertial sensor.
 * @param   device Device handle.
 * @param   data   Structure filled in with the x, y and z values. VEX does not
 *                 document their units.
 */
void vexDeviceGpsRawGyroGet(V5_DeviceT device, V5_DeviceGpsRaw *data);

/**
 * @brief   Reads the raw accelerometer values of the GPS Sensor's inertial
 *          sensor.
 * @param   device Device handle.
 * @param   data   Structure filled in with the x, y and z values. VEX does not
 *                 document their units.
 */
void vexDeviceGpsRawAccelGet(V5_DeviceT device, V5_DeviceGpsRaw *data);

/**
 * @brief   Reads the GPS Sensor's status code.
 * @param   device Device handle.
 * @return  The status code. VEX does not publish its meaning.
 */
uint32_t vexDeviceGpsStatusGet(V5_DeviceT device);

/**
 * @brief   Sets the GPS Sensor's mode word.
 * @param   device Device handle.
 * @param   mode   Mode value.
 * @details Neither PROS nor vexide calls this, and VEX does not publish the
 *          mode values.
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
 * @param   rate   Interval in milliseconds. PROS raises it to at least 5 and
 *                 rounds it down to a multiple of 5 before calling this.
 * @details Only affects the inertial sensor's internal computation. It does
 *          not change how often the brain reads the sensor, which is every
 *          10 ms.
 */
void vexDeviceGpsDataRateSet(V5_DeviceT device, uint32_t rate);

/**
 * @brief   Tells the GPS Sensor where it sits on the robot.
 * @param   device Device handle.
 * @param   ox     X offset of the sensor from the robot's reference point,
 *                 such as its centre of turning, in metres.
 * @param   oy     Y offset of the sensor from the same point, in metres.
 * @details Positions read afterwards are for the reference point rather than
 *          the sensor.
 */
void vexDeviceGpsOriginSet(V5_DeviceT device, double ox, double oy);

/**
 * @brief   Reads back the GPS Sensor's mounting offset.
 * @param   device Device handle.
 * @param   ox     Filled in with the X offset in metres.
 * @param   oy     Filled in with the Y offset in metres.
 */
void vexDeviceGpsOriginGet(V5_DeviceT device, double *ox, double *oy);

/**
 * @brief   Sets the GPS Sensor's rotation value.
 * @param   device Device handle.
 * @param   value  Rotation in degrees.
 * @details Neither PROS nor vexide calls this, and VEX does not document
 *          what it sets.
 */
void vexDeviceGpsRotationSet(V5_DeviceT device, double value);

/**
 * @brief   Reads back the GPS Sensor's rotation value.
 * @param   device Device handle.
 * @return  The rotation value; see vexDeviceGpsRotationSet().
 */
double vexDeviceGpsRotationGet(V5_DeviceT device);

/**
 * @brief   Tells the GPS Sensor where the robot starts on the field.
 * @param   device           Device handle.
 * @param   initial_x        Starting X position in metres, with (0, 0) at the
 *                           centre of the field.
 * @param   initial_y        Starting Y position in metres.
 * @param   initial_rotation Starting heading in degrees, 0 to 360, with 0
 *                           being north on the field and clockwise positive.
 * @details The sensor cannot read the field strips when it is too close to a
 *          wall, so a known starting pose gives it a frame of reference until
 *          it can.
 */
void vexDeviceGpsInitialPositionSet(V5_DeviceT device, double initial_x,
                                    double initial_y, double initial_rotation);

/**
 * @brief   Reads how uncertain the GPS Sensor's position is.
 * @param   device Device handle.
 * @return  The root mean squared position error in metres.
 */
double vexDeviceGpsErrorGet(V5_DeviceT device);

// Generic serial port comms to any device

/**
 * @brief   Switches a smart port into generic serial mode.
 * @param   device  Device handle.
 * @param   options Option flags. VEX does not publish their meaning; PROS
 *                  and vexide pass 0.
 * @details Call this before any other generic serial function on the port.
 *          Until it is called, VEXos does not report the port as
 *          #kDeviceTypeGenericSerial, so code that checks the port type first
 *          will never get to enable it.
 */
void vexDeviceGenericSerialEnable(V5_DeviceT device, int32_t options);

/**
 * @brief   Sets the baud rate of a generic serial port.
 * @param   device   Device handle.
 * @param   baudrate Baud rate in bits per second. vexide documents 921600 as
 *                   the maximum for user programs.
 */
void vexDeviceGenericSerialBaudrate(V5_DeviceT device, int32_t baudrate);

/**
 * @brief   Queues one byte for transmission.
 * @param   device Device handle.
 * @param   c      Byte to send.
 * @return  The number of bytes written, or -1 on error.
 */
int32_t vexDeviceGenericSerialWriteChar(V5_DeviceT device, uint8_t c);

/**
 * @brief   Reports how much room is left in the transmit buffer.
 * @param   device Device handle.
 * @return  The number of free bytes. vexide treats -1 as an error.
 * @details vexide documents the transmit and receive buffers as 1024 bytes
 *          each.
 */
int32_t vexDeviceGenericSerialWriteFree(V5_DeviceT device);

/**
 * @brief   Queues a buffer for transmission.
 * @param   device Device handle.
 * @param   buffer Bytes to send.
 * @param   length Maximum number of bytes to send.
 * @return  The number of bytes written, or -1 on error.
 * @details PROS documents this as writing up to length bytes, so check the
 *          return value rather than assuming everything was queued. Queued
 *          bytes are sent as soon as possible without further calls.
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
 * @return  The number of buffered bytes. vexide treats -1 as an error.
 */
int32_t vexDeviceGenericSerialReceiveAvail(V5_DeviceT device);

/**
 * @brief   Reads buffered bytes into a caller supplied buffer.
 * @param   device Device handle.
 * @param   buffer Destination buffer.
 * @param   length Maximum number of bytes to read.
 * @return  The number of bytes read. vexide treats -1 as an error.
 * @details Only returns bytes that have already arrived; it does not wait for
 *          more.
 */
int32_t vexDeviceGenericSerialReceive(V5_DeviceT device, uint8_t *buffer,
                                      int32_t length);

/**
 * @brief   Discards the contents of a generic serial port's transmit and
 *          receive buffers.
 * @param   device Device handle.
 * @details Despite the name, this does not send the pending transmit data;
 *          it throws it away.
 */
void vexDeviceGenericSerialFlush(V5_DeviceT device);

// Radio link between two brains

/**
 * @brief   Starts a VEXlink connection on a VEXnet radio.
 * @param   device  Device handle for the radio.
 * @param   link_id Name of the link. The two radios to be linked use the same
 *                  name, which should differ from any other link nearby.
 * @param   type    1 for the manager (PROS: transmitter) end, which can send
 *                  about 1040 bytes per second, or 0 for the worker (PROS:
 *                  receiver) end, which can send about 520. A link needs one
 *                  of each. The official VEX header names this bMaster.
 * @param   ov      Whether the link may take over the radio from the
 *                  controller. The official VEX header names this
 *                  bAllowRadioOverride.
 * @details PROS documents a 1 to 2 second delay before the link is up, so
 *          vexDeviceGenericRadioLinkStatus() keeps reporting false for a while
 *          after the call. Afterwards the port is reported as
 *          #kDeviceTypeGenericSerial rather than #kDeviceTypeRadioSensor.
 */
void vexDeviceGenericRadioConnection(V5_DeviceT device, char *link_id,
                                     int32_t type, bool ov);

/**
 * @brief   Reports whether a radio link is up.
 * @param   device Device handle.
 * @return  True once the radio is linked with another radio.
 */
bool vexDeviceGenericRadioLinkStatus(V5_DeviceT device);

/**
 * @brief   Reports how much room is left in the link's transmit buffer.
 * @param   device Device handle.
 * @return  The number of free bytes.
 * @details vexide documents the transmit and receive buffers as 512 bytes
 *          each. The official VEX header and vex-sdk declare this and the
 *          other radio buffer functions as returning int32_t, and vexide
 *          treats -1 as an error.
 */
uint32_t vexDeviceGenericRadioWriteFree(V5_DeviceT device);

/**
 * @brief   Queues a buffer for transmission over the link.
 * @param   device Device handle.
 * @param   buffer Bytes to send.
 * @param   length Number of bytes to send.
 * @return  The number of bytes queued.
 * @details PROS checks vexDeviceGenericRadioWriteFree() first and refuses to
 *          send more than fits.
 */
uint32_t vexDeviceGenericRadioTransmit(V5_DeviceT device, uint8_t *buffer,
                                       uint32_t length);

/**
 * @brief   Reports how many bytes the link has waiting to be read.
 * @param   device Device handle.
 * @return  The number of buffered bytes.
 */
uint32_t vexDeviceGenericRadioReceiveAvail(V5_DeviceT device);

/**
 * @brief   Reads buffered bytes from the link into a caller supplied buffer.
 * @param   device Device handle.
 * @param   buffer Destination buffer.
 * @param   length Maximum number of bytes to read.
 * @return  The number of bytes read.
 */
uint32_t vexDeviceGenericRadioReceive(V5_DeviceT device, uint8_t *buffer,
                                      uint32_t length);

// Display/graphics

/*----------------------------------------------------------------------------*/
/** @brief      Brain display
 *
 * @details
 *  The physical screen is #SYSTEM_DISPLAY_WIDTH by #SYSTEM_DISPLAY_HEIGHT
 *  (480 by 272) pixels. VEXos draws the program header across the top 32
 *  rows, leaving 480 by 240 for user code.
 *
 *  The libv5rt versions of the drawing calls that take pixel coordinates add
 *  a y offset, 32 by default, before passing them on to VEXos, so y = 0 is
 *  the first row below the header. This applies to the pixel, line,
 *  rectangle, circle, CopyRect, ScrollRect, Printf and String...At calls.
 *  vexDisplayScroll() and the calls that take a text line number are passed
 *  through unchanged. libv5rt also has an undeclared vexDisplayOffsetSet()
 *  that changes the offset.
 *
 *  Colors are packed as 0x00RRGGBB, the same encoding v5_color.h uses.
 *  Drawing calls take their color from the current foreground, and the
 *  "Clear" variants take it from the current background.
 *
 *  Rectangle coordinates are inclusive at both ends, so a rectangle covers
 *  (1 + x2 - x1) by (1 + y2 - y1) pixels.
 */
/*----------------------------------------------------------------------------*/

/**
 * @brief   Sets the color used by drawing operations.
 * @param   col Color as 0x00RRGGBB.
 */
void vexDisplayForegroundColor(uint32_t col);

/**
 * @brief   Sets the color used by erasing operations.
 * @param   col Color as 0x00RRGGBB.
 * @details Also used behind opaque text and for the pixels left behind when
 *          the screen scrolls.
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
 *                     Everything from there to the bottom of the screen
 *                     moves. This is a physical row: libv5rt does not add
 *                     the header offset here.
 * @param   nLines     How far to move, in pixels. Positive moves up,
 *                     negative moves down.
 * @details Pixels pushed out of the region are discarded, and the space
 *          left behind is filled with the background color.
 */
void vexDisplayScroll(int32_t nStartLine, int32_t nLines);

/**
 * @brief   Scrolls a rectangular region vertically.
 * @param   x1     Left edge of the region.
 * @param   y1     Top edge of the region.
 * @param   x2     Right edge of the region.
 * @param   y2     Bottom edge of the region.
 * @param   nLines How far to move, in pixels. Positive moves up, negative
 *                 moves down.
 * @details Behaves like vexDisplayScroll() but leaves everything outside
 *          the rectangle alone.
 * @note    The vex-sdk jumptable bindings report that on VEXos 1.1.4 this
 *          clears one row too many, leaving the bottom row of the region
 *          filled with the background color.
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
 * @brief   Erases a rectangle using the background color.
 * @param   x1 Left edge.
 * @param   y1 Top edge.
 * @param   x2 Right edge.
 * @param   y2 Bottom edge.
 * @details VEX does not document whether this clears the outline or the
 *          whole area.
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
 * @brief   Erases a circle using the background color.
 * @param   xc     X coordinate of the centre.
 * @param   yc     Y coordinate of the centre.
 * @param   radius Radius in pixels.
 * @details VEX does not document whether this clears the outline or the
 *          whole area.
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
 *                  background color, 0 to draw only the glyphs.
 * @param   format  printf style format string.
 * @param   ...     Arguments consumed by the format string.
 * @details Uses the size set by vexDisplayTextSize() and the font set by
 *          vexDisplayFontNamedSet().
 */
void vexDisplayPrintf(int32_t xpos, int32_t ypos, uint32_t bOpaque,
                      const char *format, ...);

/**
 * @brief   Draws formatted text on a text line.
 * @param   nLineNumber Zero based line. vexide treats lines as 20 pixels
 *                      apart, with 12 of them visible.
 * @param   format      printf style format string.
 * @param   ...         Arguments consumed by the format string.
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
 * @brief   Draws formatted large text on a text line.
 * @param   nLineNumber Zero based line.
 * @param   format      printf style format string.
 * @param   ...         Arguments consumed by the format string.
 */
void vexDisplayBigString(const int32_t nLineNumber, const char *format, ...);

/**
 * @brief   Draws formatted large text at a pixel position.
 * @param   xpos   X coordinate of the text's top left corner.
 * @param   ypos   Y coordinate of the text's top left corner.
 * @param   format printf style format string.
 * @param   ...    Arguments consumed by the format string.
 */
void vexDisplayBigStringAt(int32_t xpos, int32_t ypos, const char *format, ...);

/**
 * @brief   Draws formatted small text at a pixel position.
 * @param   xpos   X coordinate of the text's top left corner.
 * @param   ypos   Y coordinate of the text's top left corner.
 * @param   format printf style format string.
 * @param   ...    Arguments consumed by the format string.
 */
void vexDisplaySmallStringAt(int32_t xpos, int32_t ypos, const char *format,
                             ...);

/**
 * @brief   Draws formatted text centred on a text line.
 * @param   nLineNumber Zero based line.
 * @param   format      printf style format string.
 * @param   ...         Arguments consumed by the format string.
 */
void vexDisplayCenteredString(const int32_t nLineNumber, const char *format,
                              ...);

/**
 * @brief   Draws formatted large text centred on a text line.
 * @param   nLineNumber Zero based line.
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
 */
void vexDisplayVPrintf(int32_t xpos, int32_t ypos, uint32_t bOpaque,
                       const char *format, va_list args);

/**
 * @brief   va_list form of vexDisplayString().
 * @param   nLineNumber Zero based line.
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
 * @param   nLineNumber Zero based line.
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
 * @param   nLineNumber Zero based line.
 * @param   format      printf style format string.
 * @param   args        Argument list, already started with va_start().
 */
void vexDisplayVCenteredString(const int32_t nLineNumber, const char *format,
                               va_list args);

/**
 * @brief   va_list form of vexDisplayBigCenteredString().
 * @param   nLineNumber Zero based line.
 * @param   format      printf style format string.
 * @param   args        Argument list, already started with va_start().
 */
void vexDisplayVBigCenteredString(const int32_t nLineNumber, const char *format,
                                  va_list args);

/**
 * @brief   Sets the text size as a fraction of the font's full size.
 * @param   n Numerator.
 * @param   d Denominator.
 * @details vexide names 1/5, 1/4, 1/3, 1/2, 2/3 and 1/1 as extra small,
 *          small, medium, large, extra large and full, and uses 1/3 as its
 *          default.
 */
void vexDisplayTextSize(uint32_t n, uint32_t d);

/**
 * @brief   Selects the font used for text.
 * @param   pFontName Font name. vexide uses "monospace" (Noto Mono) and
 *                    "proportional" (Noto Sans).
 */
void vexDisplayFontNamedSet(const char *pFontName);

/**
 * @brief   Measures how wide a string would be if drawn.
 * @param   pString Null terminated string to measure.
 * @return  The width in pixels.
 * @details Uses the text size of the last text drawing call.
 */
int32_t vexDisplayStringWidthGet(const char *pString);

/**
 * @brief   Measures how tall a string would be if drawn.
 * @param   pString Null terminated string to measure.
 * @return  The height in pixels.
 * @details Uses the text size of the last text drawing call.
 */
int32_t vexDisplayStringHeightGet(const char *pString);

/**
 * @brief   Switches to double buffering, or presents the back buffer.
 * @param   bVsyncWait    True to wait for vsync before presenting.
 * @param   bRunScheduler True to run background tasks while waiting.
 * @return  True if the back buffer was copied to the screen. vex-sdk
 *          declares no return value.
 * @details The first call enables double buffering. From then on, drawing
 *          only appears when this is called again, so it has to be called
 *          once per frame. Use vexDisplayDoubleBufferDisable() to go back.
 */
bool vexDisplayRender(bool bVsyncWait, bool bRunScheduler);

/**
 * @brief   Returns the display to immediate mode.
 * @details Undoes vexDisplayRender(), so drawing calls appear straight away
 *          again.
 */
void vexDisplayDoubleBufferDisable(void);

/**
 * @brief   Sets the clip region for screen output.
 * @param   x1 Left edge.
 * @param   y1 Top edge.
 * @param   x2 Right edge.
 * @param   y2 Bottom edge.
 * @details VEXcode documents its clip region as only affecting the current
 *          thread.
 */
void vexDisplayClipRegionSet(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/**
 * @brief   Removes the clip region.
 */
void vexDisplayClipRegionClear();

/**
 * @brief   Decodes a BMP image into a pixel buffer.
 * @param   ibuf The BMP file's bytes.
 * @param   oBuf Image structure whose data member points at a buffer of at
 *               least maxw * maxh pixels (4 bytes each), or null. The
 *               decoded image is written there.
 * @param   maxw Width capacity of that buffer, in pixels.
 * @param   maxh Height capacity of that buffer, in pixels.
 * @return  1 on success, 0 on failure.
 * @details The decoded pixels use the display's 0x00RRGGBB layout.
 */
uint32_t vexImageBmpRead(const uint8_t *ibuf, v5_image *oBuf, uint32_t maxw,
                         uint32_t maxh);

/**
 * @brief   Decodes a PNG image into a pixel buffer.
 * @param   ibuf    The PNG file's bytes.
 * @param   oBuf    Image structure whose data member points at a buffer of
 *                  at least maxw * maxh pixels (4 bytes each), or null.
 * @param   maxw    Width capacity of that buffer, in pixels.
 * @param   maxh    Height capacity of that buffer, in pixels.
 * @param   ibuflen Length of the input buffer in bytes.
 * @return  1 on success, 0 on failure.
 * @details Decodes with libpng into the display's 0x00RRGGBB layout.
 */
uint32_t vexImagePngRead(const uint8_t *ibuf, v5_image *oBuf, uint32_t maxw,
                         uint32_t maxh, uint32_t ibuflen);

// special use only ! Talk to James.

/**
 * @brief   Retrieves a pointer to the VEXos scratch memory region.
 * @param   ptr Filled in with a pointer to the region.
 * @return  VEX does not document the return value.
 * @details The original header marks this as special use only. Neither PROS
 *          nor vexide calls it, and VEX does not document what the region
 *          is for.
 */
int32_t vexScratchMemoryPtr(void **ptr);

/**
 * @brief   Takes the lock guarding scratch memory.
 * @return  VEX does not document the return value.
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
 * @details PROS calls this before every file open. The Rust standard library
 *          for VEXos never calls it and opens files directly.
 */
FRESULT vexFileMountSD(void);

/**
 * @brief   Lists the files in a directory.
 * @param   path   Directory to list. PROS documents "\" for the root
 *                 directory, with no "/usd/" prefix.
 * @param   buffer Destination buffer for the listing.
 * @param   len    Size of the destination buffer in bytes.
 * @return  #FR_OK on success, otherwise the reason the listing failed.
 * @details File names are written separated by newlines. Subdirectories are
 *          not included.
 */
FRESULT vexFileDirectoryGet(const char *path, char *buffer, uint32_t len);

/**
 * @brief   Opens a file for reading.
 * @param   filename Path to the file.
 * @param   mode     Ignored. PROS and the Rust standard library pass an empty
 *                   string.
 * @return  A file handle, or null if the file could not be opened.
 * @details Only opens for reading. Use vexFileOpenWrite() or
 *          vexFileOpenCreate() to write. A PROS comment says at most 8 files
 *          can be open at once as of VEXos 0.7.4b55.
 */
FIL *vexFileOpen(const char *filename, const char *mode);

/**
 * @brief   Opens a file for appending.
 * @param   filename Path to the file.
 * @return  A file handle, or null if the file could not be opened.
 * @details Writes go after the existing contents. The Rust standard library
 *          seeks back to the start when it wants to overwrite without
 *          truncating.
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
 */
void vexFileClose(FIL *fdp);

/**
 * @brief   Reads from a file.
 * @param   buf    Destination buffer.
 * @param   size   Size of one item in bytes.
 * @param   nItems Number of items to read.
 * @param   fdp    File handle.
 * @return  The amount read, or a negative value on error. PROS and the Rust
 *          standard library always pass a size of 1, so this is a byte count
 *          for them.
 */
int32_t vexFileRead(char *buf, uint32_t size, uint32_t nItems, FIL *fdp);

/**
 * @brief   Writes to a file.
 * @param   buf    Source buffer.
 * @param   size   Size of one item in bytes.
 * @param   nItems Number of items to write.
 * @param   fdp    File handle.
 * @return  The amount written, or a negative value on error. PROS and the
 *          Rust standard library always pass a size of 1.
 * @details PROS calls vexFileSync() after every write; the Rust standard
 *          library calls it to flush.
 */
int32_t vexFileWrite(char *buf, uint32_t size, uint32_t nItems, FIL *fdp);

/**
 * @brief   Reports a file's size.
 * @param   fdp File handle.
 * @return  The size in bytes, or -1 on error.
 */
int32_t vexFileSize(FIL *fdp);

/**
 * @brief   Moves a file's read and write position.
 * @param   fdp    File handle.
 * @param   offset Offset in bytes. Negative offsets are not supported.
 * @param   whence Where the offset is measured from: #FS_SEEK_SET,
 *                 #FS_SEEK_CUR or #FS_SEEK_END.
 * @return  #FR_OK on success, otherwise the reason the seek failed.
 */
FRESULT vexFileSeek(FIL *fdp, uint32_t offset, int32_t whence);

/**
 * @brief   Reports whether a drive is present.
 * @param   drive Drive number. PROS checks drive 0 for the microSD card.
 * @return  True if the drive is installed.
 */
bool vexFileDriveStatus(uint32_t drive);

/**
 * @brief   Reports a file's current position.
 * @param   fdp File handle.
 * @return  The position in bytes from the start of the file. The Rust
 *          standard library treats a negative value as an error.
 */
int32_t vexFileTell(FIL *fdp);

/**
 * @brief   Flushes buffered writes to the card.
 * @param   fdp File handle.
 */
void vexFileSync(FIL *fdp);

/**
 * @brief   Reports what exists at a path.
 * @param   filename Path to check.
 * @return  0 if nothing exists there, 1 (#FS_FILE_EXIST) for a file, or 3
 *          (#FS_FILE_EXIST | #FS_FILE_DIR) for a directory.
 */
uint32_t vexFileStatus(const char *filename);

// CDC

/**
 * @brief   Queues one byte on a serial channel.
 * @param   channel Channel number. Channel 1 is stdio.
 * @param   c       Byte to send.
 * @return  The number of bytes written, or -1 on error.
 */
int32_t vexSerialWriteChar(uint32_t channel, uint8_t c);

/**
 * @brief   Queues a buffer on a serial channel.
 * @param   channel  Channel number. Channel 1 is stdio.
 * @param   data     Bytes to send.
 * @param   data_len Number of bytes to send.
 * @return  The number of bytes written, or -1 on error.
 * @details The output buffer holds 2048 bytes, so a larger write may be
 *          truncated. PROS and vexide check vexSerialWriteFree() first and
 *          compare the return value with the length they asked for. A PROS
 *          comment notes that this call does not seem to be thread safe.
 */
int32_t vexSerialWriteBuffer(uint32_t channel, uint8_t *data,
                             uint32_t data_len);

/**
 * @brief   Removes one byte from a serial channel's input buffer.
 * @param   channel Channel number. Channel 1 is stdio.
 * @return  The byte, or -1 if nothing is available.
 */
int32_t vexSerialReadChar(uint32_t channel);

/**
 * @brief   Looks at the next byte on a serial channel without removing it.
 * @param   channel Channel number. Channel 1 is stdio.
 * @return  The byte, or -1 if nothing is available.
 */
int32_t vexSerialPeekChar(uint32_t channel);

/**
 * @brief   Reports how much room is left in a serial channel's output
 *          buffer.
 * @param   channel Channel number. Channel 1 is stdio.
 * @return  The number of free bytes, out of 2048.
 * @details PROS and vexide wait for output to drain by looping until this
 *          returns 2048, running VEXos background processing on each pass.
 */
int32_t vexSerialWriteFree(uint32_t channel);

// rtos hooks

/*----------------------------------------------------------------------------*/
/** @brief      RTOS and interrupt hooks
 *
 * @details
 *  These are the hooks PROS's FreeRTOS port and vexide's startup code use to
 *  run their own scheduler and exception vectors on top of VEXos. They belong
 *  to a runtime's port layer rather than to robot code.
 */
/*----------------------------------------------------------------------------*/

/**
 * @brief   Stops the system tick timer.
 * @details PROS calls this first in rtos_initialize(), before disabling
 *          interrupts and installing the FreeRTOS vector table.
 */
void vexSystemTimerStop();

/**
 * @brief   Clears the system tick interrupt.
 * @details PROS calls this from its FreeRTOS tick-clear hook.
 */
void vexSystemTimerClearInterrupt();

/**
 * @brief   Restarts the system tick timer with a replacement handler.
 * @param   priority Interrupt priority for the tick.
 * @param   handler  Function to call on every tick.
 * @return  VEX does not document the return value, and PROS ignores it.
 * @details PROS passes the FreeRTOS tick handler and FreeRTOS's lowest usable
 *          interrupt priority.
 */
int32_t vexSystemTimerReinitForRtos(uint32_t priority,
                                    void (*handler)(void *data));

/**
 * @brief   Hands an interrupt to VEXos.
 * @param   ulICCIAR Value read from the interrupt controller's acknowledge
 *                   register.
 * @details PROS's FreeRTOS IRQ hook forwards every interrupt here.
 */
void vexSystemApplicationIRQHandler(uint32_t ulICCIAR);

/**
 * @brief   Reinitialises the watchdog timer for use by an RTOS.
 * @return  VEX does not document the return value, and PROS ignores it.
 * @details PROS calls this to set up the FreeRTOS run-time statistics timer.
 */
int32_t vexSystemWatchdogReinitRtos(void);

/**
 * @brief   Reads the watchdog timer's counter.
 * @return  The counter value.
 * @details PROS uses this as the FreeRTOS run-time statistics counter.
 */
uint32_t vexSystemWatchdogGet(void);

// Hooks into the interrupt system, some of these will be used by FreeRTOS
// the others are just placeholders for now

/**
 * @brief   VEXos boot entry point.
 * @details vexide's exception vector table branches here on reset, and PROS's
 *          _boot() calls it.
 */
void vexSystemBoot(void);

/**
 * @brief   VEXos handler for the undefined instruction exception.
 * @details vexide installs its own handler instead and does not call this.
 */
void vexSystemUndefinedException(void);

/**
 * @brief   VEXos handler for fast interrupts.
 * @details PROS and vexide forward FIQs here from their own vectors.
 */
void vexSystemFIQInterrupt(void);

/**
 * @brief   VEXos handler for normal interrupts.
 * @note    The name is misspelt. libv5rt and vex-sdk define
 *          vexSystemIRQInterrupt, which is what vexide calls from its IRQ
 *          vector, so a call to this name will not link against libv5rt.
 */
void vexSystemIQRQnterrupt(void);

/**
 * @brief   VEXos handler for the software interrupt (SVC).
 * @details vexide's SVC vector calls this with the SVC immediate number in
 *          the first argument register.
 */
void vexSystemSWInterrupt(void);

/**
 * @brief   VEXos handler for the data abort exception.
 * @details PROS and vexide install their own handlers instead.
 */
void vexSystemDataAbortInterrupt(void);

/**
 * @brief   VEXos handler for the prefetch abort exception.
 * @details PROS and vexide install their own handlers instead.
 */
void vexSystemPrefetchAbortInterrupt(void);

// touch

/**
 * @brief   Installs a callback for touchscreen events.
 * @param   callback Function called with the event kind and the x and y
 *                   coordinates where it happened.
 * @details Neither PROS nor vexide uses this; both poll vexTouchDataGet().
 */
void vexTouchUserCallbackSet(void (*callback)(V5_TouchEvent, int32_t, int32_t));

/**
 * @brief   Reads the current touchscreen state.
 * @param   status Structure filled in with the last event, its position and
 *                 the press and release counts.
 * @return  VEX does not document the return value. vex-sdk declares no
 *          return value, and PROS and vexide ignore it.
 */
bool vexTouchDataGet(V5_TouchStatus *status);

// system utility

/**
 * @brief   Returns the VEXos version.
 * @return  The version packed into one word, one byte per component from
 *          most to least significant: major, minor, build and beta.
 * @details libv5rt reads it from address 0x037fd000. PROS compares it as an
 *          integer, for example against 0x1000C38 (1.0.12.56) to pick how it
 *          clears a controller line.
 */
uint32_t vexSystemVersion(void);

/**
 * @brief   Returns the version of the standard library inside VEXos.
 * @return  The version word libv5rt reads from address 0x037fd004.
 */
uint32_t vexStdlibVersion(void);

// get SDK version

/**
 * @brief   Returns the SDK version.
 * @return  A constant compiled into libv5rt: 0x01020000 in the
 *          V5_20240802_15_00_00 SDK.
 */
uint32_t vexSdkVersion(void);

// duplication of prototypes in v5_util.h for use by user code

/**
 * @brief   Returns the standard library version this program was linked
 *          against.
 * @return  The address of the weak linker symbol __stdlib_version, which the
 *          SDK's stdlib_0.lib defines as 0x00020000.
 */
uint32_t vexStdlibVersionLinked(void);

/**
 * @brief   Checks the linked standard library version against the one in
 *          VEXos.
 * @return  True if __stdlib_version is 0 (not linked) or equals
 *          vexStdlibVersion().
 */
bool vexStdlibVersionVerify(void);

// comp status

/**
 * @brief   Reads the competition control state.
 * @return  A bit field built from the V5_COMP_BIT_* values.
 * @details #V5_COMP_BIT_EBL is set when the robot is disabled, not enabled.
 *          vexide reports the mode as disabled whenever that bit is set,
 *          and only looks at #V5_COMP_BIT_MODE otherwise.
 */
uint32_t vexCompetitionStatus(void);

/**
 * @brief   Writes the competition control state.
 * @param   data Bit field. VEX does not document its layout.
 * @details Neither PROS nor vexide calls this.
 */
void vexCompetitionControl(uint32_t data);

// battery

/**
 * @brief   Reads the battery voltage.
 * @return  The voltage in millivolts. vexide documents the nominal voltage
 *          as 12.8 V.
 */
int32_t vexBatteryVoltageGet(void);

/**
 * @brief   Reads the current the battery is supplying.
 * @return  The current in milliamps. vexide documents the maximum draw as
 *          20 A.
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
