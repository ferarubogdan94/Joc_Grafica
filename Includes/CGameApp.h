/**
 * Desc: Game Application class, this is the central hub for all app processing
 *
 * Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
 */
#ifndef _CGAMEAPP_H_
#define _CGAMEAPP_H_

#include "Main.h"
#include "CTimer.h"
#include "CPlayer.h"
#include "CGameObject.h"
#include "BackBuffer.h"
#include "Singleton.h"
#include "TmxMap.h"
#include "Score.h"
#include "Bee.h"
#include <vector>
#include <memory>

#define GRAVITATIONAL_CONSTANT 10

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if (p) { delete (p); (p)=NULL; } }
#endif

/**
 * Central game engine, initializes the game and handles core processes.
 */
class CGameApp : public Singleton<CGameApp>
{
	SINGLETON_DECL(CGameApp);
public:
	virtual ~CGameApp();
	LRESULT DisplayWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	bool InitInstance(LPCTSTR lpCmdLine, int iCmdShow);
	int BeginGame();
	bool ShutDown();
	HDC GetBackBufferDC() const 
	{
		return m_pBBuffer ? m_pBBuffer->getDC() : NULL;
	}

	ULONG GetViewWidth() const
	{
		return m_nViewWidth;
	}

	ULONG GetViewHeight() const
	{
		return m_nViewHeight;
	}


private:
	bool BuildObjects();
	void ReleaseObjects();
	void FrameAdvance();
	bool CreateDisplay();
	void SetupGameState();
	void AnimateObjects();
	void DrawObjects();
	void ProcessInput();
	void CollisionDetection();
	void DoGameLogic();

	/**
	 * Displays some information in the head of the window(heads up display). 
	 */
	void DrawHUD();

	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	/**
	 * Game timer.
	 */
	CTimer m_Timer;        

	/**
	 * Used for making sure we update only when fps changes.
	 */
	ULONG m_LastFrameRate;

	/**
	 * Main window HWND.
	 */
	HWND m_hWnd;      

	/**
	 * Window Icon.
	 */
	HICON m_hIcon; 

	/**
	 * Window Menu.
	 */
	HMENU m_hMenu;            

	/**
	 * True if this application is active otherwise false.
	 */
	bool m_bActive;          

	/**
	 * X Position of render viewport(The x-coordinate of the upper-left corner of the game window.).
	 */
	ULONG m_nViewX;

	/**
	 * Y Position of render viewport(The y-coordinate of the upper-left corner of the game window.).
	 */
	ULONG m_nViewY;           

	/** 
	 * Width of render viewport(The with of the game window).
	 */
	ULONG m_nViewWidth;

	/**
	 * Height of render viewport(The height of the game window).
	 */
	ULONG m_nViewHeight;

	/**
	 * Window instance.
	 */
	HINSTANCE m_hInstance;

	/**
	 The BackBuffer of this game application.
	*/
	BackBuffer* m_pBBuffer;

	/**
	 * Vector with all game objects.
	 */
	std::vector<std::shared_ptr<CGameObject>> m_vGameObjects;  

	/** 
	 * A pointer to player.
	 */
	std::weak_ptr<CPlayer> m_pPlayer; 

	Score *m_pScore;

	TmxMap* m_pMap;

	Sprite* m_pBackground;

	Bee* bee;
};

#endif