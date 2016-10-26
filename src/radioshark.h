#ifndef __RADIOSHARK_H__
#define __RADIOSHARK_H__

#include <unistd.h>
#include <stdbool.h>
#include <hid.h>
#include <string>



namespace Radioshark
{
	enum Band
	{
		FM = 0,
		AM = 1
	};
	
	const static unsigned int SHARK_VENDID=0x077d;/* Griffin's Vendor ID */
	const static unsigned int SHARK_DEVID=0x627a;/* The radioSHARK's Device ID */

	const static unsigned int READ_EP=0x5;/* libhid read command? */
	const static unsigned int WRITE_EP=0x5;/* libhid write command? */
	const static unsigned int SEND_PACKET_LENGTH= 7;/* size of an instruction packet: shark2=7 */

	const static HIDInterfaceMatcher matcher = { SHARK_VENDID, SHARK_DEVID, NULL, NULL, 0 };

	class AbstractRadiosharkInterface{
		public:
		AbstractRadiosharkInterface();
		virtual ~AbstractRadiosharkInterface();
		
		bool Open(void);
		void Close(void);
	
		float GetFMFreq(void);
		float GetAMFreq(void);
		
		Radioshark::Band GetBand(void);
	
		virtual void SetAM(void) = 0;
		virtual void SetFM(void) = 0;
	
		virtual void SetFMFreq( float freq ) = 0;
		virtual void SetAMFreq( float freq ) = 0;
		
		virtual void FMFreqUp( float step = 0.1f) = 0;
		virtual void FMFreqDown( float step = 0.1f) = 0;
		
		virtual void AMFreqUp( float step = 1.0f ) = 0;
		virtual void AMFreqDown( float step = 1.0f ) = 0;
		
		virtual void SetRedLED( bool on )=0;
		virtual void SetBlueLED( float zero_to_one ) = 0;
		
		bool ToStationTimeDateString(std::string& str);
		
		protected:
		float m_FMFreq;
		float m_AMFreq;
		HIDInterface* m_pHid;
		Radioshark::Band m_Band;
	};
	
	class Radioshark2Interface : public AbstractRadiosharkInterface
	{
	public:
		Radioshark2Interface();
		virtual ~Radioshark2Interface();
		
		virtual void SetAM(void);
		virtual void SetFM(void);
	
		virtual void SetFMFreq(float freq );
		virtual void SetAMFreq(float freq );
		
		virtual void FMFreqUp( float step = 0.1f);
		virtual void FMFreqDown( float step = 0.1f);
		
		virtual void AMFreqUp( float step = 1.0f );
		virtual void AMFreqDown( float step = 1.0f );
		
		virtual void SetRedLED( bool on );
		virtual void SetBlueLED( float zero_to_one );
	};
	
};/* end namespace radioshark */


#endif // __RADIOSHARK_H__
