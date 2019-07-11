#include "Shader_Base.hpp"

float			g_fDetail;

vector			g_vLightDir = (vector)0.f;

vector			g_vLightDiffuse;
vector			g_vLightAmbient;

vector			g_vMtrlDiffuse;
vector			g_vMtrlAmbient;


// 정반사광
vector			g_vLightSpecular;
vector			g_vMtrlSpecular;
vector			g_vCamPos;

float			g_fPower = 1.f;


struct		VS_IN
{
	vector			vPosition : POSITION;
	vector			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	//vector		vNormal : NORMAL;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);


	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	//Out.vNormal = mul(vector(In.vNormal.xyz, 0.f), matWVP);

	vector vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);

	vector vWorldLigtDir = g_vLightDir * -1.f;

	float	fIntensity = max(dot(normalize(vWorldNormal), normalize(vWorldLigtDir)), 0.f);

	//float	fIntensity = satulate(dot(normalize(vWorldNormal), normalize(vWorldLigtDir)));

	Out.vShade = fIntensity;
	Out.vShade.a = 1.f;

	// 정반사광
	vector		vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));

	vector		vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	vector		vLook = g_vCamPos - vWorldPos;

	Out.vSpecular = pow(max(dot(normalize(vLook), normalize(vReflect)), 0.f), g_fPower);
	Out.vSpecular.a = 1.f;

	Out.vTexUV = In.vTexUV;


	return Out;
}

struct PS_IN
{
	vector			vShade : COLOR0;
	vector			vSpecular : COLOR1;
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV * g_fDetail);


	//Out.vColor  = (vColor * In.vShade) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient );

	// Out.vColor = (vColor * (g_vLightDiffuse * g_vMtrlDiffuse)) + (In.vShade * (g_vLightAmbient * g_vMtrlAmbient));

	Out.vColor = (vColor * (g_vLightDiffuse * g_vMtrlDiffuse))
		* (In.vShade + (g_vLightAmbient * g_vMtrlAmbient))
		+ (In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular));

	return Out;
}

technique Default_Device		// 작성한 쉐이더 코드를 장치에게 명시적으로 알려주는 구문
{
	// PASS : 기능의 캡슐화
	pass
{

	vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}
};