/////////////////////////////////////////////////////////////////////////////
// GBuffer textures and Samplers
/////////////////////////////////////////////////////////////////////////////
Texture2D<float> DepthTexture			: register(t0);
Texture2D<float4> AlbedoRoughTexture	: register(t1);
Texture2D<float3> NormalTexture			: register(t2);
Texture2D<float4> F0MetalTexture		: register(t3);

SamplerState PointSampler				: register(s0);

/////////////////////////////////////////////////////////////////////////////
// constant buffer
/////////////////////////////////////////////////////////////////////////////
cbuffer cbGBufferUnpack				: register(b0)
{
	float4 PerspectiveValues		: packoffset(c0);
	float4x4 ViewInv				: packoffset(c1);
}
#define EyePosition (ViewInv[3].xyz)

struct Material
{
	float3 normal;
	float4 diffuseColor;
	float roughness;
	float f0;
	float metalness;
};

float ConvertZToLinearDepth(float depth)
{
	float linearDepth = PerspectiveValues.z / (depth + PerspectiveValues.w);

	return linearDepth;
}

float3 CalcWorldPos(float2 csPos, float depth)
{
	float4 position;

	position.xy = csPos.xy * PerspectiveValues.xy * depth;
	position.z = depth;
	position.w = 1.0;

	return mul(position, ViewInv).xyz;
}

struct SURFACE_DATA
{
	float LinearDepth;
	float3 Color;
	float3 Normal;
	float roughness;
	float f0;
	float metalness;
};

SURFACE_DATA UnpackGBuffer(float2 UV)
{
	SURFACE_DATA Out;

	float depth = DepthTexture.Sample(PointSampler, UV.xy).x;
	Out.LinearDepth = ConvertZToLinearDepth(depth);
	float4 baseColorRough = AlbedoRoughTexture.Sample(PointSampler, UV.xy);
	Out.Color = baseColorRough.xyz;
	Out.roughness = baseColorRough.w;
	Out.Normal = NormalTexture.Sample(PointSampler, UV.xy).xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);
	Out.f0 = F0MetalTexture.Sample(PointSampler, UV.xy).x;
	Out.metalness = F0MetalTexture.Sample(PointSampler, UV.xy).y;

	return Out;
}

SURFACE_DATA UnpackGBuffer_Loc(int2 location)
{
	SURFACE_DATA Out;
	int3 location3 = int3(location, 0);

	float depth = DepthTexture.Load(location3).x;
	Out.LinearDepth = ConvertZToLinearDepth(depth);
	float4 baseColorRough = AlbedoRoughTexture.Load(location3);
	Out.Color = baseColorRough.xyz;
	Out.roughness = baseColorRough.w;
	Out.Normal = NormalTexture.Load(location3).xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);
	float2 F0Metalness = F0MetalTexture.Load(location3).xy;
	Out.f0 = F0Metalness.x;
	Out.metalness = F0Metalness.y;

	return Out;
}

void MaterialFromGBuffer(SURFACE_DATA gbd, inout Material mat)
{
	mat.normal = gbd.Normal;
	mat.diffuseColor.xyz = gbd.Color;
	mat.diffuseColor.w = 1.0;
	mat.roughness = gbd.roughness; 
	mat.f0 = gbd.f0; 
	mat.metalness = gbd.metalness;
}