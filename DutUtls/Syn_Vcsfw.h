class VCSFW_CMD
{
public:
    enum
    {
        NOOP                         = 0,
        GET_VERSION                  = 1, 
        GET_PRINT                    = 2,
        GET_PRINT_SIGNATURE          = 3,
        STOP_PRINT                   = 4,
        RESET                        = 5,
        PATCH                        = 6,
        PEEK                         = 7,
        POKE                         = 8,
        SET_REFCLK                   = 9,
        SET_SPIMODE                  = 10, /* deprecated */
        SPI_TRANSFER                 = 11,
        WOE_ENTER                    = 12,
        GPIO                         = 13,
        PROVISION                    = 14,
        TAKE_OWNERSHIP               = 15,
        RESET_OWNERSHIP              = 16,
        SSL                          = 17,
        WOE_SETUP                    = 18,
        SET_CPUCLK                   = 19,
        RESERVED_1                   = 20,
        GET_CONFIGURATION            = 21,
        DECOMMISSION                 = 22,
        GET_FINGER_STATUS            = 23,
        RESERVED_2                   = 24,
        GET_STARTINFO                = 25,
        UNLOAD_PATCH                 = 26,
        SENSOR_LOCK                  = 27,
        MATCH_VERIFY                 = 28,
        SIGN_ENC_TEMPLATE_DATA       = 29,
        DEC_VERIFY_TEMPLATE_DATA     = 30,
        SPISLV_FAST_READ             = 31,
        TEST_USB                     = 32,
        TEST_ROM_CHECKSUM            = 33,
        TEST_OTP_READ                = 34,
        TEST_OTP_WRITE               = 35,
        LED                          = 36,
        TEST_PROVISION               = 37,
        GET_OWNERSHIP_INFO           = 38,
        GET_UID                      = 39,
        KEY_EXCHANGE_DH              = 40,
        GET_CERTIFICATE              = 41,
        LED_EX                       = 42,
        GET_AUTH_DATA                = 43,
        TAKE_OWNERSHIP_EX            = 44,
        REWRITE_PM_SECTOR            = 45,
        TEST_AESSHA_EXPO             = 46,
        GEN_VCK                      = 47,
        GET_ECC_CORRECTION_INFO      = 48,
        VIP_CREATE_CLIENT_AUTH_TOKEN = 49,
        VIP_PROVISION_OTP_SEED       = 50,
        VIP_CREATE_TOTP_TOKEN        = 51,
        TEST_RESET_OWNERSHIP         = 52,
        POKE32                       = 53,/* eagle */
        INPLACE_PATCH                = 54, /* eagle */
        GEN_SEC_KEY                  = 55,
        GET_CFGTSBL                  = 56,/* eagle */
        LED_EX2                      = 57, /* eagle */
        SPI_FLASH                    = 58, /* eagle */
        TEST_OTP_READ_EX             = 59, /* eagle */
        TEST_OTP_WRITE_EX            = 60, /* eagle */
        TEST_GET_SPECIAL_GPIO_STATE  = 61,
        STORAGE_INFO_GET             = 62, /* windsor */
        STORAGE_PART_FORMAT          = 63, /* windsor */
        STORAGE_PART_READ            = 64, /* windsor */
        STORAGE_PART_WRITE           = 65, /* windsor */
        STORAGE_FWPART_VALIDATE      = 66, /* windsor */
        STORAGE_FWPART_INFO_GET      = 67, /* windsor */
        TLS_HANDSHAKE                = 68, /* windsor */
        DB_INFO_GET                  = 69, /* windsor */
        DB_OBJECT_INFO_GET           = 70, /* windsor */
        DB_OBJECT_CREATE             = 71, /* windsor */
        DB_OBJECT_DELETE             = 72, /* windsor */
        DB_OBJECT_DATA_GET           = 73, /* windsor */
        DB_USER_INFO_GET             = 74, /* windsor */
        DB_USER_CONTAINER_INFO_GET   = 75, /* windsor */
        DB_OBJECT_RESTORE            = 76, /* windsor */
        DB_SYSTEM_PARAM_GET          = 77, /* windsor */
        DB_SYSTEM_PARAM_SET          = 78, /* windsor */
        TAKE_OWNERSHIP_EX2           = 79, /* windsor */
        GET_CERTIFICATE_EX           = 80, /* windsor */
        READ_FPDATA                  = 81, /* windsor */
        FLUSH_FPDATA                 = 82, /* windsor */
        SSL_FAST_RECOVERY            = 83,
        TEST_SET_VCKIV               = 84, /* eagle */
        TEST_GEN_SSK                 = 85, /* eagle */
        TEST_HASH_SHA256             = 86,
        TIDLE_SET                    = 87, /* cosworth */
        SLEEP                        = 88, /* cosworth */
        TEST_SET_MT_PASSED           = 89, /* eagle */
        FIDOOP                       = 90,
        GET_PATCHINFO                = 91,
        GLOBAL_KEY_GEN               = 92,
        GET_NAV_STATUS               = 93, /* metallica */
        AUTH_SESSION_START           = 94,
        AUTH_SESSION_STATUS_GET      = 95,
        AUTH_SESSION_RESULT_GET      = 96,
        AUTH_SESSION_DATA_GET        = 97,
        AUTH_SESSION_FINISH          = 98,
        CAL_WOF2                     = 99, /* metallica */
        CAL_STATUS_GET               = 100, /* metallica */
        TEST_SPIFLASH_OP             = 101, /* metallica */
        TEST_CLOCKTUNE               = 102,
        EVENTSIGCLR                  = 103,
        ENROLL_SESSION_START         = 104, /* metallica */
        ENROLL_SESSION_FINISH        = 105, /* metallica */
        ENROLL_SESSION_STATUS_GET    = 106, /* metallica */
        TEMPLATE_UPDATE              = 107, /* metallica */
        CAL_WOF2_V3                  = 108, /* metallica */
        TSCONFIG_SSC_PROGRAM         = 109, /* metallica */
        SECURE_WRAP                  = 110, /* metallica */
        OTPROM_TAG_FIND              = 111,
        TEST_OTPROM_TAG_WRITE        = 112,
        TEST_OTPROM_TAG_INVALIDATE   = 113,
        IFS_STATS_GET                = 114,
        APPLICATION_REQUEST          = 115,
        STORAGE_FWPART_INFO_GET_V2   = 116,
        CUSTID_GET                   = 117,
        WOF2_CONFIGURE               = 118,
        WOF2_STATUS_GET              = 119,
        READ_RAW_FPDATA              = 120,
        RXSELECT                     = 121, /* viper2 */
		TEST_RXSELECT                = RXSELECT,
		TEMPERATURE_READ             = 122,
		AUTORESTART_STAT_GET         = 123,
		ECHO						 = 124,
		BOOTLDR_PATCH				 = 125,
		GET_NAV_STATUS_V2            = 126,
		FRAME_READ                   = 127,
		FRAME_ACQ                    = 128,
		FRAME_FINISH                 = 129,
		FRAME_STATE_GET              = 130,
		IOTA_WRITE                   = 131,
		FLASH_ERASE                  = 132,
		TEST_SPIFLASH_CHECKSUM       = 133,
		EVENT_CONFIG                 = 134,
		EVENT_READ                   = 135,

		/*
		* These following commands are for ATE, characterization and firmware test
		* patches the also get used in the USDK5 for device specific calibration.
		*/
		TEST_PARAMETER_SET           = 250,
		TEST_CONTINUE                = 251,
		TEST_LENGTH_READ             = 252,
		TEST_READ                    = 253,
		TEST_RUN                     = 254
    };
};
