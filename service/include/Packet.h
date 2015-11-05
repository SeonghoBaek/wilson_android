/**
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef _PACKET_H_
#define _PACKET_H_

#include "Types.h"
#include "XMLTypes.h"
#include "XMLParser.h"

#define NBUS_PACKET_START_CODE "AsTaLaViStA"
#define NBUS_PACKET_HEADER_LEGNTH_BYTES sizeof(unsigned int)

class NBUSPacket
{
	private:
		char* mpHeader;
		int	  mHeaderLength;

		void* mpData;
		unsigned int   mDataLength;
		int   mDataCopied;

		IXML* mpXml;

		void *mpPacket;
		unsigned int   mPacketLength;

	public:
		NBUSPacket()
		{
			mpHeader = NULL;
			mpData = NULL;
			mpXml = NULL;
			mpPacket = NULL;
			mPacketLength = 0;

			mHeaderLength = 0;
			mDataLength = 0;
			mDataCopied = 0;
		}

		virtual ~NBUSPacket();

		int parseHeader(void *data, unsigned int length);
		int parserPacket(void *data, unsigned int length);
		int copyData(void *data, unsigned int length);
		int copyHeader(void *header, unsigned int length);

		bool isCompleted();

		void *getPacket() {return this->mpPacket;}
		unsigned int getPacketLength(){return this->mPacketLength;}

		void *getData() {return this->mpData;}
		void *getHeader() {return this->mpHeader;}
		unsigned int getDataLength() {return this->mDataLength;}
		int getHeaderLength() {return this->mHeaderLength;}

        IXML* getXML() {return mpXml;}
};
#endif
