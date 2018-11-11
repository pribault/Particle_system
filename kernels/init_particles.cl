#define PI         3.141592653
#define SQRT_2     1.414213562

__kernel void   init_speed(__global __read_write float2 *pos)
{
    pos[get_global_id(0)] = (float2){0, 0};
}

__kernel void   init_square(__global __read_write float2 *pos, double n)
{
    size_t  index = get_global_id(0);
    double  length = sqrt(n);
    double  a = 2 / (length - 1);

    pos[index] = (float2){trunc(fmod((double)index, length)) * a - 1, trunc((double)index / length) * a - 1};
}

__kernel void   init_circle(__global __read_write float2 *pos, double n)
{
    size_t  index = get_global_id(0);
    double  length = sqrt(n);
    double  a = 2 / (length - 1);
    float2  tmp;

    tmp = (float2){trunc(fmod((double)index, length)) * a - 1, trunc((double)index / length) * a - 1};
    pos[index] = (float2){cos(tmp.x * 0.9995 * PI), sin(tmp.x * 0.9995 * PI)};
    pos[index].x *= (sqrt(tmp.y + 1) / SQRT_2);
    pos[index].y *= (sqrt(tmp.y + 1) / SQRT_2);
}
