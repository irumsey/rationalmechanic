
///
///
///
float4x4 matrixFromQuaternion(float4 q)
{
	float xx = q.x * q.x;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float xw = q.x * q.w;
	float yy = q.y * q.y;
	float yz = q.y * q.z;
	float yw = q.y * q.w;
	float zz = q.z * q.z;
	float zw = q.z * q.w;

	return float4x4(
		1 - 2 * ( yy + zz ),     2 * ( xy - zw ),     2 * ( xz + yw ), 0,
		    2 * ( xy + zw ), 1 - 2 * ( xx + zz ),     2 * ( yz - xw ), 0,
		    2 * ( xz - yw ),     2 * ( yz + xw ), 1 - 2 * ( xx + yy ), 0,
		                  0,                   0,                   0, 1
	);
}

///
///
///
float4x4 matrixFromQuaternion(float4 q, float3 p)
{
	float xx = q.x * q.x;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float xw = q.x * q.w;
	float yy = q.y * q.y;
	float yz = q.y * q.z;
	float yw = q.y * q.w;
	float zz = q.z * q.z;
	float zw = q.z * q.w;

	return float4x4(
		1 - 2 * ( yy + zz ),     2 * ( xy - zw ),     2 * ( xz + yw ), p.x,
		    2 * ( xy + zw ), 1 - 2 * ( xx + zz ),     2 * ( yz - xw ), p.y,
		    2 * ( xz - yw ),     2 * ( yz + xw ), 1 - 2 * ( xx + yy ), p.z,
		                  0,                   0,                   0,   1
		);
}
