#define ITERATIONS	(float)100
#define SIZE		(float)1
#define LIMIT		(float)1.5
#define POWER		(float)8

#define SIZEOF_STATIC_ARRAY(array)	(sizeof(array) / sizeof(array[0]))

float4	mult_complex(float4 a, float4 b);
float4	exp_complex(float4 n, size_t e);

float4	exp_complex(float4 n, size_t e)
{
	float4	result = n;

	for (size_t i = 1; i < e; i++)
		result = mult_complex(result, n);
	return (result);
}

float4	mult_complex(float4 a, float4 b)
{
	return ((float4){
		a.w * b.x + a.x * b.w - a.y * b.z + a.z * b.y,
		a.w * b.y + a.x * b.z + a.y * b.w - a.z * b.x,
		a.w * b.z - a.x * b.y + a.y * b.x + a.z * b.w,
		a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
	});
}

__constant float4	palette[] = {
	{0.5, 0, 0, 1},
	{1, 0.5, 0, 1},
	{0.5, 0, 0.5, 1},
	{0, 0, 0.5, 1}
};

__kernel void	init_colors_mandelbulb(__global __read_write float4 *colors, double n, double alpha)
{
	size_t	index = get_global_id(0);
	double	l = cbrt(n);
	float3	pos = SIZE * (float3){
		2 * fmod(index, l) / l - 1,
		2 * fmod(index / l, l) / l - 1,
		2 * (index / pow(l, 2)) / l - 1
	};
	float3	z = pos;
	float	theta;
	float	phi;
	float	dr = 1;
	float	r;
	size_t	i;

	for (i = 0; i < ITERATIONS; i++)
	{
		r = length(z);
		if (r > 1.5)
		{
			colors[index] = (float4){0, 0, 0, 0};
			return ;
		}
		theta = acos(z.z / r);
		phi = atan(z.y / z.x);
		dr = pow(r, POWER - 1) * POWER * dr + 1;
		z = pow(r, POWER) * (float3){
			sin(POWER * theta) * cos(POWER * phi),
			sin(POWER * theta) * sin(POWER * phi),
			cos(POWER * theta)
		} + pos;
	}
	colors[index] = ((float)i / ITERATIONS) * palette[(int)round(SIZEOF_STATIC_ARRAY(palette) * r / LIMIT)];
	colors[index].w = alpha;
}

__kernel void	init_colors_rainbow(__global __read_write float4 *colors, double n, double alpha)
{
	size_t	index = get_global_id(0);
	double	length = cbrt(n);
	double	a = 1 / (length - 1);

	colors[index] = (float4){
		fmod((double)index, length) * a,
		fmod((double)index / length, length) * a,
		((double)index / pow(length, 2)) * a, alpha};
}
