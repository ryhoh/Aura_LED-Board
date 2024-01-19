#include "AccelSensorMock.h"

#define MOCK_ACL_1g (16384)  /* [g,Q14] 1g */

/* functions */
int16_t Mock_AccelSensor_getX(void)
{
    return MOCK_ACL_1g;
}

int16_t Mock_AccelSensor_getY(void)
{
    return 0;
}

int16_t Mock_AccelSensor_getZ(void)
{
    return 0;
}
