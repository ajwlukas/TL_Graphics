#include "pch_dx_11.h"
#include "PostProcessor.h"

#include "Pipeline.h"

PostProcessor::PostProcessor(ID3D11DeviceContext* dc, Resources* resources, Pipeline* pipeline, OnResizeNotice* resizeNotice)
    :dc(dc)
    ,resources(resources)
    ,pipeline(pipeline)
{
    screenMesh = new ScreenMesh(dc, resources, pipeline);

    deferredRenderPass = new DeferredRenderPass(dc, resources, pipeline, resizeNotice);

    gridPass = new GridPass(dc, resources, pipeline, resizeNotice);

    downSamplerPass = new DownSamplerPass(dc, resources, pipeline, resizeNotice, 0.1f, 0.1f);

    colorGradingPass = new ColorGradingPass(dc, resources, pipeline, resizeNotice);

    finalPass = new FinalPass(dc, resources, pipeline, resizeNotice);
}

PostProcessor::~PostProcessor()
{
    SAFE_DELETE(finalPass);
    SAFE_DELETE(colorGradingPass);
    SAFE_DELETE(downSamplerPass);
    SAFE_DELETE(gridPass);
    SAFE_DELETE(deferredRenderPass);
    SAFE_DELETE(screenMesh);
}

void PostProcessor::Execute()
{
    screenMesh->Set();

    pipeline->SetDepthDisabled();

    if(control.doGrid)
      gridPass->Execute();

    deferredRenderPass->Execute();

    //downSamplerPass->Execute();

    if (control.doDownSample)
    colorGradingPass->Execute();

    finalPass->Execute();
}
