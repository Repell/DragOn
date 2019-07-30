//상수 테이블(전역 변수)
matrix g_matWorld, g_matView, g_matProj;

texture g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

//Vertex Shader Struct
struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	vector vPosition : POSITION;
	float2 vTexUV		: TEXCOORD0;
};


//Vertex Shader
//내보낼 타입 :: 버텍스쉐이더 함수 이름 (버텍스버퍼에서 가져온 정점)
VS_OUT VS_MAIN(VS_IN vIn)
{
	VS_OUT vOut = (VS_OUT)0;			// VS_OUT 구조체 0 초기화 . 쉐이더에서만 가능한 문법

	matrix matWV, matWVP;		// World * View, World * View * Proj

	matWV = mul(g_matWorld, g_matView);		//전역함수로 가져온 월드와 뷰를 연산한 값을 저장

	matWVP = mul(matWV, g_matProj);				//투영값을 곱해 투영가능 상태로 만듦

	vOut.vPosition = mul(vector(vIn.vPosition, 1.f), matWVP);
	vOut.vTexUV = vIn.vTexUV;

	return vOut;
}

//Pixel Shader Struct
struct PS_IN // VS_OUT과 같음
{
	vector vPosition : POSITION;
	float2 vTexUV		: TEXCOORD0;
};

vector PS_MAIN(PS_IN pIn) : COLOR0
{
	vector vColor = (vector)0;

	vColor = tex2D(BaseSampler, pIn.vTexUV);

	return vColor;
}

technique Default_Technique
{
	pass Default_Rendering
	{
		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}