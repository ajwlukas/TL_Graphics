#pragma once

#include <unordered_map>

#include "Resources.h"
#include "DXTK\SpriteFont.h"
#include "DXTK\SpriteBatch.h"

enum class fontType
{
	playpretend_25,
	playpretend_20,
	squarefont_17,
	squarefont_21,
	squarefont_24,
	squarefont_29,
	esamanru_Light_50,
	esamanru_Light_30
};

/// <summary>
/// DXTK�� �̿��� �ؽ�Ʈ ���
/// 
/// �ϼ����ۿ� �ȵȴٴµ�..
/// DirectWrite������ �����ؾ߰ڴ�.
/// 
/// 2021.03.04 LeHideOffice
/// </summary>
class DXTKFont
{
public:
	DXTKFont();
	~DXTKFont();

	void Create();

	void DrawTest();
	void DrawTextColor(int x, int y, DirectX::XMFLOAT4 color, TCHAR* text,int scaleamount = 1,...);
	void DrawTextColor(int x, int y, DirectX::XMFLOAT4 color,fontType type, TCHAR* text,int scaleamount = 1,...);

private:
	DirectX::SpriteBatch* m_pSpriteBatch;
	DirectX::SpriteFont* m_pSpriteFont;

	// ����� ��Ʈ��
	std::unordered_map<fontType, DirectX::SpriteFont*> font;

	// �׳� ����ϸ� ���� ������ �����.
	Resource<ID3D11RasterizerState> m_RasterizerState;
	Resource<ID3D11DepthStencilState> m_DepthStencilState;
	Resource<ID3D11BlendState> m_BlendState;
};

/*

�߰� �� ��
�ڰ�, ����, �� ���� ����� ��Ƽ���� �׸���

WriteFactory�� �̿��� â ��ȯ���� ����� ��Ʈ

*/