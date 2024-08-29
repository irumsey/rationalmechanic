
///	Instance
/// 
/// Much of the rendering makes use of hardware based instancing.
/// This is the per-instance stream which is shared among a
/// number of meshes.
struct Instance
{
	uint1          id : LOCATION3;	// instance id which is written to the selection target
	float4   position : LOCATION4;	// ( position(3), x)
	float4   rotation : LOCATION5;	// ( rotation in the form of a quaternion )
	float4   channel0 : LOCATION6;	// user defined
	float4   channel1 : LOCATION7;	// user defined
	float4   channel2 : LOCATION8;	// user defined
};
