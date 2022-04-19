﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <wrl/client.h>
#include <d3dcompiler.h>
#include <dcomp.h>
#include <dxgi.h>

#include <Rendering/D3D11/CubismNativeInclude_D3D11.hpp>
#include <CubismFramework.hpp>

#include "LAppAllocator.hpp"

class LAppView;
class LAppTextureManager;

/**
 * @brief   アプリケーションクラス。
 *   Cubism SDK の管理を行う。
 */
class LAppDelegate
{
public:

	/**
	 * @brief   クラスのインスタンス（シングルトン）を返す。<br>
	 *           インスタンスが生成されていない場合は内部でインスタンを生成する。
	 *
	 * @return  クラスのインスタンス
	 */
	static LAppDelegate* GetInstance();

	/**
	 * @brief   クラスのインスタンス（シングルトン）を解放する。
	 *
	 */
	static void ReleaseInstance();

	/**
	 * @brief   D3Dデバイスの取得
	 */
	static ID3D11Device* GetD3dDevice();

	/**
	 * @brief   D3Dデバイスコンテキストの取得
	 */
	static ID3D11DeviceContext* GetD3dContext();

	/**
	 * @brief   ウィンドウクライアント領域取得
	 */
	static void GetWindowRect(RECT& rect);

	/**
	 * @brief   ウィンドウクライアント領域の幅、高さ取得
	 */
	static void GetClientSize(int& rWidth, int& rHeight);

	/**
	 * @brief   APPに必要なものを初期化する。
	 */
	bool Initialize(HWND hwnd);

	/**
	 * @brief   解放処理
	 */
	void Release();

	/**
	* @brief   実行処理。
	*/
	void Run();

	/**
	 * @brief   View情報を取得する。
	 */
	LAppView* GetView() { return _view; }

	/**
	* @brief   アプリケーションを終了するかどうか。
	*/
	bool GetIsEnd() { return _isEnd; }

	/**
	* @brief   アプリケーションを終了する。
	*/
	void AppEnd() { _isEnd = true; }

	LAppTextureManager* GetTextureManager() { return _textureManager; }

	void Frame();

	/**
	 * @brief   フレーム最初の行動
	 */
	void StartFrame();

	/**
	 * @brief   フレーム最後の行動
	 */
	void EndFrame();

	/**
	 * @brief   スプライト描画用シェーダの作成と頂点宣言の作成を行う
	 */
	bool CreateShader();

	/**
	 * @brief   シェーダをコンテキストにセット
	 */
	void SetupShader();

	/**
	 * @brief   CreateShaderで確保したオブジェクトの開放
	 */
	void ReleaseShader();
	void SetMouse(float x, float y);
	void SetCaptured(bool captured);
	bool GetCaptured() { return _captured; }
private:

	enum DeviceStep
	{
		DeviceStep_None,      ///< 平常
		DeviceStep_Lost,      ///< ロスト
		DeviceStep_Size,      ///< サイズ変更
	};

	/**
	 * @brief   ウィンドウプロシージャ
	 */
	static LRESULT WINAPI MsgProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/**
	* @brief   コンストラクタ
	*/
	LAppDelegate();

	/**
	* @brief   デストラクタ
	*/
	~LAppDelegate();

	/**
	* @brief   Cubism SDK の初期化
	*/
	void InitializeCubism();

	/**
	 * @brief   レンダーターゲット作成内部関数
	 */
	bool CreateRenderTarget(UINT width, UINT height);

	/**
	 * @brief   デバイスのサイズ変更
	 */
	void ResizeDevice();

	LAppAllocator _cubismAllocator;              ///< Cubism SDK Allocator
	Csm::CubismFramework::Option _cubismOption;  ///< Cubism SDK Option

	LAppView* _view;                             ///< View情報
	bool _captured;                              ///< クリックしているか
	float _mouseX;                               ///< マウスX座標
	float _mouseY;                               ///< マウスY座標
	bool _isEnd;                                 ///< APP終了しているか
	LAppTextureManager* _textureManager;         ///< テクスチャマネージャー

	HWND                    _windowHandle;  ///< ウィンドウハンドル
	ID3D11Device* _device;        ///< D3Dデバイス
	ID3D11DeviceContext* _deviceContext; ///< D3D描画コンテキスト
	IDXGISwapChain1* _swapChain;     ///< スワップチェーン
	WNDCLASSEX              _windowClass;   ///< ウィンドウクラス
	DeviceStep              _deviceStep;    ///< デバイスサイズ変更などのステップ
	DXGI_SWAP_CHAIN_DESC1    _presentParameters; ///< プレゼントパラメータ

	ID3D11RenderTargetView* _renderTargetView;  ///< 描画ターゲットビュー
	ID3D11UnorderedAccessView* _uavPostBuffer;
	ID3D11Texture2D* _postBuffer;
	ID3D11Texture2D* _backBuffer;
	ID3D11Texture2D* _depthTexture;      ///< Zバッファ
	ID3D11DepthStencilView* _depthStencilView;  ///< Zバッファビュー
	ID3D11DepthStencilState* _depthState;       ///< スプライト描画用深度オブジェクト

	ID3D11RasterizerState* _rasterizer;    ///< スプライト描画用ラスタライザ
	ID3D11SamplerState* _samplerState;  ///< スプライト描画用サンプラーステート

	ID3D11VertexShader* _vertexShader;  ///< スプライト描画シェーダ
	ID3D11PixelShader* _pixelShader;   ///< スプライト描画シェーダ
	ID3D11BlendState* _blendState;    ///< スプライト描画用ブレンドステート
	ID3D11InputLayout* _vertexFormat;  ///< スプライト描画用型宣言

	//
	//ID3D11ComputeShader* _postProcessingShader;
	IDXGIDevice* _dxgiDevice;
	IDXGIFactory2* _dxgiFactory;
	// Direct Composition
	IDCompositionDevice* _dCompDevice;
	IDCompositionTarget* _composTarget;
	IDCompositionVisual* _dCompVisual;
};
