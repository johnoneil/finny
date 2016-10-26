#include "radioshark.h"
#include <time.h>
#include "logging.h"


namespace Radioshark
{
	AbstractRadiosharkInterface::AbstractRadiosharkInterface()
	:m_FMFreq(99.5f)
	,m_AMFreq(630.0f)
	,m_pHid(NULL)
	,m_Band(Radioshark::FM)
	{
		
	}
	
	AbstractRadiosharkInterface::~AbstractRadiosharkInterface()
	{
		
	}
	
	bool AbstractRadiosharkInterface::Open(void)
	{
		hid_return ret =  HID_RET_SUCCESS;
		
		if(m_pHid)
		{
			return true;//already opened.
		}
	
		//Initialize the hid library
		ret = hid_init();
		if (ret != HID_RET_SUCCESS) {
			Logger::Write("HID ERROR: Could not init hid.");
			return false;
		}
	
		//Initialize the hid object
		m_pHid = hid_new_HIDInterface();
		if ( !m_pHid ) {
			Logger::Write("HID ERROR: Could not get new HID interface.");
			return false;
		}
	
		//Open the shark
		ret = hid_force_open(m_pHid, 2, &matcher, 3);
		if (ret != HID_RET_SUCCESS) {
			Logger::Write("HID ERROR: Could not open HID");
			//make sure we clean up
			this->Close();
			m_pHid = NULL;
			return false;
		}
		return true;
	}
	void AbstractRadiosharkInterface::Close(void)
	{
		hid_return ret = HID_RET_SUCCESS;
		
		if( !m_pHid)
		{
			return;
		}
	
		//close
		ret = hid_close(m_pHid);
		if (ret != HID_RET_SUCCESS) {
			return;
		}
	
		//delete the hid
		hid_delete_HIDInterface(&m_pHid);
	
		/* Clean up the hid library */
		ret = hid_cleanup();
		if (ret != HID_RET_SUCCESS)
		{
			return;
		}
	}
	
	float AbstractRadiosharkInterface::GetFMFreq(void)
	{
		return m_FMFreq;
	}
	float AbstractRadiosharkInterface::GetAMFreq(void)
	{
		return m_AMFreq;
	}
	Radioshark::Band  AbstractRadiosharkInterface::GetBand(void)
	{
		return m_Band;
	}
	bool  AbstractRadiosharkInterface::ToStationTimeDateString(std::string& str)
	{
	
		char buffer[200];
		time_t t;
		struct tm *tmp;
		t = time(NULL);
		tmp = localtime(&t);
		if (tmp == NULL)
		{
			
		}else{
			strftime(buffer,200, "%a-%b-%d-%y-%I%M%S%p", tmp);
		}
		std::string time(buffer);
	
		if( this->GetBand() == Radioshark::AM)
		{
			sprintf(buffer,"AM%3.0f",this->GetAMFreq());
		}else{
			sprintf(buffer,"FM%3.1f",this->GetFMFreq());
		}
		std::string station(buffer);
		str = station+"_"+time;
	
		return true;
	}
	
	Radioshark2Interface::Radioshark2Interface()
		:AbstractRadiosharkInterface()
	{
		
	}
	Radioshark2Interface::~Radioshark2Interface()
	{
		
	}
	
	void Radioshark2Interface::SetAM(void)
	{
		if(!m_pHid)
		{
			return;
		}
		m_Band = Radioshark::AM;
		
		unsigned char PACKET[SEND_PACKET_LENGTH] = { 0xc1, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00 };
		PACKET[0] = 0x81;
		unsigned short encodedFreq  = ((unsigned short)m_AMFreq * 4 ) + 16300;
		PACKET[1] = (encodedFreq >> 8) & 0xFF;
		PACKET[2] = encodedFreq & 0xFF;
		PACKET[3] = 0xF3;
		PACKET[4] = 0x36;
		PACKET[5] = 0x00;
		PACKET[6] = 0x24;
		
		hid_interrupt_write(m_pHid, WRITE_EP, (char*)PACKET, 
										SEND_PACKET_LENGTH, 10000);
	}
	void Radioshark2Interface:: SetFM(void)
	{
		if(!m_pHid)
		{
			return;
		}
		
		m_Band = Radioshark::FM;
		
		unsigned char PACKET[SEND_PACKET_LENGTH] = { 0xc1, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00 };
	 	PACKET[0] = 0x81;
		unsigned short encodedFreq  = ((m_FMFreq * 10 * 2) - 3 );
		PACKET[1] = (encodedFreq >> 8) & 0xff;
		PACKET[2] = encodedFreq & 0xff;
		PACKET[3] = 0x33;
		PACKET[4] = 0x04;
		PACKET[5] = 0x00;
		PACKET[6] = 0x28;
		
		hid_interrupt_write( m_pHid , WRITE_EP, (char*)PACKET,
												SEND_PACKET_LENGTH, 10000);
	}

	void Radioshark2Interface::SetFMFreq(float freq )
	{
		if( freq < 87.5f )
		{
			freq = 87.5f;
		}
		if( freq >108.0f)
		{
			freq = 87.5f;
		}
		m_FMFreq = freq;
		this->SetFM(); 
	}
	void Radioshark2Interface::SetAMFreq(float freq )
	{
		if( freq < 520.0f )
		{
			freq = 520.0f;
		}
		if( freq >1610.0f)
		{
			freq = 1610.0f;
		}
		m_AMFreq = freq;
		this->SetAM();
	}
	
	void Radioshark2Interface::FMFreqUp( float step/* = 0.1f*/) 
	{
		m_FMFreq+=step;
		if(m_FMFreq > 108.0f )
		{
			m_FMFreq = 87.5f;
		}
		this->SetFM();
	}
	void Radioshark2Interface::FMFreqDown( float step /*= 0.1f*/)
	{
		m_FMFreq-=step;
		if(m_FMFreq < 87.5f )
		{
			m_FMFreq = 108.0f;
		}
		this->SetFM();
	}
		
	void Radioshark2Interface::AMFreqUp( float step /*= 1.0f*/ )
	{
		m_AMFreq+=step;
		if(m_AMFreq > 1610.0f)
		{
			m_AMFreq = 520.0f;
		}
		this->SetAM();
	}
	void Radioshark2Interface::AMFreqDown( float step /*= 1.0f*/ )
	{
		m_AMFreq-=step;
		if(m_AMFreq < 520.0f)
		{
			m_AMFreq = 1610.0f;
		}
		this->SetAM();
	}
	
	void Radioshark2Interface::SetRedLED( bool on )
	{
			if(!m_pHid)
		{
			return;
		}
		
		m_Band = Radioshark::FM;
		
		unsigned char PACKET[SEND_PACKET_LENGTH] = { 0xc1, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00 };
	 	PACKET[0] = 0x84;
		if ( on )
		{
			PACKET[1] = (char)0x1;
		}else{
			PACKET[1] = (char)0x2;
		}
	
		hid_interrupt_write( m_pHid , WRITE_EP, (char*)PACKET,
												SEND_PACKET_LENGTH, 10000);
	}
	void Radioshark2Interface::SetBlueLED( float zero_to_one )
	{
		if(!m_pHid)
		{
			return;
		}
		int intensity = (int)(127.0f * zero_to_one );
		if(intensity < 0)
		{
			intensity = 0;
		}else if( intensity >127)
		{
			intensity = 127;
		}
		unsigned char PACKET[SEND_PACKET_LENGTH] = { 0xc1, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00 };
		PACKET[0] = 0x83;
		PACKET[1] = (char)(intensity);
	
		hid_interrupt_write( m_pHid , WRITE_EP, (char*)PACKET,
												SEND_PACKET_LENGTH, 10000);
	}

}/* end namespace Radioshark */
