
typedef struct
{
    float x, y, z, w;
} tuple;

tuple createPoint(float x, float y, float z);
tuple createVector(float x, float y, float z);

bool equal(tuple a, tuple b);