#ifndef _ENUM_H_
#define _ENUM_H_

enum class InputType : uint8_t {
    None = 0,
    Brakes,
    Gears,
    Lights,
    Stage,
    StageLock,
    Abort,
    SAS,
    RCS,
};

enum class MainControls : uint8_t {
    None = 0x00,
    Stage = 0x01, 
    Abort = 0x02, 
    Precision = 0x04, 
    Brakes = 0x08, 
    Gears = 0x10, 
    Lights = 0x20, 
    RCS = 0x40, 
    SAS = 0x80,
};

enum class SASMode : uint8_t {
    Off,          // 0
    SAS,          // 1
    Prograde,     // 2
    RetroGrade,   // 3
    Normal,       // 4
    Antinormal,   // 5
    RadialIn,     // 6
    RadialOut,    // 7
    Target,       // 8
    AntiTarget,   // 9
    ManeuverNode, // 10
};

enum class NavballMode : uint8_t {
    Ignore,
    Orbit,
    Surface,
    Target
};

#endif