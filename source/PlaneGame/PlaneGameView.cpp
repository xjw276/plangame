// PlaneGameView.cpp : CPlaneGameView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()

END_MESSAGE_MAP()

// CPlaneGameView 构造/析构
static int count = 0;
CPlaneGameView::CPlaneGameView():m_pMe(NULL)
{
	// TODO: 在此处添加构造代码
	
}

CPlaneGameView::~CPlaneGameView()
{
}

BOOL CPlaneGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPlaneGameView 绘制

void CPlaneGameView::OnDraw(CDC* /*pDC*/)
{
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPlaneGameView 打印

BOOL CPlaneGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPlaneGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPlaneGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPlaneGameView 诊断

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView 消息处理程序
void CPlaneGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	
	//初始化游戏
	if (AfxMessageBox(_T("欢迎来到飞机大战\nWelcome To The PlaneGame!\n是否开始游戏？"), MB_YESNO) == IDYES)
	{
		//BGM
		PlaySound(MAKEINTRESOURCE(IDR_WAVE2), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT | SND_LOOP);
		
		if (AfxMessageBox(_T("游戏说明：\n1、本游戏共分为十关;\n(为增加游戏体验，关卡之间没有弹窗)\n2、普通模式中，每通过一关，获得两次被击落的机会;\n3、每击落十架敌机获得一次被击落机会;\n4、按P键可以暂停游戏;\n5、进入无敌模式后，你将不会被击落，并且通关之后可以进入意想不到的特别关卡哦 快来试试吧!\n\n是否进入无敌模式？"), MB_YESNO) == IDYES)
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

	//产生随机数种子
	srand( (unsigned)time( NULL ) );

	//建立设备DC
	m_pDC = new CClientDC(this);

	//建立内存DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//建立内存位图
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC,GAME_WIDTH,GAME_HEIGHT);

	//将位图选入内存DC
	m_pMemDC->SelectObject(m_pMemBitmap);

	CMyPlane::LoadImage();
	CEnemy::LoadImage();
	CBomb::LoadImage();
	CBall::LoadImage();
	CExplosion::LoadImage();

	//产生主角(战机)
	m_pMe = new CMyPlane;

	//启动游戏
	SetTimer(1,30,NULL);

	return TRUE;
}

void CPlaneGameView::UpdateFrame(CDC* pMemDC)
{
	//绘制天空
	pMemDC->FillSolidRect(0,0,GAME_WIDTH,GAME_HEIGHT,RGB(84, 142, 239));
	
	//背景滚动
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

	//创建缓冲区，提前加载缓存背景图片，防止刷新闪屏
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(pDc, m_CliRect.Width(), m_CliRect.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);
	count++;
	pMemDC->BitBlt(0, 0, m_CliRect.Width(), m_CliRect.Height(), &m_MemDc, 0, 0, SRCCOPY);
	pMemDC->BitBlt(0, 0, m_CliRect.Width(), count, &m_MemDc, 0, m_CliRect.Height() - count, SRCCOPY);
	pMemDC->BitBlt(0, count, m_CliRect.Width(), m_CliRect.Height() - count, &m_MemDc, 0, 0, SRCCOPY);
	if (count >= m_CliRect.Height())
		count = 0;

	//在绘制完图后，使得窗口有效
	ValidateRect(&m_CliRect);

	//释放缓冲DC
	m_cacheDC.DeleteDC();

	//释放对象
	m_cacheCBitmap.DeleteObject();

	//释放窗口DC
	ReleaseDC(pDc);

	//显示关卡
	CString str3 = _T("当前关卡 ：");
	CString strgq;
	strgq.Format(_T("%d"), gq);
	if (gq == 666)
		strgq = _T("特别关卡");
	str3 += strgq;
	pMemDC->SetTextColor(RGB(255, 0, 255));
	pMemDC->TextOut(GAME_WIDTH / 2 + 130, 40, str3);

	//显示血量
	CString str = _T("剩余生命值 ：");
	CString strBlood;
	strBlood.Format(_T("%d"), Blood);
	str += strBlood;
	if (Flag_Of_WD == 1)
		str = _T("无敌模式");
	pMemDC->SetTextColor(RGB(255, 0, 255));
	pMemDC->TextOut(GAME_WIDTH / 2 + 150, 600, str);
	
	//显示得分
	CString str2 = _T("当前积分 ：");
	CString strScore;
	strScore.Format(_T("%d"), score);
	str2 += strScore;
	pMemDC->SetTextColor(RGB(255, 0, 255));
	pMemDC->TextOut(GAME_WIDTH / 2 + 130, 20, str2);

	//显示暂停游戏
	CString strp= _T("按P键暂停游戏");
	pMemDC->SetTextColor(RGB(255, 0, 255));
	pMemDC->TextOut(GAME_WIDTH / 2 + 130, 60, strp);

	//绘制我方战机
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
	
	//绘制 导弹、爆炸、敌机、子弹
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

	//复制内存DC到设备DC
	m_pDC->BitBlt(0,0,GAME_WIDTH,GAME_HEIGHT,m_pMemDC,0,0,SRCCOPY);
}
void CPlaneGameView::AI()
{
	static int nCreator = rand() %5+10;
	
	
	
	//关卡满级之后咋办呢
	if (gq == 11)
	{
		KillTimer(1);
		if (Flag_Of_WD == 1)
		{
			if (AfxMessageBox(_T("恭喜你，已经打通关了,是否进入特别关卡？\n(进入特别关卡之后，你依然是无敌的哦~~)"), MB_YESNO) == IDYES)
			{
				gq = 666;
				SetTimer(1, 30, NULL);
				
			}
			else
				exit(1);
		}
		else
		{
			if (AfxMessageBox(_T("恭喜你，已经打通关了,是否重新开始游戏？)"), MB_YESNO) == IDYES)
			{
				
				//开始游戏
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

	//不同关卡的战机产生频率
	
	
		if (nCreator <= 0)
		{
			nCreator = rand() % 5 + 15 - gq;
			m_ObjList[enEnemy].AddTail(new CEnemy);
		}
		nCreator--;
	
	//无敌模式下的特别关卡
	if (gq == 666)
	{
		nCreator = rand() % 5;
		m_ObjList[enEnemy].AddTail(new CEnemy);
	}
	
	if(m_pMe==NULL)
		return;

	//检测四个方向键，移动战机
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
	
	
	//产生战机导弹
	if(GetKey(VK_SPACE)==1)//按下了空格键
	{
		if(m_pMe!=NULL && m_pMe->Fired())
		{
			CPoint pt = m_pMe->GetPoint();
			m_ObjList[enBomb].AddTail(new CBomb(pt.x+10,pt.y+10));
			m_ObjList[enBomb].AddTail(new CBomb(pt.x+30,pt.y+10));
		}
	}


	//敌机发射子弹

	CPoint PlanePt = m_pMe->GetPoint();
	for(POSITION ePos=m_ObjList[enEnemy].GetHeadPosition();ePos!=NULL;)
	{
          CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos);
		  if(!pEnemy->Fired())
			  continue;
		  CPoint  ePt = pEnemy->GetPoint();

		  BOOL by=FALSE;

		  //敌机在战机前面
		  if(pEnemy->GetMontion()==1 && ePt.y<PlanePt.y)
		         by=  TRUE;
		  //敌机在战机后面
		  if(pEnemy->GetMontion()==-1 && ePt.y>PlanePt.y)
				 by=  TRUE;	

		  if(by && ePt.x >= PlanePt.x && ePt.x<PlanePt.x+CMyPlane::PLANE_WIDTH)
		  {
              m_ObjList[enBall].AddTail(new CBall(ePt.x+10,ePt.y+10,pEnemy->GetMontion()));
		  }
	}
	
	//敌机子弹炸掉战机
	POSITION bPos1=NULL,bPos2=NULL;
	CRect mRect = m_pMe->GetRect();
	for(bPos1=m_ObjList[enBall].GetHeadPosition();( bPos2 = bPos1 ) != NULL;)
	{
		CBall* pBall = (CBall*)m_ObjList[enBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		if(tmpRect.IntersectRect(&bRect,mRect))
		{
			//添加爆炸效果
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left,mRect.top)
				);

			//非无敌模式下，减少一次生命值
			if(Flag_Of_WD == 0)
			Blood--;
			
			//删除子弹
			m_ObjList[enBall].RemoveAt(bPos2);
			delete pBall;
		}
		
		if (Blood == 0)
		{
			KillTimer(1);
			if (AfxMessageBox(_T("游戏结束！\n是否原地复活？（还能获得6次生命值哦）"), MB_YESNO) == IDYES)
			{
				
				//开始游戏
				SetTimer(1, 30, NULL);
				Blood = 6;
				
			}
			else
			{
				exit(1);
			}
		}
	}
	
		
	//战机导弹炸掉敌机
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
				//添加爆炸效果
				m_ObjList[enExplosion].AddTail(
					new CExplosion(mRect.left, mRect.top)
				);

				//删除导弹
				m_ObjList[enBomb].RemoveAt(mPos2);
				delete pBomb;

				//删除敌机
				m_ObjList[enEnemy].RemoveAt(ePos2);
				delete pEnemy;

				//加分
				score++;

				//关卡自动生成
				if (score % 50 == 0 && score != 0)
				{
					gq++;
					Blood += 2;
				}

				//为战机补充生命值
				if (score % 10 == 0 && score != 0)
				{
					Blood++;
				}
				break;
			}
		}
	}
	
	
	
	
	//暂停
	if (GetKey('P') == 1)
	{
		KillTimer(1);
		if (AfxMessageBox(_T("是否继续游戏？"), MB_YESNO) == 6)
			SetTimer(1, 30, NULL);
		else
			exit(1);
	}
	
}
void CPlaneGameView::OnTimer(UINT_PTR nIDEvent)
{
	//刷新游戏帧画面: 在内存DC上绘图
	UpdateFrame(m_pMemDC);
	AI();
	
	CView::OnTimer(nIDEvent);
}


