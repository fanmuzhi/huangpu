// snrtest.h

// #pragma once
// 
// using namespace System;
// 
// namespace snrtest {
// 
// 	public ref class Class1
// 	{
// 		// TODO: Add your methods for this class here.
// 	};
// }
extern "C" __declspec(dllexport) int __stdcall calchuaidian(unsigned char* pImage, int iWidth, int iHeight,int* num_of_bubble,float fGrav);
extern "C" __declspec(dllexport) float __stdcall testSNRUG(unsigned char* pWiteimg,unsigned char* pSpoofimg,int iWidth,int iHeight,int iNumofimg);
extern "C" __declspec(dllexport) float __stdcall testSNR(unsigned char* pImagewhite30, unsigned char* pImagespoof30,int width,int height, int iNumofimg,int *signal,float *noise,float fcrop);
extern "C" __declspec(dllexport) int __stdcall add(int x,int y);