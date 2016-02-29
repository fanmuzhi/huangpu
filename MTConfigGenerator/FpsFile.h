#pragma once

#include <string>
#include "stdint.h"
#include <msclr/marshal_cppstd.h>

using std::string;
using namespace System::Xml;
using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class FpsFile
{
public:
	FpsFile(const string& sDescription = "", const string& sXmlTag = "", const string& sExstention = "")
		:m_sDescription(sDescription)
		,m_sExtension(sExstention)
		,m_sXmlTag(sXmlTag)
		,m_nNumBytes(0)
		,m_pBuf(NULL)
	{
	}

	~FpsFile()
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
	}

	FpsFile::FpsFile( FpsFile& rhs )
	{
		*this = rhs;
	}

	uint8_t*	GetBufPtr()						{return m_pBuf;}
	uint8_t		GetData(int idx) const			{return m_pBuf[idx];}
	void		SetXmlTag(string& s)			{m_sXmlTag = s;}
	string		GetXmlTag() const				{return m_sXmlTag;}
	void		SetDescription(string& s)		{m_sDescription = s;}
	string		GetDescription() const			{return m_sDescription;}
	void		SetName(const string& s)		{m_sName = s;}
	string		GetName() const					{return m_sName;}
	void		SetVersion(const string& s)		{m_sVersion = s;}
	string		GetVersion() const				{return m_sVersion;}
	void		SetExtension(const string& s)	{m_sExtension = s;}
	string		GetExtension() const			{return m_sExtension;}
	void		SetNumBytes(int n)				{m_nNumBytes = n;}
	int			GetNumBytes() const				{return m_nNumBytes;}

	FpsFile& operator=(FpsFile& rhs)
	{
		m_sDescription	= rhs.GetDescription();
		m_sName			= rhs.GetName();
		m_sVersion		= rhs.GetVersion();
		m_sExtension	= rhs.GetExtension();
		m_sXmlTag		= rhs.GetXmlTag();
		m_nNumBytes		= rhs.GetNumBytes();
		m_pBuf = new uint8_t[m_nNumBytes];
		memcpy(m_pBuf, rhs.GetBufPtr(), m_nNumBytes);

		return *this;
	}

	bool operator!=(const FpsFile& rhs) const
	{
		return !(*this == rhs);
	}

	bool operator==(const FpsFile& rhs) const
	{
		bool		bEqual = true;

		if (m_sVersion != rhs.GetVersion())
			bEqual = false;

		if (m_nNumBytes == rhs.GetNumBytes())
		{
			for (int i=0; i<m_nNumBytes; i++)
			{
				if (m_pBuf[i] != rhs.GetData(i))
					bEqual = false;
			}
		}
		else
			bEqual = false;

		return bEqual;
	}

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	virtual void NewDstBuf(int nNumBytes)
	{
		delete[] m_pBuf;
		m_pBuf = new uint8_t[nNumBytes];
	}

	virtual bool ProcessFpsFile(const string& sFileName) = 0;

protected:
	string				m_sDescription;
	string				m_sName;
	string				m_sVersion;
	string				m_sExtension;
	string				m_sXmlTag;
	int					m_nNumBytes;
	uint8_t*			m_pBuf;

private:

};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class PatchFile : public FpsFile
{
public:
	PatchFile(const string& sDescription = "", const string& sXmlTag = "", const string& sExstention = "")
		:FpsFile(sDescription, sXmlTag, sExstention)
	{
	}

	~PatchFile()
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	virtual bool ProcessFpsFile(const string& sFileName)
	{
		bool	bSuccess = true;

		//Get the name of the file without the path.
		SetName(sFileName.substr(sFileName.find_last_of("/\\")+1));
		SetVersion("");

		//Open the specified file.
		try
		{
			String^ fileName = gcnew String(sFileName.c_str());
			FileStream^ fs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read, FileShare::None);

			//Try to get the version number.
			fs->Seek(0,  SeekOrigin::Begin);
			StreamReader^ sr = gcnew StreamReader(fs);
			String^ in = sr->ReadLine();
			while (in != nullptr)
			{
				int idx = in->IndexOf("FirmwareVersion = ");
				if (idx != -1)
				{
					string stemp = msclr::interop::marshal_as< std::string >(in->Substring(idx + sizeof("FirmwareVersion = ") - 1));
					stemp = stemp.substr(0, stemp.find("*")-1);
					SetVersion(stemp);
				}
				in = sr->ReadLine();
			}

			//Try to get the patch level.
			fs->Seek(0,  SeekOrigin::Begin);
			sr = gcnew StreamReader(fs);
			in = sr->ReadLine();
			while (in != nullptr)
			{
				int idx = in->IndexOf("PatchLevel = ");
				if (idx != -1)
				{
					string stemp = msclr::interop::marshal_as< std::string >(in->Substring(idx + sizeof("PatchLevel = ") - 1));
					SetVersion(GetVersion() + "/" + stemp);
				}
				in = sr->ReadLine();
			}

			fs->Seek(0,  SeekOrigin::Begin);
			BinaryReader^ br = gcnew BinaryReader(fs);

			//Find the first instance of 0x00, 0x1A. The first patch byte follows
			//this 0x00, 0x1A.
			unsigned char chr1 = 0xFF, chr2 = 0xFF;
			int idx1stPatchByte = 0;
			while ((idx1stPatchByte < br->BaseStream->Length) && ((chr1 != 0x00) || (chr2 != 0x1A)))
			{
				chr1 = chr2;
				chr2 = br->ReadByte();
				idx1stPatchByte++;
			}

			//If we did not find the 0x00, 0x1A.
			if (idx1stPatchByte >= br->BaseStream->Length)
			{
				bSuccess = false;
			}
			else
			{
				//Copy all data found after the 0x00, 0x1A into given buffer.
				NewDstBuf((int)br->BaseStream->Length);
				for (int i=0; i<(int)(br->BaseStream->Length - idx1stPatchByte); i++)
					m_pBuf[i] = br->ReadByte();
				SetNumBytes((int)(br->BaseStream->Length - idx1stPatchByte));
			}
			fs->Close( );
		}
		catch (Exception^ e)
		{
			MessageBox::Show(e->Message, "Process Patch Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			bSuccess = false;
		}

		return bSuccess;
	}
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class BinFile : public FpsFile
{
public:
	BinFile(const string& sDescription = "", const string& sXmlTag = "", const string& sExstention = "")
		:FpsFile(sDescription, sXmlTag, sExstention)
	{
	}

	~BinFile()
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	virtual bool ProcessFpsFile(const string& sFileName)
	{
		bool	bSuccess = true;

		//Get the name of the file without the path.
		SetName(sFileName.substr(sFileName.find_last_of("/\\")+1));

		//Open the specified file.
		try
		{
			String^ fileName = gcnew String(sFileName.c_str());
			FileStream^ fs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read, FileShare::None);
			fs->Seek(0,  SeekOrigin::Begin);
			BinaryReader^ br = gcnew BinaryReader(fs);

			//Copy all file data into given buffer.
			NewDstBuf((int)br->BaseStream->Length);
			for (int i=0; i<(int)br->BaseStream->Length; i++)
				m_pBuf[i] = br->ReadByte();
			m_nNumBytes = (int)br->BaseStream->Length;

			fs->Close( );
		}
		catch (Exception^ e)
		{
			MessageBox::Show(e->Message, "Process BIN file error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			bSuccess = false;
		}

		return bSuccess;
	}
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CmdFile : public FpsFile
{
public:
	CmdFile(const string& sDescription = "", const string& sXmlTag = "", const string& sExstention = "")
		:FpsFile(sDescription, sXmlTag, sExstention)
	{
	}

	~CmdFile()
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	virtual bool ProcessFpsFile(const string& sFileName)
	{
		bool	bSuccess = true;

		//Get the name of the file without the path.
		SetName(sFileName.substr(sFileName.find_last_of("/\\")+1));

		//Open the specified file.
		try
		{
			String^ fileName = gcnew String(sFileName.c_str());
			FileStream^ fs = gcnew FileStream(fileName, FileMode::Open, FileAccess::Read, FileShare::None);
			fs->Seek(0,  SeekOrigin::Begin);
			BinaryReader^ br = gcnew BinaryReader(fs);

			//Copy all file data into given buffer.
			NewDstBuf((int)br->BaseStream->Length);
			for (int i=0; i<(int)br->BaseStream->Length; i++)
				m_pBuf[i] = br->ReadByte();
			m_nNumBytes = (int)br->BaseStream->Length;

			fs->Close( );
		}
		catch (Exception^ e)
		{
			MessageBox::Show(e->Message, "Process CMD file error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			bSuccess = false;
		}

		return bSuccess;
	}
};