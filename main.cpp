#include <iostream>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "tinyxml2.h"
using namespace std;
using namespace tinyxml2;
#define sz(x) int(x.size())
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
char * inttoch(int a)
{
	static char buf[15];
	sprintf(buf,"%d",a);
	return buf;
}
string changeTag(string str,char * tagname, char * value)
{
	string res;
	int i=0;
	while(i<sz(str))
	{
		if(str[i]!='$') { res=res+str[i]; ++i;}
		else
		{
			int idstart=i;
			int idfinish=i+1;
			while(idfinish<sz(str) && str[idfinish]!='$') ++idfinish;
			if(idfinish<sz(str))
			{
				string foundtag=str.substr(idstart,idfinish+1-idstart);
				string tagnam="$$";
				tagnam.insert(1,tagname);
				if(tagnam.compare(foundtag)==0)
					{res+=value; i=idfinish+1;}
						else { res=res+str[i]; ++i;}
			}
			else { res=res+str[i]; ++i;}
		}
	}
	return res;
}
int main()
{
	string filename="OfForestAndMen_10s_onDemand_2014_05_09.mpd";
	string begin="And$acc$Men$num$.mp4";
	begin=changeTag(begin,"num",inttoch(12));
	cout<<begin<<endl;
	begin=changeTag(begin,"acc",inttoch(2));
	cout<<begin<<endl;
	
	
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
