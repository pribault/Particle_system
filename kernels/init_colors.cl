__kernel void   init_colors_rainbow(__global __read_write float4 *colors, double n)
{
    size_t  index = get_global_id(0);
    double  length = sqrt(n);
    float   x = trunc(fmod((double)index, length)) * (2 / (length - 1)) - 1;

    if (x < -0.666666)
        colors[index] = (float4){
            1,
            0,
            -3 * x - 2,
            200000 / n};
    else if (x < -0.333333)
        colors[index] = (float4){
            1,
            3 * x + 2,
            0,
            200000 / n};
    else if (x < 0)
        colors[index] = (float4){
            -3 * x,
            1,
            0,
            200000 / n};
    else if (x < 0.333333)
        colors[index] = (float4){
            0,
            1,
            3 * x,
            200000 / n};
    else if (x < 0.666666)
        colors[index] = (float4){
            0,
            -3 * x + 2,
            1,
            200000 / n};
    else
    colors[index] = (float4){
            3 * x - 2,
            0,
            1,
            200000 / n};
}
