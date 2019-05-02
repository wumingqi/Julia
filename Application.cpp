#include "Pch.h"
#include "Application.h"


void Application::InitializeResources()
{
	//获取设备资源
	m_deviceResources = new DeviceResources(m_hWnd);
	m_deviceResources->CreateDeviceIndependentResources();
	m_deviceResources->CreateDeviceResources();
	m_d2dFactory = m_deviceResources->GetD2DFactory();
	m_d2dContext = m_deviceResources->GetD2DDeviceContext();

	//创建画笔
	m_d2dContext->CreateSolidColorBrush(ColorF(0xFF2244), &m_brush);

	//修改一下配色
	UINT  color = 0x000000;
	UINT delt = 0x0a0a0a;
	for (UINT i = 0; i < _countof(ColorTable); i++)
	{
		ColorTable[i] = color + delt*i + 0xFF001e8a;
	}
	m_c = { 0.3,0.0 };
	GenerateJulia();
}

void Application::Update()
{
	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(ColorF(0xFFFFFF));
	//开始绘制
	m_d2dContext->SetTransform(Matrix3x2F::Translation(
		floorf((int)(m_width - ImageWidth) / 2.f),
		floorf((int)(m_height - ImageHeight) / 2.f)
	));
	m_d2dContext->DrawBitmap(m_bitmap.Get());

	D2D1_RECT_F rc = {0,0,(float)ImageWidth,(float)ImageHeight};
	m_d2dContext->DrawRectangle(rc, m_brush.Get(), 2.f);

	m_d2dContext->EndDraw();
	m_swapChain->Present(1, 0);
}

void Application::GenerateJulia()
{
	auto wicFactory = m_deviceResources->GetWicImagingFactory();
	ComPtr<IWICBitmap> wicBitmap;
	wicFactory->CreateBitmap(
		ImageWidth,
		ImageHeight,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapCacheOnDemand,
		&wicBitmap);
	WICRect rcLock = { 0,0,ImageWidth,ImageHeight };
	ComPtr<IWICBitmapLock> lock;
	wicBitmap->Lock(&rcLock, WICBitmapLockFlags::WICBitmapLockWrite, &lock);

	UINT cbBufferSize = 0;
	UINT cbStride = 0;
	BYTE* pv = NULL;
	lock->GetStride(&cbStride);
	lock->GetDataPointer(&cbBufferSize, &pv);

	UINT* pData = reinterpret_cast<UINT*>(pv);
	
	//Complex c(-0.3128, 0.756);
	//Complex c(0.3128, 0.64);
	int iterationCount = 15;
	CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), m_hWnd, DialogProc);
	ShowWindow(m_hDlg, SW_SHOWDEFAULT);

	SendMessage(m_hProcess, PBM_SETRANGE32, 0, ImageWidth);
	TCHAR buf[64];
	float halfWidth = ImageWidth / 2.f, halfHeight = ImageHeight / 2.f;
	float ratio = ImageHeight / 3.f;
	for (INT x = 0; x < ImageWidth; x++)
	{
		for (INT y = 0; y < ImageHeight; y++, pData++)
		{
			Complex z;
			z.real = (ComplexType)(x- halfWidth) / ratio;
			z.image = (ComplexType)(y - halfHeight)/ ratio;
			auto dist = Julia::Iteration(z, m_c, iterationCount);
			auto color= Julia::MapColor(dist);
			*pData = color;
		}
		SendMessage(m_hProcess, PBM_SETPOS, x + 1, 0);
		wsprintf(buf, L"%d / %d行", x + 1, ImageWidth);
		SetWindowText(m_hStaticText, buf);
	}
	DestroyWindow(m_hDlg);
	lock.Reset();

	ComPtr<IWICBitmapFlipRotator> rotator,flip;
	wicFactory->CreateBitmapFlipRotator(&rotator);
	rotator->Initialize(wicBitmap.Get(), WICBitmapTransformRotate90);
	wicFactory->CreateBitmapFlipRotator(&flip);
	flip->Initialize(rotator.Get(), WICBitmapTransformFlipVertical);

	ThrowIfFailed(m_d2dContext->CreateBitmapFromWicBitmap(flip.Get(), &m_bitmap));
	SetText();
	return;
}
