matrix			g_matWorld;				// 상수 테이블 : 클라이언트에 존재하는 데이터를 넘겨 받아 작동 코드에 적용하기 위한 상징적인 명칭
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;			// 샘플러란 : 색상 정보만 담고 있는 팔레트 형태의 텍스쳐를 쉐이더에서는 일컫는 명칭

sampler BaseSampler = sampler_state		// 팔레트 상태의 텍스쳐를 hlsl에서 담아두는 자료형의 형태, 구조체
{										// sampler_state : 샘플러를 정의하며 샘플러의 상태도 정의했다고 말해주는 코드
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;

};
