#pragma 

#include "ShaderResource.h"

class TextureBuffer : public ShaderResource
{
public:
	TextureBuffer(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, UINT slot, TL_Graphics::E_SHADER_TYPE type, void* data, size_t dataSize);
	~TextureBuffer();

	virtual void Update(void* data, size_t dataSize) override;

private:
	Resource<ID3D11Buffer> buffer;
	ID3D11DeviceContext* dc;
};