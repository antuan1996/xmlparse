#include <iostream>
#include <string>
#include <cstring>
#include "tinyxml2.h"
using namespace std;
using namespace tinyxml2;

XMLElement *findRepresentbyAttribute(XMLElement *curn,char *attname, char * value)
{
	while(curn)
	{
		const char * curvalue=curn->Attribute(attname);
		if(strcmp(curvalue,value)==0) return curn;
		curn=curn->NextSiblingElement("Representation");
	}
	return curn;
}
int main()
{
	string filename="OfForestAndMen_10s_onDemand_2014_05_09.mpd";
	XMLDocument doc;
	if(doc.LoadFile(filename.c_str()))
	{
		cout<<"Error of Reading file"<<endl;	
	}
	else
	{
		XMLHandle dochand(&doc);
		dochand=dochand.FirstChildElement("MPD");
		cout<<"File opened"<<endl;
		XMLElement* period=dochand.FirstChildElement("Period").ToElement();
		while(period)
		{
			string duration=period->Attribute("duration");
			cout<<"Period with duration "<<duration<<endl;
			
			XMLElement* adaptation=period->FirstChildElement("AdaptationSet");
			while(adaptation)
			{
				cout<<"Adaptation with par "<<adaptation->Attribute("par")<<endl;
				XMLElement *nesspres=adaptation->FirstChildElement("Representation");
				cout<<"Finding video with codec avc1.42c01f..."<<endl;
				nesspres=findRepresentbyAttribute(nesspres,"codecs","avc1.42c01f");
				if(nesspres)
				{
					 cout<<"I found nessesary video!"<<endl;
					 string url=nesspres->FirstChildElement("BaseURL")->GetText();
					 cout<<"Url is:\n"<<url<<endl;
				}
				else cout<<"necessary video not found";
				adaptation=adaptation->NextSiblingElement("Adaptation");
			}
			
			period=period->NextSiblingElement("Period");
		}
	}
	return 0;
}
