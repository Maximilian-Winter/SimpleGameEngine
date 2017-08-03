/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "DeferredShaderCommon.hlsl"
#include "PBRCommon.hlsl"

/////////////////////////////////////////////////////////////////////////////
// Constant Buffers
/////////////////////////////////////////////////////////////////////////////
cbuffer CapsuleLightScaleData			: register( b0 )
{
	float4x4 LightProjection			: packoffset(c0);
	float SinAngle						: packoffset(c4.x);
	float CosAngle						: packoffset(c4.y);
}

cbuffer LightBufferType					: register( b1 )
{
	float3 SpotLightPos					: packoffset(c0);
	float SpotLightRange				: packoffset(c0.w);
	float3 SpotDirToLight				: packoffset(c1);
	float SpotCosOuterCone				: packoffset(c1.w);
	float3 SpotColor					: packoffset(c2);
	float SpotCosConeAttRange			: packoffset(c2.w);
}


/////////////////////////////////////////////////////////////////////////////
// Vertex shader
/////////////////////////////////////////////////////////////////////////////
float4 DeferredSpotLightVertexShader() : SV_Position
{
    return float4(0.0, 0.0, 0.0, 1.0); 
}

/////////////////////////////////////////////////////////////////////////////
// Hull shader
/////////////////////////////////////////////////////////////////////////////
struct HS_CONSTANT_DATA_OUTPUT
{
	float Edges[4] : SV_TessFactor;
	float Inside[2] : SV_InsideTessFactor;
};

HS_CONSTANT_DATA_OUTPUT SpotLightConstantHS()
{
	HS_CONSTANT_DATA_OUTPUT Output;
	
	float tessFactor = 18.0;
	Output.Edges[0] = Output.Edges[1] = Output.Edges[2] = Output.Edges[3] = tessFactor;
	Output.Inside[0] = Output.Inside[1] = tessFactor;

	return Output;
}

struct HS_OUTPUT
{
	float3 Position : POSITION;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("SpotLightConstantHS")]
HS_OUTPUT DeferredSpotLightHullShader()
{
	HS_OUTPUT Output;

	Output.Position = float3(0.0, 0.0, 0.0);

	return Output;
}

/////////////////////////////////////////////////////////////////////////////
// Domain Shader shader
/////////////////////////////////////////////////////////////////////////////
struct DS_OUTPUT
{
	float4 Position	: SV_POSITION;
	float2 cpPos	: TEXCOORD0;
};

#define CylinderPortion 0.2
#define ExpendAmount    (1.0 + CylinderPortion)

[domain("quad")]
DS_OUTPUT DeferredSpotLightDomainShader(HS_CONSTANT_DATA_OUTPUT input, float2 UV : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 4> quad)
{
	// Transform the UV's into clip-space
	float2 posClipSpace = UV.xy * float2(2.0, -2.0) + float2(-1.0, 1.0);

	// Find the vertex offsets based on the UV
	float2 posClipSpaceAbs = abs(posClipSpace.xy);
	float maxLen = max(posClipSpaceAbs.x, posClipSpaceAbs.y);

	// Force the cone vertices to the mesh edge
	float2 posClipSpaceNoCylAbs = saturate(posClipSpaceAbs * ExpendAmount);
	float maxLenNoCapsule = max(posClipSpaceNoCylAbs.x, posClipSpaceNoCylAbs.y);
	float2 posClipSpaceNoCyl = sign(posClipSpace.xy) * posClipSpaceNoCylAbs;

	// Convert the positions to half sphere with the cone vertices on the edge
	float3 halfSpherePos = normalize(float3(posClipSpaceNoCyl.xy, 1.0 - maxLenNoCapsule));

	// Scale the sphere to the size of the cones rounded base
	halfSpherePos = normalize(float3(halfSpherePos.xy * SinAngle, CosAngle));

	// Find the offsets for the cone vertices (0 for cone base)
	float cylinderOffsetZ = saturate((maxLen * ExpendAmount - 1.0) / CylinderPortion);

	// Offset the cone vertices to thier final position
	float4 posLS = float4(halfSpherePos.xy * (1.0 - cylinderOffsetZ), halfSpherePos.z - cylinderOffsetZ * CosAngle, 1.0);

	// Transform all the way to projected space and generate the UV coordinates
	DS_OUTPUT Output;
	Output.Position = mul( posLS, LightProjection );
	Output.cpPos = Output.Position.xy / Output.Position.w;

	return Output;
}

/////////////////////////////////////////////////////////////////////////////
// Pixel shader
/////////////////////////////////////////////////////////////////////////////

float3 CalcSpot(float3 position, Material material)
{
	float3 ToLight = SpotLightPos - position;
	float3 ToEye = EyePosition.xyz - position;
	ToEye = normalize(ToEye);
	float DistToLight = length(ToLight);

	// Calculate Lightning Dot Products 
	ToLight /= DistToLight;										// Normalize
	float NDotL = saturate(dot(material.normal, ToLight));
	float3 HalfWay = normalize(ToEye + ToLight);

		
	float NDotH = saturate(dot(material.normal, HalfWay));
	float NDotV = saturate(dot(material.normal, ToEye));
	float VDotH = saturate(dot(ToEye, HalfWay));

	float3 finalColor = CalcCookTorrance(material, SpotColor, NDotL, NDotH, NDotV, VDotH);

	// Cone attenuation
	float cosAng = dot(SpotDirToLight, ToLight);
	float conAtt = saturate((cosAng - SpotCosOuterCone) / SpotCosConeAttRange);
	conAtt *= conAtt;

	// Attenuation
	float DistToLightNorm = 1.0 - saturate(DistToLight * SpotLightRange);
	float Attn = DistToLightNorm * DistToLightNorm;
	finalColor *= Attn * conAtt;
   
   // Return the final color
   return finalColor;
}

float4 DeferredSpotLightPixelShader(DS_OUTPUT In) : SV_TARGET
{
	// Unpack the GBuffer
	SURFACE_DATA gbd = UnpackGBuffer_Loc(In.Position.xy);
	
	// Convert the data into the material structure
	Material mat;
	MaterialFromGBuffer(gbd, mat);

	// Reconstruct the world position
	float3 position = CalcWorldPos(In.cpPos, gbd.LinearDepth);

	// Calculate the light contribution
	float3 finalColor = CalcSpot(position, mat);

	return float4(finalColor, 1.0);
}

