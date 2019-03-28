__kernel void	move_particles_to_cursor(
	__global __read_write float3 *pos,
	__global __read_write float3 *speed,
	__global __read_write float4 *color,
	double n, float4 cursor, double g)
{
	size_t		index = get_global_id(0);
	float3		vec;

	vec = normalize(cursor.xyz - pos[index]);
	g /= pow(sqrt(pow(vec.x, 2) + pow(vec.y, 2)), 2);
	vec.x *= g;
	vec.y *= g;
	vec.z *= g;
	speed[index] += vec;
	pos[index] += speed[index];
	color[index].x += speed[index].x;
	color[index].y += speed[index].y;
	color[index].z += speed[index].z;
}

__kernel void	move_particles(
	__global __read_write float3 *pos,
	__global __read_write float3 *default_pos,
	__global __read_write float3 *speed,
	__global __read_write float4 *color,
	double n, double g)
{
	size_t		index = get_global_id(0);
	double		f = 0.002;
	float3		vec;
	float3		tmp;

	tmp = speed[index];
	tmp.x *= f;
	tmp.y *= f;
	speed[index] -= tmp;
	vec = normalize(default_pos[index] - pos[index]);
	if (vec.x || vec.y || vec.z)
	{
		g /= pow(sqrt(pow(vec.x, 2) + pow(vec.y, 2)), 2);
		vec.x *= g;
		vec.y *= g;
		vec.z *= g;
		speed[index] += vec;
	}
	pos[index] += speed[index];
	color[index].x += speed[index].x;
	color[index].y += speed[index].y;
	color[index].z += speed[index].z;
}
