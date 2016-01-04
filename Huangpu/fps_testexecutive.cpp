#include <QtWidgets>
#include "fps_testexecutive.h"

FPS_TestExecutive::FPS_TestExecutive(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_deviceHandle = Init();
	//QObject::connect(ui.pushButtonGetVer, SIGNAL(clicked()), this, SLOT(GetVersion()));
	//QObject::connect(ui.pushButtonPowerOn, SIGNAL(clicked()), this, SLOT(DutPowerOn()));
	//QObject::connect(ui.pushButtonPowerOff, SIGNAL(clicked()), this, SLOT(DutPowerOff()));
	//QObject::connect(ui.pushButtonStart, SIGNAL(clicked()), ui.textEdit, SLOT(setText("Clicked.\n")));
}

uint32_t FPS_TestExecutive::Init()
{
	ui.textBrowser->append("Hello World");
	/*MPC_Initialize(); 
	int deviceNum;
	MPC_GetNumberOfDevices(&deviceNum);
	QString s = "DeviceNum: " + QString::number(deviceNum);
	ui.textBrowser->append(s);
	//parse_config();
	uint32_t DeviceSN[8];
	MPC_GetDeviceSerialNumList(DeviceSN);

	for(int i=0; i<deviceNum; i++){
		QString s = "Device SN: " + QString::number(DeviceSN[i]);
		ui.textBrowser->append(s);
	}
	uint32_t deviceHandle;
	MPC_GetMpcDeviceHandle(DeviceSN[0], &deviceHandle);

	MPC_Connect(deviceHandle);
	::Sleep(2);
	if(MPC_IsConnected(deviceHandle)){
		ui.textBrowser->append("Device Connected.");

		// Set SPI port
		MPC_SetPortFpSensor(deviceHandle, 0, 0x0008, 8000, 0, 0, 2000);

		return deviceHandle;
	}
	else{
		ui.textBrowser->append("Device Not Connected.");
		return -1;
	}*/

	/*QString qstrValue;
	std::string strValue;
	bool bResult = Syn_DutOperation::StartDutOperation(_ListOfDutPtr, strValue);

	QString qCounts = _ListOfDutPtr.size();

	qstrValue.fromStdString(strValue);
	//ui.textBrowser->append(qstrValue);
	//ui.textBrowser->append(qCounts);

	if (0 == _ListOfDutPtr.size())
	{
		QString qResult("Can't retrieve the device!");
		ui.textBrowser->append(qResult);
		
	}
	else
	{
		QString qResult("Devices are ready!");
		ui.textBrowser->append(qResult);
	}*/

	//xml test
	/*SysConfig *pSysConfig = NULL;
	QString strXMLFilePath("D:\\Project\\ProjectData\\580-005337-01rB (Viper B2 86A Module).xml");
	bool bResult = SysConfig::CreateSysConfigInstance(strXMLFilePath, pSysConfig);
	if (!bResult || NULL == pSysConfig)
	{
		return 0;
	}

	QString strNodeName("PixelPatch"), strNodeValue("");
	QString strAtrributeName("Args"), strAtrributeValue("");
	pSysConfig->GetElementNodeTextAndAttribute(strNodeName,strNodeValue,strAtrributeName,strAtrributeValue);

	ui.textBrowser->append(strNodeName);
	ui.textBrowser->append(strAtrributeValue);

	uint8_t *pArray = new uint8_t[strNodeValue.length() / 2];
	pSysConfig->ConvertAsciiToBinary(strNodeValue.toStdString(), pArray, strNodeValue.length() / 2);

	for (int i = 0; i < strNodeValue.length() / 2; i++)
	{
		//std::string strTempValue = to_string(pArray[i]);
		//QString qstrTempValue = QString::fromStdString(strTempValue);
		
		QString qstrTempValue = QString::number(pArray[i],16);


		ui.textBrowser->append(qstrTempValue.toUpper());
	}

	delete[] pArray;
	pArray = NULL;

	delete pSysConfig;
	pSysConfig = NULL;*/

	QString qstrXMLFilePath("C:\\test2.xml");
	Syn_SysConfig SysConfig;
	bool result = ConstructSyn_SysConfig(qstrXMLFilePath.toStdString(), SysConfig);
	if (!result)
	{
		ui.textBrowser->append("Can't find the xml config file!");
		return -1;
	}

	std::vector<Syn_Site*> listOfSyn_SiteInstance;
	bool rc = Syn_Site::ConstructSiteList(SysConfig, listOfSyn_SiteInstance);
	size_t ilistCounts = listOfSyn_SiteInstance.size();
	if (0 == ilistCounts)
	{
		ui.textBrowser->append("Can't construct Stie list!");
		return -1;
	}

	ui.textBrowser->append("construct Stie list is success!");

	/*uint8_t *pOTPReadWritePatchArray = NULL;
	int iOTPReadWritePatchSize(0);
	Syn_XepatchInfo NeedSyn_XepatchInfo;
	bool rc = SysConfig.GetSyn_XepatchInfo(std::string("OtpReadWritePatch"), NeedSyn_XepatchInfo);
	if (rc)
	{
		pOTPReadWritePatchArray = NeedSyn_XepatchInfo._pArrayBuf;
		iOTPReadWritePatchSize = NeedSyn_XepatchInfo._uiArraySize;

		Display(pOTPReadWritePatchArray, iOTPReadWritePatchSize);
	}*/

	for (size_t i = 0; i < ilistCounts; i++)
	{
		if (NULL != listOfSyn_SiteInstance[i])
		{
			//uint8_t	arMS0[MS0_SIZE] = { 0 };
			uint8_t	*arMS0 = new uint8_t[MS0_SIZE];

			listOfSyn_SiteInstance[i]->Run(arMS0, MS0_SIZE);

			for (int j = 1; j <= MS0_SIZE / 8; j++)
			{
				int StartPos = (j-1)*8;
				int EndPos = j*8-1;
				Display(arMS0, StartPos, EndPos);
			}

			/*delete[] arMS0;
			arMS0 = NULL;*/
		}
	}

	return 0;
}

int FPS_TestExecutive::DutPowerOn()
{
	uint16_t error(1);
	/*
	// Power on sensor
	error = MPC_SetVoltages(m_deviceHandle, 3300, 3300, 3300, 3300, 200);
	::Sleep(5);

	// Disable Sleep
	uint8_t pDst[4]={0};
	error = MPC_FpGetStatus(m_deviceHandle, pDst, 4, 2000);
	Display(pDst, sizeof(pDst));
	uint8_t timeout = 200;
	while(timeout && ((pDst[0] != 0x01) || (pDst[1] != 0x00) || (pDst[2] != 0x00) || (pDst[3] != 0x08)))
	{
		error = MPC_FpGetStatus(m_deviceHandle, pDst, 4, 2000);
		Display(pDst, sizeof(pDst));
		timeout--;
	}

	uint8_t pSrc[2]={0};
	error = MPC_FpWrite(m_deviceHandle, 1, 0x0057, pSrc, sizeof(pSrc), 200);
	error = MPC_FpGetStatus(m_deviceHandle, pDst, 4, 2000);
	Display(pDst, sizeof(pDst));*/

	/*auto iCounts = _ListOfDutPtr.size();
	for (auto i = 0; i < iCounts; i++)
	{
		uint8_t pDst[4] = { 0 };
		bool bResult = (_ListOfDutPtr[i])->GetDutCtrl()->PowerOn(pDst, 4);
		if (bResult)
		{
			Display(pDst, sizeof(pDst));
		}
	}*/

	return error;
}

int FPS_TestExecutive::DutPowerOff()
{
	uint16_t error(1);
	// Power on sensor
	/*error = MPC_SetVoltages(m_deviceHandle, 0, 0, 0, 0, 200);
	::Sleep(5);*/

	/*auto iCounts = _ListOfDutPtr.size();
	for (auto i = 0; i < iCounts; i++)
	{
		bool bResult = (_ListOfDutPtr[i])->GetDutCtrl()->PowerOff();
		if (bResult)
		{
			QString qResult("Device is powerof!");
			ui.textBrowser->append(qResult);
		}
	}*/


	return error;
}

int FPS_TestExecutive::GetVersion()
{
	//MPC_GpioSetPinType(m_deviceHandle, 7, 0x10, 6, 2000);	//Pin 16 = output (Ext reset)
	//MPC_GpioPinWrite(m_deviceHandle, 7, 0x10, 0x0, 2000);	//Set pin 16 high (reverse logic).

	/*int i = 1;
	while(i>0){
		::Sleep(20);
		uint8_t	pDst[VERSION_SIZE] = {0};
		MPC_FpGetVersion(m_deviceHandle, pDst, VERSION_SIZE, 2000);
		ui.textBrowser->append("GetVersion:");
		Display(pDst, VERSION_SIZE);
		i--;
	}*/

	/*auto iCounts = _ListOfDutPtr.size();
	for (auto i = 0; i < iCounts; i++)
	{
		uint8_t	pDst[VERSION_SIZE] = { 0 };
		bool bResult = (_ListOfDutPtr[i])->GetDutCtrl()->FpGetVersion(pDst, VERSION_SIZE);
		if (bResult)
		{
			Display(pDst, sizeof(pDst));
		}
	}*/
		
		//MPC_FpGetStatus(deviceHandle, pDst, 4, 2000);
		
		//::Sleep(500);
		
		
		// Load MT test patch
		//string sOTPreadPath = "02000001878486470418BDE60AF6F5AD63C9099B845D11635FCB9EC15FACC5409803708CF6C9908C9CF7DD62B0D66651D0215843070B2522D1212EE42448792219C7F24C4601296A9D96756DE3CAAB28D234FDDB73E17F1CF4CBFA371502CD01D3C7312CEFD56C18B5D74BB9E015AE386FE235428A15ED488FB401102AA0AE26DEC38F83914BD4BAF98AD79E7B4B43DD7480EA04AE54914FD4F448EE8494EA3A354BDAAB8351CFD78D94E802F6CACE8C7D6321218061EB4921156347FF4ECF62BADC8B317378BC1F76F8A5BB07BC914140FB60CAE46FAF1B36EAB3A2F53133118E2A92087360E228913DD57C6B5330CC61B8C7AB914DD561B604E9E47EFA174FF63E9196848800758AA22940840F4B43F8526F15478D50FF20E93CC30CC59F354D5944CACD6AF9D2037A1F1AB60B09339D6346AA8A1BC3B0D960B455B16DD7AFF5C053E2EC3E78911745700C9AAA811BB2F4098614789C095B8EAFA5BC257253B818D0E6D78DD9599762EA0F2F5206B1C955A115C849DE3EE4ADF53E2980C356ADF8C60BFEF7C43096991F160E4CE5333853E4FBACB9C05FECF8B3930B171BF0F515D0396A39DA933F82D4C0BB20C0D750D4842A953DAD92425295281099172FC24C5402EE833857EC8F5961C9D7784492C0947D2CAB125855A0A57933C0BCFB76B4602A5FC9A0FC26DA94B05C7636496195E2C17A1FA348FFD7B04D7EB619D97B61EC6D2C7E343C0FDAA8D367C451CE7036AF741AFB6B0F70752B9BC6C6B1BF681EFBB8F12BC7535E1CAFBEB2CD127CB68ECB8C317BE77DA694382F9BD0B0BEF0B6B3CF0DB6571BA26498BE7356A38A47D8EB84D50032FB5FA5EBB65ED4EB03C540774FFBE684D4F19C422632FB2986283C587C035C72AA5CAD3A012BAE5FFF3D6F7ECEFBD6A6877563569696BDD1897F0A25334B5B948315B24D4D0246203C4D5A6E78F40FDF036517228A2B421BA3E4BA05BCA453BB01F62BA950F3CF57AE2487415F7AB039D7D69F5C9FC0DF46E559ECC7040CFCF2A26B0D1DCCA42ED525A2CDDEAAA9FDBA4E8CD76C8071A34B2CBCC5A40C9FB77C173A255BB2F1C39FE260C9A32D5CDF33286E871E0E84EA227EE8357E5446B44F6D98EAB56D36890B194BC339F0A6B516730511C4E2156EE64EAFACACFD251EE32A1F1FEDF4EB6173F59C7C849ADC6E9FB9B1FCF71F801817E4322E71EF034AB52D1306FF5049EE61A97C1A02A1D9ACA6C7620BD56F22B336757CD29459CEC7BC82BD80D9804A5CB709959658AEC5F92764D7B5CED43CC9A3FB6FFDA79F3183013D87A22EDDCCBEBC4395A4F8653C98958FF6AF833F07EA2C33E29B4A5B8F1401558A2FFD98102E7EE6EE683D6FE729E14BBD1909CFCED00ADF11D7A69815B60F1E785A2F89AB878EEBF2EE544236C427B5D865979D45CA1AA9134EC1FDD338554D447EA49A282A4532D6BF465388A85B2305BF8F48151CB7D6AB01868D8F417BD7D3999C75ED7B31A0EFC3AA10F3D78697A2EF1C9A172B60960290DF6042A7232C70B7BF89D5D8FE56299C7836217A26CF5244A619B0B833245CAD7A4CF2893E36686CF19D7F0DCEC26F152B36EC5718871CC37620E1E5440A2627EC9F0EB63DB2A4B09D1C7F53902DF7FD9C0B5A5F9B041EB836F8F93F8624E86FC95004DF831770465D315D5A1A8841B32604E575F8C80C46E99E7E1AD67329A24744EB206111DB0C28E54820EE30A89BD0B5A102606B1C1A320F3B003ABC4D051824AA559F2ABFB563196C88C4BA62B8E34AF694B07AB4FF109887C370DCDE0D8C5DB9D83C47A69C309B3287CB31348E8E245CBC00911C8DAE24453E755AE29A977D4B3BFA94E7E4306F6C5179A77626C318C6492C8DA452BD1C4C7220742919AD58D586DA57119361C2E2AF35C2A9B436309DF530FC12B51F0A93318A3AEB792C68E587CEADD64FD262BB75BCC44585367E2A6069CF0FAF3DC15340C689B0E417BF6CCFB3B64C9C40E5BF87FB394ADD0272F81092AF2F530F34E5A3028636DE2EFBCDE8FA8445302BC409AF526BFA6EECFF8C295089DEFA2360D57C05FE8742ABB6B4F48CA91420E4F3DDD177615538B0BECCEF1705F066653F0912860B91A9E4132D9B0482CC42DE726B589CB0DBCD8AA8394463FFFB154F0839D1F0F62CD0477D7CA4B57FADEB36DDB29E24E55B550095D62E29063320BB9D1DB200A21C5191406E72AE1889E983B8EE7745E78876CDA242C6B78FF72B3FFE9388A343A8BD7EB228AC979DD2DCC4081095D61ED09A6B2F13F4CF39F7F40C9A499BB90B0D11558B40239F2E8F25617FF438D4D6429A5A8E96A849E85B0DE07F74A9774CE224CA20DFAED5850CCF4439D2B2CDE3B68562BA52FFE6CABBEA1B3A2E06D1053FC31008FEAEF6E26BEB5A8574B7575069FB58CB7D86A050305A3137F6C67EAC1C7211C474BD1509F12AD124E23E6B0169DF0AAF71E1ED1E6D24BC0D9A45289748FFEB6A02C8698F9BADE3448DBA70E7BDDC3AD825E658A84F00F0DE0643A5429A1A84C84E729966352965E7A95BA49B2C9588CF48662434B29368C1E687845D60916005FE1E7986DA719CDBF66AF5E49C4B17DD9A590B888521D7832F1FE39A469DE9F7A832113F632B695783AF25127A7F6638CB5CD643F9F31771E57832E0BC7306BE8B47855B87EB1797676EC79CC6AE286267B031B4A33B54E02F5BB15979C87DA21E57A937D8ACB2EDA45C4A78689273996ACAC4DA4C923779DE39012ADD54CE75C5F3EC62F239FF9ED543DA39A5A257C9D497E622B1E951C1D01D522E761F41F8DC0023A3CC5BCEB3497639E3B9380B6D8DB4F1F88DC4461C0996E5B704B157550662872239BEECE785697AF34545B845C69437BEDF57EEC55F241F1330363FF009D0CF9CB3CC5F1A03C29798C2890E8E0441A7BEF78064CA9757F9608A6256739ED939BE455F81352240A7B74E62C47F27D797484FDB5236D242F5B92D2C8A71901118E1EBA8192D7EEF1697275F8D0E4664526854E4D18AF871C57817AB618D9156031055EE38F6E2092CE2E9B12D76AC372B220C0AF2C61B2128C5DC34968EE72B1D9870AE1A8F01EDE54B3E2B73BA1CBC2AAD7868C7ED1CD244570BC8DD9C749428B67B0F05850C1E249A92085118E69D5ABD1F22523647EA8A5E05C884C23CD78FBDFFBDBF7FBFD68EC87447D4A9AD7184AC795117A56B5C7EFD336AAE9F5E64F28B2BC4710E56DFDB3FF32DA548DA9A86A9EFF9D986E20557EAC83A765982B92FFF1D3E483B976426FA5DAA1E7A26CCDCB5968FE225EA451EEB23B2F0FA5F759E847318ACD547C81534C4BC7C827CA5D333739402167BAEF423D459AC36EBEAA41DA5EE9B924AEE96AA3C5AD370A999DDC47F8ACFA180CC83FF2173C76E890FCDB90394E7E30DB8A6DB6A30429AC8F99B11C382BEBB32154C088CD009274199E904608A5CD83CEF59FB0CDA2AAA649C8ACBD47AE71BF735C98D5A98425DAF645F256D077B73123B343FFFFD52D732A926FE73FCE4C1B90F40E5943180EF4F8A0DF035CCD9BBE98B89641DAE29009709218963EF71D3AE9A8BABEE5754C67A4ECAD40FE6ACEEA92ED048E2C1C0BE081E9C7C53AAA2FBCAF16FF0D01FA326D6CD22D7D28A37F23BC16CB8B43736A733E92D19DE4161F3FDF26EE88083791C0B48AC300FC6EF0D4C9E6EA25A194AFF88DBA66691800CCFE8E6DC0B46E9B6DB820FC10EEFEC5490597DF35A9BC23A3E06F6E1F6AC74A0B0568B3432986C7CE2FAC6448A5A928DDCDBB1DCCD53275C0851274F827F68DC15A70431FCE79217D5A1703666A7FD1F50A19C9F09862561287093A9FFA8F2CE81AB572CFFC3B3356AA2EE19328DC1FCF5CAAA46124740080C91604315E12088D792E9130DF48E40720E8CA503EEC69FA7116EEA085EBB3F0D142786CD2C6D054214C04A95BD843954EEDB59C1FE2FEA21F263B1CD0E74AA0B1809BE40C19F318A2304BFE074D0E35460C8560A9BD790A86BC011A7757DC36F603286FFFD99D3DD4D18A1BBC7D5BF87C551C31F654AC78D721A200924CC310DF0C6A498F5750BDF79E25184D14C0C297735EC2B55AAAF19F6821F0FD8F76A2265114E67ADAFAE920D3334B62DF85EC69EDD72297F545477921B6079842201E94E41CAD3F262ABB5446D544D3EC6E627B9FC2F383C9B954B202F26353772E2E6D94300B05B5BB639DFD6FD46905ACC8A2F470C2DDEA90946258FE4D4741661D98E3CA674A038942306185C2692FD6D57031144EDDF6B05A9F875EA15EC41018D1D2CD817CBD151F03F8C3F161588099BE7B497ECD930A79CBE03FCD078C5DEEACE46A57CE8E33364CFAE06E83AD513883C7C40FBBCE2EA6D45BA1661F1431AC31E757DB57DDE689B76B99883C84C8EE953BE6930006F395AF4D9D0CB1A995B42B4BFEC72738792C09253582BA61DAE0A42762B60FB09FD929FB5D2075353215506E298648FA063E7869DADB1BF08005023F4DD89FB4DF009F09A0895B331CED71E532E41A199C8F3FF5CCB3A5DE778828164E80989E02701787165884010C008CC79388A46429A3ECB5E1AE94EA0C2EF6D953EAAB02C6689B8E59CD75D28D5EF5A9C6571BC58D42055CBD0722D5E77C5F18193A6D7EF8B5C4C1885F227A32314FA1C1F566690DE806031AE98A575272C1AD09140590B06C13E6101C4C4A44A6E1E0F189DB3321708D0F72AB60DC74EECDC336990E11DC1052B3876ED3CB196271D940B17BE7F403AE56BDBBDF13434199276C7EB9DC645700DE450263203F363F46FB4DC3474E15792E7FC4287A95185420CB60EA97BEBBBAD9F75A2431F86C8405F090A82A425BDA318905194D518F1EADE667E1CCBE26F3F763BBC0D7C060B4C9F5C8A0C1FF2AEBA0B073F53CF2413C51D4178E190A804865CE6A8D24C2DC0AA7524DD1031A84C511D110E80942D12F38C1D27F30FF030CCC677EC69A1CC3529182D5CE2D089A3A8ADB9B65F7B91093EEB62E8565F0BFFDFF6F6525CDE43E87884B935522E93BB603157BDFE9963C993D6A0401F7294CE837F327704B213772B1DF19801BDBD2CFABC121FD46498CBAD2BE44B290DB1EDEF9CAF64B35FAB29C71F1529DD5516520703D46FCE045256893565EA2936AD2FBFCAC5D48B20460175CA16D3F91937708D08CD21969ADBE794A2C157F56438B10E9C5543CE7FB18D5FE9989B24A112092C21A007D36F9AC6DA6C3B4C31DC605065C7916A303D5A780FCE91923A038550925561BE48A743A18ECA6AE0F241B7BA5E1A70912AE3F8810D496CB09409D9B7FB156586535ECF771E0E6D69349C89F4A7CCB13A3F68993BD9D1AFCB97E88B2EDC850C9709E124EB1B75FAA3B64C1D6C3E32F791033FF677CA993BEC5E04BFC722DA42CD00BA3B854AD2256EB4FC6194ECE207CB56CC7A003D87930B9D9AE3DC7B3D2E284DD090F6D7ED699EB10CD8C3D5E9F888F6322ECE4360065ED80CFEA17731F8D78FFBD605D1077A2B407EE901F4B2483F32E6A580CD8F07891A9BE85084F4FD1701822BB9E99866F6D10066CA924823CB166A7AAAB85176E29374683580FA14FDD98F67D0F67C607603C2EFF0CD844328B8B72572A078552FD9E2D636693221641F19C3B63F29748A9966368C818BEB899ED82EC9F6F671EC830FBBCE64879E8F075EA7FE820F1375C187AD84F698DD387B143CE1505F2B542C14F7BA89A359BCA66F1C22BD4F5AB17D9B146F4E5F949F420010FF130B85B0AF02D6CBAE4DFDE5A1B71982C7B86CFE76D271D9736851D2CBE3C91561100D64E305BAE28E59644E04ED0F765041404CD483774E8D6C8D75FC8ACBA48FA9C598F357C3E0C86A5CCFAAC0B64C6F4EC439A9B4D9ADA83E94C9E2102BA1E8207134D7964AA51A733C744574680245FFF0320BE6941FF0E4A0EAA93FCDC2D6C33C459B9DBC48EA42FB46902C6124AB0E45FE67CAB926231F74120D5F1B54749F9893F0846EB81ADDDCF3F2A03B630A129D35B268BD661D3DA03EA57305174534D59032B783D637978E3BFD11744179CEAD210FF8495846598C0E5D92110774133A9D241CBCC2E17CBEFB741AEBFEA139703D59F57CEB89E3A6C5D959E6D23EFEE01351B4A7B387CD93D3D9C15D2FD2C9C1553A3762E0A37D738392A04B25F0DDBC05C7EC07CE3FB889CA16422659B52FE07E537F3E9407D5F97F035D6284B625B4A3E9012ACF2F55A4548AA59C92FF57911C192F849C7014FA649BD340EF265DF427701FE78991AED3F3EDFFECCA1DDE408611E8B5A6DC2E9EA6EBA96D8B4C37887707A0A0EA620BBB8A9D7CF01023AD2DDB1C67524076148EF79217DD0AB22DA2FE4B499F836024D0990FAA924B6CA149969C5909F2BD181436D75ACD0E1251E12FA05A9951721CCEB2C7FD7F3121B036CB9AB2386A134242A1DF804300763677C8E2B2272B60F75F134924A468D8DDEB5A3A353940BEB0520DB84B2EAA5F238E50331919326EFE49AFBC74088A8AB567C0D29F3F85460B38FDC2EED22B28ADA2B96213E2EE060ED0417BF489EBB61E0C2DD262B3AFA104B06C873DF9A80D5D703DC78B1B9F4A81D84E5C153DC02C8573A28ECE5AAB287A0159E0E156EDB4E7F2CEF7F98E4BA38DA3DFCD456842288FCEA2847A3A151E1EB8375CB12AF2C779F5E0D08ECDA71A9B1B7DDC98BE9739FC56A01512060DE4344950702B7A5743F62E204814098335260B1D1C97CCBFE82DD5A9D84ADC371DC02BC863F4AB330D614991F0D4FA5EF405610811F5D7266B6945096945908C7CB9CC8B319ABC02C7DAB558435445BB1EF5876B5885FF1B7BBF916997979E98C05766C91CA0FC61EE35FCEBD62BEDF20B6F92336CF69D8963ADA332302325531C85F67D2A09E7FB35D0F59473D9EC9973F1525ADED38F47BD83AF8843FAB38280B946B697EA5BC9C8291AE2C81B037181CA8B953A2EC91DD5E6D7F1CC669A321792ACEE8C0B66F0415D951AF333AC70414C4D31962053F2C1E1FD6B0BF158C08BC7F4DA6C2B0761945664D68A784D7685D228C24A1A5AA01D239734827E2F36E7C9F5162F39A96C0939C40E4CE351070E80FF3D7F97055333A807AA5EA3535214145F44122CECD877458B0501E6A3EEA7C1E296C55D9A15B5D6E2E49553166E9EA81B589F27724C8B0E4525EFECA4CCA4E754018FDDF4FCFE2F4DB5A555FC0DC8D5DA4F6C87830A250F5DF8E4A0765364649261D38C3231D50A5430337E8B347B5C20096F156B1386ED184E29EF97D93208237FC4EC5C78C106A50F5AE33B59FB0F333E51D96C55D4939D2C9BC827DFF66EE969940E0173C271ED7459A7869AFA96C24E877B5196CA4CDA4E27DA39B3031CD3D92C0046327CB6802B159423FB8EE3E7E773E8E5C84BDDAE2F53E8C7AFA36C7B9F8F701AB18CE0AE834E8A48C90AB7EF1AE03EB61A0AD2AC6405D3865FA6BA9033D7C942649B31253D0405D47344B8B43BB926E8A1DE457ED7CB2B3BF93AA004B769A37739FDCD2A79C0A6D209DD990D2FA316B2185E9D03709EDB870973EDC277EA74E4D1242176DBC475B325B5A57CB0A23B5D9C11E3CF85610140D050B10711C9D80AEA7F8C468208BB8801A88724586411872B41AE6628FC47A33809DCD5F2B905957ACB130F868BCE9E213413E86DD35BFF0543AB58D6E749306DDB045E049F506B9D40C7AD153A815A5E13F9625456E9649543BA58FE38A35BC6153B41BDEB3503C1826B283282AB4DBE045F70BD7084D7DF8A0B2E8BFB64D7A70A5BD5A20A6A04720595B7210FF4619CC938964C3BB644CDE77E8EB23326ECD39591E57B7F3C4B2F73B8CC2C6F707642E8B9F834C010B1FE36DC0D6FE922AEE7140D46A5919A1AF9490A162E68DC0BE35C048998CD10625D05C79841878E4EC9AB3A0A5ED6F2F297E7033354A249E9930A5A44084C7C712300823AD33B83E1268C5D81DC83DBC505209A9965F80022345034354C548EE24825CDDC938859C385ED0BF3288E42F69C730CBE3A6991ECAE41F065B6B3AB47AD4DC476DB93D2A79BE4404B3A5906D2481FDFA1379007D60BDC582A595B33DB2F3011EDBD53EAF6F366E6F5D5D86CD99A4C41460FA549611E70253DDB57DC3C1F9984491AA4EE87D457685723B7069E6C3BF28A3090DC13364F2C91392FD555DEEA96268CC11B83164F260B5A5C4342DB4E6F1BD1C43B312941138AAC49E64D1B020AFC5D4966DE95511AA98BA4CD6BE2AD7CF9042A5186D72D8E39410D0A6CC80E97F921264A857BB083EEF9C4A3069069FD2749A00BB183A620277111535EA27E8203390D25D64A20177F32AA3D1CFB693DA4FB802E623E9FC134D366E4922307BAF4115F4C4B37DFB6B504F8DCE86A68EDD827DBD812E5BAFC5F81932EC8C9561920E582C7284953C0F9CB04D11B6470C546AE5C819DA1265C0A74EBAD76BE75D7B90CA8BA0183CFB0A59D77B460D33CCCC78CF4DDE3DEDD8F85EABFD3C6257D10E9E3A8FDDED1CE0917E04098D69571D671F7C8E15F0D8C33F2DE5BDE3414A70DB211C6530C1DF171BCE4C1007B7E3C951FA1E3932BDEA2C90668DAA869F02A9FD0FC3AA6FD9CF70E5B947CF7782704082B9A6C7085ABA089F2817DAC034993C05521F426F764628AC04780FEE63705A1530BFF4DD530341EB2C31AC23B4D8785CDF0ABD133CCE84E2455F092B73A100DED08A87BF70D607EFDB435767244A73733882D8CE040570F20A03D74811A2F3A680F9A4F81AC949E332002DB90E52919259672183D21EA38CCD4707FF9C4BDC091866BD62A378D274A40B260A7A95D3860BDF49A928892AA0310EF5D945039A3715845460B50264F2C039CF6228EE6C94DB7D8527BAF3C3993CEA05230FD6F7D36741D3F62B15901FFA7C11627E90707EB6B210F7325F770613406AE95F44E33E1B3276D0930C053E9AA066252E793EB483F14F0C2B2989445B7336DE9F1CA48C7D2D12B63A01568B32DB93425099F35C0573A2F7B0C5FD16A048CC736BA5967E89233372401DF180CA095618601910D4DD5BEE3B7022FB42EEE23B5637B6060CDDA94A4FB283BCAC9FAD6029F98D56D7F41A8BEA9CCD3B8B7042ED0FE28511091A4C773BD4CBE779E539E476C3005BBD2851357995A2DAF60A8BA8DB9A37172DAA918F1D1877B0F0E3577F64B572F43E3F501DE180E2B18EB64042D0FFC6AF95BFAA4209F3D93F281DA8274B31674CD4335D448E475025B74417BDFD3996B95B7EA5D014B8315C6D258946EABB60D0C211F08DF640F6A025BEF28153524FB5A4699E8357E6020178DE8DEA094FDDF4618B1F641823C08644151CF30B71CB56A720119760074B3FFAA0E1D387508746373BF56F7597F3C3CC92C1C2E2836A6AACB0CE216357905FA6CAAB42533852E8D2F1DEFA6559E0A92DA8A34126BC81D97652771C3382D020A0CCECCD66CEEC2F2C64615E40E6041EDE8A76346CBA9F1C318ED7F84FC654E7BAEA1A3215663C10C2DF79B0AF2D8124231440D52322513BA7C9C7DA21FD036969DA27CF9D2BD4751FC6CE67669A722CFBCFC41C1D76603B58C6A190AD4FFC4C7AFE7523F22E8E84B5F8822E64CF8A0B9BE3FDA1F215028AAD5F3F1B5AAE1AFBBC9F4F140B530D8F82197D0DCB7DDC67D9F6D182664F2FCE1DAF466FBE6E1C2FD040373A4BF60FC37C2110C8B5C938751A66DFDD05CCD235DC0E2416F744C3EBC8DBDC61518297B3B33C57B83BBDD2CDD0D6C72C15F9E449073FA137DD779CBBE7BA7D3E4822EE8EB82FEA3E6FE1BFBFD7BC7578F51C3054ACFDA90D28B0907BF80903EC1E0BCCAA69DA12BB98A9F89C288189F31CF475990B074605450EF051228EC318AC1F8FD78C2FFD57758B33B9D5702195DD857CBFBC48FFA4FE88571B6B2B6A2E0612A8536995E928A646AAD19CD23246FE4751A98493A81A8456A26A333DDC6C4B6D328F15C4D69C94CEB7FAF226CB270C125B88070C9AE5E0699A07B0294038DF5DE300080A830575CA0AF8AFB293D2658D8E881FA0A934E46CF4BF2FB293BEB401DB66200CA65CAEC2BCD78B6465EB6A92369DF4C61210F1BD376D5A859220EC4B35DCCE164828C855E7F710EAF160F3350368151F3A6226F562CD357307DC59CE6E53179198F70C50E68D81DBE243B69EC84BAC1B02BA66F2A160BE253FECB7039AB42B2E78392886EEE69916A1DD412B38623519A17722EA3EE4F72F25A373446CC6487427314091AE558CACC89C2A3CE285D74267EDCD627BA3EAE00CCFEA7195E66279E09C032ED9172F2668EC799FCCBF72F1C6B0A08CB39EAB7AB676A2CBED9411197C7ECDDFB0B48966B21729F41FA5839A5F9720F02DDC32449E569AF694D649CB2DA2A3B82D13DFA57F1374FF45C0100D3454484C2DE0AA7B34C18AD7C87A8357B7117BCC83F9CBE6DAD874C407D0881D86846A9D9656787CFF8DEB2502DF6473C471EF23C0D74B6895C54F89D8D3D5F20A4CD3EC9A6946DB6B2B424734F28052A931E5F1E3451B9C3079AF2E2037D761C551DC09A937F4EBFE91D8AA738CD567BC340F562BA40ED7F2C7385C66E1BE2D8E6088B6D0F07E18AC902053BDFEF79B9738A4F1B50C4DF8F67870CA070FBA4C9398C807F36653FE7962B7E52A3C3D1D37CB50D34DA09F872ADBEFF6CF09458DAE285203847D6D5EABF77F270EB7204420DDDC8D5238749E8839049FDD461D6E1775A8DD676B8EDF6958C87F11DEAA65A9AF11B2B3A80C48446AD83DFD410EF936BCFFE7CA7E37A78D604D02E37C6A51481554C52859CDCB6684989A7F3287D00961D51C2614EACFC7420F00741BAEB6A534AB304F4FB1E2409D189547DD2AAE9B27EAFCF5E5718383ECDF4CE0B9E76D1E3F007DE1E80D92AF160BE6C91A2F65F4C00B810DA7D445188307D2E42FF465FB3CCE2EE3966417C3CA113214C8EAF3CFA7A21328E3118E939DE0C90F430322E7290B372C9078B201419EA0F76049F9E06D876331681BD8101B153EBEB55ECEC113AC3948466ADB74B9479D60C9A6385F81996F1A216D82EA99E8A4CDEB80BD85E4488633B20A7C09D657B02BE43ADC8894424104DAF0C69D8814F5F954F8B2BC6DC9E5CFDA8FE36F60CBD3145A7469D6024ECF56FC5117A53924B8916C73B5B42A72DAC83D9D38E936D7E56A06CDC1263D9DD6E1D9088A700AA6822D0FFA70D3BF8F7897ABA01B68C6CF6FB55539C0967D14E91B75A09401492A233C020A2761532984081AE4A144190627A053824FD7DE2C0F789237FAB1E7823A0D43F200C764633FF1BDBB059E80C6FC790B03461EB5B1AB5E6C977FF908CE684E5123BFB10EF02CE717425850614C97831144E51E6792A5AA349B4743795AD302679070F10DC40A9E23ADC8E0C98B1DD8478C01574C98295BA9F459001BF3997E330BCF588DD246C62E79AC151CA4AB25F594ACC8FD93A6A77E727E07FF9958EAF524BC8AD601680E46CB4D298C206616E616E352ADA37A6FE051B2A3D7E4668FCE4ED12BF0C2861F51261D1D042F28858BE08CA3F8FF52EB9888331F3B7E3BCB44B8198D75D82F4B96EC0E1E9A77D1934588D0CE71ADDBA4E5ADEFB044E72DD8D5F2EC5FAB6F3091CEFDFDFF9FE64FA92828EC7CB3D15B6B0300B854D23C99C8549E56C1E6FB2543F035352B1FCFD45F661B1FA26AEBC785C2F5B95D5DD44DAB7E2F6D4AC0C0C073369C509A26DC03EA630E1A0327729E2F9E2D84023FC1B5AB6872D4CF84AB485CBF5C1245A72038507E8D9E442521C3F9C8B114F86C1D46EF0FFCC148DAFF4E18D87F56AE0C9A84BE418F8F1A304BFEFB5F87B66D91C4A1C877B16136C5A0C60B1599A95D37E50816B8655537B7454944ECA8CA38E43BAC44016506B07CEDEFCB3BEBDB3062A0C71AFE9F43E735354772C4804A3C1A39935A8E88328F26B5E4245859B175304B372CA3A9383E4FD3BB42051B00F1DDDB406B78787B6A5FCFC5267A2B6DE615CBC5B5B4F65D89AA6479F53DBECF92CF39E323699AA9FA199B63800A2DAAFC4D75C90A2558716879032A72C0A30DD55C837B24A725610F77969C7F087BA9C49170A2FA5171BFDEF8DE1E0B2C6CCA074E12981B526DF72AB6769553548D0E37D6A6B76B57530F5D6F6818ED25D15FC97B6E6036881CB18502CBB51CF4329631E125D3D4AC5ECDA62F4E77BFD00925229828DAC7866115DB47DF8973FDFF46DA5EC77EAE98CE5570323CB22545CFDA5DCF50F374841BCA853A52553ED7ED663F28D8A832CFB19B323B4621F231653B125C48FE23B8C0B8AF4A09BA15F9E205E60A5CF78BE10F7E81B574899399E8745DB203A18F104DE440D8B9DFB4FB99113E18471AE09C67DB7BDFE3E4C35FFFDFBA3D13DB5BA97AE3BAE27F6FCF9BF1472026C247687DFF464F83F4C9772ADA464DEDD15A5551E9E16E924FE11BFDF4F6FFDC4AF234E3878B714F9E752EBDD1E8AE0D17445D67A5DA1983AEC5F8A9FFBCDE6ADE8CCF5EF2B6AF4590B9A5DD345891551DD86CCB13C0D8C7AF806A113D69104DD440721F5031014C7B9051750CABEC62DAE2C6306E025F827B3D397D9868D786DC0584ECD48CB153AC2D54F275D6E7346D05FDADECB2DAD1D0E52F3313B4F014CF44AA6BD7BBC75AE82BAAFD64D9479AF8C30759AE91270C8019425C214E9D0F4F5EB75F3E05C5E94AA12D0CFE1DFEC36D6ADC4FDAB26865C5572E48DB39E181C381395A4ECD0506945E24307F69508A32339C67A83C107FACCEB96B305EB256C114B9EE440CC5956DDC02FC442CA124574A63F06DB130648A0F29C8F7FEAE6212533A94D88C998283772ABEADD56B0F7BD4816804D407A572E88C25A6ECA8FCB000BA2C422BAB44CAD0F8929DA69EFFB8CA925D2BECF9ABB181226C1072C72F0B3817474DF085B939701E33721A2450F545AF07D0561131D861E2158BA2FF33B1E05EC12AC12DA0C67650B7D41897A863AB2AB8C67DC6FC0B03927475D3BE6BC4B0D92044B3340493921087CD9A180CD1FB724B718C18D59C065233353DCA75EF539F291B0A8B54AD00DB6129A2379B1D655408197C2F9ADFEA240DD8EC89684B308B4A525C61619B08D8AF5728732D7E10FC97D8C11FF3EF6D5D3061A2DAE981152FB48260D089805D6D764EC0887618993839D87B6582B360F16021BB9F5C36C3FA0D8B2762DD774A12FA53C3C780E63E7AA35DB2E6705F79D53BDDE78EA01EA1665469CF2AD7C534B722B1AD1B685BEF0B4F3F2B29FEED62BBEF0C29D287A6D1235881E1F0814B8883D4DFF44A70C497C309F9D37092A85DDF1E7768C72107570A34E253470B73B5CD85EBC8B1CDFC3F41FF0D40B4D21EAB4B6836D01D6A4F8CBD2163217024524B95ECEFF84C84996B077A948843737BA34732304159395EA71D154B7EE31B2EAC55A3AE9A23A82698AD34552740E328FEF6A6C667C3682FF510DBC87346BDCB5577115308BB430AF39EBBCA47336D7F84A43ED3CB95757B7BFA02CE54AF43A09BD335FFE7F567194B5216B464819B06C37E5608E0FAE8B7F3DED6D4DE53C577CEEACEB10F073AE3481FBA38D195792A80236CCEC8096518CDFBED4A6DA0FE166645476ABFC8AF5EC233A0E8148D033C31946F05924E949B4D1D2FC07FAE477D5F664C14E8691B1D1B019FACB3A95DB0AFBE2455EB795BE68FD78BBB54D77404DF88DA183A90D7EABA90913F40B3D88EF9A15EA7A47A8BD161BE8A73558FA443AB8FAD8BD51204CA1C60E7FC1636D41919DD7FEF6EAB022A5FDCA413A2759F89F18143BC5E2205247035DBE123B967C5D5EF9A8F09217C904F8534216AB5BC20EE41152658D9897E9F1D8255BE883F77A91D038083A8618727BB71BD5F4FC425BEA9EEE7B51FF0BC9F9A6579C4A7BEAA4A6C44662B9AE09D408154ABF6FE5D632FA39664078809F9C563C92B6240B8F943BFF5D0701A6C8C3B79337727E95A28E379F8D8D8D8A775BDEA593262939028022360DC9D3709D0FB4CB2CE6F721F826234F60FDFE84C82D961F4D69DC247A05ECE739A1BE2CE205A91E37638B004F6A8DE62CC479B0598F1125F6BBAD3A42A14DB55BA2F136119F8877733EB5B83A977491FEE15BA75D00996E4B23F54FA2EE3F59779B9C82184A04D6096D521FBA4573C0663498F35C41A09B09D5DE6EFE4175E428F5149404E0E12B14CFC92BF984FDCC01BB1E636134199F4AF889A5DC35894BE542315671D6EBE97AF953411C9F0B35CD87A423A0A9EBC5F19E1E0470CE685786D1B0FAE48FA467031EC58B153EC39BE86043F5E101875E9CCF72CABBACE0DE4BFDEF425802FE08B1FEAFDA15F7C1F92EC1795E9F1779";
		
		//string sOTPreadPath = "0200000187";
		//int m_nSize = sOTPreadPath.length() / 2;
		//int* m_pBuf = new int[m_nSize];

		//ConvertAsciiToBinary(sOTPreadPath, m_pBuf, m_nSize);

		

		//Get boot sector 0
		//uint8_t	bootSector0[BS0_SIZE] = {0};
		//MPC_FpOtpRomRead(deviceHandle, BOOT_SEC, 0, bootSector0, BS0_SIZE,2000);
		
		//uint8_t mainSector0[MS0_SIZE] = {0};
		//MPC_FpOtpRomRead(deviceHandle, MAIN_SEC, 0, mainSector0, MS0_SIZE,2000);

		//QString s2 = "";
		//for(int i=0; i<MS0_SIZE; i++){

			//s2 += QString::number(mainSector0[i], 16) + ",";
			
			//if((i+1)%8 == 0){
				//ui.textBrowser->append(s);
				//s2 = "";
			//}
			
		//}
	return 0;
}


void FPS_TestExecutive::Display(uint8_t* pDst, int DstSize)
{
	QString s = "";
	for(int i=0; i<DstSize; i++){
		s += QString::number(pDst[i], 16) + ",";
	}
	ui.textBrowser->append(s);
}

void FPS_TestExecutive::Display(uint8_t* pDst, unsigned int StartPos, unsigned int EndPos)
{
	if ((EndPos - StartPos) <= 0)
	{
		ui.textBrowser->append("StartPos>=EndPos");
		return;
	}

	QString s = "";
	for (int i = StartPos; i<=EndPos; i++)
	{
		s += (QString::number(pDst[i], 16)).toUpper() + ",";
	}
	ui.textBrowser->append(s);
}


void FPS_TestExecutive::ConvertAsciiToBinary(const string& sAscii, int* pDst, int nDstSize)
{
	string sTemp;
	int*	p;
	int		nNumBytes = sAscii.length() / 2;

	//Translate the ASCII into binary values.
	for (int i=0; (i<nNumBytes) && (i<nDstSize); i++)
	{
		sTemp = sAscii[i*2];
		sTemp += sAscii[(i*2)+1];

		stringstream ss;
		ss << sTemp;
		int nVal;
		ss >> nVal;
		pDst[i] = nVal;
	}
}

void FPS_TestExecutive::parse_config()
{
	QString strFilePath("C:\\Users\\hfan\\Desktop\\580-00xxx-01r4(FM-000133)_ZTE.xml");
	//SysConfig *pSysConfig = new SysConfig(strFilePath);

	SysConfig *pSysConfig = NULL;
	bool bResult = SysConfig::CreateSysConfigInstance(strFilePath, pSysConfig);
	if (!bResult || NULL == pSysConfig)
	{
		return;
	}

	QString strNodeName("CreatedByRevision"),strNodeText("");
	bResult = pSysConfig->GetElementNodeText(strNodeName,strNodeText);
	//cout<<"Node Text is "<<strNodeText.toStdString()<<endl;
	ui.textBrowser->append(strNodeName+"---------->"+strNodeText);

	QString strImageAcqPatchName("ImageAcqPatch"),strImageAcqPatchValue("");
	QString strAttributeName("Args"),strAttributeValue("");
	bResult = pSysConfig->GetElementNodeTextAndAttribute(strImageAcqPatchName,strImageAcqPatchValue,strAttributeName,strAttributeValue);
	//cout<<"Result is "<<strImageAcqPatchValue.toStdString()<<","<<strAttributeValue.toStdString()<<endl;
	ui.textBrowser->append(strImageAcqPatchName+"---------->"+strImageAcqPatchValue+"----------->"+strAttributeName+"----->"+strAttributeValue);

	std::vector<TestSeqInfo> ListOfTestSeqInfo;
	bResult = pSysConfig->GetTestSeqList(ListOfTestSeqInfo);
	for(int i=1;i<=ListOfTestSeqInfo.size();i++)
	{
		TestSeqInfo CurrentInfo = ListOfTestSeqInfo[i-1];
		cout<<i<<" is "<<CurrentInfo.strNodeName.toStdString()<<","<<CurrentInfo.strSeqText.toStdString()<<","<<CurrentInfo.strSeqAttribute.toStdString()<<endl;
	}

	delete pSysConfig;
	pSysConfig = NULL;
}

FPS_TestExecutive::~FPS_TestExecutive()
{
	//MPC_Disconnect(m_deviceHandle);
	//MPC_CloseMpcDeviceHandle(m_deviceHandle);
	m_deviceHandle = NULL;
}







bool FPS_TestExecutive::ConstructSyn_SysConfig(const std::string &strConfigFilePath,Syn_SysConfig &oSyn_SysConfig)
{
	bool rc(false);//return code

	QString qstrConfigFilePath(QString::fromStdString(strConfigFilePath));
	SysConfig *pSysConfigOperation = NULL;
	rc = SysConfig::CreateSysConfigInstance(qstrConfigFilePath, pSysConfigOperation);
	if (!rc || NULL == pSysConfigOperation)
	{
		cout << "Error:ConstructSyn_SysConfig() - ::CreateSysConfigInstance is failed!" << endl;
		return false;
	}

	char *p = NULL;

	//1st
	QString qstrAutoControllerName("AutoController"), qstrAutoControllerValue("");
	QString qstrDutTypeName("DutType"), qstrDutTypeValue("");
	QString qstrDutControllerName("DutController"), qstrDutControllerValue("");
	QString qstrDutComName("DutCom"), qstrDutComValue("");
	rc = pSysConfigOperation->GetElementNodeText(qstrAutoControllerName, qstrAutoControllerValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutTypeName, qstrDutTypeValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutControllerName, qstrDutControllerValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutComName, qstrDutComValue);

	//2nd
	QString qstrDutPwrVdd_mVName("DutPwrVdd_mV"), qstrDutPwrVdd_mVValue("");
	QString qstrDutPwrVio_mVName("DutPwrVio_mV"), qstrDutPwrVio_mVValue("");
	QString qstrDutPwrVled_mVName("DutPwrVled_mV"), qstrDutPwrVled_mVValue("");
	QString qstrDutPwrVddh_mVName("DutPwrVddh_mV"), qstrDutPwrVddh_mVValue("");
	rc = pSysConfigOperation->GetElementNodeText(qstrDutPwrVdd_mVName, qstrDutPwrVdd_mVValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutPwrVio_mVName, qstrDutPwrVio_mVValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutPwrVled_mVName, qstrDutPwrVled_mVValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrDutPwrVddh_mVName, qstrDutPwrVddh_mVValue);
	
	//3rd
	QString qstrNumRowsName("NumRows"), qstrNumRowsValue("");
	QString qstrNumColsName("NumCols"), qstrNumColsValue("");
	rc = pSysConfigOperation->GetElementNodeText(qstrNumRowsName, qstrNumRowsValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrNumColsName, qstrNumColsValue);

	//4th
	QString qstrWriteBootSectorsName("WriteBootSectors"), qstrWriteBootSectorsValue("");
	QString qstrBootSector0Name("BootSector0"), qstrBootSector0Value("");
	QString qstrBootSector1Name("BootSector1"), qstrBootSector1Value("");
	rc = pSysConfigOperation->GetElementNodeText(qstrWriteBootSectorsName, qstrWriteBootSectorsValue);
	rc = pSysConfigOperation->GetElementNodeText(qstrBootSector0Name, qstrBootSector0Value);
	rc = pSysConfigOperation->GetElementNodeText(qstrBootSector1Name, qstrBootSector1Value);

	//5th
	std::vector<TestSeqInfo> listOfTestSeqInfo;
	rc = pSysConfigOperation->GetTestSeqList(listOfTestSeqInfo);
	std::vector<Syn_TestStepInfo> listOfTestSteps;
	for (int i = 1; i <= listOfTestSeqInfo.size(); i++)
	{
		Syn_TestStepInfo CurrentTestStepInfo;
		CurrentTestStepInfo._strNodeName = listOfTestSeqInfo[i-1].strNodeName.toStdString();
		CurrentTestStepInfo._strTestStepName = listOfTestSeqInfo[i-1].strSeqText.toStdString();
		CurrentTestStepInfo._strTestStepArgs = listOfTestSeqInfo[i-1].strSeqAttribute.toStdString();
	}

	//6th
	std::vector<std::string> listOfXepathName;
	
	listOfXepathName.push_back(std::string("ImageAcqPatch"));
	listOfXepathName.push_back(std::string("PrintFile"));
	listOfXepathName.push_back(std::string("OtpReadWritePatch"));
	listOfXepathName.push_back(std::string("OpensShortsPatch"));
	listOfXepathName.push_back(std::string("WofPatch"));
	listOfXepathName.push_back(std::string("WofLowPowerPatch"));
	listOfXepathName.push_back(std::string("ScmWofPatch"));
	listOfXepathName.push_back(std::string("AfePatch"));

	//RAM Patch Test
	listOfXepathName.push_back(std::string("CacheDataRam"));
	listOfXepathName.push_back(std::string("CacheTagRam"));
	listOfXepathName.push_back(std::string("CachInstDataRam"));
	listOfXepathName.push_back(std::string("CachInstTagRam"));
	listOfXepathName.push_back(std::string("ScmAndMainRam"));

	listOfXepathName.push_back(std::string("Cmd1ScmWofPlot"));
	listOfXepathName.push_back(std::string("Cmd2ScmWofBin"));
	listOfXepathName.push_back(std::string("Cmd3SweepTag"));

	listOfXepathName.push_back(std::string("WofCmd1"));
	listOfXepathName.push_back(std::string("WofCmd2"));
	listOfXepathName.push_back(std::string("WofCmd3"));
	listOfXepathName.push_back(std::string("WofCmd4"));
	listOfXepathName.push_back(std::string("PixelPatch"));
	listOfXepathName.push_back(std::string("SpiFlashRamPatch"));
	listOfXepathName.push_back(std::string("BulkEraseCmd"));
	listOfXepathName.push_back(std::string("ProgramCmd"));
	listOfXepathName.push_back(std::string("ReadCmd"));
	listOfXepathName.push_back(std::string("WovarPatch"));
	listOfXepathName.push_back(std::string("VCEK_IV"));
	listOfXepathName.push_back(std::string("WofLowPowerBin"));
	listOfXepathName.push_back(std::string("ProdOtpReadWritePatch"));
	listOfXepathName.push_back(std::string("SecurityMgtEngrPatch"));
	listOfXepathName.push_back(std::string("SecurityMgtProdPatch"));
	listOfXepathName.push_back(std::string("TakeOwnershipBin"));

	std::vector<Syn_XepatchInfo> listofXepatchInfo;
	for (auto i = 0; i < listOfXepathName.size(); i++)
	{
		Syn_XepatchInfo CurrentSyn_XepatchInfo;

		std::string strXepatchName(listOfXepathName[i]);

		QString qstrXepatchName(QString::fromStdString(strXepatchName)), qstrXepatchTextValue("");
		QString qstrXepatchArgsName("Args"), qstrXepatchDisplayName("");

		rc = pSysConfigOperation->GetElementNodeTextAndAttribute(qstrXepatchName, qstrXepatchTextValue, qstrXepatchArgsName, qstrXepatchDisplayName);

		std::string strXepatchTextValue(qstrXepatchTextValue.toStdString());
		int iXepatchTextLenth = strXepatchTextValue.length();
		if (0 == iXepatchTextLenth)
		{
			CurrentSyn_XepatchInfo._strXepatchName = strXepatchName;
			CurrentSyn_XepatchInfo._strXepatchFileName = qstrXepatchDisplayName.toStdString();
			CurrentSyn_XepatchInfo._pArrayBuf = NULL;
			CurrentSyn_XepatchInfo._uiArraySize = 0;
		}
		else
		{
			uint8_t *pArray = new uint8_t[iXepatchTextLenth / 2];
			pSysConfigOperation->ConvertAsciiToBinary(strXepatchTextValue, pArray, iXepatchTextLenth/2);

			CurrentSyn_XepatchInfo._strXepatchName = strXepatchName;
			CurrentSyn_XepatchInfo._strXepatchFileName = qstrXepatchDisplayName.toStdString();
			CurrentSyn_XepatchInfo._pArrayBuf = pArray;
			CurrentSyn_XepatchInfo._uiArraySize = iXepatchTextLenth/2;
		}

		listofXepatchInfo.push_back(CurrentSyn_XepatchInfo);
	}


	//Fill Syn_SysConfig
	oSyn_SysConfig._strAutoController = qstrAutoControllerValue.toStdString();
	oSyn_SysConfig._strDutType = qstrDutTypeValue.toStdString();
	oSyn_SysConfig._strDutController = qstrDutControllerValue.toStdString();
	oSyn_SysConfig._strDutCom = qstrDutComValue.toStdString();

	oSyn_SysConfig._uiDutpwrVdd_mV = (uint16_t)strtol(qstrDutPwrVdd_mVValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiDutpwrVio_mV = (uint16_t)strtol(qstrDutPwrVio_mVValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiDutpwrVled_mV = (uint16_t)strtol(qstrDutPwrVled_mVValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiDutpwrVddh_mV = (uint16_t)strtol(qstrDutPwrVddh_mVValue.toStdString().c_str(), &p, 10);

	oSyn_SysConfig._uiNumRows = (uint16_t)strtol(qstrNumRowsValue.toStdString().c_str(), &p, 10);
	oSyn_SysConfig._uiNumCols = (uint16_t)strtol(qstrNumColsValue.toStdString().c_str(), &p, 10);

	oSyn_SysConfig._bWriteBootSectors = (uint16_t)strtol(qstrWriteBootSectorsValue.toStdString().c_str(), &p, 10);
	pSysConfigOperation->ConvertAsciiToBinary(qstrBootSector0Value.toStdString(), oSyn_SysConfig._arrUserSpecifiedBS0, BS0_SIZE);
	pSysConfigOperation->ConvertAsciiToBinary(qstrBootSector1Value.toStdString(), oSyn_SysConfig._arrUserSpecifiedBS1, BS1_SIZE);
	
	oSyn_SysConfig._listTestSteps = listOfTestSteps;

	oSyn_SysConfig._listXepatchInfo = listofXepatchInfo;

	delete pSysConfigOperation;
	pSysConfigOperation = NULL;

	return true;
}