// Include for passing the material strut to the lightning functions.
//#include "ForwardShaderCommon.hlsl"

// Globals
static float g_eul = 2.718281828;
static float g_pi = 3.141592654;



// Lightning Helper Functions

float3 CalcDiffuse(float3 diffuseColor)
{
	float3 DiffuseColor = saturate(diffuseColor / g_pi);
		return DiffuseColor;
}

// Normal Distribution function
float CalcDBeckman(float NDotH, float roughness)
{
	/*
	Beckman's Normal Distrubiton Function

	m = 1 / Pi * roughness^2 * cos^4 * exp(cos^2 - 1 / roughness^2 * cos^2)

	*/

	float cosPowTwo = pow(NDotH, 2);

	float cosPowFour = pow(NDotH, 4);

	float roughnessPowTwo = saturate(pow(roughness, 2));

	float tanPowerTwoDivByM = (1 - cosPowTwo) / (cosPowTwo * roughnessPowTwo);

	float eulPowNegTan = pow(g_eul, -tanPowerTwoDivByM);

	float piTimesRoughness = g_pi * roughnessPowTwo;

	float NormalDistribution = 1 / (piTimesRoughness * cosPowFour);

	NormalDistribution *= eulPowNegTan;

	return NormalDistribution;

}

//Geometric attenuation calculation function
float CalcGSmithBeckman(float NDotV, float roughness)
{
	float k = sqrt(roughness / g_pi);

	// Calculate the geometric term
	float geo_numerator = NDotV;
	float geo_denominator = NDotV * (1 - k) + k;

	float geo = geo_numerator / geo_denominator;

	return geo;

}

//Fresenel calculation function
float3 CalcFresnel(float VDotH, float3 f0)
{
	// Calculate the Fresnel value
	float3 fresnel = pow(1.0f - VDotH, 5.0f);
	fresnel *= (1.0f - f0);
	fresnel += f0;

	return fresnel;

}

//Calc Cook Torrance Lightning
float3 CalcCookTorrance(Material material, float3 lightColor, float NDotL,float NDotH, float NDotV, float VDotH )
{

	// Calculate Lambert diffuse
	float3 kDiffuse = CalcDiffuse(material.diffuseColor.rgb);

	float3 kLight = (NDotL * lightColor.rgb);

	// Calculate the Normal Distribution Function term
	float roughness = CalcDBeckman(NDotH, material.roughness);

	// Calculate the Geometric attenuation term
	float geo = CalcGSmithBeckman(NDotV, material.roughness);
	
	//Trick to handle Metal materials
	float3 f0 = lerp(material.f0, material.diffuseColor.rgb, material.metalness);

	// Calculate the Fresnel value
	float3 fresnel = CalcFresnel(VDotH, f0);

	// Put all the terms together to compute
	// the specular term in the equation
	float3 Rs_numerator = fresnel * geo * roughness;
	float Rs_denominator = 4 * NDotL *NDotV;
	float3 Rs = Rs_numerator / Rs_denominator;

	float3 kSpecular = Rs;

	kDiffuse *= ((1 - kSpecular) * (1 - material.metalness));

	float3 finalColor = saturate((kDiffuse + kSpecular) * kLight);

	return finalColor;

}

