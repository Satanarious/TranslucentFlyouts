#pragma once
#include "pch.h"

namespace TranslucentFlyoutsLib
{
	typedef enum _WINDOWCOMPOSITIONATTRIBUTE
	{
		WCA_UNDEFINED,
		WCA_NCRENDERING_ENABLED,
		WCA_NCRENDERING_POLICY,
		WCA_TRANSITIONS_FORCEDISABLED,
		WCA_ALLOW_NCPAINT,
		WCA_CAPTION_BUTTON_BOUNDS,
		WCA_NONCLIENT_RTL_LAYOUT,
		WCA_FORCE_ICONIC_REPRESENTATION,
		WCA_EXTENDED_FRAME_BOUNDS,
		WCA_HAS_ICONIC_BITMAP,
		WCA_THEME_ATTRIBUTES,
		WCA_NCRENDERING_EXILED,
		WCA_NCADORNMENTINFO,
		WCA_EXCLUDED_FROM_LIVEPREVIEW,
		WCA_VIDEO_OVERLAY_ACTIVE,
		WCA_FORCE_ACTIVEWINDOW_APPEARANCE,
		WCA_DISALLOW_PEEK,
		WCA_CLOAK,
		WCA_CLOAKED,
		WCA_ACCENT_POLICY,
		WCA_FREEZE_REPRESENTATION,
		WCA_EVER_UNCLOAKED,
		WCA_VISUAL_OWNER,
		WCA_HOLOGRAPHIC,
		WCA_EXCLUDED_FROM_DDA,
		WCA_PASSIVEUPDATEMODE,
		WCA_USEDARKMODECOLORS,
		WCA_CORNER_STYLE,
		WCA_PART_COLOR,
		WCA_DISABLE_MOVESIZE_FEEDBACK,
		WCA_LAST
	} WINDOWCOMPOSITIONATTRIBUTE;

	typedef struct _WINDOWCOMPOSITIONATTRIBUTEDATA
	{
		WINDOWCOMPOSITIONATTRIBUTE Attribute;
		PVOID pvData;
		SIZE_T cbData;
	} WINDOWCOMPOSITIONATTRIBUTEDATA;

	typedef enum _ACCENT_STATE
	{
		ACCENT_DISABLED,
		ACCENT_ENABLE_GRADIENT,
		ACCENT_ENABLE_TRANSPARENTGRADIENT,
		ACCENT_ENABLE_BLURBEHIND,
		ACCENT_ENABLE_ACRYLICBLURBEHIND,
		ACCENT_ENABLE_HOSTBACKDROP,
		ACCENT_INVALID_STATE
	} ACCENT_STATE;

	typedef enum _ACCENT_FLAG
	{
		ACCENT_NONE,
		ACCENT_ENABLE_BORDER_LEFT = 1 << 5,
		ACCENT_ENABLE_BORDER_TOP = 1 << 6,
		ACCENT_ENABLE_BORDER_RIGHT = 1 << 7,
		ACCENT_ENABLE_BORDER_BOTTOM = 1 << 8,
		ACCENT_ENABLE_BORDER = ACCENT_ENABLE_BORDER_LEFT | ACCENT_ENABLE_BORDER_TOP | ACCENT_ENABLE_BORDER_RIGHT | ACCENT_ENABLE_BORDER_BOTTOM,
		ACCENT_ENABLE_ALL = ACCENT_ENABLE_BORDER | ACCENT_ENABLE_LUMINOSITY
	} ACCENT_FLAG;

	typedef struct _ACCENT_POLICY
	{
		ACCENT_STATE AccentState;
		ACCENT_FLAG AccentFlags;
		DWORD dwGradientColor;
		DWORD dwAnimationId;
	} ACCENT_POLICY;

	typedef BOOL(WINAPI* pfnSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBUTEDATA*);

	static void DwmSetWindowTransparent(HWND hWnd, BOOL Enable)
	{
		DWM_BLURBEHIND bb = {DWM_BB_ENABLE | DWM_BB_BLURREGION | DWM_BB_TRANSITIONONMAXIMIZED, Enable, CreateRectRgn(0, 0, -1, -1), TRUE};
		DwmEnableBlurBehindWindow(hWnd, &bb);
		DeleteObject(bb.hRgnBlur);
	}

	static void SetWindowEffect(HWND hwnd, DWORD dwEffect, BOOL bBorderOption)
	{
		static const auto& SetWindowCompositionAttribute =
		    (pfnSetWindowCompositionAttribute)GetProcAddress(
		        GetModuleHandle(TEXT("User32")),
		        "SetWindowCompositionAttribute"
		    );

		ACCENT_POLICY policy = {static_cast<ACCENT_STATE>(dwEffect), bBorderOption ? ACCENT_ENABLE_ALL : ACCENT_ENABLE_LUMINOSITY, 0x01000000, 0};
		WINDOWCOMPOSITIONATTRIBUTEDATA data = {WCA_ACCENT_POLICY, &policy, sizeof(ACCENT_POLICY)};
		
		if (dwEffect == ACCENT_ENABLE_TRANSPARENTGRADIENT)
		{
			policy.AccentState = ACCENT_DISABLED;
			policy.dwAnimationId = 0;
			policy.dwGradientColor = 0;
			DwmSetWindowTransparent(hwnd, TRUE);
		}
		else
		{
			DwmSetWindowTransparent(hwnd, FALSE);
		}

		SetWindowCompositionAttribute(hwnd, &data);
	}

	static void RemoveAllEffects(HWND hWnd)
	{
		DWM_BLURBEHIND bb = { DWM_BB_ENABLE, FALSE, nullptr, FALSE};
		DwmEnableBlurBehindWindow(hWnd, &bb);
		static const auto& SetWindowCompositionAttribute =
			(pfnSetWindowCompositionAttribute)GetProcAddress(
				GetModuleHandle(TEXT("User32")),
				"SetWindowCompositionAttribute"
			);
		ACCENT_POLICY policy = { ACCENT_DISABLED, ACCENT_NONE, 0, 0};
		WINDOWCOMPOSITIONATTRIBUTEDATA data = { WCA_ACCENT_POLICY, &policy, sizeof(ACCENT_POLICY) };
		SetWindowCompositionAttribute(hWnd, &data);
	}
};