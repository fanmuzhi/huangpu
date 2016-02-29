#include "StdAfx.h"
#include "Config.h"
#include "Form1.h"
#include <msclr/marshal_cppstd.h>
#include "Crc32.h"
#include <stdio.h>

using namespace System;
using namespace System::Data;
using namespace System::Xml;
using namespace System::IO;
using namespace System::Windows::Forms;
using std::string;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Config::Config()
	:m_pwrVdd_mV(0)
	,m_pwrVio_mV(0)
	,m_pwrVled_mV(0)
	,m_pwrVddh_mV(0)
	,m_nNumRows(0)
	,m_nNumCols(0)
	,m_bWriteBootSecs(false)
{
	m_arTestSteps.push_back(StepAndArgs("InitializationStep",	""));
	m_arTestSteps.push_back(StepAndArgs("FinalizationStep",		""));

	m_arFpsFilePtrs.push_back(new PatchFile("Image acquisition patch", "ImageAcqPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("Print File", "PrintFile", "print"));
	m_arFpsFilePtrs.push_back(new PatchFile("Opens/shorts patch", "OpensShortsPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("Eng Mod Test Patch", "OtpReadWritePatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new BinFile("Firmware update", "FirmwareUpdate", "pbin"));
	m_arFpsFilePtrs.push_back(new PatchFile("WOF patch", "WofPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new CmdFile("WOF command 1", "WofCmd1", "cmd"));
	m_arFpsFilePtrs.push_back(new CmdFile("WOF command 2", "WofCmd2", "cmd"));
	m_arFpsFilePtrs.push_back(new CmdFile("WOF command 3", "WofCmd3", "cmd"));
	m_arFpsFilePtrs.push_back(new CmdFile("WOF command 4", "WofCmd4", "cmd"));
	m_arFpsFilePtrs.push_back(new PatchFile("Pixel Patch", "PixelPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("Wake on Variance", "WovarPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("Cache data RAM", "CacheDataRam", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("Cache tag RAM", "CacheTagRam", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("Cache instruction data RAM", "CachInstDataRam", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("Cache instruction tag RAM", "CachInstTagRam", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("SCM and main RAM", "ScmAndMainRam", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("AFE", "AfePatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("WOE sleep current", "WoeSleepCurrentPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("Layer ID Patch", "LayerIdPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("Flex ID Patch", "FlexIdPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("SCM WOF Patch", "ScmWofPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new CmdFile("Cmd1: SCM WOF Plot", "Cmd1ScmWofPlot", "cmd"));
	m_arFpsFilePtrs.push_back(new BinFile("Cmd2: SCM WOF Bin", "Cmd2ScmWofBin", "bin"));
	m_arFpsFilePtrs.push_back(new CmdFile("Cmd3: Sweep Tag", "Cmd3SweepTag", "cmd"));
	m_arFpsFilePtrs.push_back(new CmdFile("Cmd4: Sweep Tag", "Cmd4SweepTag", "cmd"));
	m_arFpsFilePtrs.push_back(new PatchFile("Production Mod Test Patch", "ProdOtpReadWritePatch", "xppatch"));
	m_arFpsFilePtrs.push_back(new CmdFile("Bulk Erase Cmd", "BulkEraseCmd", "cmd"));
	m_arFpsFilePtrs.push_back(new CmdFile("Program Cmd", "ProgramCmd", "cmd"));
	m_arFpsFilePtrs.push_back(new CmdFile("Read Cmd", "ReadCmd", "cmd"));
	m_arFpsFilePtrs.push_back(new PatchFile("VCEK_IV", "VCEK_IV", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("SPI Flash Patch", "SpiFlashRamPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("OTP Test (9-12) Patch", "SidenseOtpTestPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new PatchFile("Finger Detection Patch", "FingerDetectionPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new BinFile("WOF-LowPower Bin", "WofLowPowerBin", "bin"));
	m_arFpsFilePtrs.push_back(new PatchFile("WOF-LowPower patch", "WofLowPowerPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new BinFile("Security Management Engr Patch", "SecurityMgtEngrPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new BinFile("Security Management Prod Patch", "SecurityMgtProdPatch", "xppatch"));
	m_arFpsFilePtrs.push_back(new BinFile("Take Ownership Bin", "TakeOwnershipBin", "bin"));
	m_arFpsFilePtrs.push_back(new BinFile("Verify Flash Clear Engr Patch", "VerifyFlashClearEngrPatch", "xepatch"));
	m_arFpsFilePtrs.push_back(new BinFile("Verify Flash Clear Prod Patch", "VerifyFlashClearProdPatch", "xppatch"));
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Config::~Config()
{
	for (int i=0; i<(int)m_arFpsFilePtrs.size(); i++)
		delete m_arFpsFilePtrs[i];
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Config& Config::operator=(Config& rhs)
{
	m_sComment			= rhs.GetComment();
	m_sFileFullPath		= rhs.GetFileFullPath();
	m_sAutoCtrl			= rhs.GetAutoCtrlName();
	m_sDutType			= rhs.GetDutTypeName();
	m_sDutCtrl			= rhs.GetDutCtrlName();
	m_sDutCom			= rhs.GetDutComName();
	m_pwrVdd_mV			= rhs.GetPwrVdd();
	m_pwrVio_mV			= rhs.GetPwrVio();
	m_pwrVled_mV		= rhs.GetPwrVled();
	m_pwrVddh_mV		= rhs.GetPwrVddh();
	m_nNumRows			= rhs.GetNumRows();
	m_nNumCols			= rhs.GetNumCols();
	m_bWriteBootSecs	= rhs.GetWriteBootSecsFlg();

	for (int i=0; i<kNumBootSecs; i++)
		m_arBootSecStr[i] = rhs.GetBootSec(i);

	for (int i=0; i<(int)m_arFpsFilePtrs.size(); i++)
		*m_arFpsFilePtrs.at(i) = *(rhs.GetFpsFilePtr(i));

	m_arTestSteps.resize(rhs.GetNumSteps());
	for (int i=0; i<(int)m_arTestSteps.size(); i++)
		m_arTestSteps.at(i) = rhs.GetTestStep(i);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool Config::operator!=(Config& rhs) const
{
	return !(*this == rhs);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool Config::operator==(Config& rhs) const
{
	bool	bEqual = true;

	if ((this->m_sFileFullPath != rhs.GetFileFullPath()) || (this->m_sComment != rhs.GetComment()) ||
		(this->m_sAutoCtrl != rhs.GetAutoCtrlName()) || (this->m_sDutType != rhs.GetDutTypeName()) ||
		(this->m_sDutCtrl != rhs.GetDutCtrlName()) || (this->m_sDutCom != rhs.GetDutComName()))
	{
		bEqual = false;
	}

	if ((this->m_pwrVdd_mV != rhs.GetPwrVdd()) || (this->m_pwrVio_mV != rhs.GetPwrVio()) ||
		(this->m_pwrVled_mV != rhs.GetPwrVled()) || (this->m_pwrVddh_mV != rhs.GetPwrVddh()))
	{
		bEqual = false;
	}

	if ((this->m_nNumRows != rhs.GetNumRows()) || (this->m_nNumCols != rhs.GetNumCols()))
	{
		bEqual = false;
	}

	//Have any of the patch files changed.
	for (int i=0; i<(int)m_arFpsFilePtrs.size(); i++)
	{
		if (*m_arFpsFilePtrs[i] != *rhs.m_arFpsFilePtrs[i])
			bEqual = false;
	}

	//Has the test sequence changed.
	if ((m_arTestSteps.size()) == (rhs.GetNumSteps()))
	{
		for (int i=0; i<(int)m_arTestSteps.size(); i++)
		{
			if (m_arTestSteps[i] != rhs.GetTestStep(i))
				bEqual = false;
		}
	}
	else
		bEqual = false;

	//Has the Write Boot Sector parameters changed?
	if (m_bWriteBootSecs == rhs.GetWriteBootSecsFlg())
	{
		//If the boot sectors will be written, check if anything has been changed.
		if (m_bWriteBootSecs)
		{
			for (int i=0; i<kNumBootSecs; i++)
			{
				if (m_arBootSecStr[i] != rhs.GetBootSec(i))
					bEqual = false;
			}
		}
	}
	else
		bEqual = false;

	return bEqual;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void Config::SetNewValues()
{
	//Use the constructor to get the new values.
	Config* pTempConfig = new Config;
	*this = *pTempConfig;
	delete pTempConfig;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool Config::SaveXML(const string& sFileName, const string& sRevision)
{
	bool		bSuccess = true;
	string		sTemp;
	String^		str;
	String^		sFilename;
	//String^		str_Temp;

	//Open the specified file.
	try
	{
		XmlWriterSettings^ settings = gcnew XmlWriterSettings();
		settings->Indent = true;

		sFilename = gcnew String(sFileName.c_str());
		XmlWriter^ xmlWr = XmlTextWriter::Create(sFilename, settings);
		xmlWr->WriteStartDocument();
		xmlWr->WriteStartElement("TesterConfig");

		str = gcnew String(sRevision.c_str());
        xmlWr->WriteElementString( L"CreatedByRevision", str );

		str = gcnew String((GetComment()).c_str());
        xmlWr->WriteElementString( L"Comment", str );
		str = gcnew String((GetAutoCtrlName()).c_str());
        xmlWr->WriteElementString( L"AutoController", str );
		str = gcnew String((GetDutTypeName()).c_str());
        xmlWr->WriteElementString( L"DutType", str );
		str = gcnew String((GetDutCtrlName()).c_str());
        xmlWr->WriteElementString( L"DutController", str );
		str = gcnew String((GetDutComName()).c_str());
        xmlWr->WriteElementString( L"DutCom", str );
        xmlWr->WriteElementString( L"DutPwrVdd_mV",	Convert::ToString(GetPwrVdd()) );
        xmlWr->WriteElementString( L"DutPwrVio_mV", Convert::ToString(GetPwrVio()) );
        xmlWr->WriteElementString( L"DutPwrVled_mV", Convert::ToString(GetPwrVled()) );
        xmlWr->WriteElementString( L"DutPwrVddh_mV", Convert::ToString(GetPwrVddh()) );
        xmlWr->WriteElementString( L"NumRows", Convert::ToString(GetNumRows()) );
        xmlWr->WriteElementString( L"NumCols", Convert::ToString(GetNumCols()) );
        xmlWr->WriteElementString( L"WriteBootSectors", GetWriteBootSecsFlg() ? "1" : "0" );
		str = gcnew String((GetBootSec(0)).c_str());
        xmlWr->WriteElementString( L"BootSector0", str );
		str = gcnew String((GetBootSec(1)).c_str());
        xmlWr->WriteElementString( L"BootSector1", str );

		//Write the patch file names, xml tags and data.
		for (int i=0; i<(int)m_arFpsFilePtrs.size(); i++)
		{
			//If no patch data exists, make sure the patch name is blank.
			if (m_arFpsFilePtrs[i]->GetNumBytes() == 0)
			{
				str = gcnew String((m_arFpsFilePtrs[i]->GetXmlTag()).c_str());
				xmlWr->WriteElementString( str, L"" );
			}
			else
			{
				str = gcnew String((m_arFpsFilePtrs[i]->GetXmlTag()).c_str());
				xmlWr->WriteStartElement(str);
				str = gcnew String((m_arFpsFilePtrs[i]->GetName()).c_str());

				if ((gcnew String((m_arFpsFilePtrs[i]->GetVersion()).c_str()))->Trim() == "")
					str->Trim();
				else
					str = str->Trim() + " " + (gcnew String((m_arFpsFilePtrs[i]->GetVersion()).c_str()))->Trim();

				xmlWr->WriteAttributeString( L"Args", str );
				ConvertByteArrayToAsciiHex(m_arFpsFilePtrs[i]->GetBufPtr(), m_arFpsFilePtrs[i]->GetNumBytes(), sTemp);
				str = gcnew String(sTemp.c_str());
				xmlWr->WriteString( str );
				xmlWr->WriteEndElement();
			}
		}

		//Write the test sequence.
		StepAndArgs sAndA;
		xmlWr->WriteStartElement("TestSeq");
		for (int i=0; i<(int)m_arTestSteps.size(); i++)
		{
			sAndA = m_arTestSteps[i];
			xmlWr->WriteStartElement("Test" + (i+1));
			str = gcnew String((sAndA.m_sStepArgs).c_str());
			xmlWr->WriteAttributeString( L"Args", (str->TrimStart())->TrimEnd() );
			str = gcnew String((sAndA.m_sStepName).c_str());
			xmlWr->WriteString( str );
			xmlWr->WriteEndElement();		//End "Test(n)".
		}
		xmlWr->WriteEndElement();			//End "TestSeq".
		xmlWr->WriteEndElement();			//End "TesterConfig".

		xmlWr->WriteEndDocument();
		xmlWr->Flush();
		xmlWr->Close();

		//Put in CRC value. Always last element.
		uint32_t nCrc = CalculateCrc(sFileName);
		XmlDocument^ doc = gcnew XmlDocument();
		doc->Load(sFilename);
		XmlNode^ root = doc->DocumentElement;
		XmlElement^ elem = doc->CreateElement( "CRC" );
		elem->InnerText = System::Convert::ToString(nCrc);
		root->InsertAfter( elem, root->LastChild );
		doc->Save(sFilename);
	}
	catch (Exception^ e)
	{
		MessageBox::Show(e->Message, "XML Save Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		bSuccess = false;
	}

	return bSuccess;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
uint32_t Config::CalculateCrc(const string& sFileName)
{
	uint32_t	nCrc = 0;
	
	XmlReaderSettings^ settings = gcnew XmlReaderSettings();
	settings->CloseInput = true;
	String^ str = gcnew String(sFileName.c_str());
	XmlReader^ xmlRd = XmlTextReader::Create(str, settings);
	//XmlReader^ xmlRd;
	//xmlRd = XmlReader.Create(new FileStream(str, FileMode.Open), settings);

	//Parse through all nodes in this xml file.
	while (xmlRd->Read())
	{
		switch (xmlRd->NodeType)
		{
		case XmlNodeType::Element:
			//Do not include data in node labeled "CRC".
			if ((xmlRd->Name != "TesterConfig") && (xmlRd->Name != "TestSeq") && (xmlRd->Name != "CRC"))
			{
				//Include all attributes in CRC.
				if (xmlRd->AttributeCount > 0)
				{
					while (xmlRd->MoveToNextAttribute())
					{
						std::string sAttribute = msclr::interop::marshal_as< std::string >(xmlRd->Value);
						nCrc = CalculateStringCrc(nCrc, sAttribute);
					}
				}

				//Include element content in CRC.
				xmlRd->MoveToContent();
				std::string sContent = msclr::interop::marshal_as< std::string >(xmlRd->ReadElementContentAsString());
				nCrc = CalculateStringCrc(nCrc, sContent);
			}
			break;
		}
	}
	xmlRd->Close();

	return nCrc;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
uint32_t Config::CalculateStringCrc(uint32_t nStartCrc, const string& str)
{
	uint32_t	nCrc	= nStartCrc;
	int			nSize	= str.size();
	uint8_t*	pBuf;

	pBuf = new uint8_t[nSize];
	memset(pBuf, 0, nSize);

	for (int i=0; i<(int)str.size(); i++)
		pBuf[i] = str[i];

	nCrc = Crc32::slow_crc32(nCrc, pBuf, nSize);
	delete[] pBuf;

	return nCrc;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void Config::ConvertByteArrayToAsciiHex(uint8_t* pBytes, int numBytes, string& strDst)
{
	char	arHexNums[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	strDst = "";
	for (int i=0; i<numBytes; i++)
	{
		strDst += arHexNums[pBytes[i] >> 4];
		strDst += arHexNums[pBytes[i] &  0x0F];
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool Config::ReadXML(const string& sFileName)
{
	bool	bSuccess = true;
	bool	bTestSeqFnd = false;
	string	sInitializeArgs, sFinalizeArgs;

	uint32_t nCrc = CalculateCrc(sFileName);

	//Read the specified file.
	String^ str = gcnew String(sFileName.c_str());
	XmlReader^ rdr = XmlTextReader::Create(str);

	m_sFileFullPath.erase();
	m_sComment.erase();

	//Start with an empty test step list.
	m_arTestSteps.clear();

	//Start with an empty patch file list.
	for (int i=0; i<(int)m_arFpsFilePtrs.size(); i++)
	{
		m_arFpsFilePtrs[i]->SetName("None selected");
		m_arFpsFilePtrs[i]->SetVersion("");
		m_arFpsFilePtrs[i]->SetNumBytes(0);
	}

	//Default values for boot sector write option. In the boot sector
	//every other double word is 0xFFFFFFFF.
	SetWriteBootSecsFlg(false);
	m_arBootSecStr[0] = "";
	m_arBootSecStr[1] = "";
	for (int i=0; i<kBS0Size; i++)
	{
		m_arBootSecStr[0] += (((i / 4) % 2)) ? "FF" : "00";
		m_arBootSecStr[1] += (((i / 4) % 2)) ? "FF" : "00";
	}

	// Read nodes
	while (rdr->Read())
	{
		//Parse the data.
		switch (rdr->NodeType)
		{
		case XmlNodeType::XmlDeclaration:
			break;
		case XmlNodeType::Document:
			break;

		case XmlNodeType::Element:
			if (rdr->Name == L"Comment")
				m_sComment = msclr::interop::marshal_as< std::string >(rdr->ReadElementContentAsString());
			if (rdr->Name == L"AutoController")
				m_sAutoCtrl = msclr::interop::marshal_as< std::string >(rdr->ReadElementContentAsString());
			else if (rdr->Name == L"DutController")
				m_sDutCtrl = msclr::interop::marshal_as< std::string >(rdr->ReadElementContentAsString());
			else if (rdr->Name == L"DutType")
				m_sDutType = msclr::interop::marshal_as< std::string >(rdr->ReadElementContentAsString());
			else if (rdr->Name == L"DutCom")
				m_sDutCom = msclr::interop::marshal_as< std::string >(rdr->ReadElementContentAsString());
			else if (rdr->Name == L"DutPwrVdd_mV")
				m_pwrVdd_mV = rdr->ReadElementContentAsInt();
			else if (rdr->Name == L"DutPwrVio_mV")
				m_pwrVio_mV = rdr->ReadElementContentAsInt();
			else if (rdr->Name == L"DutPwrVled_mV")
				m_pwrVled_mV = rdr->ReadElementContentAsInt();
			else if (rdr->Name == L"DutPwrVddh_mV")
				m_pwrVddh_mV = rdr->ReadElementContentAsInt();
			else if (rdr->Name == L"NumRows")
				m_nNumRows = rdr->ReadElementContentAsInt();
			else if (rdr->Name == L"NumCols")
				m_nNumCols = rdr->ReadElementContentAsInt();
			else if (rdr->Name == L"CRC")
			{
				try
				{
					m_crc = (uint32_t)rdr->ReadElementContentAsLong();
					bSuccess = (m_crc == nCrc);
				}
				catch (...)
				{
					bSuccess = false;
				}
			}
			else if (rdr->Name == L"WriteBootSectors")
				SetWriteBootSecsFlg(rdr->ReadElementContentAsInt() != 0);
			else if ((rdr->Name == L"BootSector0") && GetWriteBootSecsFlg())
				SetBootSec(0, msclr::interop::marshal_as< std::string >(rdr->ReadElementContentAsString()));
			else if ((rdr->Name == L"BootSector1") && GetWriteBootSecsFlg())
				SetBootSec(1, msclr::interop::marshal_as< std::string >(rdr->ReadElementContentAsString()));
			else if (rdr->Name == L"TestSeq")
				bTestSeqFnd = true;
			else if ((bTestSeqFnd) && (rdr->Name != L""))
			{
				string sArgs = "";
				if (rdr->AttributeCount > 0)
				{
					rdr->MoveToAttribute( 0 );
					sArgs = msclr::interop::marshal_as< std::string >(rdr->Value);
					sArgs.erase(sArgs.find_last_not_of(" \n\r\t")+1);
					sArgs.erase(0, sArgs.find_first_not_of(" "));
					rdr->MoveToElement();
				}

				string sStepName = msclr::interop::marshal_as< std::string >(rdr->ReadElementContentAsString());
				if (sStepName == "InitializationStep")
					sInitializeArgs = sArgs;
				else if (sStepName == "FinalizationStep")
				{
					sFinalizeArgs = sArgs;
					bTestSeqFnd = false;
				}
				else
					m_arTestSteps.insert(m_arTestSteps.end(), StepAndArgs(sStepName, sArgs));
			}
			else
			{
				//Check if this is a patch file node.
				for (int i=0; i<(int)m_arFpsFilePtrs.size(); i++)
				{
					String^ sXml = gcnew String((m_arFpsFilePtrs[i]->GetXmlTag()).c_str());
					if (rdr->Name == sXml)
					{
						string	sName, sVersion;
						int		nNumBytes = 0;
						bSuccess = GetFpsFile(rdr, sName, sVersion, nNumBytes, *m_arFpsFilePtrs[i]);
						m_arFpsFilePtrs[i]->SetName(sName);
						m_arFpsFilePtrs[i]->SetVersion(sVersion);
						m_arFpsFilePtrs[i]->SetNumBytes(nNumBytes);
					}
				}
			}
			break;

		case XmlNodeType::EndElement:
			break;
		case XmlNodeType::Text:
			break;
		case XmlNodeType::Comment:
			break;
		case XmlNodeType::Whitespace:
			break;
		default:
			break;
		}
	}

	//Make sure the Initialization and Finalization steps are always present.
	m_arTestSteps.insert(m_arTestSteps.begin(), StepAndArgs("InitializationStep", sInitializeArgs));
	m_arTestSteps.insert(m_arTestSteps.end(), StepAndArgs("FinalizationStep", sFinalizeArgs));

	if (bSuccess)
	{
		SetFileFullPath(sFileName);
	}

	rdr->Close();
	return bSuccess;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool Config::GetFpsFile(XmlReader^ rdr, string& sName, string& sVersion, int& nSize, FpsFile& fpsFile)
{
	bool		bSuccess = true;
	string		sTemp;
	uint8_t*	pPatchBuf = NULL;

	sName = "";
	sVersion = "";
	nSize = 0;

	//Get the patch name and version from the attribute string.
	if (rdr->AttributeCount > 0)
	{
		rdr->MoveToAttribute( 0 );
		sTemp = msclr::interop::marshal_as< std::string >(rdr->Value);
		if (sTemp.find(" ") != -1)
		{
			sName = sTemp.substr(0, sTemp.find(" "));
			sVersion = sTemp.substr(sTemp.find(" "));
		}
		else
		{
			sName = sTemp;
			sVersion = "";
		}
		rdr->MoveToElement(); //Moves the reader back to the element node.
	}

	//Read in the patch data and size.
	string sPatch = msclr::interop::marshal_as< std::string >(rdr->ReadElementContentAsString());
	nSize = sPatch.size() / 2;
	fpsFile.NewDstBuf(nSize);
	pPatchBuf = fpsFile.GetBufPtr();
	for (int i=0; i<nSize; i++)
	{
		char *p;
		sTemp = sPatch[i*2];
		sTemp += sPatch[(i*2)+1];
		long n = strtol(sTemp.c_str(), &p, 16);
		pPatchBuf[i] = (uint8_t)n;
	}

	return bSuccess;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void Config::ClearFpsFile(int idx)
{
	m_arFpsFilePtrs[idx]->SetNumBytes(0);
	m_arFpsFilePtrs[idx]->SetVersion("");
	m_arFpsFilePtrs[idx]->SetName("");
}
