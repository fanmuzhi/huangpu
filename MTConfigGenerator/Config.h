#pragma once

#include <string>
#include <vector>
#include "FpsFile.h"
#include "stdint.h"

using namespace System::Xml;
using std::string;
using std::vector;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class StepAndArgs
{
public:
	StepAndArgs()
		:m_sStepName()
		,m_sStepArgs()
	{
	}

	StepAndArgs(const string& sName, const string& sArgs)
		:m_sStepName(sName)
		,m_sStepArgs(sArgs)
	{
	}

	bool operator==(const StepAndArgs& rhs) const
	{
		return !((this->m_sStepName != rhs.m_sStepName) || (this->m_sStepArgs != rhs.m_sStepArgs));
	}

	bool operator!=(const StepAndArgs& rhs) const
	{
		return !(*this == rhs);
	}

	void		SetStepName(string& s)	{m_sStepName = s;}
	string		GetStepName()			{return m_sStepName;}
	void		SetStepArgs(string& s)	{m_sStepArgs = s;}
	string		GetStepArgs()			{return m_sStepArgs;}

	string		m_sStepName;
	string		m_sStepArgs;
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Config
{
public:
	enum	{kNumBootSecs = 2, kBS0Size = 64, kBS1Size = 64};
	enum	{MAX_PATCH_SIZE = 20 * 1024};

	Config();
	~Config();

	void		SetNewValues();

	Config&		operator=(Config& rhs);
	bool		operator!=(Config& rhs) const;
	bool		operator==(Config& rhs) const;

	bool		SaveXML(const string& sFileName, const string& sRevision);
	bool		ReadXML(const string& sFileName);
	void		ConvertByteArrayToAsciiHex(uint8_t* pBytes, int numBytes, string& strDst);
	uint32_t	CalculateCrc(const string& sXmlFilename);
	uint32_t	CalculateStringCrc(uint32_t nStartCrc, const string& str);
	bool		GetFpsFile(XmlReader^ rdr, string& sName, string& sVersion, int& nSize, FpsFile& file);

	void		SetFileFullPath(const string& s)	{m_sFileFullPath = s;}
	string		GetFileFullPath() const				{return m_sFileFullPath;}
	string		GetFilePath() const					{return m_sFileFullPath.substr(0, m_sFileFullPath.rfind("\\")+1);}
	string		GetFileName() const					{return m_sFileFullPath.substr(m_sFileFullPath.rfind("\\")+1);}

	int			GetNumFpsFiles() const				{return m_arFpsFilePtrs.size();}
	FpsFile*	GetFpsFilePtr(int idx)				{return m_arFpsFilePtrs[idx];}
	void		SetNumFpsFileBytes(int idx, int n)	{(m_arFpsFilePtrs[idx])->SetNumBytes(n);}
	void		SetFpsFileName(int idx, string& s)	{(m_arFpsFilePtrs[idx])->SetName(s);}
	void		ClearFpsFile(int idx);

	int			GetNumSteps() const								{return m_arTestSteps.size();}
	void		SetTestStep(int idx, StepAndArgs& s)			{m_arTestSteps[idx] = s;}
	StepAndArgs	GetTestStep(int idx) const						{return m_arTestSteps[idx];}
	void		InsertTestStepAfter(int idx, StepAndArgs& s)	{m_arTestSteps.insert(m_arTestSteps.begin( )+idx+1, s);}
	void		DeleteTestStep(int idx)							{m_arTestSteps.erase(m_arTestSteps.begin( )+idx);}

	void		SetComment(string& s)				{m_sComment = s;}
	string		GetComment() const					{return m_sComment;}
	void		SetAutoCtrlName(string& s)			{m_sAutoCtrl = s;}
	string		GetAutoCtrlName() const				{return m_sAutoCtrl;}
	void		SetDutTypeName(string& s)			{m_sDutType = s;}
	string		GetDutTypeName() const				{return m_sDutType;}
	void		SetDutCtrlName(string& s)			{m_sDutCtrl = s;}
	string		GetDutCtrlName() const				{return m_sDutCtrl;}
	void		SetDutComName(string& s)			{m_sDutCom = s;}
	string		GetDutComName() const				{return m_sDutCom;}
	void		SetPwrVdd(int n)					{m_pwrVdd_mV = n;}
	int			GetPwrVdd() const					{return m_pwrVdd_mV;}
	void		SetPwrVio(int n)					{m_pwrVio_mV = n;}
	int			GetPwrVio() const					{return m_pwrVio_mV;}
	void		SetPwrVled(int n)					{m_pwrVled_mV = n;}
	int			GetPwrVled() const					{return m_pwrVled_mV;}
	void		SetPwrVddh(int n)					{m_pwrVddh_mV = n;}
	int			GetPwrVddh() const					{return m_pwrVddh_mV;}
	void		SetNumRows(int n)					{m_nNumRows = n;}
	int			GetNumRows() const					{return m_nNumRows;}
	void		SetNumCols(int n)					{m_nNumCols = n;}
	int			GetNumCols() const					{return m_nNumCols;}
	void		SetWriteBootSecsFlg(bool b)			{m_bWriteBootSecs = b;}
	bool		GetWriteBootSecsFlg() const			{return m_bWriteBootSecs;}
	void		SetBootSec(int nSec, string& s)		{m_arBootSecStr[nSec] = s;}
	string		GetBootSec(int nSec) const			{return m_arBootSecStr[nSec];}

protected:

private:
	string				m_sComment;
	string				m_sFileFullPath;
	string				m_sAutoCtrl;
	string				m_sDutType;
	string				m_sDutCtrl;
	string				m_sDutCom;
	int					m_pwrVdd_mV;
	int					m_pwrVio_mV;
	int					m_pwrVled_mV;
	int					m_pwrVddh_mV;
	int					m_nNumRows;
	int					m_nNumCols;
	bool				m_bWriteBootSecs;
	string				m_arBootSecStr[kNumBootSecs];
	uint32_t			m_crc;

	vector<StepAndArgs>	m_arTestSteps;
	vector<FpsFile*>	m_arFpsFilePtrs;
};
