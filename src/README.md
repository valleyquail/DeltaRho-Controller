# Conventions

***

### <u> Subscriptions </u>

MQTT Topic subscriptions determine how the robots communicate with the broker. The only required subscription that all
robots have is a subscription to the instruction topic since that is where the robot coordinator will publish their
instructions.

Each robot is identified by a number defined in the 'Robot_Config.h' file, and all of that robot's subscriptions will be
registered in the broker as:

> R#/< topic >/< subtopic >

### <u> Data Formatting </u>

For the robot to understand its instructions, it is important that a convention for sending commands is established. For
sake of communication efficiency, it makes sense to send as much information as possible all in one message. Thus,
commands should be formatted using the following syntax:

| Flag | Meaning     | Description                                                                      |
|------|-------------|----------------------------------------------------------------------------------|
| 'i'  | Instruction | Necessary flag indicating that this message contains an instructions             |
| 'F'  | Speed/Feed  | The speed at which the robot should move in cm/s                                 |
| 'T'  | Theta       | The angle in radians from the robot's absolute heading in which it should travel |
| 'W'  | Omega       | The rate at which the robot should turn in rad/s                                 |
| 'D'  | Distance    | How far the robot should move in the provided direction                          |
| "P"  | Phi         | The angle to which the robot should turn relative to its absolute heading        |

The order of commands should always be: "Speed, Theta, Distance" or "Omega, Phi"

#### Examples

`iF10T0D50` corresponds to 10cm/s at an angle 0 radians away from absolute heading for a distance of 50 cm  
`iW0.5P1` corresponds to a rotation of 0.5 rad/s and for the robot to turn 1 radian

#### Notes:

* Currently, linear speed cannot be mixed with angular rotation and have the robot linearly translate: it would travel
  in an arc
* Not all flags are required in a single message. The only necessary flag is the instruction flag