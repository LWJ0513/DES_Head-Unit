#include "PiRacer/PiRacer.hpp"
#include "Gamepad/ShanwanGamepad.hpp"
#include <cstdlib>

/**
 * A : DRIVE
 * B : NEUTRAL
 * X : REVERSE
 * Y : PARKING
 */

void cleanup()
{
    gpioTerminate();
}
int main()
{
    atexit(cleanup);
    if (gpioInitialise() < 0)
    {
        std::cerr << "pigpio initialization failed" << std::endl;
        return 1;
    }
    atexit(gpioTerminate);

    PiRacer racer;
    ShanWanGamepad gamepad;

    while (true)
    {
        ShanWanGamepadInput input = gamepad.read_data();

        std::cout << input.button_a << std::endl;

        float steering = input.analog_stick_left.x;
        float throttle = input.analog_stick_right.y * 0.5; // limit throtle to 50%
        float steering = input.analog_stick_left.x * -1;
        float throttle = input.analog_stick_right.x * 0.7; // limit throtle to 50%

        std::cout << "Throttle: " << throttle
                  << ", Steering: " << steering
                  << std::endl;

        racer.setSteeringPercent(steering);
        racer.setThrottlePercent(throttle);
    }

    return 0;
}
