#pragma once

#include "d3dUtil.h"
#include "GameTimer.h"
#include <string>
#include <wrl/client.h>
#include <d3d11_1.h>

class D3DApp
{
public:
    D3DApp(HINSTANCE hInstance);
    virtual ~D3DApp();

    HINSTANCE AppInst() const;
    HWND MainWnd() const;
    float AspectRatio()const;

    int Run();

    // Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.

    virtual bool Init();
	virtual void OnResize(); 
	virtual void UpdateScene(float dt)=0;
	virtual void DrawScene()=0; 
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Convenience overrides for handling mouse input.
	virtual void OnMouseDown(WPARAM btnState, int x, int y){ }
	virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y){ }

protected:
    bool InitMainWindow();
    bool InitDirect3D();
    void CalculateFrameStats();

protected:
    HINSTANCE   mhAppInst;
    HWND        mhMainWnd;
    bool        mAppPaused;
	bool        mMinimized;
	bool        mMaximized;
	bool        mResizing; 
    UINT        m4xMsaaQuality;

    GameTimer mTimer;

    template<class T>
    using Comptr=Microsoft::WRL::ComPtr<T>;

    //檢測顯示卡狀態功能與分配資源
    Comptr<ID3D11Device> mPtrD3dDevice;
    // device使用環境以及設定，設置渲染管線模式以及生成device的渲染命令
    Comptr<ID3D11DeviceContext> mPtrD3dImmediateContext;
    // 後臺以及前台緩衝區的交換鍊
    Comptr<IDXGISwapChain> mPtrSwapChain;

    Comptr<ID3D11Device1> mPtrD3dDevice1;
    Comptr<ID3D11DeviceContext1> mPtrD3dImmediateContext1;
    Comptr<IDXGISwapChain1> mPtrSwapChain1;
 
    // 深度緩衝區
    Comptr<ID3D11Texture2D> mPtrDepthStencilBuffer;
    //渲染目標視圖 ps:視圖，在不同渲染管線下 同一資源有不同使用方式...
    Comptr<ID3D11RenderTargetView> mPtrRenderTargetView;
    //深度緩衝視圖
    Comptr<ID3D11DepthStencilView> mPtrDepthStencilView;
    
    //渲染的範圍
    D3D11_VIEWPORT mScreenViewport;

    //窗口屬性
    std::wstring mMainWndCaption;
    D3D_DRIVER_TYPE md3dDriverType;
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;


};