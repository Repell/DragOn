//��� ���̺�(���� ����)
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
//������ Ÿ�� :: ���ؽ����̴� �Լ� �̸� (���ؽ����ۿ��� ������ ����)
VS_OUT VS_MAIN(VS_IN vIn)
{
	VS_OUT vOut = (VS_OUT)0;			// VS_OUT ����ü 0 �ʱ�ȭ . ���̴������� ������ ����

	matrix matWV, matWVP;		// World * View, World * View * Proj

	matWV = mul(g_matWorld, g_matView);		//�����Լ��� ������ ����� �並 ������ ���� ����

	matWVP = mul(matWV, g_matProj);				//�������� ���� �������� ���·� ����

	vOut.vPosition = mul(vector(vIn.vPosition, 1.f), matWVP);
	vOut.vTexUV = vIn.vTexUV;

	return vOut;
}

//Pixel Shader Struct
struct PS_IN // VS_OUT�� ����
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