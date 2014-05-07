/*Copyright (c) Nova Institute for Neuro-immune Medicine - All Right Reserved 
 * 
 * Written by Patrick Gourdet <patrickgourdet79@gmail.com> and Ryan del Rosario <rdelr011@fiu.edu> May 2014
 */


#include <fstream>
#include <iostream>
#include <cstring>
#include <string>


#include "tinyxml.h"

using namespace std;
const int    _NRELE = 100;
const string _DATA = "data";
string 	nodeNames[_NRELE],
	targetNames[_NRELE],
	sourceNames[_NRELE],
	outcome[_NRELE],
        stringComplete[_NRELE],
        finalOutPut[_NRELE];
        
string  circle = "circle", standard = "standard";
int i = 0;
int y = 0;
int z = 0;
int nrNodes = 0;
int nrEdges = 0;
int nodeS[_NRELE];
int nodeS2[_NRELE];


int main (int argc, char *argv[])
{

    
    string buf;

    if (argc < 2) {
        printf("Error: missing argument 1: xml file input path\n");
        return 1;
    }
    //Open file to read out to
    std::string argStr = argv[1];
    int pos = argStr.find_last_of(".");
    argStr = argStr.substr(0,pos);
    argStr.append(".input");
    string fileOut = argStr;
  
    std::ofstream ofs (fileOut.c_str(), std::ofstream::out);


    /* instantiate the tiny xml document object and load the xml file */
    TiXmlDocument doc(argv[1]);
    bool readsucc = doc.LoadFile();

    /* error loading the file */
    if (!readsucc) {
        printf("Error: failed to open specified file: %s\n", argv[1]);
        return 1;
    }


    /*
     * success loading the file
     */

    /* get a pointer to the first _graphml_ "node" child */
    TiXmlHandle dh(&doc);
    TiXmlElement *ele;
    TiXmlNode *node;
    TiXmlNode *gml = dh.FirstChild("graphml")
                       .FirstChild("graph")
                       .FirstChild("node")
                       .ToNode();

    /* search for nodes */
    for (; gml; gml = gml->NextSibling()) {
        buf = gml->ToElement()->Value();
        /* found node, parse id and name */
        if (buf.compare("node") == 0) {
            // look for data > y:ShapeNode > y:NodeLabel > text
            node = TiXmlHandle(gml).FirstChild().ToNode();
            for ( ; node; node = node->NextSibling()) {
                if (_DATA.compare(node->ToElement()->Value()) != 0) continue;
                if (node->NoChildren()) continue;
                ele = TiXmlHandle(node)
                        .FirstChild("y:ShapeNode")
                        .FirstChild("y:NodeLabel")
                        .ToElement();
                if (! ele) continue;
                else {
		string tempHolder = (gml->ToElement()->Attribute("id"));string tempHolder2 = (ele->GetText());
		nodeNames[i]   = tempHolder;
		nodeNames[i+1] = tempHolder2;
		ofs << nodeNames[i+1]+" ";
		i+=2;
                nrNodes = i;    
		break;
                }
                ofs << endl;
            }
        }
    }

    /* reset gml back to first child of graphml tag */
    gml = dh.FirstChild("graphml")
                       .FirstChild("graph")
                       .FirstChild("node")
                       .ToNode();
	i = 0;
    /* search for edges */
    for (; gml; gml = gml->NextSibling()) {
        buf = gml->ToElement()->Value();

        /* found edge, parse id, source, and target */
        if (buf.compare("edge") == 0) {
            // look for data > y:PolyLineEdge > y:Arrows (attrib:target)
            node = TiXmlHandle(gml).FirstChild().ToNode();
            for ( ; node; node = node->NextSibling()) {
                if (_DATA.compare(node->ToElement()->Value()) != 0) continue;
                if (node->NoChildren()) continue;
                ele = TiXmlHandle(node)
                        .FirstChild("y:PolyLineEdge")
                        .FirstChild("y:Arrows")
                        .ToElement();
                if (! ele) continue;
                else {
                    
		string tempHolder = (gml->ToElement()->Attribute("source"));
                string tempHolder2 = (gml->ToElement()->Attribute("target"));
                string tempHolder3 = (ele->Attribute("target"));
		targetNames[i] = tempHolder2;
	 	sourceNames[i] = tempHolder;
		outcome[i]     = tempHolder3;
		i++;
                nrEdges = i;    
		break;
                }
            }
        }
    }
    //Find and replace meta data with readable info
        cout << endl;
	i = 0;
        bool targetX, sourceX;
	while(z < nrNodes){
            i = 0;
            targetX = false;
            sourceX = false;
              
                string check = nodeNames[z];
		while(i < nrEdges){

               if(outcome[i] == circle)
                   outcome[i] = "PROMOTES";
               if(outcome[i] == standard)
                   outcome[i] = "INHIBITS";

               if(targetNames[i] == check){
                targetX = true;
		targetNames[i] = nodeNames[z+1];
                
               }

	       if(sourceNames[i] == check){
                sourceX = true;
                sourceNames[i] = nodeNames[z+1];
	               }
                	                     i++;
                if(targetX == true && sourceX == true){
                    nodeS[y] = 3;nodeS2[y] = -1;}
                else{
                    nodeS[y] = 1;nodeS2[y] = 0;}
		}z += 2;y++;

    }       
       
        //Write info to file 
       
            ofs << endl;
               
         for(i =0;i<nrNodes/2;i++){
          ofs << nodeS[i]; 
            ofs << ' ';}
                 ofs << endl;
         for(i =0;i<nrNodes/2;i++){
             ofs << nodeS2[i] << "  ";
         }
                 ofs << endl;
        i = 0;
        z = 1;    
        while(z<nrNodes){
            i=0;
            string check = nodeNames[z];
            while(i<nrEdges){
                if(targetNames[i] == check){
              finalOutPut[z].append(" " +  sourceNames[i] + " " + outcome[i]);}
                i++;}
            nodeNames[z].append(" = " + finalOutPut[z]);
            ofs << nodeNames[z] << endl;
                z +=2;

        }ofs.close();
 
return 0;
}
