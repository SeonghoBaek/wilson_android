/** 
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef _XMLPARSER_H
#define _XMLPARSER_H

#include <ctype.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "XMLTypes.h"

class XMLParser
{
	private:
		XMLParser() {}

	public:
		virtual ~XMLParser() {}
		static XMLParser* getInstance();

	    IXML* parseXML(const char *xml, int length);

	    CommandXML* parseCommandXml(xmlDoc* pDoc, xmlNode* pXmlNode);
	    TestCaseXML* parseTestCaseXML(xmlDoc* pDoc, xmlNode* pXmlNode);

	    char *trimwhiteSpace(char *str)
	    {
		  char *end;

		  // Trim leading space
		  while(isspace(*str)) str++;

		  if(*str == 0)  // All spaces?
			return str;

		  // Trim trailing space
		  end = str + strlen(str) - 1;
		  while(end > str && isspace(*end)) end--;

		  // Write new null terminator
		  *(end+1) = 0;

		  return str;
		}
};
#endif
