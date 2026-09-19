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
 *  the port indexed wrappers in v5_apiuser.h. Nothing here allocates storage or
 *  calls into VEXos, so the file is safe to include on its own.
 *
 *  Structures that VEXos reads or writes directly are declared packed so that
 *  their layout matches the wire format used between the user processor and the
 *  devices. Do not reorder their fields.
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
        uint32_t magic; ///< Magic number, must be set to #V5_SIG_MAGIC or VEXos
                        ///< will refuse to run the program.
        uint32_t type;  ///< Program type. Only #V5_SIG_TYPE_USER is defined.
        uint32_t
            owner; ///< Program originator, one of the V5_SIG_OWNER_* values.
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

/** Program belongs to the V5 system itself. */
#define V5_SIG_OWNER_SYS 0
/** Program was produced by VEX. */
#define V5_SIG_OWNER_VEX 1
/** Program was produced by a VEX partner. */
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
     * @brief   Wall clock time read from the brain's real time clock.
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
     * @brief   Calendar date read from the brain's real time clock.
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
     * @details Returned by vexDeviceGetStatus() for every port index, and
     * accepted by vexDevicesGetNumberByType(). Several values cover hardware
     * that was never sold and only appeared on development systems.
     */
    typedef enum
    {
        kDeviceTypeNoSensor = 0,     ///< Nothing plugged into the port.
        kDeviceTypeMotorSensor = 2,  ///< V5 Smart Motor.
        kDeviceTypeLedSensor = 3,    ///< LED module, development hardware only.
        kDeviceTypeAbsEncSensor = 4, ///< V5 Rotation Sensor.
        kDeviceTypeCrMotorSensor =
            5, ///< Continuous rotation motor, development hardware only.
        kDeviceTypeImuSensor = 6,      ///< V5 Inertial Sensor.
        kDeviceTypeRangeSensor = 7,    // obsolete
        kDeviceTypeDistanceSensor = 7, ///< V5 Distance Sensor. Shares its value
                                       ///< with the obsolete range sensor.
        kDeviceTypeRadioSensor = 8,    ///< VEXnet radio.
        kDeviceTypeTetherSensor =
            9, ///< Controller tethered over the wired smart port.
        kDeviceTypeBrainSensor =
            10, ///< The brain itself, reported for internal ports.
        kDeviceTypeVisionSensor = 11, ///< V5 Vision Sensor.
        kDeviceTypeAdiSensor =
            12, ///< 3-Wire Expander, or the brain's built in 3-Wire ports.
        kDeviceTypeRes1Sensor =
            13, ///< Reserved. Reported for the partner controller.
        kDeviceTypeRes2Sensor = 14,    ///< Reserved. Reported for the battery.
        kDeviceTypeRes3Sensor = 15,    ///< Reserved.
        kDeviceTypeOpticalSensor = 16, ///< V5 Optical Sensor.
        kDeviceTypeMagnetSensor = 17,  ///< Workcell electromagnet.
        kDeviceTypeGpsSensor = 20,     ///< V5 GPS Sensor.
        kDeviceTypeBumperSensor =
            0x40, ///< Bumper switch, development hardware only.
        kDeviceTypeGyroSensor =
            0x46, ///< Dedicated gyro, development hardware only.
        kDeviceTypeSonarSensor =
            0x47, ///< Dedicated sonar, development hardware only.
        kDeviceTypeGenericSensor =
            128, ///< Generic sensor read with vexDeviceGenericValueGet().
        kDeviceTypeGenericSerial =
            129, ///< Port switched to generic serial mode.
        kDeviceTypeUndefinedSensor =
            255 ///< Device present but not recognised by VEXos.
    } V5_DeviceType;

    /**
     * @brief   Opaque handle to a device.
     * @details Obtained from vexDeviceGetByIndex() or vexDevicesGet(). The
     * pointed to structure is owned by VEXos and must not be dereferenced or
     *          freed by user code. A handle stays valid for the life of the
     *          program, but the device it refers to may be unplugged at any
     * time.
     */
    typedef struct _V5_Device *V5_DeviceT;

/**
 * @brief   Upper bound on the number of device port indexes VEXos can report.
 * @details Larger than the number of physical smart ports because VEXos also
 *          exposes internal ports for the controllers, the battery and the
 *          brain. Use this as the length of any buffer passed to
 *          vexDeviceGetStatus().
 */
#define V5_MAX_DEVICE_PORTS 32

    /** Buffer large enough to hold the type of every port VEXos can report. */
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
     * @details The first block of names describes the hardware channels. The
     *          aliases at the end of the enum are the labels printed on the
     *          production controller, so ButtonA and Button8R are the same
     * channel.
     *
     *          Analog channels return -127 to 127, button channels return 0 or
     * 1, and the battery channels return a percentage. A disconnected
     *          controller reads as 0 on every channel.
     */
    typedef enum _V5_ControllerIndex
    {
        AnaLeftX = 0, ///< Left stick, horizontal axis.
        AnaLeftY,     ///< Left stick, vertical axis.
        AnaRightX,    ///< Right stick, horizontal axis.
        AnaRightY,    ///< Right stick, vertical axis.
        AnaSpare1,    ///< Unused analog channel.
        AnaSpare2,    ///< Unused analog channel.

        Button5U, ///< Upper button in the left shoulder pair.
        Button5D, ///< Lower button in the left shoulder pair.
        Button6U, ///< Upper button in the right shoulder pair.
        Button6D, ///< Lower button in the right shoulder pair.
        Button7U, ///< Up on the left button cluster.
        Button7D, ///< Down on the left button cluster.
        Button7L, ///< Left on the left button cluster.
        Button7R, ///< Right on the left button cluster.
        Button8U, ///< Up on the right button cluster.
        Button8D, ///< Down on the right button cluster.
        Button8L, ///< Left on the right button cluster.
        Button8R, ///< Right on the right button cluster.

        ButtonSEL, ///< Centre select button, normally consumed by VEXos.

        BatteryLevel, ///< Controller battery level as a percentage.

        ButtonAll,       ///< All button states packed into a single word.
        Flags,           ///< Controller status flags.
        BatteryCapacity, ///< Controller battery capacity as a percentage.

        // Final V5 names
        Axis1 =
            AnaRightX, ///< Production label for the right stick, horizontal.
        Axis2 = AnaRightY, ///< Production label for the right stick, vertical.
        Axis3 = AnaLeftY,  ///< Production label for the left stick, vertical.
        Axis4 = AnaLeftX,  ///< Production label for the left stick, horizontal.

        ButtonL1 =
            Button5U, ///< Production label for the upper left shoulder button.
        ButtonL2 =
            Button5D, ///< Production label for the lower left shoulder button.
        ButtonR1 =
            Button6U, ///< Production label for the upper right shoulder button.
        ButtonR2 =
            Button6D, ///< Production label for the lower right shoulder button.

        ButtonUp = Button7U, ///< Production label for up on the left cluster.
        ButtonDown =
            Button7D, ///< Production label for down on the left cluster.
        ButtonLeft =
            Button7L, ///< Production label for left on the left cluster.
        ButtonRight =
            Button7R, ///< Production label for right on the left cluster.

        ButtonX = Button8U, ///< Production label for up on the right cluster.
        ButtonB = Button8D, ///< Production label for down on the right cluster.
        ButtonY = Button8L, ///< Production label for left on the right cluster.
        ButtonA = Button8R ///< Production label for right on the right cluster.
    } V5_ControllerIndex;

    /**
     * @brief   How a controller is attached to the brain.
     * @see     vexControllerConnectionStatusGet
     */
    typedef enum _V5_ControllerStatus
    {
        kV5ControllerOffline = 0, ///< No controller is connected.
        kV5ControllerTethered,    ///< Connected over a wired tether.
        kV5ControllerVexnet       ///< Connected wirelessly over VEXnet.
    } V5_ControllerStatus;

    /** Selects one of the two controllers the brain can pair with. */
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
     * @details The values are ordinary 0x00RRGGBB colors, so vexDeviceLedSet()
     * and vexDeviceLedRgbSet() accept the same encoding. This device was never
     *          sold and only exists on development systems.
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
     * @details Set with vexDeviceAdiPortConfigSet(). The configuration decides
     * how vexDeviceAdiValueGet() and vexDeviceAdiValueSet() interpret the
     *          channel, so it must be set before the channel is used.
     *
     *          Configuring a channel as a quadrature encoder or sonar claims
     * the adjacent channel as well, since both need two wires. Those pairs
     *          always start on an even numbered channel.
     */
    typedef enum _V5_AdiPortConfiguration
    {
        kAdiPortTypeAnalogIn = 0, ///< 12 bit analog input, reads 0 to 4095.
        kAdiPortTypeAnalogOut,    ///< Analog output.
        kAdiPortTypeDigitalIn,    ///< Digital input, reads 0 or 1.
        kAdiPortTypeDigitalOut,   ///< Digital output, driven with 0 or 1.

        kAdiPortTypeSmartButton, ///< Smart button input.
        kAdiPortTypeSmartPot,    ///< Smart potentiometer input.

        kAdiPortTypeLegacyButton,        ///< Cortex era bumper or limit switch.
        kAdiPortTypeLegacyPotentiometer, ///< Cortex era potentiometer.
        kAdiPortTypeLegacyLineSensor,    ///< Cortex era line tracker.
        kAdiPortTypeLegacyLightSensor,   ///< Cortex era light sensor.
        kAdiPortTypeLegacyGyro, ///< Cortex era yaw rate gyro. Calibrates for
                                ///< about one second after configuration.
        kAdiPortTypeLegacyAccelerometer, ///< Cortex era accelerometer.

        kAdiPortTypeLegacyServo, ///< Cortex era servo output.
        kAdiPortTypeLegacyPwm,   ///< Cortex era motor controller output.

        kAdiPortTypeQuadEncoder, ///< Quadrature encoder. Uses this channel and
                                 ///< the next one.
        kAdiPortTypeSonar, ///< Ultrasonic rangefinder. This channel is the ping
                           ///< output, the next is the echo input.

        kAdiPortTypeLegacyPwmSlew, ///< Cortex era motor controller output with
                                   ///< slew rate limiting.

        kAdiPortTypeUndefined = 255 ///< Channel not configured.
    } V5_AdiPortConfiguration;

/** Number of 3-Wire channels on a brain or a 3-Wire Expander. Channels are
 * numbered 0 to 7. */
#define V5_ADI_PORT_NUM 8

    /*----------------------------------------------------------------------------*/
    /** @brief      V5 Bumper switch device definitions */
    /*----------------------------------------------------------------------------*/

    /**
     * @brief   State of a smart port bumper switch.
     * @note    The two trailing comments below are swapped in the original
     * header. kBumperReleased means the switch is released and kBumperPressed
     *          means it is pressed.
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
     * @brief   Closed loop control mode a Smart Motor is running in.
     * @details Set implicitly by the various target functions, so most code
     * reads this with vexDeviceMotorModeGet() rather than setting it directly.
     *          Calling vexDeviceMotorVoltageSet() or vexDeviceMotorPwmSet()
     * leaves the motor in open loop and does not use these modes.
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
     * @brief   What a Smart Motor does once it stops being commanded.
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
     * @details Applies to vexDeviceMotorPositionGet(),
     * vexDeviceMotorPositionSet() and the position targets. Raw counts are
     * unaffected by the gearset, degrees and rotations are scaled to the output
     * shaft.
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
     * @details VEXos needs this to convert between raw encoder counts and
     * degrees, and to work out the motor's free speed. The cartridge colors are
     *          red for 36:1, green for 18:1 and blue for 6:1.
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
 * @details The 18:1 cartridge produces 900 counts per revolution and the 6:1
 *          cartridge produces 300, because all three share the same encoder on
 *          the motor's input shaft.
 */
#define V5_MOTOR_COUNTS_PER_ROT 1800.0

    //
    // preliminary, used for position and velocity
    //

    /**
     * @brief   Gain set for a motor's internal position or velocity controller.
     * @details Passed to vexDeviceMotorPositionPidSet() and
     *          vexDeviceMotorVelocityPidSet(). VEX has never published the
     * scaling of the individual terms or the motor's default values, so
     * changing them is a trial and error exercise. The padding fields exist
     * only to match the packet layout sent to the motor and should be left at
     * zero.
     */
    typedef struct __attribute__((__packed__)) _V5_DeviceMotorPid
    {
        uint8_t kf;        ///< Feedforward gain.
        uint8_t kp;        ///< Proportional gain.
        uint8_t ki;        ///< Integral gain.
        uint8_t kd;        ///< Derivative gain.
        uint8_t filter;    ///< Error filtering strength.
        uint8_t pad1;      ///< Padding, set to zero.
        uint16_t limit;    ///< Integral windup limit.
        uint8_t threshold; ///< Error threshold below which the loop treats the
                           ///< target as reached.
        uint8_t loopspeed; ///< Control loop interval.
        uint8_t pad2[2];   ///< Padding, set to zero.
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
        kVisionModeNormal =
            0, ///< Detect color signatures and color codes only.
        kVisionModeMixed =
            1, ///< Detect color signatures, color codes and lines.
        kVisionModeLineDetect = 2, ///< Detect lines only.
        kVisionTypeTest = 3        ///< Factory test mode.
    } V5VisionMode;

    /**
     * @brief   What produced a detected object.
     * @see     V5_DeviceVisionObject::type
     */
    typedef enum
    {
        kVisionTypeNormal = 0, ///< Matched a single color signature.
        kVisionTypeColorCode =
            1, ///< Matched a color code built from several signatures.
        kVisionTypeLineDetect = 2 ///< Produced by line detection.
    } V5VisionBlockType;

    // White balance

    /**
     * @brief   White balance behaviour of the Vision Sensor.
     * @see     vexDeviceVisionWhiteBalanceModeSet
     */
    typedef enum
    {
        kVisionWBNormal = 0, ///< Continuous automatic white balance.
        kVisionWBStart = 1,  ///< Balance once at startup, then hold.
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
        kVisionLedModeAuto = 0, ///< The sensor drives the LED itself.
        kVisionLedModeManual =
            1 ///< Use the color set by vexDeviceVisionLedColorSet().
    } V5VisionLedMode;

    // Wifi mode

    /**
     * @brief   Vision Sensor onboard access point state.
     * @details Turning the access point on lets a browser view the camera feed,
     * but the sensor stops reporting detected objects while it is enabled.
     * @see     vexDeviceVisionWifiModeSet
     */
    typedef enum
    {
        kVisionWifiModeOff = 0, ///< Access point disabled, detection running.
        kVisionWifiModeOn = 1   ///< Access point enabled, detection suspended.
    } V5VisionWifiMode;

// signature should be 40 bytes and is the same data
// as sent to the vision sensor

/**
 * @brief   Set in V5_DeviceVisionSignature::flags once the sensor has filled in
 *          a signature read back with vexDeviceVisionSignatureGet().
 */
#define VISION_SIG_FLAG_STATUS (1 << 0)

    /**
     * @brief   One color signature stored on the Vision Sensor.
     * @details The sensor holds seven signatures in volatile memory, so they
     * have to be written again after every power cycle. Thresholds are given in
     *          the U and V chroma planes of the Y'UV color space; the tuning
     *          utility in VEXcode produces them from a sample image.
     */
    typedef struct __attribute__((__packed__)) _V5_DeviceVisionSignature
    {
        uint8_t id;     ///< Signature slot, 1 to 7.
        uint8_t flags;  ///< Status bits, see #VISION_SIG_FLAG_STATUS.
        uint8_t pad[2]; ///< Padding, set to zero.
        float range;  ///< Match leniency, roughly 0 to 11. Larger values accept
                      ///< lighter and darker shades.
        int32_t uMin; ///< Lower bound of the U chroma threshold.
        int32_t uMax; ///< Upper bound of the U chroma threshold.
        int32_t uMean; ///< Centre of the U chroma threshold.
        int32_t vMin;  ///< Lower bound of the V chroma threshold.
        int32_t vMax;  ///< Upper bound of the V chroma threshold.
        int32_t vMean; ///< Centre of the V chroma threshold.
        uint32_t
            mRgb; ///< Representative color of the signature, as 0x00RRGGBB.
        uint32_t mType; ///< Distinguishes a plain signature from a color code
                        ///< member.
    } V5_DeviceVisionSignature;

    /**
     * @brief   One object reported by the Vision Sensor.
     * @details Coordinates are in the sensor's 316 by 212 pixel frame with the
     *          origin at the top left. The centre of an object is
     *          (xoffset + width / 2, yoffset + height / 2).
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
     * @details Used both for the manual white balance reference and for the
     *          illumination LED color.
     */
    typedef struct __attribute__((__packed__)) _V5_DeviceVisionRgb
    {
        uint8_t red;        ///< Red channel, 0 to 255.
        uint8_t green;      ///< Green channel, 0 to 255.
        uint8_t blue;       ///< Blue channel, 0 to 255.
        uint8_t brightness; /// not used yet
    } V5_DeviceVisionRgb;

    /*----------------------------------------------------------------------------*/
    /** @brief      V5 IMU sensor definitions */
    /*----------------------------------------------------------------------------*/

    // Quaternion data from IMU

    /**
     * @brief   Unit quaternion describing the Inertial Sensor's orientation.
     * @details The members are named a, b, c and d rather than the more usual
     *          w, x, y and z.
     * @see     vexDeviceImuQuaternionGet
     */
    typedef struct __attribute__((__packed__)) _V5_DeviceImuQuaternion
    {
        double a; ///< First quaternion component.
        double b; ///< Second quaternion component.
        double c; ///< Third quaternion component.
        double d; ///< Fourth quaternion component.
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
     *          readings are in g. Only x, y and z are filled in; w is left
     *          untouched by both vexDeviceImuRawGyroGet() and
     *          vexDeviceImuRawAccelGet().
     */
    typedef struct __attribute__((__packed__)) _V5_DeviceImuRaw
    {
        double x; ///< Reading about or along the x axis.
        double y; ///< Reading about or along the y axis.
        double z; ///< Reading about or along the z axis.
        double w; ///< Not written by the raw gyro or accelerometer calls.
    } V5_DeviceImuRaw;

    // native is same direction as 3wire gyro
    // clockwise is positive

    /**
     * @brief   Sign convention used for the Inertial Sensor's heading.
     * @details Part of the mode word passed to vexDeviceImuModeSet(). The name
     *          carries a typo that is kept for source compatibility.
     */
    typedef enum _V5ImuHeadingnMode
    {
        kImuHeadingNative =
            0x00, ///< Same direction as the 3-Wire gyro, clockwise positive.
        kImuHeadingIQ = 0x01 ///< IQ compatible direction.
    } _V5ImuHeadingnMode;

    // Orientation mode

    /**
     * @brief   Which way the Inertial Sensor is mounted.
     * @details Part of the mode word passed to vexDeviceImuModeSet(). The same
     *          three bits are reported back in bits 1 to 3 of
     *          vexDeviceImuStatusGet().
     */
    typedef enum _V5ImuOrientationMode
    {
        kImuOrientationZUp = 0x00,   ///< Z axis up, VEX logo facing down.
        kImuOrientationZDown = 0x10, ///< Z axis down, VEX logo facing up.
        kImuOrientationXUp = 0x20,   ///< X axis up.
        kImuOrientationXDown = 0x30, ///< X axis down.
        kImuOrientationYUp = 0x40,   ///< Y axis up.
        kImuOrientationYDown = 0x50, ///< Y axis down.
        kImuOrientationAuto = 0x80 ///< Let the sensor work out its orientation
                                   ///< during calibration.
    } V5ImuOrientationMode;

    // Quaternion mode

    /**
     * @brief   Whether the Inertial Sensor reports fused or raw quaternions.
     * @details Part of the mode word passed to vexDeviceImuModeSet().
     */
    typedef enum _V5ImuQuaternionMode
    {
        kImuQuaternionProcessed =
            0x000,                ///< Quaternion from the sensor fusion filter.
        kImuQuaternionRaw = 0x100 ///< Unfiltered quaternion.
    } V5ImuQuaternionMode;

    /*----------------------------------------------------------------------------*/
    /** @brief      V5 Color sensor definitions */
    /*----------------------------------------------------------------------------*/
    // for raw crgb data

    /**
     * @brief   Unprocessed channel counts from the Optical Sensor's color front
     * end.
     * @details Absolute magnitudes depend on the integration time and on how
     * much light reaches the sensor, so these are mainly useful as ratios.
     * @see     vexDeviceOpticalRawGet
     */
    typedef struct _V5_DeviceOpticalRaw
    {
        uint16_t clear; ///< Unfiltered channel count.
        uint16_t red;   ///< Red channel count.
        uint16_t green; ///< Green channel count.
        uint16_t blue;  ///< Blue channel count.
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
        double brightness; ///< Overall brightness, 0 to 1.
    } V5_DeviceOpticalRgb;

    // gesture data

    /**
     * @brief   Raw gesture data from the Optical Sensor.
     * @details Only filled in while gesture detection is enabled. A recognised
     *          gesture is cleared about 500 ms after it is detected.
     * @see     vexDeviceOpticalGestureGet
     */
    typedef struct _V5_DeviceOpticalGesture
    {
        uint8_t udata; ///< Up photodiode reading.
        uint8_t ddata; ///< Down photodiode reading.
        uint8_t ldata; ///< Left photodiode reading.
        uint8_t rdata; ///< Right photodiode reading.
        uint8_t type;  ///< Recognised direction: 0 none, 1 up, 2 down, 3 right,
                       ///< 4 left.
        uint8_t pad;   ///< Padding.
        uint16_t count; ///< Number of gestures recognised so far.
        uint32_t
            time; ///< Time since the gesture was recognised, in milliseconds.
    } V5_DeviceOpticalGesture;

    /*----------------------------------------------------------------------------*/
    /** @brief      V5 magnet definitions */
    /*----------------------------------------------------------------------------*/

    /**
     * @brief   How long the Workcell electromagnet energises for.
     * @details Used by vexDeviceMagnetPickup() and vexDeviceMagnetDrop(). VEX
     * does not publish the duration each value maps to.
     */
    typedef enum _V5_DeviceMagnetDuration
    {
        kMagnetDurationShort,     ///< Shortest pulse.
        kMagnetDurationMedium,    ///< Medium pulse.
        kMagnetDurationLong,      ///< Long pulse.
        kMagnetDurationExtraLong, ///< Longest pulse.
    } V5_DeviceMagnetDuration;

    /*----------------------------------------------------------------------------*/
    /** @brief      V5 gps definitions */
    /*----------------------------------------------------------------------------*/
    // Quaternion data from GPS

    /**
     * @brief   Unit quaternion describing the GPS Sensor's orientation.
     * @see     vexDeviceGpsQuaternionGet
     */
    typedef struct __attribute__((__packed__)) _V5_DeviceGpsQuaternion
    {
        double a; ///< First quaternion component.
        double b; ///< Second quaternion component.
        double c; ///< Third quaternion component.
        double d; ///< Fourth quaternion component.
    } V5_DeviceGpsQuaternion;

    // Attitude data from GPS, collect all useful info into this structure now

    /**
     * @brief   Combined position and orientation reported by the GPS Sensor.
     * @details Positions are in meters relative to the centre of the field, and
     *          are corrected for the mounting offset set with
     *          vexDeviceGpsOriginSet(). Angles are in degrees.
     * @see     vexDeviceGpsAttitudeGet
     */
    typedef struct __attribute__((__packed__)) _V5_DeviceGpsAttitude
    {
        double pitch; // x
        double roll;  // y
        double yaw;   // z

        // spacial position on the field
        double position_x; ///< X position on the field, in meters.
        double position_y; ///< Y position on the field, in meters.
        double position_z; ///< Z position on the field, in meters.

        // alternative roll, pitch and yaw
        double az; ///< Azimuth, an alternative expression of the orientation.
        double el; ///< Elevation, an alternative expression of the orientation.
        double rot; ///< Rotation, an alternative expression of the orientation.
    } V5_DeviceGpsAttitude;

    // Raw data from IMU

    /**
     * @brief   Raw three axis reading from the GPS Sensor's onboard IMU.
     * @details Gyroscope readings are in degrees per second, accelerometer
     *          readings are in g. Only x, y and z are filled in.
     */
    typedef struct __attribute__((__packed__)) _V5_DeviceGpsRaw
    {
        double x; ///< Reading about or along the x axis.
        double y; ///< Reading about or along the y axis.
        double z; ///< Reading about or along the z axis.
        double w; ///< Not written by the raw gyro or accelerometer calls.
    } V5_DeviceGpsRaw;

    /*----------------------------------------------------------------------------*/
    /** @brief      V5 SD File system definitions */
    /*----------------------------------------------------------------------------*/

    /**
     * @brief   Opaque file handle.
     * @details Returned as a FIL* by vexFileOpen() and the other open calls.
     * The structure belongs to VEXos and must not be dereferenced or freed.
     *          VEXos supports a limited number of files open at once.
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

/** Set in the vexFileStatus() result when the named path exists. */
#define FS_FILE_EXIST 1
/** Set in the vexFileStatus() result when the named path is a directory. */
#define FS_FILE_DIR 2

    /**
     * @brief   Result code returned by the file system calls.
     * @details These are the FatFs result codes, passed through unchanged.
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
        FR_DENIED, /// (7) Access denied due to prohibited access or directory
                   /// full
        FR_EXIST,  /// (8) Access denied due to prohibited access
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
        FR_NOT_ENOUGH_CORE, /// (17) LFN working buffer could not be allocated
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
        kTouchEventRelease,  ///< The screen was released.
        kTouchEventPress,    ///< The screen was pressed.
        kTouchEventPressAuto ///< Auto repeat while the screen is held down.
    } V5_TouchEvent;

    /**
     * @brief   Snapshot of the touchscreen state.
     * @details The position is in display pixels, and reflects where the last
     *          event happened rather than where a finger currently is. The two
     *          counters let code detect a new press or release without polling
     *          fast enough to catch the event itself.
     * @see     vexTouchDataGet
     */
    typedef struct _V5_TouchStatus
    {
        V5_TouchEvent lastEvent; ///< Kind of the most recent event.
        int16_t lastXpos;        ///< X coordinate of the most recent event.
        int16_t lastYpos;        ///< Y coordinate of the most recent event.
        int32_t pressCount;      ///< Running total of presses.
        int32_t releaseCount;    ///< Running total of releases.
    } V5_TouchStatus;

/*----------------------------------------------------------------------------*/
/** @brief      V5 competition status bits                                    */
/*----------------------------------------------------------------------------*/

/**
 * @name    Competition status bits
 * @brief   Bits returned by vexCompetitionStatus() and accepted by
 *          vexCompetitionControl().
 *
 * @details
 *  V5_COMP_BIT_COMP is set whenever anything is plugged into the competition
 *  port, and V5_COMP_BIT_GAME distinguishes real field control from a plain
 *  competition switch. The autonomous bit is only meaningful while the robot is
 *  enabled.
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
     * to a buffer of at least maxw * maxh 32 bit pixels. The decoder fills in
     *          width, height and the pixels themselves. Pixels use the same
     *          0x00RRGGBB layout as the rest of the display API, so the buffer
     * can be handed straight to vexDisplayCopyRect().
     */
    typedef struct __attribute__((__packed__)) _v5_image
    {
        uint16_t
            width; ///< Decoded image width in pixels, written by the decoder.
        uint16_t
            height; ///< Decoded image height in pixels, written by the decoder.
        uint32_t *data; ///< Caller supplied pixel buffer. Must be set before
                        ///< decoding.
        uint32_t *p;    ///< Points at the first pixel of the second row after a
                        ///< BMP read.
    } v5_image;

/** Width of the brain's display in pixels. */
#define SYSTEM_DISPLAY_WIDTH 480
/** Full height of the brain's display in pixels, including the VEXos header. */
#define SYSTEM_DISPLAY_HEIGHT 272

#ifdef __cplusplus
}
#endif
#endif /* V5_APITYPES_H_*/
