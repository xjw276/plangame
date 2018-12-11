// PlaneGameView.cpp : CPlaneGameView ���ʵ��
//
#include "stdafx.h"
#include "PlaneGame.h"

#include "PlaneGameDoc.h"
#include "PlaneGameView.h"
#include "MyPlane.h"
#include "Enemy.h"
#include "Bomb.h"
#include "Ball.h"
#include "Explosion.h"
#include <atlimage.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneGameView

IMPLEMENT_DYNCREATE(CPlaneGameView, CView)

BEGIN_MESSAGE_MAP(CPlaneGameView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()

END_MESSAGE_MAP()

// CPlaneGameView ����/����
static int count = 0;
CPlaneGameView::CPlaneGameView():m_pMe(NULL)
{
	// TODO: �ڴ˴���ӹ������
	
}

CPlaneGameView::~CPlaneGameView()
{
}

BOOL CPlaneGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPlaneGameView ����

void CPlaneGameView::OnDraw(CDC* /*pDC*/)
{
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPlaneGameView ��ӡ

BOOL CPlaneGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPlaneGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPlaneGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CPlaneGameView ���

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView ��Ϣ�������
void CPlaneGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: �ڴ����ר�ô����/����û���
	
	//��ʼ����Ϸ
	if (AfxMessageBox(_T("��ӭ�����ɻ���ս\nWelcome To The PlaneGame!\n�Ƿ�ʼ��Ϸ��"), MB_YESNO) == IDYES)
	{
		//BGM
		PlaySound(MAKEINTRESOURCE(IDR_WAVE2), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT | SND_LOOP);
		
		if (AfxMessageBox(_T("��Ϸ˵����\n1������Ϸ����Ϊʮ��;\n(Ϊ������Ϸ���飬�ؿ�֮��û�е���)\n2����ͨģʽ�У�ÿͨ��һ�أ�������α�����Ļ���;\n3��ÿ����ʮ�ܵл����һ�α��������;\n4����P��������ͣ��Ϸ;\n5�������޵�ģʽ���㽫���ᱻ���䣬����ͨ��֮����Խ������벻�����ر�ؿ�Ŷ �������԰�!\n\n�Ƿ�����޵�ģʽ��"), MB_YESNO) == IDYES)
		{
			Blood = 1;
			Flag_Of_WD = 1;
		}
		else
		{	
			Blood = 6;
			Flag_Of_WD = 0;
		}
		gq = 1;
		score = 0;
	}
	else
		exit(1);

	InitGame();
}

void CPlaneGameView::StopGame()
{
	delete m_pMe;
	delete m_pMemDC;
	delete m_pDC;
	delete m_pMemBitmap;
}

BOOL CPlaneGameView::InitGame()
{
	CRect rc;
	GetClientRect(rc);

	//�������������
	srand( (unsigned)time( NULL ) );

	//�����豸DC
	m_pDC = new CClientDC(this);

	//�����ڴ�DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//�����ڴ�λͼ
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC,GAME_WIDTH,GAME_HEIGHT);

	//��λͼѡ���ڴ�DC
	m_pMemDC->SelectObject(m_pMemBitmap);

	CMyPlane::LoadImage();
	CEnemy::LoadImage();
	CBomb::LoadImage();
	CBall::LoadImage();
	CExplosion::LoadImage();

	//��������(ս��)
	m_pMe = new CMyPlane;

	//������Ϸ
	SetTimer(1,30,NULL);

	return TRUE;
}

void CPlaneGameView::UpdateFrame(CDC* pMemDC)
{
	//�������
	pMemDC->FillSolidRect(0,0,GAME_WIDTH,GAME_HEIGHT,RGB(84, 142, 239));
	
	//��������
	CDC m_MemDc;
	CDC m_cacheDC;
	CBitmap m_BKBmp, m_cacheCBitmap;
	BITMAP m_Bmplnfo;
	CDC*pDc = GetDC();
	m_BKBmp.LoadBitmapW(IDB_BITMAP5);
	m_BKBmp.GetBitmap(&m_Bmplnfo);
	m_MemDc.CreateCompatibleDC(pDc);
	m_MemDc.SelectObject(&m_BKBmp);
	CRect m_CliRect;
	GetClientRect(&m_CliRect);

	//��������������ǰ���ػ��汳��ͼƬ����ֹˢ������
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(pDc, m_CliRect.Width(), m_CliRect.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);
	count++;
	pMemDC->BitBlt(0, 0, m_CliRect.Width(), m_CliRect.Height(), &m_MemDc, 0, 0, SRCCOPY);
	pMemDC->BitBlt(0, 0, m_CliRect.Width(), count, &m_MemDc, 0, m_CliRect.Height() - count, SRCCOPY);
	pMemDC->BitBlt(0, count, m_CliRect.Width(), m_CliRect.Height() - count, &m_MemDc, 0, 0, SRCCOPY);
	if (count >= m_CliRect.Height())
		count = 0;

	//�ڻ�����ͼ��ʹ�ô�����Ч
	ValidateRect(&m_CliRect);

	//�ͷŻ���DC
	m_cacheDC.DeleteDC();

	//�ͷŶ���
	m_cacheCBitmap.DeleteObject();

	//�ͷŴ���DC
	ReleaseDC(pDc);

	//��ʾ�ؿ�
	CString str3 = _T("��ǰ�ؿ� ��");
	CString strgq;
	strgq.Format(_T("%d"), gq);
	if (gq == 666)
		strgq = _T("�ر�ؿ�");
	str3 += strgq;
	pMemDC->SetTextColor(RGB(255, 0, 255));
	pMemDC->TextOut(GAME_WIDTH / 2 + 130, 40, str3);

	//��ʾѪ��
	CString str = _T("ʣ������ֵ ��");
	CString strBlood;
	strBlood.Format(_T("%d"), Blood);
	str += strBlood;
	if (Flag_Of_WD == 1)
		str = _T("�޵�ģʽ");
	pMemDC->SetTextColor(RGB(255, 0, 255));
	pMemDC->TextOut(GAME_WIDTH / 2 + 150, 600, str);
	
	//��ʾ�÷�
	CString str2 = _T("��ǰ���� ��");
	CString strScore;
	strScore.Format(_T("%d"), score);
	str2 += strScore;
	pMemDC->SetTextColor(RGB(255, 0, 255));
	pMemDC->TextOut(GAME_WIDTH / 2 + 130, 20, str2);

	//��ʾ��ͣ��Ϸ
	CString strp= _T("��P����ͣ��Ϸ");
	pMemDC->SetTextColor(RGB(255, 0, 255));
	pMemDC->TextOut(GAME_WIDTH / 2 + 130, 60, strp);

	//�����ҷ�ս��
	if(m_pMe!=NULL)
	{
	   m_pMe->Draw(m_pMemDC,FALSE);
	}
	else
	{  
		//Game Over
		CString str=_T("Game Over!");
		pMemDC->SetBkMode(TRANSPARENT);
		pMemDC->SetTextAlign(TA_CENTER);
		pMemDC->SetTextColor(RGB(255,0,0));
		pMemDC->TextOut(GAME_WIDTH/2,GAME_HEIGHT/2,str);
	}
	
	//���� ��������ը���л����ӵ�
	for(int i=0;i<4;i++)
	{
		POSITION pos1,pos2;
		for( pos1 = m_ObjList[i].GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
		{
			CGameObject* pObj = (CGameObject*)m_ObjList[i].GetNext( pos1 );
			if(!pObj->Draw(pMemDC,FALSE))
			{
				m_ObjList[i].RemoveAt(pos2);
				delete pObj;
			}
		}
	}

	//�����ڴ�DC���豸DC
	m_pDC->BitBlt(0,0,GAME_WIDTH,GAME_HEIGHT,m_pMemDC,0,0,SRCCOPY);
}
void CPlaneGameView::AI()
{
	static int nCreator = rand() %5+10;
	
	
	
	//�ؿ�����֮��զ����
	if (gq == 11)
	{
		KillTimer(1);
		if (Flag_Of_WD == 1)
		{
			if (AfxMessageBox(_T("��ϲ�㣬�Ѿ���ͨ����,�Ƿ�����ر�ؿ���\n(�����ر�ؿ�֮������Ȼ���޵е�Ŷ~~)"), MB_YESNO) == IDYES)
			{
				gq = 666;
				SetTimer(1, 30, NULL);
				
			}
			else
				exit(1);
		}
		else
		{
			if (AfxMessageBox(_T("��ϲ�㣬�Ѿ���ͨ����,�Ƿ����¿�ʼ��Ϸ��)"), MB_YESNO) == IDYES)
			{
				
				//��ʼ��Ϸ
				SetTimer(1, 30, NULL);
				Blood = 6;
				score = 0;
				gq = 1;
				m_pMe = new CMyPlane;
			}
			else
				exit(1);
		}
	}

	//��ͬ�ؿ���ս������Ƶ��
	
	
		if (nCreator <= 0)
		{
			nCreator = rand() % 5 + 15 - gq;
			m_ObjList[enEnemy].AddTail(new CEnemy);
		}
		nCreator--;
	
	//�޵�ģʽ�µ��ر�ؿ�
	if (gq == 666)
	{
		nCreator = rand() % 5;
		m_ObjList[enEnemy].AddTail(new CEnemy);
	}
	
	if(m_pMe==NULL)
		return;

	//����ĸ���������ƶ�ս��
	for(int i=0;i<4;i++)
	{
		int nMeMotion=0;
		m_pMe->SetVerMotion(0);
		m_pMe->SetHorMotion(0);

		nMeMotion = GetKey(VK_UP);
		if(nMeMotion==1)
			m_pMe->SetVerMotion(1);
		    
		nMeMotion = GetKey(VK_DOWN);
		if(nMeMotion==1)
			m_pMe->SetVerMotion(-1);

		nMeMotion = GetKey(VK_RIGHT);
		if(nMeMotion==1)
			m_pMe->SetHorMotion(1);

		nMeMotion = GetKey(VK_LEFT);
		if(nMeMotion==1)
			m_pMe->SetHorMotion(-1);
	}
	
	
	//����ս������
	if(GetKey(VK_SPACE)==1)//�����˿ո��
	{
		if(m_pMe!=NULL && m_pMe->Fired())
		{
			CPoint pt = m_pMe->GetPoint();
			m_ObjList[enBomb].AddTail(new CBomb(pt.x+10,pt.y+10));
			m_ObjList[enBomb].AddTail(new CBomb(pt.x+30,pt.y+10));
		}
	}


	//�л������ӵ�

	CPoint PlanePt = m_pMe->GetPoint();
	for(POSITION ePos=m_ObjList[enEnemy].GetHeadPosition();ePos!=NULL;)
	{
          CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos);
		  if(!pEnemy->Fired())
			  continue;
		  CPoint  ePt = pEnemy->GetPoint();

		  BOOL by=FALSE;

		  //�л���ս��ǰ��
		  if(pEnemy->GetMontion()==1 && ePt.y<PlanePt.y)
		         by=  TRUE;
		  //�л���ս������
		  if(pEnemy->GetMontion()==-1 && ePt.y>PlanePt.y)
				 by=  TRUE;	

		  if(by && ePt.x >= PlanePt.x && ePt.x<PlanePt.x+CMyPlane::PLANE_WIDTH)
		  {
              m_ObjList[enBall].AddTail(new CBall(ePt.x+10,ePt.y+10,pEnemy->GetMontion()));
		  }
	}
	
	//�л��ӵ�ը��ս��
	POSITION bPos1=NULL,bPos2=NULL;
	CRect mRect = m_pMe->GetRect();
	for(bPos1=m_ObjList[enBall].GetHeadPosition();( bPos2 = bPos1 ) != NULL;)
	{
		CBall* pBall = (CBall*)m_ObjList[enBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		if(tmpRect.IntersectRect(&bRect,mRect))
		{
			//��ӱ�ըЧ��
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left,mRect.top)
				);

			//���޵�ģʽ�£�����һ������ֵ
			if(Flag_Of_WD == 0)
			Blood--;
			
			//ɾ���ӵ�
			m_ObjList[enBall].RemoveAt(bPos2);
			delete pBall;
		}
		
		if (Blood == 0)
		{
			KillTimer(1);
			if (AfxMessageBox(_T("��Ϸ������\n�Ƿ�ԭ�ظ�������ܻ��6������ֵŶ��"), MB_YESNO) == IDYES)
			{
				
				//��ʼ��Ϸ
				SetTimer(1, 30, NULL);
				Blood = 6;
				
			}
			else
			{
				exit(1);
			}
		}
	}
	
		
	//ս������ը���л�
	POSITION mPos1=NULL,mPos2=NULL;
	for (mPos1 = m_ObjList[enBomb].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
	{
		CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
		CRect bRect = pBomb->GetRect();

		POSITION ePos1 = NULL, ePos2 = NULL;
		for (ePos1 = m_ObjList[enEnemy].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
		{
			CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
			CRect mRect = pEnemy->GetRect();
			CRect tmpRect;
			if (tmpRect.IntersectRect(&bRect, mRect))
			{
				//��ӱ�ըЧ��
				m_ObjList[enExplosion].AddTail(
					new CExplosion(mRect.left, mRect.top)
				);

				//ɾ������
				m_ObjList[enBomb].RemoveAt(mPos2);
				delete pBomb;

				//ɾ���л�
				m_ObjList[enEnemy].RemoveAt(ePos2);
				delete pEnemy;

				//�ӷ�
				score++;

				//�ؿ��Զ�����
				if (score % 50 == 0 && score != 0)
				{
					gq++;
					Blood += 2;
				}

				//Ϊս����������ֵ
				if (score % 10 == 0 && score != 0)
				{
					Blood++;
				}
				break;
			}
		}
	}
	
	
	
	
	//��ͣ
	if (GetKey('P') == 1)
	{
		KillTimer(1);
		if (AfxMessageBox(_T("�Ƿ������Ϸ��"), MB_YESNO) == 6)
			SetTimer(1, 30, NULL);
		else
			exit(1);
	}
	
}
void CPlaneGameView::OnTimer(UINT_PTR nIDEvent)
{
	//ˢ����Ϸ֡����: ���ڴ�DC�ϻ�ͼ
	UpdateFrame(m_pMemDC);
	AI();
	
	CView::OnTimer(nIDEvent);
}


