/**
 * File: CGameApp.cpp
 *
 * Desc: Game Application class, this is the central hub for all app processing
 *
 * Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
 */

#include <algorithm>
#include "resource.h"
#include "CGameApp.h"
#include "CPlayer.h"

SINGLETON_IMPL(CGameApp);

extern HINSTANCE g_hInst;

#define PLAYER_START_X 100
#define PLAYER_START_Y 300

#define MAP_CENTER_X 400
#define MAP_CENTER_Y 256

#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 552

CGameApp::CGameApp()
{
	// Reset / Clear all required values
	m_hWnd = NULL;
	m_hIcon = NULL;
	m_hMenu = NULL;
	m_pBBuffer = NULL;
	m_LastFrameRate = 0;
}

CGameApp::~CGameApp()
{
	// Shut the engine down
	ShutDown();
}

/**
 * Initializes the entire Engine here.
 */
bool CGameApp::InitInstance(LPCTSTR lpCmdLine, int iCmdShow)
{
	// Create the primary display device
	if (!CreateDisplay())
	{
		ShutDown();
		return false;
	}

	// Build Objects
	if (!BuildObjects())
	{
		MessageBox(0, _T("Failed to initialize properly. Reinstalling the application may solve this problem.\nIf the problem persists, please contact technical support."), _T("Fatal Error"), MB_OK | MB_ICONSTOP);
		ShutDown();
		return false;
	}

	// Set up all required game states
	SetupGameState();

	// Success!
	return true;
}

/**
 * Create the display windows, devices etc, ready for rendering.
 */
bool CGameApp::CreateDisplay()
{
	LPTSTR      WindowTitle = _T("Honey Collector");
	LPCSTR      WindowClass = _T("GameFramework_Class");
	USHORT      Width = DISPLAY_WIDTH;
	USHORT      Height = DISPLAY_HEIGHT;
	RECT        rc;
	WNDCLASSEX  wcex;


	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CGameApp::StaticWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = WindowClass;
	wcex.hIconSm = NULL;

	if (RegisterClassEx(&wcex) == 0)
	{
		return false;
	}

	// Retrieve the final client size of the window
	::GetClientRect(m_hWnd, &rc);
	m_nViewX = rc.left;
	m_nViewY = rc.top;
	m_nViewWidth = rc.right - rc.left;
	m_nViewHeight = rc.bottom - rc.top;

	m_hWnd = CreateWindow(WindowClass, WindowTitle, WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, g_hInst, this);

	if (!m_hWnd)
	{
		return false;
	}

	// Show the window
	ShowWindow(m_hWnd, SW_SHOW);

	// Success!!
	return true;
}

/**
 * Signals the beginning of the physical post-initialization stage.
 * From here on, the game engine has control over processing.
 */
int CGameApp::BeginGame()
{
	MSG	msg;

	// Start main loop
	while (true)
	{
		// Did we receive a message, or are we idling ?
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Advance Game Frame.
			FrameAdvance();
		}
	} // Until quit message is received

	return 0;
}

/**
 * Shuts down the game engine, and frees up all resources.
 */
bool CGameApp::ShutDown()
{
	// Release any previously built objects
	ReleaseObjects();

	// Destroy menu, it may not be attached
	if (m_hMenu)
	{
		DestroyMenu(m_hMenu);
	}
	m_hMenu = NULL;

	// Destroy the render window
	SetMenu(m_hWnd, NULL);
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
	}
	m_hWnd = NULL;

	// Shutdown Success
	return true;
}

/**
 * This is the main message pump for ALL display devices, it captures
 * the appropriate messages, and routes them through to the application
 * class for which it was intended, therefore giving full class access.
 * Note : It is VITALLY important that you should pass your 'this' pointer to
 * the lpParam parameter of the CreateWindow function if you wish to be
 * able to pass messages back to that app object.
 */
LRESULT CALLBACK CGameApp::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// If this is a create message, trap the 'this' pointer passed in and store it within the window.
	if (Message == WM_CREATE) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);
	}

	// Obtain the correct destination for this message
	CGameApp *Destination = (CGameApp*)GetWindowLong(hWnd, GWL_USERDATA);

	// If the hWnd has a related class, pass it through
	if (Destination) {
		return Destination->DisplayWndProc(hWnd, Message, wParam, lParam);
	}

	// No destination found, defer to system...
	return DefWindowProc(hWnd, Message, wParam, lParam);
}

/**
 * Window procedure for pause menu (dialog box).
 */
BOOL CALLBACK
AboutDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_RESUME_BUTTON:
			EndDialog(hDlg, 0);
			return true;
		case ID_MAIN_MENU_BUTTON:
			PostQuitMessage(0);
		}
		break;
	}
	return false;
}

/**
 * The display devices internal WndProc function. All messages being
 * passed to this function are relative to the window it owns.
 */
LRESULT CGameApp::DisplayWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// Determine message type
	switch (Message)
	{
	case WM_CREATE:
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			// App is inactive
			m_bActive = false;

		} // App has been minimized
		else
		{
			// App is active
			m_bActive = true;

			// Store new viewport sizes
			m_nViewWidth = LOWORD(lParam);
			m_nViewHeight = HIWORD(lParam);
		} // End if !Minimized
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			if (m_bActive == true) {
				m_bActive = false;
				DialogBox(
					g_hInst, // Application instance.
					MAKEINTRESOURCE(IDD_PAUSE_MENU), // Dialog resource ID.
					m_hWnd, // Parent window of dialog box.
					AboutDlgProc); // Ptr to dialog box window procedure.
				m_bActive = true;
				return 0;
			}

			//PostQuitMessage(0);
			break;
		}
		break;

	case WM_KEYUP:
		m_pPlayer->m_State = PlayerState::Idle;
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);

	} // End Message Switch

	return 0;
}

/**
 * Build our demonstration meshes, and the objects that instance them.
 */
bool CGameApp::BuildObjects()
{
	m_pBBuffer = new BackBuffer(m_hWnd, m_nViewWidth, m_nViewHeight);

	m_pPlayer = std::make_shared<CPlayer>();
	
	m_pMap = std::make_shared<TmxMap>("data/level2.in", "data/tileset.bmp");

	m_pScore = new Score(40, 40);

	m_pBackground = new Sprite("data/back.bmp");
	m_pBackground->myPosition.x = MAP_CENTER_X;
	m_pBackground->myPosition.y = MAP_CENTER_Y;

	for (int i = 0; i < 4; i++)
	{
		std::shared_ptr<Bee> bee1 = std::make_shared<Bee>(m_pMap);
		bee1->Init(Vec2(1000 + (i + 1)* 250.f, 300.f));
		m_vBees.push_back(bee1);
	}

	bee = new Bee(m_pMap);
	bee->Init(Vec2(1000, 300));

	// Success!
	return true;
}

/**
 * Sets up all the initial states required by the game.
 */
void CGameApp::SetupGameState()
{
	srand(timeGetTime());

	m_pPlayer->Init(Vec2(PLAYER_START_X, PLAYER_START_Y));

}

/**
 * Releases our objects and their associated memory so that we can
 * rebuild them, if required, during our applications life-time.
 */
void CGameApp::ReleaseObjects()
{
	// this will automatically call the d-tors for each shared pointer objects
	m_vBees.clear();
	SAFE_DELETE(m_pScore);
	SAFE_DELETE(m_pBBuffer);
	SAFE_DELETE(m_pBackground);
	if (m_pMap.unique())
	{
		m_pMap.reset();
	}
	SAFE_DELETE(bee);
	if (m_pPlayer.unique())
	{
		m_pPlayer.reset();
	}
}

/**
 * Called to signal that we are now rendering the next frame.
 */
void CGameApp::FrameAdvance()
{
	static TCHAR FrameRate[50];
	static TCHAR TitleBuffer[255];

	// Advance the timer
	m_Timer.Tick(60);

	// Skip if app is inactive
	if (!m_bActive) {
		return;
	}


	/*
	// Get / Display the framerate
	if (m_LastFrameRate != m_Timer.GetFrameRate())
	{
	m_LastFrameRate = m_Timer.GetFrameRate(FrameRate, 50);

	int life = 0;

	if (m_pPlayer.lock())
	{
	m_pScore->setScore(m_pPlayer.lock()->m_score);
	life = m_pPlayer.lock()->m_life;
	}
	sprintf_s(TitleBuffer, _T("Honey Collector @ %s\t Player Life: %d"), FrameRate, life);

	SetWindowText(m_hWnd, TitleBuffer);
	} // End if Frame Rate Altered

	*/
	// Game logic
	DoGameLogic();

	// Poll & Process input devices
	ProcessInput();

	// Collision detection between game objects
	CollisionDetection();

	// Animate the game objects
	AnimateObjects();

	// Drawing the game objects
	DrawObjects();
}

/**
 * Simply polls the input devices and performs basic input operations
 */
void CGameApp::ProcessInput()
{
	static UCHAR pKeyBuffer[256];
	ULONG Direction = 0;
	float X = 0.0f, Y = 0.0f;

	// Retrieve keyboard state
	if (!GetKeyboardState(pKeyBuffer)) return;

	if (pKeyBuffer[VK_SPACE] & 0xF0)
	{
		m_pPlayer->Jump();
	}

	if (pKeyBuffer[VK_LEFT] & 0xF0)
	{
		// scroll the map with the player velocity
		m_pPlayer->WalkLeft();
		m_pMap->Scroll(DIRECTION::DIR_LEFT);

	}

	if (pKeyBuffer[VK_RIGHT] & 0xF0)
	{
		// scroll the map with the player velocity
		m_pPlayer->WalkRight();
		m_pMap->Scroll(DIRECTION::DIR_RIGHT);
	}
}

/**
 * Animates the objects we currently have loaded.
 */
void CGameApp::AnimateObjects()
{
	if (m_pPlayer->IsAlive())
	{
		ExpiredPredicate expiredPred;
		m_vBees.erase(std::remove_if(m_vBees.begin(), m_vBees.end(), expiredPred), m_vBees.end());

		float dt = m_Timer.GetTimeElapsed();

		m_pPlayer->Update(dt);
		m_pMap->Update(dt);

		UpdateFunctor updateFn(dt);
		std::for_each(m_vBees.begin(), m_vBees.end(), updateFn);
		bee->Update(dt);
	}
}

/**
 * Draws the game objects
 */
void CGameApp::DrawObjects()
{
	m_pBBuffer->reset();

	m_pBackground->Draw();

	// nu cred ca e nevoie de asta
	//HDC hdc = m_pBBuffer->getDC();

	m_pScore->setScore(m_pPlayer->m_score);
	m_pScore->drawScore();

	m_pPlayer->Draw();

	m_pMap->Draw();

	DrawHUD();

	DrawFunctor drawFn;
	std::for_each(m_vBees.begin(), m_vBees.end(), drawFn);

	bee->Draw();
	
	m_pBBuffer->present();
}


/**
 * Detects and handles collision
 */
void CGameApp::CollisionDetection()
{
	m_pPlayer->myCollisionSide = CS_None;
	/*
	// collision detection with the main frame
	for (auto it = m_vGameObjects.begin(); it != m_vGameObjects.end(); ++it)
	{
	CGameObject * pGameObj = it->get();
	Vec2 pos = pGameObj->myPosition;

	pGameObj->myCollisionSide = CS_None;

	if ((pGameObj->myCollisionMask & CF_Screen) == 0) {
	continue;
	}
	if (pGameObj->myBodyIsStatic) {
	continue;
	}

	int dx = (int)pos.x - pGameObj->GetWidth() / 2;
	if (dx < 0)
	{
	pGameObj->myCollisionSide |= CS_Left;
	}

	dx = (int)pos.x - (m_nViewWidth - pGameObj->GetWidth() / 2);
	if (dx > 0)
	{
	pGameObj->myCollisionSide |= CS_Right;
	}

	int dy = (int)pos.y - pGameObj->GetHeight() / 2;
	if (dy < 0)
	{
	pGameObj->myCollisionSide |= CS_Top;
	}

	dy = (int)pos.y - (m_nViewHeight - pGameObj->GetHeight() / 2);
	if (dy > 0)
	{
	pGameObj->myCollisionSide |= CS_Bottom;
	}
	}
	*/

	CRectangle plR = m_pPlayer->GetRectangle();

	if (plR.IntersectsRect(bee->GetRectangle())) {
		if (bee->checkActive())
		{
			bee->m_bIsAlive = false;
		}
	}

	//check collision between player and bees
	for (auto it = m_vBees.begin(); it != m_vBees.end(); ++it)
	{
		Bee* bee_tmp = dynamic_cast<Bee*>(it->get());
		if (!bee_tmp->checkActive())
		{
			continue;
		}
		CRectangle rect = bee_tmp->GetRectangle();
		if (rect.IntersectsRect(plR))
		{
			bee_tmp->m_bIsAlive = false;
		}
	}

	Vec2 pos;

	vector<CRectangle> rect = m_pMap->GetSurroundingTiles(plR, "walls");

	for (CRectangle r : rect)
	{
		if (plR.IntersectsRect(r))
		{
			m_pPlayer->myCollisionMask |= CollisionFlag::CF_Wall;
			CRectangle intersRect = plR.GetIntersectRect(r);
			// Oy collision
			if (intersRect.width >= intersRect.height)
			{
				if (plR.IsAbove(r))
				{
					if (intersRect.width > 5)
					{
						m_pPlayer->myCollisionSide |= CollisionSide::CS_Bottom;
						pos.y = r.y;
					}
				}
				else
				{
					if (intersRect.width > 5)
					{
						m_pPlayer->myCollisionSide |= CollisionSide::CS_Top;
						pos.y = r.y + r.height;
					}
				}
				continue;
			}
			//Ox collision

			if (plR.IsLeft(r))
			{
				m_pPlayer->myCollisionSide |= CollisionSide::CS_Right;
				m_pMap->myCollisionSide |= CollisionSide::CS_Right;
			}
			else
			{
				m_pMap->myCollisionSide |= CollisionSide::CS_Left;
				m_pPlayer->myCollisionSide |= CollisionSide::CS_Left;
			}
		}
	}
	m_pPlayer->m_prevPosition = pos;

	// detect collison with pickup objects
	rect = m_pMap->GetSurroundingTiles(plR, "pickup");
	for (CRectangle r : rect)
	{
		if (plR.IntersectsRect(r))
		{
			m_pPlayer->myCollisionMask |= CollisionFlag::CF_Pickup;
			m_pMap->RemoveTile(CPoint(r.x + r.width / 2, r.y + r.height / 2), "pickup");
		}
	}

}

void CGameApp::DoGameLogic()
{
	if (!m_pPlayer->IsAlive())
	{
		int msgboxID = MessageBox(
			NULL,
			"Game Over",
			"",
			MB_ICONEXCLAMATION | MB_OK
			);

		if (msgboxID == IDOK)
		{
			PostQuitMessage(0);
		}

	}
}

void CGameApp::DrawHUD()
{
	static TCHAR TitleBuffer[255];
	int life = 0;
	if (m_pPlayer)
	{
		life = m_pPlayer->m_life;
	}
	sprintf_s(TitleBuffer, _T("Honey Collector \t \t Life: %d"), life);
	SetWindowText(m_hWnd, TitleBuffer);
}