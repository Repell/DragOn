#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	:	CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMesh->Set_AnimationSet(57);

	return S_OK;
}

HRESULT CPlayer::Late_Init()
{
	m_pTransform->m_vScale = { 0.01f, 0.01f, 0.01f };
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 2.f, 2.f, 2.f };

	return S_OK;
}

_int CPlayer::Update_Object(const _float & fTimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(fTimeDelta);

	m_pMesh->Play_AnimationSet(fTimeDelta);

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_PRIORITY, this);
	return NO_EVENT;
}

void CPlayer::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CPlayer::Render_Object()
{
	Render_Set();

	m_pMesh->Render_Meshes();

	Render_ReSet();
}

void CPlayer::Render_Set()
{
	//Alpha Test Begin
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	//Render State
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//SetTransform
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
}

void CPlayer::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CPlayer::UI_Sample()
{
	//���� ����
	_matrix matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);	 // �׵�
	D3DXMatrixIdentity(&matView);	// ������, ��ġ
	D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);		//Near 0.f, Far 1.f;

	float fScale[4];
	fScale[0] = 450.f;	// �ؽ�ó�� X ũ�� ��
	fScale[1] = 250.f;	// �ؽ�ó�� Y ũ�� ��
	fScale[2] = 1.f;        // �ؽ�ó�� Z ũ�� ��(Z�� 1.f ����)
	_vec3 vScale = { 450.f, 250.f, 1.f };

	//ũ�� ����(Scale)
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
			matView(i, j) *= fScale[i];
	}

	//��ġ ����(Pos)
	matView.m[3][1] = -225.f;	

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);	//�׵�
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);		//�׵� *= �����ϰ�, ��ġ�� ����
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);	//������� ��ȯ

	//���� ������ķ� Reset(d3dts)�Ͽ� �Ϲ� ��ü�� ������Ŀ� ������ �����ʵ��� ����	

}

HRESULT CPlayer::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	pComponent = m_pMesh =dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Player"));
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	//Transform Component
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//Renderer Component
	pComponent = m_pRenderer = ENGINE::Get_Renderer();
	pComponent->AddRef();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);

	////////////////////////////
	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free()
{
	ENGINE::CGameObject::Free();
}
