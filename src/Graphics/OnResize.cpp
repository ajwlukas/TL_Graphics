#include "pch_dx_11.h"
#include "OnResize.h"

OnResize_Observer::OnResize_Observer()
{
	OnResizeNotice::Get().AddObserver(this); 
}

OnResize_Observer::~OnResize_Observer()
{
	OnResizeNotice::Get().SubObserver(this); 
}
