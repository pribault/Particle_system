__kernel void   move_particles_to_cursor(
    __global __read_write float2 *pos,
    __global __read_write float2 *speed,
    __global __read_write float4 *color,
    double n, double x, double y, double g)
{
    size_t      index = get_global_id(0);
    float2      vec;

    vec = normalize((float2){x, y} - pos[index]);
    g /= pow(sqrt(pow(vec.x, 2) + pow(vec.y, 2)), 2);
    vec.x *= g;
    vec.y *= g;
    speed[index] += vec;
    pos[index] += speed[index];
    color[index].x += speed[index].x * 2;
    color[index].y += speed[index].y;
    color[index].z += speed[index].x;
}

__kernel void   move_particles(
    __global __read_write float2 *pos,
    __global __read_write float2 *default_pos,
    __global __read_write float2 *speed,
    __global __read_write float4 *color,
    double n, double g)
{
    size_t      index = get_global_id(0);
    float2      vec = (float2){0, 0};
    double      f = 0.002;
    float2      tmp;

    tmp = speed[index];
    tmp.x *= f;
    tmp.y *= f;
    speed[index] -= tmp;
    vec = normalize(default_pos[index] - pos[index]);
    if (vec.x || vec.y)
    {
        g /= pow(sqrt(pow(vec.x, 2) + pow(vec.y, 2)), 2);
        vec.x *= g;
        vec.y *= g;
        speed[index] += vec;
    }
    pos[index] += speed[index];
    color[index].x += speed[index].x * 2;
    color[index].y += speed[index].y;
    color[index].z += speed[index].x;
}
