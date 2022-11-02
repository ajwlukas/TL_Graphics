#include "pch_dx_11.h"

#include "DXTKFont.h"
#include <tchar.h>

//#include <DirectXMath.h> // XMFLOAT
#include "DXTK\SimpleMath.h"

DXTKFont::DXTKFont()
	: m_pSpriteBatch(nullptr), m_pSpriteFont(m_pSpriteFont), m_RasterizerState{}, m_DepthStencilState{}
{

}

DXTKFont::~DXTKFont()
{
	SAFE_DELETE(m_pSpriteFont);
	SAFE_DELETE(m_pSpriteBatch);

	font.clear();
}
#include "PathDefine.h"
void DXTKFont::Create()
{
	//ID3D11DeviceContext* pDC = nullptr;
	//pDevice->GetImmediateContext(&pDC);

	DirectX::SpriteFont* temp_font;

	m_pSpriteBatch = new DirectX::SpriteBatch(DC);

	wstring path;

	TCHAR* _fileName;

	//==============================================================================
	// Play Pretend 25
	//==============================================================================
	path = FONTPATH(L"play_pretend_25.spritefont");
	_fileName = (TCHAR*)path.c_str();
	temp_font = new DirectX::SpriteFont(DEVICE, _fileName);
	temp_font->SetLineSpacing(14.0f);

	m_pSpriteFont = temp_font;	// �⺻ ��Ʈ�� ����
	font.insert(make_pair(fontType::playpretend_25, temp_font));
	//==============================================================================
	// Play Pretend 20
	//==============================================================================
	path = FONTPATH(L"play_pretend_20.spritefont");
	_fileName = (TCHAR*)path.c_str();	
	temp_font = new DirectX::SpriteFont(DEVICE, _fileName);
	temp_font->SetLineSpacing(14.0f);

	font.insert(make_pair(fontType::playpretend_20, temp_font));
	//==============================================================================
	// Square Font_17
	//==============================================================================
	path = FONTPATH(L"suare_font_17.spritefont");
	_fileName = (TCHAR*)path.c_str();
	temp_font = new DirectX::SpriteFont(DEVICE, _fileName);
	temp_font->SetLineSpacing(14.0f);

	font.insert(make_pair(fontType::squarefont_17, temp_font));
	//==============================================================================
	// Square Font_21
	//==============================================================================
	path = FONTPATH(L"suare_font_21.spritefont");
	_fileName = (TCHAR*)path.c_str();
	temp_font = new DirectX::SpriteFont(DEVICE, _fileName);
	temp_font->SetLineSpacing(14.0f);

	font.insert(make_pair(fontType::squarefont_21, temp_font));
	//==============================================================================
	// Square Font_24
	//==============================================================================
	path = FONTPATH(L"suare_font_24.spritefont");
	_fileName = (TCHAR*)path.c_str();
	temp_font = new DirectX::SpriteFont(DEVICE, _fileName);
	temp_font->SetLineSpacing(14.0f);

	font.insert(make_pair(fontType::squarefont_24, temp_font));
	//==============================================================================
	// Square Font_29
	//==============================================================================
	path = FONTPATH(L"suare_font_29.spritefont");
	_fileName = (TCHAR*)path.c_str();
	temp_font = new DirectX::SpriteFont(DEVICE, _fileName);
	temp_font->SetLineSpacing(14.0f);
	
	font.insert(make_pair(fontType::squarefont_29, temp_font));

	//==============================================================================
	// esamanru_Light_50
	//==============================================================================
	path = FONTPATH(L"esamanru_Light_50.spritefont");
	_fileName = (TCHAR*)path.c_str();
	temp_font = new DirectX::SpriteFont(DEVICE, _fileName);
	temp_font->SetLineSpacing(14.0f);
	
	font.insert(make_pair(fontType::esamanru_Light_50, temp_font));
	
	//==============================================================================
	// esamanru_Light_30
	//==============================================================================
	path = FONTPATH(L"esamanru_Light_30.spritefont");
	_fileName = (TCHAR*)path.c_str();
	temp_font = new DirectX::SpriteFont(DEVICE, _fileName);
	temp_font->SetLineSpacing(14.0f);
	
	font.insert(make_pair(fontType::esamanru_Light_30, temp_font));

	//SAFE_RELEASE(pDC);

	RESOURCES->rasterStates->GetDefault(m_RasterizerState);
	RESOURCES->depthStencilStates->GetDefault(m_DepthStencilState);
	RESOURCES->blendStates->GetDefault(m_BlendState);
}

void DXTKFont::DrawTest()
{
	//m_pSpriteBatch->Begin();
	//m_pSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, m_BlendState, nullptr, m_DepthStencilState, m_RasterizerState);
	//m_pSpriteFont->DrawString(m_pSpriteBatch, L"Hello, world! �ѱ��� �ȵǳ�? �c�氢�� ���� �ݶ� �z�z", DirectX::XMFLOAT2(10.f, 10.f));
	//m_pSpriteBatch->End();
}

void DXTKFont::DrawTextColor(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text, int scaleamount,...)
{
	TCHAR _buffer[1024] = L"";
	va_list vl;
	va_start(vl, text);
	_vstprintf(_buffer, 1024, text, vl);
	va_end(vl);

	// SpriteBatch�� ���� ���� ������Ʈ�� ������ �� �ִ�.
	// ������, �׳� Begin�� �ϸ� �������Ľǹ��� �ɼ��� D3D11_DEPTH_WRITE_MASK_ZERO�� �Ǵ� ��. DSS�� �ٷ��� �ʴ� ���ݿ��� ������ �� �� �ִ�.
	// �Ʒ�ó�� ���⿡ �ɼ��� ALL�� �־��༭ ZERO�� �Ǵ� ���� ���� ���� �ְ�, �ٸ� �� ������Ʈ�� �׸� �� ����������Ʈ�� ���� �� �� ���� �ִ�.
	// DX12���� ����������Ʈ���� �׷��� �������� �͵� ���ش� ����. ���� ���� �ȵ������..
	m_pSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, m_BlendState, nullptr, m_DepthStencilState, m_RasterizerState);
	m_pSpriteFont->DrawString(m_pSpriteBatch, _buffer, DirectX::XMFLOAT2((float)x, (float)y), DirectX::SimpleMath::Vector4(color),(0.0f),float2(0,0), scaleamount);
	m_pSpriteBatch->End();
}

void DXTKFont::DrawTextColor(int x, int y, DirectX::XMFLOAT4 color, fontType type, TCHAR* text, int scaleamount, ...)
{
	TCHAR _buffer[1024] = L"";
	va_list vl;
	va_start(vl, text);
	_vstprintf(_buffer, 1024, text, vl);
	va_end(vl);

	m_pSpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, m_BlendState, nullptr, m_DepthStencilState, m_RasterizerState);
	font[type]->DrawString(m_pSpriteBatch, _buffer, DirectX::XMFLOAT2((float)x, (float)y), DirectX::SimpleMath::Vector4(color), (0.0f), float2(0, 0), scaleamount);
	m_pSpriteBatch->End();
}

