//Deferred buffer class handle the render target and dsv for the deferred pipeline
#ifndef _DEFERREDBUFFER_H_
#define _DEFERREDBUFFER_H_

//Globals
const int BUFFER_COUNT = 4;	//Number of rendertaget view arrays for depthstencil, albedo/roughnes, normals and f0/metalness


//Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <istream>
#include <vector>
#include <sstream>
#include <windows.h>
#include <wincodec.h>

#include "DDSUtil\ScreenGrab\ScreenGrab.h"

struct DeferredRenderData 
{
	ID3D11ShaderResourceView* DepthData;
	ID3D11ShaderResourceView* AlbedoRoughnesData;
	ID3D11ShaderResourceView* NormalData;
	ID3D11ShaderResourceView* F0Metalness;
};


class DeferredBuffer
{
public:


	DeferredBuffer();
	DeferredBuffer(const DeferredBuffer&);
	~DeferredBuffer();

	bool Initialize(ID3D11Device*, int, int, float, float);
	void Shutdown();

	void BindDeferredBufferToPipeline(ID3D11DeviceContext * deviceContext);
	void ClearDeferredRenderTargets(ID3D11DeviceContext*);
	void SaveDeferredRenderTargetsToFile(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetShaderResourceView(int);
	DeferredRenderData GetDeferredRenderData();

	ID3D11DepthStencilView* GetDeferredDSV();
	ID3D11DepthStencilView* GetDeferredReadOnlyDSV();

private:
	int m_textureWidth, m_textureHeight;
	
	ID3D11Texture2D* m_renderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView* m_renderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView* m_shaderResourceViewArray[BUFFER_COUNT];

	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11DepthStencilView* m_depthStencilViewReadOnly;

	D3D11_VIEWPORT m_viewport;

};

#endif
