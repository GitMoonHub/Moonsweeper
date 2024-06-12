#include "Win32_Helper.h"

float Win32_Helper::DPIScale::scale = 0.0f;

void Win32_Helper::DPIScale::Initialize(HWND hwnd)
{
    float dpi = GetDpiForWindow(hwnd);
    scale = dpi / 96.0f;
}

template <typename T>
D2D1_POINT_2F Win32_Helper::DPIScale::PixelsToDips(T x, T y)
{
    return D2D1::Point2F(static_cast<float>(x) / scale, static_cast<float>(y) / scale);
}