/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Innovation First 2016, All rights reserved.               */
/*                                                                            */
/*    Module:     v5_apitypes.h                                               */
/*    Author:     James Pearman                                               */
/*    Created:    8 Nov 2016                                                  */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifndef V5_APITYPES_H_
#define V5_APITYPES_H_

#include "stdbool.h"
#include "stdint.h"

/*-----------------------------------------------------------------------------*/
/** @file    v5_apitypes.h
 * @brief   Header for V5 API - type definitions
 *
 * @details
 *  Enums, structures and constants shared by the jumptable API in v5_api.h and
 *  the port indexed wrappers in v5_apiuser.h. The file only declares types and
 *  macros.
 *
 *  Structures passed to VEXos are declared packed, as in VEX's own header, so
 *  their layout has to match what VEXos expects. Do not reorder their fields.
 *
 *  See docs/dev/sdk-doc-evidence.md for the sources behind these comments.
 *//*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif

/*----------------------------------------------------------------------------*/
/** @brief      Code signature */
/*----------------------------------------------------------------------------*/
/** @details
 *   The first 16 bytes of a user code binary should contain the user code
 *   signature.  For simple user code programs this will be created by the
 *   startup code in the runtime library, certain types of user code,
 *   for example a virtual machine, may override the default settings to
 * cause the V5 system code to enable custom functionality yet TBD.
 *
 *   to override the default use a definition like this in one of the user
 *   code source files.
 *
 *   __attribute__ ((section (".boot_data"))) vcodesig vexCodeSig =
 *   { V5_SIG_MAGIC,
 *     V5_SIG_TYPE_USER,
 *     V5_SIG_OWNER_VEX,
 *     V5_SIG_OPTIONS_NONE
 *   };
 *
 *   V5_SIG_MAGIC must be set, other fields and their behavior are TBD
 *
 *   vexCodeSig is defined as "weak" and this definition will override it.
 */
typedef struct __attribute__((__packed__)) _vcodesig
{
    uint32_t magic;   ///< Magic number. Must be #V5_SIG_MAGIC.
    uint32_t type;    ///< Program type. Only #V5_SIG_TYPE_USER is defined.
    uint32_t owner;   ///< Program originator, one of the V5_SIG_OWNER_* values.
    uint32_t options; ///< Bitwise OR of the V5_SIG_OPTIONS_* flags.
} vcodesig;

//

/**
 * @brief   Magic word that marks a binary as V5 user code.
 * @details Read as bytes in memory order the four characters spell "XVX5".
 */
#define V5_SIG_MAGIC 0x35585658

/** Ordinary user program. This is the only program type currently defined. */
#define V5_SIG_TYPE_USER 0

/** Owner: the V5 system. */
#define V5_SIG_OWNER_SYS 0
/** Owner: VEX. */
#define V5_SIG_OWNER_VEX 1
/** Owner: a VEX partner. */
#define V5_SIG_OWNER_PARTNER 2

/** No startup options requested. */
#define V5_SIG_OPTIONS_NONE 0
#define V5_SIG_OPTIONS_INDG (1 << 0) // Invert default graphics colors
#define V5_SIG_OPTIONS_EXIT (1 << 1) // Kill threads when main exits
#define V5_SIG_OPTIONS_THDG (1 << 2) // Invert graphics based on theme

/**
 * @brief   Defines the code signature for a user program.
 * @param   type    Program type, normally #V5_SIG_TYPE_USER.
 * @param   owner   Program originator, one of the V5_SIG_OWNER_* values.
 * @param   options Bitwise OR of the V5_SIG_OPTIONS_* flags.
 * @details Places a #vcodesig named vexCodeSig in the .boot_data section so it
 *          lands in the first 16 bytes of the binary. Use this once, at file
 *          scope, in exactly one translation unit. It overrides the weak
 *          definition supplied by the runtime library.
 */
#define V5_CODE_SIG(type, owner, options)                                      \
    vcodesig vexCodeSig __attribute__((section(".boot_data"))) = {             \
        V5_SIG_MAGIC, type, owner, options};

/*----------------------------------------------------------------------------*/
/** @brief      Structures used by system time functions */
/*----------------------------------------------------------------------------*/

/**
 * @brief   Time of day, as filled in by vexGettime().
 * @see     vexGettime
 */
struct time
{
    uint8_t ti_hour; /// Hours
    uint8_t ti_min;  /// Minutes
    uint8_t ti_sec;  /// Seconds
    uint8_t ti_hund; /// Hundredths of seconds
};

/**
 * @brief   Calendar date, as filled in by vexGetdate().
 * @see     vexGetdate
 */
struct date
{
    uint16_t da_year; /// Year - 1980
    uint8_t da_day;   /// Day of the month
    uint8_t da_mon;   /// Month (1 = Jan)
};

// DEVICE IDs

/*----------------------------------------------------------------------------*/
/** @brief      V5 Device type definitions */
/*----------------------------------------------------------------------------*/
// TODO
// Do we really want to include the declare.h file ??

/**
 * @brief   Identifies the kind of device attached to a port.
 * @details Filled in by vexDeviceGetStatus(), one entry per port index, and
 *          accepted by vexDevicesGetNumberByType(). VEX's 2024 header also
 *          defines 26 (AI camera), 27 (light tower), 28 (arm), 30 (pneumatic)
 *          and 31 (MC55 motor), which are missing here.
 */
typedef enum
{
    kDeviceTypeNoSensor = 0,         ///< Nothing plugged into the port.
    kDeviceTypeMotorSensor = 2,      ///< V5 Smart Motor.
    kDeviceTypeLedSensor = 3,        ///< LED module.
    kDeviceTypeAbsEncSensor = 4,     ///< V5 Rotation Sensor.
    kDeviceTypeCrMotorSensor = 5,    ///< Continuous rotation motor.
    kDeviceTypeImuSensor = 6,        ///< V5 Inertial Sensor.
    kDeviceTypeRangeSensor = 7,      // obsolete
    kDeviceTypeDistanceSensor = 7,   ///< V5 Distance Sensor. Shares its value
                                     ///< with the obsolete range sensor.
    kDeviceTypeRadioSensor = 8,      ///< VEXnet radio.
    kDeviceTypeTetherSensor = 9,     ///< Tether. VEX does not document it.
    kDeviceTypeBrainSensor = 10,     ///< Brain. VEX does not document it.
    kDeviceTypeVisionSensor = 11,    ///< V5 Vision Sensor.
    kDeviceTypeAdiSensor = 12,       ///< 3-Wire (ADI) expander, or the brain's
                                     ///< own 3-Wire ports (PROS port 22).
    kDeviceTypeRes1Sensor = 13,      ///< Reserved.
    kDeviceTypeRes2Sensor = 14,      ///< Reserved.
    kDeviceTypeRes3Sensor = 15,      ///< Reserved.
    kDeviceTypeOpticalSensor = 16,   ///< V5 Optical Sensor.
    kDeviceTypeMagnetSensor = 17,    ///< Workcell electromagnet.
    kDeviceTypeGpsSensor = 20,       ///< V5 GPS Sensor.
    kDeviceTypeAiVisionSensor = 29,  ///< VEX AI Vision Sensor.
    kDeviceTypeBumperSensor = 0x40,  ///< Bumper switch.
    kDeviceTypeGyroSensor = 0x46,    ///< Dedicated gyro (obsolete).
    kDeviceTypeSonarSensor = 0x47,   ///< Dedicated sonar (obsolete).
    kDeviceTypeGenericSensor = 128,  ///< Generic sensor.
    kDeviceTypeGenericSerial = 129,  ///< Port in generic serial mode.
    kDeviceTypeUndefinedSensor = 255 ///< Undefined device type.
} V5_DeviceType;

/**
 * @brief   Opaque handle to a device.
 * @details Obtained from vexDeviceGetByIndex(). The structure it points to
 *          belongs to VEXos, and PROS and vexide only ever pass the pointer
 *          back to VEXos.
 */
typedef struct _V5_Device *V5_DeviceT;

/**
 * @brief   Size of the port type buffer used with vexDeviceGetStatus().
 * @details PROS sizes its registry with this, while it only scans the first
 *          22 indexes (21 smart ports plus the built-in 3-Wire ports).
 */
#define V5_MAX_DEVICE_PORTS 32

/** Buffer large enough to hold one #V5_DeviceType per port index. */
typedef V5_DeviceType V5_DeviceTypeBuffer[V5_MAX_DEVICE_PORTS];

/*----------------------------------------------------------------------------*/
/** @brief      V5 Controller definitions */
/*----------------------------------------------------------------------------*/
//
// March 2018, obsolete types now removed
// additional enum for final V5 controller labels added
//

/**
 * @brief   Selects which controller value vexControllerGet() returns.
 * @details The first block of names is the original channel list. The
 *          aliases at the end are the labels on the V5 controller, so ButtonA
 *          and Button8R are the same channel.
 *
 *          Analog channels return -127 to 127, and 0 when the controller is
 *          not connected. Button channels are non-zero while pressed.
 */
typedef enum _V5_ControllerIndex
{
    AnaLeftX = 0, ///< Left stick, horizontal axis.
    AnaLeftY,     ///< Left stick, vertical axis.
    AnaRightX,    ///< Right stick, horizontal axis.
    AnaRightY,    ///< Right stick, vertical axis.
    AnaSpare1,    ///< Spare analog channel.
    AnaSpare2,    ///< Spare analog channel.

    Button5U, ///< L1, the first trigger on the left.
    Button5D, ///< L2, the second trigger on the left.
    Button6U, ///< R1, the first trigger on the right.
    Button6D, ///< R2, the second trigger on the right.
    Button7U, ///< Up arrow on the left pad.
    Button7D, ///< Down arrow on the left pad.
    Button7L, ///< Left arrow on the left pad.
    Button7R, ///< Right arrow on the left pad.
    Button8U, ///< X on the right pad.
    Button8D, ///< B on the right pad.
    Button8L, ///< Y on the right pad.
    Button8R, ///< A on the right pad.

    ButtonSEL, ///< Power button. PROS exposes it as
               ///< E_CONTROLLER_DIGITAL_POWER.

    BatteryLevel, ///< Controller battery level. The unit is not documented.

    ButtonAll,       ///< All buttons. The encoding is not documented.
    Flags,           ///< Controller flags. Not documented.
    BatteryCapacity, ///< Controller battery capacity as a percentage.

    // Final V5 names
    Axis1 = AnaRightX, ///< Controller label for the right stick, horizontal.
    Axis2 = AnaRightY, ///< Controller label for the right stick, vertical.
    Axis3 = AnaLeftY,  ///< Controller label for the left stick, vertical.
    Axis4 = AnaLeftX,  ///< Controller label for the left stick, horizontal.

    ButtonL1 = Button5U, ///< Controller label L1.
    ButtonL2 = Button5D, ///< Controller label L2.
    ButtonR1 = Button6U, ///< Controller label R1.
    ButtonR2 = Button6D, ///< Controller label R2.

    ButtonUp = Button7U,    ///< Controller label Up.
    ButtonDown = Button7D,  ///< Controller label Down.
    ButtonLeft = Button7L,  ///< Controller label Left.
    ButtonRight = Button7R, ///< Controller label Right.

    ButtonX = Button8U, ///< Controller label X.
    ButtonB = Button8D, ///< Controller label B.
    ButtonY = Button8L, ///< Controller label Y.
    ButtonA = Button8R  ///< Controller label A.
} V5_ControllerIndex;

/**
 * @brief   How a controller is attached to the brain.
 * @see     vexControllerConnectionStatusGet
 */
typedef enum _V5_ControllerStatus
{
    kV5ControllerOffline = 0, ///< No controller is connected.
    kV5ControllerTethered,    ///< Connected through a wired smart port cable.
    kV5ControllerVexnet       ///< Connected wirelessly over VEXnet.
} V5_ControllerStatus;

/** Selects one of the two controllers. */
typedef enum _V5_ControllerId
{
    kControllerMaster = 0, ///< Primary controller.
    kControllerPartner     ///< Partner controller.
} V5_ControllerId;

/*----------------------------------------------------------------------------*/
/** @brief      V5 LED device definitions */
/*----------------------------------------------------------------------------*/
// The LED is an obsolete sensor but we will leave as it's available on the
// Dev systems

/**
 * @brief   Preset colors for the LED module.
 * @details The values are written as 0xRRGGBB. VEX's header calls the LED an
 *          obsolete sensor that is still available on development systems.
 */
typedef enum _V5_DeviceLedColor
{
    kLedColorBlack = 0,          ///< LED off.
    kLedColorRed = 0xFF0000,     ///< Full red.
    kLedColorGreen = 0x00FF00,   ///< Full green.
    kLedColorBlue = 0x0000FF,    ///< Full blue.
    kLedColorYellow = 0xFFFF00,  ///< Red and green.
    kLedColorCyan = 0x00FFFF,    ///< Green and blue.
    kLedColorMagenta = 0xFF00FF, ///< Red and blue.
    kLedColorWhite = 0xFFFFFF    ///< All three channels at full.
} V5_DeviceLedColor;

/*----------------------------------------------------------------------------*/
/** @brief      V5 ADI device definitions */
/*----------------------------------------------------------------------------*/

/**
 * @brief   Selects what a single 3-Wire (ADI) channel is wired to.
 * @details Set with vexDeviceAdiPortConfigSet(). PROS and vexide configure a
 *          channel before using it.
 *
 *          Quadrature encoders and ultrasonics use two adjacent channels,
 *          starting on an even numbered channel (A, C, E or G). PROS and
 *          vexide only configure the first of the two.
 */
typedef enum _V5_AdiPortConfiguration
{
    kAdiPortTypeAnalogIn = 0, ///< 12 bit analog input, reads 0 to 4095.
    kAdiPortTypeAnalogOut,    ///< 8 bit PWM output (vexide's PwmOut).
    kAdiPortTypeDigitalIn,    ///< Digital input.
    kAdiPortTypeDigitalOut,   ///< Digital output.

    kAdiPortTypeSmartButton, ///< V2 bumper switch.
    kAdiPortTypeSmartPot,    ///< V2 potentiometer.

    kAdiPortTypeLegacyButton,        ///< Limit or bumper switch.
    kAdiPortTypeLegacyPotentiometer, ///< Cortex era potentiometer.
    kAdiPortTypeLegacyLineSensor,    ///< Cortex era line tracker.
    kAdiPortTypeLegacyLightSensor,   ///< Cortex era light sensor.
    kAdiPortTypeLegacyGyro, ///< Cortex era yaw rate gyro. Configuring it
                            ///< starts a calibration; PROS waits 1300 ms.
    kAdiPortTypeLegacyAccelerometer, ///< Cortex era accelerometer.

    kAdiPortTypeLegacyServo, ///< Cortex era servo output.
    kAdiPortTypeLegacyPwm,   ///< Motor controller (MC29) output.

    kAdiPortTypeQuadEncoder, ///< Quadrature encoder, on this channel and
                             ///< the next one.
    kAdiPortTypeSonar,       ///< Ultrasonic rangefinder. This channel is the
                       ///< OUTPUT (ping) cable, the next the INPUT (echo).

    kAdiPortTypeLegacyPwmSlew, ///< Motor controller output with slew rate
                               ///< limiting.

    kAdiPortTypeUndefined = 255 ///< Undefined. vexide notes that
                                ///< unconfigured channels report AnalogIn,
                                ///< not this.
} V5_AdiPortConfiguration;

/** Number of 3-Wire channels on a brain or a 3-Wire Expander, numbered 0 to
 * 7 for A to H. */
#define V5_ADI_PORT_NUM 8

/*----------------------------------------------------------------------------*/
/** @brief      V5 Bumper switch device definitions */
/*----------------------------------------------------------------------------*/

/**
 * @brief   State of a smart port bumper switch.
 * @note    The two trailing comments below are copied from VEX's header and
 *          contradict the names: kBumperReleased (0) is commented "Switch
 *          pressed". No runtime uses this type, so the sources do not settle
 *          which is right.
 */
typedef enum _V5_DeviceBumperState
{
    kBumperReleased = 0, /// Switch pressed
    kBumperPressed = 1   /// Switch released
} V5_DeviceBumperState;

/*----------------------------------------------------------------------------*/
/** @brief      V5 Motor definitions */
/*----------------------------------------------------------------------------*/
// Most of this is still TBD and is currently based on the IQ implementation

/**
 * @brief   Control mode a Smart Motor is running in.
 * @details The trailing comments are VEX's. Neither PROS nor vexide reads or
 *          sets these modes.
 * @see     vexDeviceMotorModeSet
 */
typedef enum
{
    kMotorControlModeOFF = 0,     /// Motor is off and in coast mode
    kMotorControlModeBRAKE = 1,   /// Motor is off and in brake mode
    kMotorControlModeHOLD = 2,    /// Motor is holding at current position
    kMotorControlModeSERVO = 3,   /// Motor is in "Servo" mode. Move to
                                  /// position and hold at that position
    kMotorControlModePROFILE = 4, /// Motor moves to set position and stops.
    kMotorControlModeVELOCITY =
        5, /// Motor is unlimited movement at set 'velocity'
    kMotorControlModeUNDEFINED = 6 ///
} V5MotorControlMode;

/**
 * @brief   What a Smart Motor does when it is stopped.
 * @see     vexDeviceMotorBrakeModeSet
 */
typedef enum _V5_MotorBrakeMode
{
    kV5MotorBrakeModeCoast = 0, /// Motor will coast when stopped
    kV5MotorBrakeModeBrake = 1, /// Motor will brake when stopped
    kV5MotorBrakeModeHold = 2   /// Motor will hold position when stopped
} V5MotorBrakeMode;

/**
 * @brief   Units used for motor position values.
 * @details Applies to the motor position and target functions.
 * @see     vexDeviceMotorEncoderUnitsSet
 */
typedef enum
{
    kMotorEncoderDegrees = 0,   /// degrees Encoder Count Mode
    kMotorEncoderRotations = 1, /// rotations Encoder Count Mode
    kMotorEncoderCounts = 2     /// Raw Encoder Count Mode
} V5MotorEncoderUnits;

/**
 * @brief   Gear cartridge fitted to a Smart Motor.
 * @details PROS lists red as 36:1 (100 RPM), green as 18:1 (200 RPM) and
 *          blue as 6:1 (600 RPM).
 * @see     vexDeviceMotorGearingSet
 */
typedef enum _V5MotorGearset
{
    kMotorGearSet_36 = 0, /// 36:1 gear set, torque
    kMotorGearSet_18 = 1, /// 18:1 gear set, speed
    kMotorGearSet_06 = 2  /// 6:1 gear set, high speed
} V5MotorGearset;

/**
 * @brief   Raw encoder counts per output revolution for the 36:1 cartridge.
 * @details vexide gives 1800 for the 36:1 cartridge, 900 for 18:1 and 300 for
 *          6:1.
 */
#define V5_MOTOR_COUNTS_PER_ROT 1800.0

//
// preliminary, used for position and velocity
//

/**
 * @brief   Gain set for a motor's internal position or velocity controller.
 * @details Passed to vexDeviceMotorPositionPidSet() and
 *          vexDeviceMotorVelocityPidSet(). PROS documents the gains as 4.4
 *          fixed point. VEX has not published the motor's default values.
 */
typedef struct __attribute__((__packed__)) _V5_DeviceMotorPid
{
    uint8_t kf;        ///< Feedforward constant.
    uint8_t kp;        ///< Proportional constant.
    uint8_t ki;        ///< Integral constant.
    uint8_t kd;        ///< Derivative constant.
    uint8_t filter;    ///< Constant used for filtering the profile
                       ///< acceleration.
    uint8_t pad1;      ///< Padding.
    uint16_t limit;    ///< Integral limit.
    uint8_t threshold; ///< Threshold for deciding a position move has
                       ///< reached its goal. No effect on velocity control.
    uint8_t loopspeed; ///< Rate the PID computation runs at, in ms.
    uint8_t pad2[2];   ///< Padding.
} V5_DeviceMotorPid;

/*----------------------------------------------------------------------------*/
/** @brief      V5 Vision sensor definitions */
/*----------------------------------------------------------------------------*/

// subject to change

/**
 * @brief   What the Vision Sensor is looking for.
 * @see     vexDeviceVisionModeSet
 */
typedef enum
{
    kVisionModeNormal = 0,     ///< Color signatures and codes.
    kVisionModeMixed = 1,      ///< Color signatures and lines.
    kVisionModeLineDetect = 2, ///< Lines only.
    kVisionTypeTest = 3        ///< Test mode, of unknown use.
} V5VisionMode;

/**
 * @brief   What produced a detected object.
 * @see     V5_DeviceVisionObject::type
 */
typedef enum
{
    kVisionTypeNormal = 0,    ///< A plain color signature.
    kVisionTypeColorCode = 1, ///< A color code built from several signatures.
    kVisionTypeLineDetect = 2 ///< Line detection.
} V5VisionBlockType;

// White balance

/**
 * @brief   White balance behaviour of the Vision Sensor.
 * @see     vexDeviceVisionWhiteBalanceModeSet
 */
typedef enum
{
    kVisionWBNormal = 0, ///< Automatic, using the brightest part of the image.
    kVisionWBStart = 1,  ///< Automatic, but only once at power on.
    kVisionWBManual =
        2 ///< Use the color set by vexDeviceVisionWhiteBalanceSet().
} V5VisionWBMode;

// LED control, from V5 or by the vision sensor

/**
 * @brief   Who drives the Vision Sensor's illumination LED.
 * @see     vexDeviceVisionLedModeSet
 */
typedef enum
{
    kVisionLedModeAuto = 0, ///< Shows the most prominent object's color.
    kVisionLedModeManual =
        1 ///< Use the color set by vexDeviceVisionLedColorSet().
} V5VisionLedMode;

// Wifi mode

/**
 * @brief   Vision Sensor Wi-Fi hotspot state.
 * @details vexide documents that the hotspot streams the camera feed at
 *          192.168.1.1 and disables all object detection while it is on.
 * @see     vexDeviceVisionWifiModeSet
 */
typedef enum
{
    kVisionWifiModeOff = 0, ///< Hotspot off.
    kVisionWifiModeOn = 1   ///< Hotspot on, object detection disabled.
} V5VisionWifiMode;

// signature should be 40 bytes and is the same data
// as sent to the vision sensor

/**
 * @brief   Status flag in V5_DeviceVisionSignature::flags.
 * @details PROS treats a zero flags byte after vexDeviceVisionSignatureGet()
 *          as the signature not being valid.
 */
#define VISION_SIG_FLAG_STATUS (1 << 0)

/**
 * @brief   One color signature stored on the Vision Sensor.
 * @details vexide documents the sensor's memory as volatile, so signatures
 *          are lost when it loses power. Thresholds are on the U and V
 *          chroma components of the Y'UV color space.
 */
typedef struct __attribute__((__packed__)) _V5_DeviceVisionSignature
{
    uint8_t id;     ///< Signature slot, 1 to 7.
    uint8_t flags;  ///< Status flags, see #VISION_SIG_FLAG_STATUS.
    uint8_t pad[2]; ///< Padding.
    float range;    ///< How lenient matching is; 0 to 11 in Vision Utility.
                    ///< Larger values accept lighter and darker shades.
    int32_t uMin;   ///< Minimum of the U threshold.
    int32_t uMax;   ///< Maximum of the U threshold.
    int32_t uMean;  ///< Mean of the U threshold.
    int32_t vMin;   ///< Minimum of the V threshold.
    int32_t vMax;   ///< Maximum of the V threshold.
    int32_t vMean;  ///< Mean of the V threshold.
    uint32_t mRgb;  ///< Signature color.
    uint32_t mType; ///< #kVisionTypeNormal, or #kVisionTypeColorCode for a
                    ///< signature used in a color code.
} V5_DeviceVisionSignature;

/**
 * @brief   One object reported by the Vision Sensor.
 * @details Coordinates are in the sensor's 316 by 212 pixel field of view.
 * @see     vexDeviceVisionObjectGet
 */
typedef struct __attribute__((__packed__)) _V5_DeviceVisionObject
{
    uint16_t signature;     /// block signature
    V5VisionBlockType type; /// block type
    uint16_t xoffset;       /// left side of block
    uint16_t yoffset;       /// top of block
    uint16_t width;         /// width of block
    uint16_t height;        /// height of block
    uint16_t angle;         /// angle of CC block in 0.1 deg units
} V5_DeviceVisionObject;

// Color structure, used for LED and white balance

/**
 * @brief   An 8 bit per channel color used by the Vision Sensor.
 * @details Used for the manual white balance color and for the LED color.
 */
typedef struct __attribute__((__packed__)) _V5_DeviceVisionRgb
{
    uint8_t red;        ///< Red channel, 0 to 255.
    uint8_t green;      ///< Green channel, 0 to 255.
    uint8_t blue;       ///< Blue channel, 0 to 255.
    uint8_t brightness; /// not used yet
} V5_DeviceVisionRgb;

/*----------------------------------------------------------------------------*/
/** @brief      VEX AI Vision sensor definitions */
/*----------------------------------------------------------------------------*/

/**
 * @brief   One color descriptor stored on the AI Vision Sensor.
 * @details hangle and hdsat set how far a detected color's hue and saturation
 *          may vary from the reference color.
 * @see     vexDeviceAiVisionColorSet
 */
typedef struct __attribute__((__packed__)) _V5_DeviceAiVisionColor
{
    uint8_t id;   ///< Descriptor slot, 1 to 7.
    uint8_t red;  ///< Red channel of the reference color.
    uint8_t grn;  ///< Green channel of the reference color.
    uint8_t blu;  ///< Blue channel of the reference color.
    float hangle; ///< Hue range. PROS documents 1 to 40; vexide says VEXcode
                  ///< limits it to 0 to 20.
    float hdsat;  ///< Saturation range. PROS documents 0.1 to 1.
} V5_DeviceAiVisionColor;

/**
 * @brief   One code descriptor stored on the AI Vision Sensor.
 * @details A code merges nearby objects matching its color descriptors into
 *          one object. vexide fills the ids in order and sets len to the
 *          number of non-zero ids.
 * @warning VEX's 2024 header and vex-sdk declare this structure with seven
 *          ids, c1 to c7. This copy has only five, so it is 4 bytes shorter
 *          than what VEXos reads and writes.
 * @see     vexDeviceAiVisionCodeSet
 */
typedef struct __attribute__((__packed__)) _V5_DeviceAiVisionCode
{
    uint8_t id;  ///< Descriptor slot. PROS documents 1 to 5, vexide accepts
                 ///< 1 to 8.
    uint8_t len; ///< Number of color descriptor ids in use.
    int16_t c1;  ///< Id of the first color descriptor.
    int16_t c2;  ///< Id of the second color descriptor.
    int16_t c3;  ///< Id of the third color descriptor.
    int16_t c4;  ///< Id of the fourth color descriptor.
    int16_t c5;  ///< Id of the fifth color descriptor.
} V5_DeviceAiVisionCode;

/**
 * @brief   One object reported by the AI Vision Sensor.
 * @details type decides which member of the union holds the object: color
 *          for color and code objects, model for AI model objects and tag
 *          for AprilTags. vexide documents the sensor's resolution as 320 by
 *          240.
 * @see     vexDeviceAiVisionObjectGet
 */
typedef struct __attribute__((__packed__)) _V5_DeviceAiVisionObject
{
    uint8_t id;   ///< Color or code descriptor id, model class id, or
                  ///< AprilTag id, depending on type.
    uint8_t type; ///< Kind of object, which selects the union member.

    union
    {
        /** @brief  A color or code object, as a bounding box. */
        struct __attribute__((__packed__))
        {
            uint16_t xoffset; ///< X of the box's position.
            uint16_t yoffset; ///< Y of the box's position.
            uint16_t width;   ///< Width of the box.
            uint16_t height;  ///< Height of the box.
            uint16_t angle;   ///< Angle in 0.1 degree units. vexide only reads
                              ///< it for codes.
        } color;

        /** @brief  An AprilTag, as its four corners. */
        struct __attribute__((__packed__))
        {
            int16_t x0; ///< X of the top left corner.
            int16_t y0; ///< Y of the top left corner.
            int16_t x1; ///< X of the top right corner.
            int16_t y1; ///< Y of the top right corner.
            int16_t x2; ///< X of the bottom right corner.
            int16_t y2; ///< Y of the bottom right corner.
            int16_t x3; ///< X of the bottom left corner.
            int16_t y3; ///< Y of the bottom left corner.
        } tag;

        /** @brief  An AI model object, as a bounding box and a score. */
        struct __attribute__((__packed__))
        {
            uint16_t xoffset; ///< X of the box's position.
            uint16_t yoffset; ///< Y of the box's position.
            uint16_t width;   ///< Width of the box.
            uint16_t height;  ///< Height of the box.
            uint16_t score;   ///< Confidence reported by the model.
        } model;
    } object; ///< The object itself, selected by type.
} V5_DeviceAiVisionObject;

/*----------------------------------------------------------------------------*/
/** @brief      V5 IMU sensor definitions */
/*----------------------------------------------------------------------------*/

// Quaternion data from IMU

/**
 * @brief   Quaternion describing the Inertial Sensor's orientation.
 * @details vexide reads a, b and c as the vector part (x, y, z) and d as the
 *          scalar part.
 * @see     vexDeviceImuQuaternionGet
 */
typedef struct __attribute__((__packed__)) _V5_DeviceImuQuaternion
{
    double a; ///< x component.
    double b; ///< y component.
    double c; ///< z component.
    double d; ///< Scalar (w) component.
} V5_DeviceImuQuaternion;

// Attitude data from IMU

/**
 * @brief   Euler angles describing the Inertial Sensor's orientation.
 * @details All three are in degrees and bounded to -180 through 180.
 * @see     vexDeviceImuAttitudeGet
 */
typedef struct __attribute__((__packed__)) _V5_DeviceImuAttitude
{
    double pitch; // x
    double roll;  // y
    double yaw;   // z
} V5_DeviceImuAttitude;

// Raw data from IMU

/**
 * @brief   Raw three axis reading from the Inertial Sensor.
 * @details Gyroscope readings are in degrees per second, accelerometer
 *          readings are in g. PROS documents that w is never filled in.
 */
typedef struct __attribute__((__packed__)) _V5_DeviceImuRaw
{
    double x; ///< Reading about or along the x axis.
    double y; ///< Reading about or along the y axis.
    double z; ///< Reading about or along the z axis.
    double w; ///< Not filled in.
} V5_DeviceImuRaw;

// native is same direction as 3wire gyro
// clockwise is positive

/**
 * @brief   Heading direction setting for the Inertial Sensor.
 * @details Used in the mode word of vexDeviceImuModeSet(). The misspelt
 *          name is the one in VEX's header.
 */
typedef enum _V5ImuHeadingnMode
{
    kImuHeadingNative =
        0x00, ///< Native. VEX's header says this is the same direction as
              ///< the 3-Wire gyro, clockwise positive.
    kImuHeadingIQ = 0x01 ///< IQ. VEX does not document it further.
} _V5ImuHeadingnMode;

// Orientation mode

/**
 * @brief   Which way the Inertial Sensor is mounted.
 * @details Used in the mode word of vexDeviceImuModeSet(). vexide decodes
 *          the orientation found at calibration from bits 1 to 3 of
 *          vexDeviceImuStatusGet(), as 0 to 5 in the same order as these
 *          values.
 */
typedef enum _V5ImuOrientationMode
{
    kImuOrientationZUp = 0x00,   ///< Z axis up, VEX logo facing down.
    kImuOrientationZDown = 0x10, ///< Z axis down, VEX logo facing up.
    kImuOrientationXUp = 0x20,   ///< X axis up.
    kImuOrientationXDown = 0x30, ///< X axis down.
    kImuOrientationYUp = 0x40,   ///< Y axis up.
    kImuOrientationYDown = 0x50, ///< Y axis down.
    kImuOrientationAuto = 0x80   ///< Automatic. VEX does not document it
                                 ///< further.
} V5ImuOrientationMode;

// Quaternion mode

/**
 * @brief   Quaternion mode setting for the Inertial Sensor.
 * @details Used in the mode word of vexDeviceImuModeSet(). VEX does not
 *          document the difference between the two.
 */
typedef enum _V5ImuQuaternionMode
{
    kImuQuaternionProcessed = 0x000, ///< Processed.
    kImuQuaternionRaw = 0x100        ///< Raw.
} V5ImuQuaternionMode;

/*----------------------------------------------------------------------------*/
/** @brief      V5 Color sensor definitions */
/*----------------------------------------------------------------------------*/
// for raw crgb data

/**
 * @brief   Raw clear, red, green and blue readings from the Optical Sensor.
 * @see     vexDeviceOpticalRawGet
 */
typedef struct _V5_DeviceOpticalRaw
{
    uint16_t clear; ///< Clear reading.
    uint16_t red;   ///< Red reading.
    uint16_t green; ///< Green reading.
    uint16_t blue;  ///< Blue reading.
} V5_DeviceOpticalRaw;

/**
 * @brief   Processed color reading from the Optical Sensor.
 * @see     vexDeviceOpticalRgbGet
 */
typedef struct _V5_DeviceOpticalRgb
{
    double red;        ///< Red component.
    double green;      ///< Green component.
    double blue;       ///< Blue component.
    double brightness; ///< Brightness.
} V5_DeviceOpticalRgb;

// gesture data

/**
 * @brief   Raw gesture data from the Optical Sensor.
 * @details PROS documents that gestures are cleared after 500 ms.
 * @see     vexDeviceOpticalGestureGet
 */
typedef struct _V5_DeviceOpticalGesture
{
    uint8_t udata;  ///< Up data.
    uint8_t ddata;  ///< Down data.
    uint8_t ldata;  ///< Left data.
    uint8_t rdata;  ///< Right data.
    uint8_t type;   ///< Gesture: 0 none, 1 up, 2 down. PROS reads 3 as right
                    ///< and 4 as left; vexide reads them the other way round.
    uint8_t pad;    ///< Padding.
    uint16_t count; ///< Number of gestures.
    uint32_t time;  ///< Time since the gesture was recognised. vexide treats
                    ///< it as milliseconds.
} V5_DeviceOpticalGesture;

/*----------------------------------------------------------------------------*/
/** @brief      V5 magnet definitions */
/*----------------------------------------------------------------------------*/

/**
 * @brief   Duration presets for the Workcell electromagnet.
 * @details Used by vexDeviceMagnetPickup() and vexDeviceMagnetDrop(). VEX
 *          does not publish the duration each value maps to.
 */
typedef enum _V5_DeviceMagnetDuration
{
    kMagnetDurationShort,     ///< Short.
    kMagnetDurationMedium,    ///< Medium.
    kMagnetDurationLong,      ///< Long.
    kMagnetDurationExtraLong, ///< Extra long.
} V5_DeviceMagnetDuration;

/*----------------------------------------------------------------------------*/
/** @brief      V5 gps definitions */
/*----------------------------------------------------------------------------*/
// Quaternion data from GPS

/**
 * @brief   Quaternion describing the GPS Sensor's orientation.
 * @details vex-sdk names the members x, y, z and w, and vexide reads them as
 *          the vector part and the scalar part in that order.
 * @see     vexDeviceGpsQuaternionGet
 */
typedef struct __attribute__((__packed__)) _V5_DeviceGpsQuaternion
{
    double a; ///< x component.
    double b; ///< y component.
    double c; ///< z component.
    double d; ///< Scalar (w) component.
} V5_DeviceGpsQuaternion;

// Attitude data from GPS, collect all useful info into this structure now

/**
 * @brief   Combined position and orientation reported by the GPS Sensor.
 * @details Positions are in metres with (0, 0) at the centre of the field,
 *          for the reference point set with vexDeviceGpsOriginSet(). Angles
 *          are in degrees.
 * @see     vexDeviceGpsAttitudeGet
 */
typedef struct __attribute__((__packed__)) _V5_DeviceGpsAttitude
{
    double pitch; // x
    double roll;  // y
    double yaw;   // z

    // spacial position on the field
    double position_x; ///< X position on the field, in metres.
    double position_y; ///< Y position on the field, in metres.
    double position_z; ///< Z position. No runtime reads it.

    // alternative roll, pitch and yaw
    double az;  ///< Not documented beyond the comment above.
    double el;  ///< Not documented beyond the comment above.
    double rot; ///< Not documented beyond the comment above.
} V5_DeviceGpsAttitude;

// Raw data from IMU

/**
 * @brief   Raw three axis reading from the GPS Sensor's inertial sensor.
 * @details VEX does not document the units, or whether w is written.
 */
typedef struct __attribute__((__packed__)) _V5_DeviceGpsRaw
{
    double x; ///< x axis reading.
    double y; ///< y axis reading.
    double z; ///< z axis reading.
    double w; ///< Not read by PROS or vexide.
} V5_DeviceGpsRaw;

/*----------------------------------------------------------------------------*/
/** @brief      V5 SD File system definitions */
/*----------------------------------------------------------------------------*/

/**
 * @brief   Opaque file handle.
 * @details Returned as a FIL* by vexFileOpen() and the other open calls. The
 *          Rust standard library describes it as a pointer to a FatFs file
 *          object managed by VEXos. A PROS comment gives the open file limit
 *          as 8 as of VEXos 0.7.4b55.
 */
typedef void FIL;

// seek flags
// changed, Mar 6 2018 to be more consistent with stdio.h

/** Seek relative to the start of the file. */
#define FS_SEEK_SET 0
/** Seek relative to the current file position. */
#define FS_SEEK_CUR 1
/** Seek relative to the end of the file. */
#define FS_SEEK_END 2

// new 1.0.13, file status

/** Set in the vexFileStatus() result when the path exists. */
#define FS_FILE_EXIST 1
/** Set, along with #FS_FILE_EXIST, when the path is a directory. */
#define FS_FILE_DIR 2

/**
 * @brief   Result code returned by the file system calls.
 * @details The Rust standard library describes VEX's file system as a
 *          derivative of FatFs (Xilinx's xilffs), and these are FatFs's
 *          result codes.
 */
typedef enum
{
    FR_OK = 0U,      /// (0) Succeeded
    FR_DISK_ERR,     /// (1) A hard error occurred in the low level disk I/O
                     /// layer
    FR_INT_ERR,      /// (2) Assertion failed
    FR_NOT_READY,    /// (3) The physical drive cannot work
    FR_NO_FILE,      /// (4) Could not find the file
    FR_NO_PATH,      /// (5) Could not find the path
    FR_INVALID_NAME, /// (6) The path name format is invalid
    FR_DENIED,       /// (7) Access denied due to prohibited access or directory
                     /// full
    FR_EXIST,        /// (8) Access denied due to prohibited access
    FR_INVALID_OBJECT,  /// (9) The file/directory object is invalid
    FR_WRITE_PROTECTED, /// (10) The physical drive is write protected
    FR_INVALID_DRIVE,   /// (11) The logical drive number is invalid
    FR_NOT_ENABLED,     /// (12) The volume has no work area
    FR_NO_FILESYSTEM,   /// (13) There is no valid FAT volume
    FR_MKFS_ABORTED,    /// (14) The f_mkfs() aborted due to any parameter
                        /// error
    FR_TIMEOUT, /// (15) Could not get a grant to access the volume within
                /// defined period
    FR_LOCKED,  /// (16) The operation is rejected according to the file
                /// sharing policy
    FR_NOT_ENOUGH_CORE,     /// (17) LFN working buffer could not be allocated
    FR_TOO_MANY_OPEN_FILES, /// (18) Number of open files > _FS_SHARE
    FR_INVALID_PARAMETER    /// (19) Given parameter is invalid
} FRESULT;

/*----------------------------------------------------------------------------*/
/** @brief      V5 touch events */
/*----------------------------------------------------------------------------*/

/**
 * @brief   The most recent kind of touchscreen event.
 * @see     vexTouchDataGet, vexTouchUserCallbackSet
 */
typedef enum _touchEvent
{
    kTouchEventRelease,  ///< Released.
    kTouchEventPress,    ///< Pressed.
    kTouchEventPressAuto ///< Held down (PROS's E_TOUCH_HELD).
} V5_TouchEvent;

/**
 * @brief   Touchscreen state.
 * @see     vexTouchDataGet
 */
typedef struct _V5_TouchStatus
{
    V5_TouchEvent lastEvent; ///< Kind of the most recent event.
    int16_t lastXpos;        ///< X coordinate of the most recent event.
    int16_t lastYpos;        ///< Y coordinate of the most recent event.
    int32_t pressCount;      ///< Number of presses.
    int32_t releaseCount;    ///< Number of releases.
} V5_TouchStatus;

/*----------------------------------------------------------------------------*/
/** @brief      V5 competition status bits                                    */
/*----------------------------------------------------------------------------*/

/**
 * @name    Competition status bits
 * @brief   Bits returned by vexCompetitionStatus().
 *
 * @details
 *  V5_COMP_BIT_COMP is set when a competition switch or field control is
 *  connected, and V5_COMP_BIT_GAME then means field control rather than a
 *  switch; PROS and vexide read them that way. V5_COMP_BIT_EBL is set when
 *  the robot is disabled, not enabled. VEX's 2024 header also defines
 *  V5_COMP_BIT_OPT1 (16) and V5_COMP_BIT_OPT2 (32), "option" bits for
 *  VEXos 1.1.3 and later, which are missing here.
 * @{
 */
#define V5_COMP_BIT_EBL 1  // if set then robot disabled
#define V5_COMP_BIT_MODE 2 // if set then robot in autonomous
#define V5_COMP_BIT_COMP                                                       \
    4 // if set then either comp switch or field control connected
#define V5_COMP_BIT_GAME 8 // if set then field control connected

/** @} */

/*----------------------------------------------------------------------------*/
/** @brief   structure holding image info - used by bmp/png read code */
/*----------------------------------------------------------------------------*/
// data must point to suitable buffer now

/**
 * @brief   Describes a decoded image.
 * @details Before calling vexImageBmpRead() or vexImagePngRead(), set data
 *          to a buffer of at least maxw * maxh 32 bit pixels. Pixels are
 *          RGB8, the same encoding as the display colors.
 */
typedef struct __attribute__((__packed__)) _v5_image
{
    uint16_t width;  ///< Image width in pixels.
    uint16_t height; ///< Image height in pixels.
    uint32_t *data;  ///< Caller supplied pixel buffer.
    uint32_t *p;     ///< Not documented.
} v5_image;

/** Width of the brain's display in pixels. */
#define SYSTEM_DISPLAY_WIDTH 480
/** Height of the brain's display in pixels, including the 32 row program
 * header. */
#define SYSTEM_DISPLAY_HEIGHT 272

#ifdef __cplusplus
}
#endif
#endif /* V5_APITYPES_H_*/
