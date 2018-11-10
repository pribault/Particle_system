__kernel void   init_speed(__global __read_write float3 *pos)
{
    pos[get_global_id(0)] = (float3){0, 0, 0};
}

__kernel void   init_square(__global __read_write float3 *pos, double n)
{
    size_t  index = get_global_id(0);
    double  length = sqrt(n);
    double  a = 2 / (length - 1);

    pos[index] = (float3){trunc(fmod((double)index, length)) * a - 1, trunc((double)index / length) * a - 1, 0};
}
