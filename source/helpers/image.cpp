#include "image.h"


// Thanks to ChatGPT for this function
Gdiplus::Bitmap* image_from_resource(HINSTANCE hInstance, int resourceID, const wchar_t* resourceType) {
    HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(resourceID), resourceType);
    if (!hResource) return nullptr;

    DWORD imageSize = SizeofResource(hInstance, hResource);
    const void* pResourceData = LockResource(LoadResource(hInstance, hResource));
    if (!pResourceData) return nullptr;

    HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (hBuffer) {
        void* pBuffer = GlobalLock(hBuffer);
        if (pBuffer) {
            CopyMemory(pBuffer, pResourceData, imageSize);
            GlobalUnlock(hBuffer);
        }

        IStream* pStream = nullptr;
        if (SUCCEEDED(CreateStreamOnHGlobal(hBuffer, TRUE, &pStream))) {
            return Gdiplus::Bitmap::FromStream(pStream);
        }

        GlobalFree(hBuffer);
    }

    return nullptr;
}
