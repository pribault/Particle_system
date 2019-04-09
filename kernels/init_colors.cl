#define ITERATIONS	100

__kernel void	init_colors_burning_ship(__global __read_write float4 *colors, double n, double alpha)
{
	size_t	index = get_global_id(0);
	double	length = sqrt(n);
	double	a = 2 / (length - 1);
	float2	c = (float2){(trunc(fmod((double)index, length)) * (2 / (length - 1)) - 1) * 2, -(trunc((double)index / length) * a - 1) * 2};
	float2	z, tmp;
	size_t	i;
	float	d;

	z = (float2){0, 0};
	for (i = 0; pow(z.x, 2) + pow(z.y, 2) < 4 && i < ITERATIONS; i++)
	{
		tmp = (float2){fabs(z.x), fabs(z.y)};
		z = (float2){pow(tmp.x, 2) - pow(tmp.y, 2) + c.x, 2 * tmp.x * tmp.y + c.y};
	}
	d = (float)i / ITERATIONS;
	colors[index] = (float4){c.x / 2 + 0.5, c.y / 2 + 0.5, 0.2, d};
}

__kernel void	init_colors_rainbow(__global __read_write float4 *colors, double n, double alpha)
{
	size_t	index = get_global_id(0);
	double	length = cbrt(n);
	double	a = 1 / (length - 1);
	/*float	x = trunc(fmod((double)index, length)) * (2 / (length - 1)) - 1;

	if (x < -0.666666)
		colors[index] = (float4){
			1,
			0,
			-3 * x - 2,
			10000 / sqrt(n)};
	else if (x < -0.333333)
		colors[index] = (float4){
			1,
			3 * x + 2,
			0,
			10000 / sqrt(n)};
	else if (x < 0)
		colors[index] = (float4){
			-3 * x,
			1,
			0,
			10000 / sqrt(n)};
	else if (x < 0.333333)
		colors[index] = (float4){
			0,
			1,
			3 * x,
			10000 / sqrt(n)};
	else if (x < 0.666666)
		colors[index] = (float4){
			0,
			-3 * x + 2,
			1,
			10000 / sqrt(n)};
	else
		colors[index] = (float4){
			3 * x - 2,
			0,
			1,
			10000 / sqrt(n)};*/
	colors[index] = (float4){
		fmod((double)index, length) * a,
		fmod((double)index / length, length) * a,
		((double)index / pow(length, 2)) * a, alpha};
}
