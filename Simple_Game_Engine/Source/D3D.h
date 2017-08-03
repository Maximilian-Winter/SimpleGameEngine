/*
D3D
Class to initialize directx 11 and handle render stuff.
Author 
*/
#ifndef _D3D_H_
#define _D3D_H_


//Includes
#include <d3d11.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <d2d1.h>
#include <dwrite.h>
#include <dxgi.h>
#include <time.h>
#include <string>
#include <wincodec.h>

#include "DDSUtil\ScreenGrab\ScreenGrab.h"


class D3D
{
public:
	D3D();
	D3D(const D3D&);
	~D3D();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	ID2D1RenderTarget* GetD2DRenderTarget();

	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetWorldMatrix();
	DirectX::XMMATRIX GetOrthoMatrix();

	void GetVideoCardInfo(char*, int&);
	
	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void TurnStencilStateGreaterEqualOn();

	void TurnStencilStateLessEqualOn();
	void TurnStencilStateLessOn();

	void TurnAdditiveAlphaBlendingOn();
	void TurnAdditiveAlphaBlendingOff();

	void TurnOnFrontFaceCWCulling();
	void TurnOnBackFaceCWCulling();

	void TurnOnFrontFaceCCWCulling();
	void TurnOnBackFaceCCWCulling();

	void TurnOffCulling();

	void SetBackBufferToRenderTarget();

	void BindBackBufferToPipeline();

	void SetDepthStencilView(ID3D11DepthStencilView*);

	void SaveRenderTargetToFile();

	void ResetViewport();

private:
	
	std::wstring StringToWString(const std::string& s);

	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];

	IDXGISwapChain* m_swapChain;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	ID3D11RenderTargetView* m_renderTargetView;
	ID2D1RenderTarget* m_d2dRenderTarget;
	ID3D11Texture2D* m_depthStencilBuffer;

	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11DepthStencilState* m_depthStencilStateLess;
	ID3D11DepthStencilState* m_depthStencilStateLessEqual;
	ID3D11DepthStencilState* m_depthStencilStateGreaterEqual;

	ID3D11DepthStencilView* m_depthStencilView;

	ID3D11RasterizerState* m_rasterStateCullBackCW;
	ID3D11RasterizerState* m_rasterStateCullFrontCW;

	ID3D11RasterizerState* m_rasterStateCullBackCCW;
	ID3D11RasterizerState* m_rasterStateCullFrontCCW;

	ID3D11RasterizerState* m_rasterStateNoCulling;

	ID3D11BlendState* m_additiveAlphaBlendingState;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	ID3D11BlendState* m_oldBlendState;
	ID3D11DepthStencilState* m_oldDepthStencilState;
	ID3D11RasterizerState* m_oldRasterState;

	DirectX::XMFLOAT4X4 m_projectionMatrix;
	DirectX::XMFLOAT4X4 m_worldMatrix;
	DirectX::XMFLOAT4X4 m_orthoMatrix;

	D3D11_VIEWPORT m_viewport;
};

#endif