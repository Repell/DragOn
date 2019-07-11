#ifndef Engine_Struct_h__
#define Engine_Struct_h__


namespace ENGINE
{
	typedef struct tag3D_Line
	{
		public:
			tag3D_Line() {};
			tag3D_Line(const _vec3 vStartPos, const _float fEnd)
			{
				vLine_X[0] = vStartPos;
				vLine_Y[0] = vStartPos;
				vLine_Z[0] = vStartPos;
				vLine_X[1] = vStartPos;
				vLine_X[1].x = vStartPos.x + fEnd;
				vLine_Y[1] = vStartPos;
				vLine_Y[1].y = vStartPos.y + fEnd;
				vLine_Z[1] = vStartPos;
				vLine_Z[1].z = vStartPos.z + fEnd;
			}
	public:
		void Set_LinePos_X(const _vec3 vStart, const _vec3 vEnd)
		{
			vLine_X[0] = vStart;
			vLine_X[1] = vEnd;
		}
		void Set_LinePos_Y(const _vec3 vStart, const _vec3 vEnd)
		{
			vLine_Y[0] = vStart;
			vLine_Y[1] = vEnd;
		}
		void Set_LinePos_Z(const _vec3 vStart, const _vec3 vEnd)
		{
			vLine_Z[0] = vStart;
			vLine_Z[1] = vEnd;
		}

		_vec3 vLine_X[2];
		_vec3 vLine_Y[2];
		_vec3 vLine_Z[2];

	}LINE_3D;

	typedef struct tagNavi
	{
		_ulong Index;
		_vec3 vPointA;
		_vec3 vPointB;
		_vec3 vPointC;
	}NAVI;

	typedef  struct tagObb
	{
		_vec3		vPoint[8];		// ť�긦 �̷�� ���� 8��
		D3DXVECTOR3 vCenterPos;				//�߾� ��ġ��
		D3DXVECTOR3 vProjAxis[3];	//�������� ť���� �� ���� ���� ����� ����
		D3DXVECTOR3 vAxisDir[3];	//���ڿ� ������ �� ���� ���� ����
		float fAxisLen[3];					//������ ������ �� ���� ���� / fAxisLen[n]�� vAxisDir[n]�� �����Ѵ�.

	}OBB;

	typedef struct tagMeshInfo
	{
		_int iUnitType;
		_ushort wMeshType;
		_ushort wContainerIdx;
		wstring pMeshTag;
		//wstring pFilePath;
		wstring pFileName;
		
	}MESH_INFO;

	typedef struct tagVertexColor
	{
		_vec3 vPos;
		_ulong dwColor;

	}VTX_COL;

	const _ulong FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef struct tagVertexTexture
	{
		_vec3	vPos;
		_vec3	vNormal;
		_vec2	vTexUV;

	}VTX_TEX;

	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTexture
	{
		_vec3	vPos;
		_vec3	vTex;

	}VTX_CUBE;

	const _ulong FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagScreenVertex
	{
		_vec4		vPos;
		_vec2		vTexUV;
	}VTX_SCREEN;
			
	const _ulong FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;

	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix		combinedTransformMatrix;

	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*	ppTexture;

		LPD3DXMESH						pOriMesh;	//���� ���� ���� �Ž�(��ȭ X)
		
		_ulong									dwNumBones;

		//�ִϸ��̼��� ������ ���·� �������� �� ������ ���븦 ������ �ִ� ���
		_matrix*							pFrameOffSetMatrix;	//���� �迭�� �Ҵ��� �ּҰ� ������

		//������ ������ �ִ� CombinedTransformationMatrix�� ��� ���ŵǱ� ������
		//�ƿ� �� CombinedTransformationMatrix ����� �ּҸ� �����ؼ� �Ź� ���ŵ� ����� ������ ���� ����
		_matrix**							ppFrameCombinedMatrix;

		_matrix*							pRenderingMatrix;


	}D3DXMESHCONTAINER_DERIVED;

}

#endif // Engine_Struct_h__
