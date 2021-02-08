#include "d3dApp.h"

class GameApp:public D3DApp
{
public:
    GameApp(HINSTANCE hInstance);
    ~GameApp();

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();

};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
	_In_ LPSTR cmdLine, _In_ int showCmd)
{
    UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(showCmd);
#if defined(DEBUG) | defined(_DEBUG)
	    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    GameApp theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

GameApp::~GameApp()
{
}
void GameApp::DrawScene()
{
    assert(md3dImmediateContext);
	assert(mSwapChain);
    md3dImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(),Colors::Blue);
    md3dImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(),D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,1.0f,0);
    HR(mSwapChain->Present(0, 0));
}
bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	return true;
}
void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{

}