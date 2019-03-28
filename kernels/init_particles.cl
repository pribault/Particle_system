#define PI			3.141592653

__kernel void	init_speed(__global __read_write float3 *pos)
{
	pos[get_global_id(0)] = (float3){0, 0, 0};
}

__kernel void	init_square(__global __read_write float3 *pos, double n)
{
	size_t	index = get_global_id(0);
	double	length = cbrt(n);
	double	a = 2 / (length - 1);

	pos[index] = (float3){
		fmod((double)index, length) * a - 1,
		fmod((double)index / length, length) * a - 1,
		(double)index / pow(length, 2) * a};
}

__kernel void	init_circle(__global __read_write float3 *pos, double n)
{
	size_t	index = get_global_id(0);
	double	length = sqrt(n);
	double	a = (2 * PI) / (length - 1);
	double	h = fabs(sin(index * PI / (1 - n)));

	pos[index] = (float3){h * cos(fmod(index, length) * a), h * sin(fmod(index, length) * a), cos(index * PI / (1 - n))};
}
