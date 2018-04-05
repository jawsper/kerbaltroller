#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include "enum.h"

// struct __attribute__((__packed__)) MainControls {
//     bool stage : 1;
//     bool abort : 1;
//     bool precision : 1;
//     bool brakes : 1;
//     bool gears : 1;
//     bool lights : 1;
//     bool RCS : 1;
//     bool SAS : 1;
// };

struct __attribute__((__packed__)) InputOutput {
  uint8_t type; // 1: normal momentary; 2: danger switch
  uint8_t input;
  uint8_t output;
  bool invert;

  // MainControls control;
  // int8_t lock;
};

struct __attribute__((__packed__)) Input {
    InputType id;
    // 1: momentary
    // 2: toggle
    uint8_t type;
    uint8_t input;
    bool invert;
    MainControls control;

    InputType lock;
};

struct __attribute__((__packed__)) Output {
    
};


struct __attribute__((__packed__)) ActionGroupStatus {
    bool sas_on : 1;
    bool rcs_on : 1;
    bool lights_on : 1;
    bool gears_on : 1;
    bool brakes_on : 1;
    bool abort_on : 1;
    bool action1_on : 1;
    bool action2_on : 1;

    bool action3_on : 1;
    bool action4_on : 1;
    bool action5_on : 1;
    bool action6_on : 1;
    bool action7_on : 1;
    bool action8_on : 1;
    bool action9_on : 1;
    bool action10_on : 1;
};

struct __attribute__((__packed__)) ControlToButton {
  MainControls control;
  uint8_t button;
  int8_t lock;
};

struct __attribute__((__packed__)) HandshakePacket {
    uint8_t id;
    uint8_t M1;
    uint8_t M2;
    uint8_t M3;

    HandshakePacket() : id(0), M1(3), M2(1), M3(4) {}
};

struct __attribute__((__packed__)) ControlPacket  {
    uint8_t id;
    uint8_t main_controls;
    uint8_t mode;
    uint16_t action_groups;

    SASMode sas_mode : 4;
    NavballMode navball_mode : 4;
    uint8_t additional_controls;

    int16_t pitch;       // -1000 to 1000
    int16_t roll;        // -1000 to 1000
    int16_t yaw;         // -1000 to 1000
    int16_t translate_x; // -1000 to 1000
    int16_t translate_y; // -1000 to 1000
    int16_t translate_z; // -1000 to 1000

    int16_t wheel_steer; // -1000 to 1000
    
    int16_t throttle;        // 0 to 1000
    int16_t wheel_throttle; // 0 to 1000

    ControlPacket() : id(101) {}

    void set_main_control(uint8_t bit, bool state) {
        if(state) {
            main_controls |= bit;
        } else {
            main_controls &= ~bit;
        }
    }
};

struct __attribute__((__packed__)) VesselData {
    //the following data is available in the packet received from the plugin (defined by the KSPSerialIO plugin)
    uint8_t id;                //1   packet id
    float AP;               //2   apoapsis (m to sea level)
    float PE;               //3   periapsis (m to sea level)
    float SemiMajorAxis;    //4   orbit semi major axis (m)
    float SemiMinorAxis;    //5   orbit semi minor axis (m)
    float VVI;              //6   vertical velocity (m/s)
    float e;                //7   orbital eccentricity (unitless, 0 = circular, > 1 = escape)
    float inc;              //8   orbital inclination (degrees)
    float G;                //9   acceleration (Gees)
    int32_t TAp;               //10  time to AP (seconds)
    int32_t TPe;               //11  time to Pe (seconds)
    float TrueAnomaly;      //12  orbital true anomaly (degrees)
    float Density;          //13  air density (presumably kg/m^3, 1.225 at sea level)
    int32_t period;            //14  orbital period (seconds)
    float RAlt;             //15  radar altitude (m)
    float Alt;              //16  altitude above sea level (m)
    float Vsurf;            //17  surface velocity (m/s)
    float Lat;              //18  Latitude (degree)
    float Lon;              //19  Longitude (degree)
    float LiquidFuelTot;    //20  Liquid Fuel Total
    float LiquidFuel;       //21  Liquid Fuel remaining
    float OxidizerTot;      //22  Oxidizer Total
    float Oxidizer;         //23  Oxidizer remaining
    float EChargeTot;       //24  Electric Charge Total
    float ECharge;          //25  Electric Charge remaining
    float MonoPropTot;      //26  Mono Propellant Total
    float MonoProp;         //27  Mono Propellant remaining
    float IntakeAirTot;     //28  Intake Air Total
    float IntakeAir;        //29  Intake Air remaining
    float SolidFuelTot;     //30  Solid Fuel Total
    float SolidFuel;        //31  Solid Fuel remaining
    float XenonGasTot;      //32  Xenon Gas Total
    float XenonGas;         //33  Xenon Gas remaining
    float LiquidFuelTotS;   //34  Liquid Fuel Total (stage)
    float LiquidFuelS;      //35  Liquid Fuel remaining (stage)
    float OxidizerTotS;     //36  Oxidizer Total (stage)
    float OxidizerS;        //37  Oxidizer remaining (stage)
    uint32_t MissionTime;   //38  Time since launch (s)
    float deltaTime;        //39  Time since last packet (s)
    float VOrbit;           //40  Orbital speed (m/s)
    uint32_t MNTime;        //41  Time to next node (s) [0 when no node]
    float MNDeltaV;         //42  Delta V for next node (m/s) [0 when no node]
    float Pitch;            //43  Vessel Pitch relative to surface (degrees)
    float Roll;             //44  Vessel Roll relative to surface (degrees)
    float Heading;          //45  Vessel Heading relative to surface (degrees)
    ActionGroupStatus action_groups;  //46  status bit order:SAS, RCS, Light, Gears, Brakes, Abort, Custom01 - 10
    uint8_t SOINumber;         //47  SOI Number (decimal format: sun-planet-moon e.g. 130 = kerbin, 131 = mun)
    uint8_t MaxOverHeat;       //48  Max part overheat (% percent)
    float MachNumber;       //49  Mach number
    float IAS;              //50  Indicated Air Speed
    uint8_t CurrentStage;      //51  Current stage number
    uint8_t TotalStage;        //52  TotalNumber of stages
    float TargetDist;       //53  Distance to targeted vessel (m)
    float TargetV;          //54  Target vessel relative velocity
    //uint8_t NavballSASMode;    //55  Combined byte for navball target mode and SAS mode
                                // First four bits indicate AutoPilot mode:
                                // 0 SAS is off  //1 = Regular Stability Assist //2 = Prograde
                                // 3 = RetroGrade //4 = Normal //5 = Antinormal //6 = Radial In
                                // 7 = Radial Out //8 = Target //9 = Anti-Target //10 = Maneuver node
                                // Last 4 bits set navball mode. (0=ignore,1=ORBIT,2=SURFACE,3=TARGET)

    SASMode sas_mode : 4;
    NavballMode navball_mode : 4;
};


#endif