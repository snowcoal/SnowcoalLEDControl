# SnowcoalLEDControl
Arduino library for controlling my LEDs

# Overview

This library can be used to add wave gradients to individually addressable LED sets, controlled by an arduino. Can be used with FastLED, or other libraries as well.

An arduino can be loaded with a set amount of waves, which can be cycled between. Each wave has some number of gradient color points, or colors that the LEDs will shift between.
Only a few points need to be added for each gradient, as the program will interpolate between points to make the gradient smooth and look nice. Each point can also be set to
have a longer or shorter time duration until the next point. This allows for finer control, and can help bring out colors that otherwise wouldnt show well in the gradient.

Points are shifted to in order. So during a cycle, if there are 2 color points a single LED will shift from color point 1, to color point 2, and so on, then back to color point 1.
It will also interpolate from point 2 back to point 1, so there is no need to make the final point match the first point.

# Usage

To use the LEDWaveGradient class, first create an object with the number of waves specified and number of LEDs. I reccomend specifing the exact amount of waves and LEDs you
are using to conserve memory.

Next, add a wave and specify the exact number of gradient color points that it has. Then add the points. When the last point is added, it will automatically set that to the current wave.
Each point can be set to have a different duration, which is a value from 0-255.

Finally, use the getRed(), getGreen(), and getBlue() methods to get the colors of the lights during the loop. After that, run step() once. If the wave is too fast, and the max duration
for the points has been set to 255, add a delay in the loop as well.

# Tips

Timing the waves is extremely dependent on basically everything. If the main loop is slower, the wave will slow down. If there are more LEDs, the wave will slow down. If there are more points,
the wave will slow down. If the point durations are all set high, the wave will slow down. And the reverse is true as well.

If some of the color changes are abrupt, add more points in between. The program follows the line between any 2 color points. So if one point is red [255, 0, 0] and the next is cyan [0, 255, 255],
the light will follow this pattern as it shifts: [255, 0, 0], [254, 1, 1], [253, 2, 2] ... [0, 255, 255]. This will result in white being shown when the color hits (approximately) [127, 127, 127].
White may not be desired in this particlar gradient, so you can add more points in between to avoid the white color.

To make any color wave look even, simply add the reversed color points. So if a wave has color points 1, 2, 3, 4, 5, change it to be 1, 2, 3, 4, 5, 4, 3, 2. At the end from 2, it will then shift
back to 1, repeating the cycle as needed.

# Memory Requirements

The amount of waves, LEDs, and gradient points that can be added depends on the type of arduino being used, as well as each of those parameters.

Each LED requires 5 bytes of dynamic memory. This could be combined with FastRGB to use even less, but I wanted this to be more general and not rely on FastRGB, in case I need it for other
kinds of LED control.

Each wave requires 8 bytes of dynamic memory.

Each point requires 17 bytes of dynamic memory. This could be reduced by using uints instead of floats, however I wanted higher color accuracy.

Overall, I was able to control 200 LEDs with this, with 2 waves, each with around 5 points, all on a Nano. For larger amounts of LEDs, I reccomend either duplicating
strands in parallel (so 2+ strands have the same wave), or using an Arduino Mega.


