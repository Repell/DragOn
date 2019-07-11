//��� ���̺� : Ŭ���̾�Ʈ�� �����͸� �Ѱܹ޾� �۵� �ڵ忡 �����ϱ����� ����
matrix		g_matWorld;
matrix		g_matView;
matrix		g_matProj;

texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;

};

struct VS_IN
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};

PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV); // 2���� ������ �ؽ��ķκ��� uv��ǥ�� �ش�Ǵ� ���� �����ϴ� �Լ�, ��ȯ Ÿ���� vector Ÿ��

	if(In.vTexUV.y < 0.5f)
		Out.vColor.a = 0.5f;
	else
		Out.vColor.a = 1.0f;

	return Out;
}

technique Default_Device
{
	pass
	{
		alphablendenable = true;
		SrcBlend = srcAlpha;
		DestBlend = Invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();

	}

}