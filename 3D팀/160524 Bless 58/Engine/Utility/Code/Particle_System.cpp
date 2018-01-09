#include "Particle_System.h"
#include "Texture.h"
#include "Export_Function.h"




bool Sort_ViewZ(Engine::ATTRIBUTE pSour, Engine::ATTRIBUTE pDest)
{
	return pSour.fViewZ > pDest.fViewZ;
}



Engine::CParticle_System::CParticle_System(LPDIRECT3DDEVICE9 pGraphicDev)
: CGameObject(pGraphicDev)
, m_vColor(0.f, 0.f, 0.f, 255.f)
, m_fSize(0.f)
, m_fSpeed(0.f)
, m_fCreateTime(1.f)
, m_dwParticleCount(0)
, m_dwMaxParticle(0)
, m_fAddTime(0.f)
{
	m_htComponent.Ready_Table(19);
}

Engine::CParticle_System::~CParticle_System(void)
{

}


void Engine::CParticle_System::Reset_Attribute()
{
	list<ATTRIBUTE>::iterator iter = m_ParticleList.begin();
	list<ATTRIBUTE>::iterator iter_end = m_ParticleList.end();


	for( ; iter != iter_end; ++iter)
	{
		
		Reset_Particle(&(*iter));
	}
}

void Engine::CParticle_System::Add_Particle()
{
	ATTRIBUTE tAttribute;

	tAttribute.bCreate = false;

	Reset_Particle(&tAttribute);
	m_ParticleList.push_back(tAttribute);
}


int Engine::CParticle_System::Update(const _float& fTimeDelta)
{
	D3DXVECTOR3 vDir = D3DXVECTOR3(0.f, 0.f, 1.f);
	D3DXVec3Normalize(&vDir, &vDir);
	((CIRCLE*)m_pShape)->vPos +=  vDir * fTimeDelta * 3.f;
	

	if(m_dwParticleCount <= m_dwMaxParticle)
	{
		m_fCreateTime += fTimeDelta;

		if(m_fCreateTime > 0.0001f)
		{
			m_fCreateTime = 0;

			list<Engine::ATTRIBUTE>::iterator iter = m_ParticleList.begin();
			list<Engine::ATTRIBUTE>::iterator iter_end = m_ParticleList.end();
			for(; iter != iter_end; iter++)
			{
				if(iter->bCreate == false)
				{
					iter->bCreate = true;
					Reset_Particle(&(*iter));
					++m_dwParticleCount;
					break;
				}
			}			
		}
	}


	list<Engine::ATTRIBUTE>::iterator iter;
	for(iter = m_ParticleList.begin(); iter != m_ParticleList.end(); iter++)
	{
		if(iter->bCreate == true)
		{
			iter->vPos += iter->vDir * fTimeDelta * iter->fSpeed;
			iter->fLifeTime -= fTimeDelta;

			if(iter->fLifeTime <= 0)
			{
				--m_dwParticleCount;
				iter->bCreate = false;
				//Reset_Particle(&(*iter));		
			}
		}
	}	

	//Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_ALPHA, this);

	return 0;//Engine::CGameObject::Update(fTimeDelta);
}


void Engine::CParticle_System::Render(void)
{
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();	

	_matrix	matWolrd;
	D3DXMatrixIdentity(&matWolrd);

	pEffect->SetMatrix("g_matWorld", &matWolrd);

	_matrix			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);	
	pEffect->SetTexture("g_BaseTexture", m_pTexture);
	//색깔
	pEffect->SetVector("g_vColor", &m_vColor);

	//사이즈

	pEffect->SetVector("g_vSize", &D3DXVECTOR4(m_fSize, m_fSize, m_fSize, m_fSize));

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	if( ! m_ParticleList.empty() )
	{	
		/*list<ATTRIBUTE>::iterator Sortiter = m_Particlelist.begin();
		list<ATTRIBUTE>::iterator Sortiter_end = m_Particlelist.end();
		for(; Sortiter != Sortiter_end; ++Sortiter)
		{
			Sortiter->fViewZ = Compute_ViewZ(&Sortiter->vPos);
		}*/
	//	m_Particlelist.sort(Sort_ViewZ);

		m_pDevice->SetTexture(0, m_pTexture);
		m_pDevice->SetFVF(PARTICLE_FVF);
		m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(PARTICLE));
	
		/*if(m_dwVBOffset >= m_iMaxParticle)
			m_dwVBOffset = 0;*/

		PARTICLE* pParticle = NULL;
	
		m_pVB->Lock( 0,	0, (void**)&pParticle,	0);

		DWORD dwNumParticlesInBatch = 0;
	
		int i = 0;
		list<ATTRIBUTE>::iterator iter = m_ParticleList.begin();

		for(; iter != m_ParticleList.end(); iter++)
		{
			if( iter->bCreate == true ) 
			{	
				pParticle[i].vPos	= iter->vPos;
				pParticle[i].vColor	= iter->vColor;

				++i; 
			}			
		}				

		m_pVB->Unlock();

		D3DXMATRIX	matIdentity;
		D3DXMatrixIdentity(&matIdentity);
		m_pDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	
		m_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_dwParticleCount);
	}

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

}

bool Engine::CParticle_System::Empty()
{
	return m_ParticleList.empty();
	
}

bool Engine::CParticle_System::Dead()
{
	list<ATTRIBUTE>::iterator iter = m_ParticleList.begin();
	list<ATTRIBUTE>::iterator iter_end = m_ParticleList.end();
	for(; iter != iter_end; ++iter)
	{
		if( iter->bAlive )
			return false;
	}	 
	return true;
}

void Engine::CParticle_System::DeleteDeadParticles()
{
	list<ATTRIBUTE>::iterator iter;
	iter = m_ParticleList.begin();
	while( iter != m_ParticleList.end() )
	{
		if( iter->bAlive == false )
		{
			// erase는 다음 반복자를 리턴하므로
			// 우리가 반복자를 증가시킬 필요가 없다.
			iter = m_ParticleList.erase(iter); 
		}
		else
		{
			iter++; // next in list
		}
	}
}

DWORD Engine::CParticle_System::FtoDw(float f)
{
	return *((DWORD*)&f);
}


//서브함수


void Engine::CParticle_System::GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	out->x = 10;
	out->y = 10;
	out->z = 10;
}

float Engine::CParticle_System::GetRandomFloat(float lowBound, float highBound)
{
	if( lowBound >= highBound ) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound; 
}


Engine::CParticle_System* Engine::CParticle_System::Create(LPDIRECT3DDEVICE9 pGraphicDev, const PARTICLEINFO* pParticleInfo)
{
	CParticle_System*	pParticle = new CParticle_System(pGraphicDev);

	if(FAILED(pParticle->Ready_Particle(pParticleInfo)))
	{
		MSG_BOX(L"Particle Created Failed");
		Engine::Safe_Release(pParticle);
	}
	return pParticle;
}

HRESULT Engine::CParticle_System::Ready_Particle(const PARTICLEINFO* pParticleInfo)
{
	HRESULT hr = NULL;

	m_eParticleShape	= pParticleInfo->eShape;	// 파티클 형태
	m_fSize				= pParticleInfo->fSize;		// 파티클 사이즈(쉐이더로 보내줌)
	m_fSpeed			= pParticleInfo->fSpeed;	// 파티클 시간
	m_fLifeTime			= pParticleInfo->fLifeTime;	// 파티클 유지시간
	m_dwMaxParticle		= pParticleInfo->iMaxCount;	// 최대갯수	
	//m_bParticle			= true;						// 게임오브젝트가 파티클인지 확인(안쓸듯)
	m_vColor			= pParticleInfo->vColor;	// 파티클의 색깔(쉐이더로 보내줌)
	m_eAngleDir			= ANGLE_X;	

	SetShape(pParticleInfo);						// 파티클 형태 결정 함수 호출

	for(int i =0; i < pParticleInfo->iMaxCount; ++i)
		Add_Particle();								// 파티클의 최대 갯수 만큼 미리 속성을 만든다

	hr = m_pDevice->CreateVertexBuffer(m_dwMaxParticle * sizeof(PARTICLE), 0, PARTICLE_FVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"ParticleBuffer Create Failed");				//	버퍼 만들어주고!!

	TCHAR	szFullPath[MAX_PATH] = L"";							
	wsprintf(szFullPath, pParticleInfo->wstrTexturePath.c_str());						//	텍스쳐 경로 복사 복사!!

	hr = D3DXCreateTextureFromFile( m_pDevice, szFullPath, &m_pTexture);				//	텍스쳐 만들자 만들자!
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"ParticleTexture Create Failed");				//	

	m_pShader = Engine::Clone_Shader(L"Shader_Particle");								//	쉐이더도 클론으로 빨아주시구요!

	return S_OK;
}

void Engine::CParticle_System::Reset_Particle(ATTRIBUTE* pAttribute)
{
	if(m_eParticleShape == SHAPE_CIRCLE)											
	{
		pAttribute->bAlive  = true;

		SetRandomInfo(&pAttribute->vPos, &pAttribute->vDir);		

		pAttribute->fAge		= 0.0f;	
		pAttribute->fLifeTime	= m_fLifeTime;
		pAttribute->fSpeed		= m_fSpeed;
		pAttribute->fSize		= m_fSize;
	}	
}

void Engine::CParticle_System::SetShape(const PARTICLEINFO* pParticleInfo)
{
	switch(m_eParticleShape)
	{
	case SHAPE_BOX:
		m_pShape = (void*)(pParticleInfo->pBox);
		break;

	case SHAPE_CIRCLE:
		m_pShape = (void*)(pParticleInfo->pCircle);
		break;

	case SHAPE_SPHERE:
		break;

	case SHAPE_CONE:
		break;
	}
}

float Engine::CParticle_System::Compute_ViewZ(const D3DXVECTOR3* pPos)
{
	D3DXMATRIX		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3		vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(D3DXVECTOR3));

	float		fDistance = D3DXVec3Length(&(vCamPos - *pPos));	

	return fDistance;
}



void Engine::CParticle_System::Free(void)
{	
	Engine::CGameObject::Free();		
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pTexture);
	Engine::Safe_Release(m_pShader);
	Engine::Safe_Delete(m_pShape);	
}

void Engine::CParticle_System::SetRandomInfo(D3DXVECTOR3* vPos, D3DXVECTOR3* vDir)
{
	if(m_eAngleDir == ANGLE_X)
	{
		D3DXVECTOR3	vLook = D3DXVECTOR3(-1.f, 0.f, 0.f);

		float fAngle = float(rand() / (float)RAND_MAX * ((CIRCLE*)m_pShape)->iAngle); 

		_matrix	matRot;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(fAngle));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		D3DXVec3Normalize(&vLook, &vLook);

		float fLength = float(rand() / (float)RAND_MAX * ((CIRCLE*)m_pShape)->iRadius);

		*vPos = ((CIRCLE*)m_pShape)->vPos + (vLook * fLength);
		*vDir = vLook;
	}
}
