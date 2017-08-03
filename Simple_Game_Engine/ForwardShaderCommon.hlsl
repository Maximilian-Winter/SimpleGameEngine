/////////////////////////////////////////////////////////////////////////////
//Textures and linear sampler
/////////////////////////////////////////////////////////////////////////////
Texture2D    AlbedoTexture					: register(t0);
Texture2D    RoughnessTexture				: register(t1);
Texture2D    MetallicTexture				: register(t2);
Texture2D    NormalMap						: register(t3);

SamplerState LinearSampler					: register(s0);


/////////////////////////////////////////////////////////////////////////////
// shader input/output structures
/////////////////////////////////////////////////////////////////////////////
struct VS_INPUT
{
	float4 Position	: POSITION;		// vertex position 
	float2 UV		: TEXCOORD0;	// vertex texture coords 
	float3 Normal	: NORMAL;		// vertex normal

};

struct VS_OUTPUT
{
	float4 Position					: SV_POSITION;
	float3 PositionInWorld			: TEXCOORD0;
	float4 PositionInWorldViewProj	: TEXCOORD1;
	float2 UV						: TEXCOORD2;
	float3 Normal					: TEXCOORD3;
};
/////////////////////////////////////////////////////////////////////////////
// Constant Buffers
/////////////////////////////////////////////////////////////////////////////
cbuffer ObjectVSBuffer						: register(b0) // Model Buffer
{
	float4x4 WorldViewProjection			: packoffset(c0);
	float4x4 World							: packoffset(c4);
}

cbuffer MaterialPSBuffer					: register(b0) //Model PS Buffer
{
	float3 Albedo							: packoffset(c0.x);
	float Roughness							: packoffset(c0.w);
	float F0								: packoffset(c1.x);
	float Metalness							: packoffset(c1.y);
	float Transparency						: packoffset(c1.z);
	int TextureInput						: packoffset(c1.w);
}

cbuffer EyePositionPSBuffer					: register(b1) //Model PS Buffer
{
	float3 EyePosition						: packoffset(c0);
}


/////////////////////////////////////////////////////////////////////////////
// Material preparation
/////////////////////////////////////////////////////////////////////////////

struct Material
{
	float3 normal;
	float4 diffuseColor;
	float roughness;
	float f0;
	float transparency;
	float metalness;
};

float3x3 invert_3x3(float3x3 M)
{
	float D = determinant(M);
	float3x3 T = transpose(M);

		return float3x3(
		cross(T[1], T[2]),
		cross(T[2], T[0]),
		cross(T[0], T[1])) / (D + 1e-6);
}



float3 resolveNormal(VS_OUTPUT vertexShaderOut)
{
	float3 normal = float3(0, 0, 0);
	float3 tangent = float3(0, 0, 0);
	float3 binormal = float3(0, 0, 0);

	// Tangent basis from screen space derivatves. 
	normal = vertexShaderOut.Normal;
	float3x3 tbnTransform;
	float4 texNormal = NormalMap.Sample(LinearSampler, vertexShaderOut.UV);
	texNormal.xyz = normalize((2.0f * texNormal.xyz) - 1.0f);

	float3 dp1 = ddx_fine(vertexShaderOut.PositionInWorld.xyz);
	float3 dp2 = ddy_fine(vertexShaderOut.PositionInWorld.xyz);
	float2 duv1 = ddx_fine(vertexShaderOut.UV.xy);
	float2 duv2 = ddy_fine(vertexShaderOut.UV.xy);
	float3x3 M = float3x3(dp1, dp2, normalize(normal));
	float3x3 inverseM = invert_3x3(M);
	float3 T = mul(inverseM, float3(duv1.x, duv2.x, 0));
	float3 B = mul(inverseM, float3(duv1.y, duv2.y, 0));
	float scaleT = 1.0f / (dot(T, T) + 1e-6);
	float scaleB = 1.0f / (dot(B, B) + 1e-6);
	tbnTransform[0] = normalize(T*scaleT);
	tbnTransform[1] = -normalize(B*scaleB);
	tbnTransform[2] = normalize(normal);
	tangent = tbnTransform[0];
	normal = normalize((tbnTransform[0] * texNormal.x) + (tbnTransform[1] * texNormal.y) + (tbnTransform[2]));

	return normal;
}

Material PrepareMaterial(VS_OUTPUT VertexShaderOutput)
{
	Material material;

	// Check which material values to choose.

	// Use constant values for the model.
	if (TextureInput == 0)
	{
		// Load the material values from the constant buffer
		material.diffuseColor = float4(Albedo, 1.0f);
		material.roughness = Roughness;
		material.f0 = F0;
		material.metalness = Metalness;
		material.transparency = Transparency;

		// Normalize the vertex normal
		material.normal = normalize(VertexShaderOutput.Normal);
	}

	// Use texture maps, constant transparency and no normal map for the model.
	if (TextureInput == 1)
	{
		// Sample the texture and convert to linear space
		float4 diffuseColor = AlbedoTexture.Sample(LinearSampler, VertexShaderOutput.UV);
		material.diffuseColor = float4(pow(diffuseColor.rgb, 2.2).rgb, diffuseColor.a);

		// Sample the material values from textures and and load from constant buffer 
		material.roughness = RoughnessTexture.Sample(LinearSampler, VertexShaderOutput.UV).r;
		material.f0 = F0;
		material.metalness = MetallicTexture.Sample(LinearSampler, VertexShaderOutput.UV).r;
		material.transparency = Transparency;

		// Normalize the vertex normal
		material.normal = normalize(VertexShaderOutput.Normal);
	}

	// Use texture maps, constant transparency and a normal map for the model.
	if (TextureInput == 2)
	{
		// Sample the texture and convert to linear space
		float4 diffuseColor = AlbedoTexture.Sample(LinearSampler, VertexShaderOutput.UV);
		material.diffuseColor = float4(pow(diffuseColor.rgb, 2.2).rgb, diffuseColor.a);

		// Sample the material values from textures and and load from constant buffer 
		material.roughness = RoughnessTexture.Sample(LinearSampler, VertexShaderOutput.UV).r;
		material.f0 = F0;
		material.metalness = MetallicTexture.Sample(LinearSampler, VertexShaderOutput.UV).r;
		material.transparency = Transparency;

		// Normalize the vertex normal
		material.normal = resolveNormal(VertexShaderOutput);
	}

	// Use texture maps, transparent alpha channel and no normal map for the model.

	if (TextureInput == 3)
	{
		// Sample the texture and convert to linear space
		float4 diffuseColor = AlbedoTexture.Sample(LinearSampler, VertexShaderOutput.UV);
		material.diffuseColor = float4(pow(diffuseColor.rgb, 2.2).rgb, diffuseColor.a);

		// Sample the material values from textures and and load from constant buffer 
		material.roughness = RoughnessTexture.Sample(LinearSampler, VertexShaderOutput.UV).r;
		material.f0 = F0;
		material.metalness = MetallicTexture.Sample(LinearSampler, VertexShaderOutput.UV).r;
		material.transparency = diffuseColor.a;

		// Normalize the vertex normal
		material.normal = normalize(VertexShaderOutput.Normal);
	}

	// Use texture maps, transparent alpha channel and a normal map for the model.
	if (TextureInput == 4)
	{
		// Sample the texture and convert to linear space
		float4 diffuseColor = AlbedoTexture.Sample(LinearSampler, VertexShaderOutput.UV);
		material.diffuseColor = float4(pow(diffuseColor.rgb, 2.2).rgb, diffuseColor.a);

		// Sample the material values from textures and and load from constant buffer 
		material.roughness = RoughnessTexture.Sample(LinearSampler, VertexShaderOutput.UV).r;
		material.f0 = F0;
		material.metalness = MetallicTexture.Sample(LinearSampler, VertexShaderOutput.UV).r;
		material.transparency = diffuseColor.a;

		// Normalize the vertex normal
		material.normal = resolveNormal(VertexShaderOutput);
	}

	return material;
}
