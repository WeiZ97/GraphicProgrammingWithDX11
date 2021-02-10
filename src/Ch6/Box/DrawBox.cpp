#pragma once

#include "d3dApp.h"

class GameApp:public D3DApp
{
public:
    struct VertexPosColor
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT4 color;
        static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
    };
    struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};
public:
    GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:
    bool InitEffect();
    bool InitResource();

private:
    Comptr<ID3D11InputLayout> mPtrVertexLayout;
    Comptr<ID3D11Buffer> mPtrVertexBuffer;
    Comptr<ID3D11VertexShader> mPtrVertexShader;	// 
	Comptr<ID3D11PixelShader> mPtrPixelShader;

    Comptr<ID3D11Buffer> mPtrIndexBuffer;
    Comptr<ID3D11Buffer> mPtrConstantBuffer;

    ConstantBuffer mCBuffer;
};

const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2]={
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

//main
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

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
    static float phi=0.0f,theta=0.0f;
    phi+=0.0001f,theta+=0.00015f;
    mCBuffer.world=DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationX(phi)*DirectX::XMMatrixRotationY(theta));

    D3D11_MAPPED_SUBRESOURCE mappedData;
    
    HR(mPtrD3dImmediateContext->Map(mPtrConstantBuffer.Get(),0,D3D11_MAP_WRITE_DISCARD,0,&mappedData));
    memcpy_s(mappedData.pData,sizeof(mCBuffer),&mCBuffer,sizeof(mCBuffer));
    mPtrD3dImmediateContext->Unmap(mPtrConstantBuffer.Get(),0);
}

void GameApp::DrawScene()
{
    assert(mPtrD3dImmediateContext);
    assert(mPtrSwapChain);

    mPtrD3dImmediateContext->ClearRenderTargetView(mPtrRenderTargetView.Get(),(const float*)&Colors::Black);
	mPtrD3dImmediateContext->ClearDepthStencilView(mPtrDepthStencilView.Get(),D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,1.0f,0);

	mPtrD3dImmediateContext->DrawIndexed(36,0,0);
	HR(mPtrSwapChain->Present(0,0));
}


bool GameApp::InitEffect()
{
    Comptr<ID3DBlob> blob;

    HR(CreateShaderFromFile(L"hlsl\\Cube_VS.cso",L"hlsl\\Cube_VS.hlsl","VS","vs_5_0",blob.ReleaseAndGetAddressOf()));
    HR(mPtrD3dDevice->CreateVertexShader(blob->GetBufferPointer(),blob->GetBufferSize(),nullptr,mPtrVertexShader.GetAddressOf()));

    HR(mPtrD3dDevice->CreateInputLayout(VertexPosColor::inputLayout,ARRAYSIZE(VertexPosColor::inputLayout),
        blob->GetBufferPointer(),blob->GetBufferSize(),mPtrVertexLayout.GetAddressOf() ) );

    HR(CreateShaderFromFile(L"hlsl\\Cube_PS.cso",L"hlsl\\Cube_PS.hlsl","PS","ps_5_0",blob.ReleaseAndGetAddressOf()));
    HR(mPtrD3dDevice->CreatePixelShader(blob->GetBufferPointer(),blob->GetBufferSize(),nullptr,mPtrPixelShader.GetAddressOf()));

    return true;
}
bool GameApp::InitResource()
{
    ///頂點緩衝創建
    VertexPosColor vertices[] =
    {
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
    };
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd,sizeof(vbd));
    vbd.Usage=D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth=sizeof(vertices);
    vbd.BindFlags=D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags=0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData,sizeof(InitData));
    InitData.pSysMem=vertices;
    HR(mPtrD3dDevice->CreateBuffer(&vbd,&InitData,mPtrVertexBuffer.GetAddressOf()));

    
    ///index緩衝創建
    DWORD indices[] = {
        // 正面
        0, 1, 2,
        2, 3, 0,
        // 左面
        4, 5, 1,
        1, 0, 4,
        // 顶面
        1, 5, 6,
        6, 2, 1,
        // 背面
        7, 6, 5,
        5, 4, 7,
        // 右面
        3, 2, 6,
        6, 7, 3,
        // 底面
        4, 0, 3,
        3, 7, 4
    };
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd,sizeof(ibd));
    ibd.Usage=D3D11_USAGE_IMMUTABLE;
    ibd.BindFlags=D3D11_BIND_INDEX_BUFFER;
    ibd.ByteWidth=sizeof(indices);
    ibd.CPUAccessFlags=0;

    InitData.pSysMem=indices;
    HR(mPtrD3dDevice->CreateBuffer(&ibd,&InitData,mPtrIndexBuffer.GetAddressOf()));
    

    ///常量緩衝創建
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd,sizeof(cbd));
    cbd.Usage=D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth=sizeof(ConstantBuffer);
    cbd.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
    HR(mPtrD3dDevice->CreateBuffer(&cbd,nullptr,mPtrConstantBuffer.GetAddressOf()));
    mCBuffer.world = DirectX::XMMatrixIdentity();

    mCBuffer.view=DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
    mCBuffer.proj=DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));

    UINT stride = sizeof(VertexPosColor);    
    UINT offset = 0;
    mPtrD3dImmediateContext->IASetVertexBuffers(0,1,mPtrVertexBuffer.GetAddressOf(),&stride,&offset);
    mPtrD3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mPtrD3dImmediateContext->IASetInputLayout(mPtrVertexLayout.Get());
    mPtrD3dImmediateContext->IASetIndexBuffer(mPtrIndexBuffer.Get(),DXGI_FORMAT_R32_UINT,0);
    
    mPtrD3dImmediateContext->VSSetShader(mPtrVertexShader.Get(),nullptr,0);
    mPtrD3dImmediateContext->VSSetConstantBuffers(0,1,mPtrConstantBuffer.GetAddressOf());
    
    mPtrD3dImmediateContext->PSSetShader(mPtrPixelShader.Get(),nullptr,0);

    return true;
}