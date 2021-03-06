
#include "Arduino.h"

#include <Wire.h>
#define ADS1115_IIC_ADDRESS0         (0x48)
#define ADS1115_IIC_ADDRESS1         (0x49)
#define ADS1115_CONVERSIONDELAY         (100)
#define ADS1115_POINTER_MASK        (0x03)
#define ADS1115_POINTER_CONVERT     (0x00)
#define ADS1115_POINTER_CONFIG      (0x01)
#define ADS1115_POINTER_LOWTHRESH   (0x02)
#define ADS1115_POINTER_HITHRESH    (0x03)

#define ADS1115_OS_MASK      (0x0001)    // Operational status/single-shot conversion start
#define ADS1115_OS_NOEFFECT  (0x0000)    // Write: Bit = 0
#define ADS1115_OS_SINGLE    (0x0001)    // Write: Bit = 1
#define ADS1115_OS_BUSY      (0x0000)    // Read: Bit = 0
#define ADS1115_OS_NOTBUSY   (0x0001)    // Read: Bit = 1

#define ADS1115_MUX_MASK     (0x0007)    // Input multiplexer configuration
#define ADS1115_MUX_1        (0x0000)    // P = AIN0, N = AIN1(default)
#define ADS1115_MUX_2        (0x0001)    // P = AIN0, N = AIN3
#define ADS1115_MUX_3        (0x0002)    // P = AIN1, N = AIN3
#define ADS1115_MUX_4        (0x0003)    // P = AIN2, N = AIN3
#define ADS1115_MUX_5        (0x0004)    // P = AIN0, N = GND
#define ADS1115_MUX_6        (0x0005)    // P = AIN1, N = GND
#define ADS1115_MUX_7        (0x0006)    // P = AIN2, N = GND
#define ADS1115_MUX_8        (0x0007)    // P = AIN3, N = GND

#define ADS1115_PGA_MASK     (0x0008)    // Programmable gain amplifier configuration
#define ADS1115_PGA_0        (0x0000)    // 6.144V 
#define ADS1115_PGA_1        (0x0001)    // 4.096V 
#define ADS1115_PGA_2        (0x0002)    // 2.048V (default)
#define ADS1115_PGA_3        (0x0003)    // 1.024V 
#define ADS1115_PGA_4        (0x0004)    // 0.512V 
#define ADS1115_PGA_5        (0x0005)    // 0.256V 

#define ADS1115_MODE_MASK    (0x0001)    // Device operating mode
#define ADS1115_MODE_CONTIN  (0x0000)    // Continuous conversion mode
#define ADS1115_MODE_SINGLE  (0x0001)    // Power-down single-shot mode (default)

#define ADS1115_DR_MASK      (0x0000)    // Data rate(samples per second)
#define ADS1115_DR_1         (0x0000)    // 8 SPS
#define ADS1115_DR_2         (0x0001)    // 16 SPS
#define ADS1115_DR_3         (0x0002)    // 32 SPS
#define ADS1115_DR_4         (0x0003)    // 64 SPS
#define ADS1115_DR_5         (0x0004)    // 128 SPS (default)
#define ADS1115_DR_6         (0x0005)    // 250 SPS
#define ADS1115_DR_7         (0x0006)    // 475 SPS
#define ADS1115_DR_8         (0x0007)    // 860 SPS

#define ADS1115_COMP_MODE_MASK   (0x0001)    // Comparator mode
#define ADS1115_COMP_MODE_TRAD   (0x0000)    // Traditional comparator with hysteresis (default)
#define ADS1115_COMP_MODE_WINDOW (0x0001)    // Window comparator

#define ADS1115_COMP_POL_MASK    (0x0001)    // Comparator polarity
#define ADS1115_COMP_POL_ACTVLOW (0x0000)    // Active low(default)
#define ADS1115_COMP_POL_ACTVHIGH  (0x0001)    // Active high

#define ADS1115_COMP_LAT_MASK    (0x0001)    // Latching comparator
#define ADS1115_COMP_LAT_NONLAT  (0x0000)    // Non-latching comparator (default)
#define ADS1115_COMP_LAT_LATCH   (0x0001)    // Latching comparator

#define ADS1115_COMP_QUE_MASK    (0x0003)    // Comparator queue and disable
#define ADS1115_COMP_QUE_1CONV   (0x0000)    // After one conversions
#define ADS1115_COMP_QUE_2CONV   (0x0001)    // After two conversions
#define ADS1115_COMP_QUE_4CONV   (0x0002)    // After four conversions
#define ADS1115_COMP_QUE_NONE    (0x0003)    // Disable the comparator(default)

typedef enum {
    eAds219_ok,
    eAds219_InitError,
    eAds219_WriteRegError,
    eAds219_ReadRegError,
}eAds219_Status;

typedef enum
{
    eOSMODE_SINGLE       = ADS1115_OS_SINGLE,
    eOSMODE_BUSY         = ADS1115_OS_BUSY,
    eOSMODE_NOTBUSY      = ADS1115_OS_NOTBUSY
    
} eADSOSMode_t;

typedef enum
{
    eGAIN_TWOTHIRDS       = ADS1115_PGA_0,
    eGAIN_ONE             = ADS1115_PGA_1,
    eGAIN_TWO             = ADS1115_PGA_2,
    eGAIN_FOUR            = ADS1115_PGA_3,
    eGAIN_EIGHT           = ADS1115_PGA_4,
    eGAIN_SIXTEEN         = ADS1115_PGA_5
} eADSGain_t;

typedef enum
{
    eMODE_CONTIN          = ADS1115_MODE_CONTIN,
    eMODE_SINGLE          = ADS1115_MODE_SINGLE
} eADSMode_t;

typedef enum
{
    eRATE_8               = ADS1115_DR_1,
    eRATE_16              = ADS1115_DR_2,
    eRATE_32              = ADS1115_DR_3,
    eRATE_64              = ADS1115_DR_4,
    eRATE_128             = ADS1115_DR_5,
    eRATE_250             = ADS1115_DR_6,
    eRATE_475             = ADS1115_DR_7,
    eRATE_860             = ADS1115_DR_8
} eADSRate_t;

typedef enum
{
    eCOMPMODE_TRAD       = ADS1115_COMP_MODE_TRAD,
    eCOMPMODE_WINDOW     = ADS1115_COMP_MODE_WINDOW
} eADSCompMode_t;

typedef enum
{
    eCOMPPOL_LOW         = ADS1115_COMP_POL_ACTVLOW,
    eCOMPPOL_HIGH        = ADS1115_COMP_POL_ACTVHIGH
} eADSCompPol_t;

typedef enum
{
    eCOMPLAT_NONLAT      = ADS1115_COMP_LAT_NONLAT,
    eCOMPLAT_LATCH       = ADS1115_COMP_LAT_LATCH
} eADSCompLat_t;

typedef enum
{
    eCOMPQUE_ONE         = ADS1115_COMP_QUE_1CONV,
    eCOMPQUE_TWO         = ADS1115_COMP_QUE_2CONV,
    eCOMPQUE_FOUR        = ADS1115_COMP_QUE_4CONV,
    eCOMPQUE_NONE        = ADS1115_COMP_QUE_NONE
} eADSCompQue_t;

typedef enum
{
    eADSMUX_1             = ADS1115_MUX_1,
    eADSMUX_2             = ADS1115_MUX_2,
    eADSMUX_3             = ADS1115_MUX_3,
    eADSMUX_4             = ADS1115_MUX_4,
    eADSMUX_5             = ADS1115_MUX_5,
    eADSMUX_6             = ADS1115_MUX_6,
    eADSMUX_7             = ADS1115_MUX_7,
    eADSMUX_8             = ADS1115_MUX_8
} eADSMux_t;

typedef enum
{
    eADSOS_NOEFFECT       = ADS1115_OS_NOEFFECT,
    eADSOS_SINGLE         = ADS1115_OS_SINGLE,
    eADSOS_BUSY           = ADS1115_OS_BUSY,
    eADSOS_NOTBUSY        = ADS1115_OS_NOTBUSY
} eADSOs_t;

class DFRobot_ADS1115
{
    protected:
        // Instance-specific properties
        uint8_t ads_conversionDelay;
        int16_t ads_lowthreshold;
        int16_t ads_highthreshold;
        eADSOSMode_t ads_osmode;
        eADSGain_t ads_gain;
        eADSMode_t ads_mode;
        eADSRate_t ads_rate;
        eADSCompMode_t ads_compmode;
        eADSCompPol_t ads_comppol;
        eADSCompLat_t ads_complat;
        eADSCompQue_t ads_compque;
        void writeReg(uint8_t i2cAddress, uint8_t reg, uint8_t *pBuf, uint16_t len);
        void readReg(uint8_t i2cAddress, uint8_t reg, uint8_t *pBuf, uint16_t len);
        int16_t readAdsReg(uint8_t i2cAddress, uint8_t reg);
        void writeAdsReg(uint8_t i2cAddress, uint8_t reg, uint16_t value);
        
        TwoWire   *_pWire;

    public:
        DFRobot_ADS1115(TwoWire *pWire){_pWire = pWire;};
        
        uint8_t ads_i2cAddress;
        void setAddr_ADS1115(uint8_t i2cAddress);
        void init(void);
        bool checkADS1115();
        void setCompQue(eADSCompQue_t value),
             setCompLat(eADSCompLat_t value),
             setCompPol(eADSCompPol_t value),
             setCompMode(eADSCompMode_t value),
             setRate(eADSRate_t value),
             setMode(eADSMode_t value),
             setGain(eADSGain_t value),
             setMux(eADSMux_t value),
             setOSMode(eADSOSMode_t value);
        uint16_t readVoltage(uint8_t channel);
        int16_t ComparatorVoltage(uint8_t channel);
        int16_t getLastConversionResults();
        void setLowThreshold(int16_t threshold);
        int16_t   getLowThreshold();
        void setHighThreshold(int16_t threshold);
        int16_t   getHighThreshold();

    private:
};

