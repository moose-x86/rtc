
we will ne to create script/cmake macro which will create library include directory,
because include path should be different from existing one right now
for example


#include "rtc/math/triangle3d.hpp"
#include "rtc/image_composer.hpp"
#include "rtc/raytracer.hpp"
#include "rtc/pixel_composer.hpp"

for better usability, so some command for geerating this in example directory would be nice
probably we install command and run some scripts
