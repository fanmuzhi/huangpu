#include "StdAfx.h"
#include <windows.h>
#include "AppInfo.h"

using std::string;

////////////////////////////////////////////////////////////////////////////////
//Statics
////////////////////////////////////////////////////////////////////////////////
AppInfo*	AppInfo::m_pInstance = NULL;
int			AppInfo::m_nRevMajor = 0;
int			AppInfo::m_nRevMinor = 0;
int			AppInfo::m_nRevBuild = 0;
int			AppInfo::m_nRevision = 0;



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
AppInfo::AppInfo()
{
	DWORD   verBufferSize;
	char    verBuffer[2048];

	wchar_t szVersionFile[MAX_PATH]; 
	GetModuleFileName(NULL, szVersionFile, MAX_PATH );
	std::wstring sFilename = szVersionFile;

	std::wstring sWideFN = std::wstring(sFilename.begin(), sFilename.end());

	//  Get the size of the version info block in the file
	verBufferSize = GetFileVersionInfoSize(sWideFN.c_str(), NULL);
	if(verBufferSize > 0 && verBufferSize <= sizeof(verBuffer))
	{
		//  get the version block from the file
		if(TRUE == GetFileVersionInfo(sWideFN.c_str(), NULL, verBufferSize, verBuffer))
		{
			UINT length;
			VS_FIXEDFILEINFO *verInfo = NULL;

			//  Query the version information for neutral language
			std::string sTemp("\\");
			std::wstring sWide = std::wstring(sTemp.begin(), sTemp.end());
			if(TRUE == VerQueryValue(
				verBuffer,
				sWide.c_str(),
				reinterpret_cast<LPVOID*>(&verInfo),
				&length))
			{
				//  Pull the version values.
				m_nRevMajor = HIWORD(verInfo->dwProductVersionMS);
				m_nRevMinor = LOWORD(verInfo->dwProductVersionMS);
				m_nRevBuild = HIWORD(verInfo->dwProductVersionLS);
				m_nRevision = LOWORD(verInfo->dwProductVersionLS);
			}
		}
	}
}
