matrix			g_matWorld;				// ��� ���̺� : Ŭ���̾�Ʈ�� �����ϴ� �����͸� �Ѱ� �޾� �۵� �ڵ忡 �����ϱ� ���� ��¡���� ��Ī
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;			// ���÷��� : ���� ������ ��� �ִ� �ȷ�Ʈ ������ �ؽ��ĸ� ���̴������� ���´� ��Ī

sampler BaseSampler = sampler_state		// �ȷ�Ʈ ������ �ؽ��ĸ� hlsl���� ��Ƶδ� �ڷ����� ����, ����ü
{										// sampler_state : ���÷��� �����ϸ� ���÷��� ���µ� �����ߴٰ� �����ִ� �ڵ�
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;

};
