#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>

void exercicio1();
void exercicio2();
void exercicio3();


void exercicio1()
{
    GraphViewer* gv = new GraphViewer(600,600,true);
    gv->setBackground("background.jpg");
    gv->createWindow(600,600);
    gv->defineVertexColor("blue");
    gv->defineEdgeColor("black");

    gv->addNode(0);
    gv->addNode(1);
    gv->addEdge(0,0,1,EdgeType::DIRECTED);
    gv->removeNode(1);
    gv->addNode(2);
    gv->addEdge(1,0,2,EdgeType::DIRECTED);
    gv->setVertexLabel(2,"This is node 2.\n");
    gv->setEdgeLabel(1,"This is edge 1.\n");
    gv->setVertexColor(2,"green");
    gv->setEdgeColor(1,"yellow");

    gv->rearrange();

}

void exercicio2()
{
    GraphViewer* gv = new GraphViewer(600,600,false);
    gv->createWindow(600,600);
    gv->addNode(0,300,50);
    gv->addNode(1,318,58);
    gv->addNode(2,325,75);
    gv->addNode(3,318,93);
    gv->addNode(4,300,100);
    gv->addNode(5,282,93);
    gv->addNode(6,275,75);
    gv->addNode(7,282,58);
    gv->addNode(8,150,200);
    gv->addNode(9,300,200);
    gv->addNode(10,450,200);
    gv->addNode(11,300,400);
    gv->addNode(12,200,550);
    gv->addNode(13,400,550);

    gv->addEdge(0,0,1,EdgeType::DIRECTED);
    gv->addEdge(1,1,2,EdgeType::DIRECTED);
    gv->addEdge(2,2,3,EdgeType::DIRECTED);
    gv->addEdge(3,3,4,EdgeType::DIRECTED);
    gv->addEdge(4,4,5,EdgeType::DIRECTED);
    gv->addEdge(5,5,6,EdgeType::DIRECTED);
    gv->addEdge(6,6,7,EdgeType::DIRECTED);
    gv->addEdge(7,7,0,EdgeType::DIRECTED);
    gv->addEdge(8,4,9,EdgeType::DIRECTED);
    gv->addEdge(9,9,8,EdgeType::DIRECTED);
    gv->addEdge(10,9,10,EdgeType::DIRECTED);
    gv->addEdge(11,9,11,EdgeType::DIRECTED);
    gv->addEdge(12,11,12,EdgeType::DIRECTED);
    gv->addEdge(13,11,13,EdgeType::DIRECTED);
/*  //alinea c)
    gv->rearrange();
    sleep(1);
    gv->removeNode(12);
    gv->removeNode(13);
    gv->addNode(14,250,550);
    gv->addNode(15,350,550);

    gv->rearrange();
*/
    //alinea d) exemplo da professora
    bool first = true;
    while(1)
    {
        sleep(1);
        if (first)
        {
            gv->removeNode(12);
            gv->removeNode(13);
            first=false;
        }
        else
        {
            gv->removeNode(20);
            gv->removeNode(21);
        }
        gv->addNode(14,250,550);
        gv->addNode(15,350,550);
        gv->addEdge(14, 11, 14, EdgeType::UNDIRECTED);
        gv->addEdge(15, 11, 15, EdgeType::UNDIRECTED);
        gv->rearrange();
        sleep(1);
        gv->removeNode(14);
        gv->removeNode(15);
        gv->addNode(16,300,550);
        gv->addNode(17,300,550);
        gv->addEdge(16, 11, 16, EdgeType::UNDIRECTED);
        gv->addEdge(17, 11, 17, EdgeType::UNDIRECTED);
        gv->rearrange();
        sleep(1);
        gv->removeNode(16);
        gv->removeNode(17);
        gv->addNode(18,250,550);
        gv->addNode(19,350,550);
        gv->addEdge(18, 11, 18, EdgeType::UNDIRECTED);
        gv->addEdge(19, 11, 19, EdgeType::UNDIRECTED);
        gv->rearrange();
        sleep(1);
        gv->removeNode(18);
        gv->removeNode(19);
        gv->addNode(20,200,550);
        gv->addNode(21,400,550);
        gv->addEdge(20, 11, 20, EdgeType::UNDIRECTED);
        gv->addEdge(21, 11, 21, EdgeType::UNDIRECTED);
        gv->rearrange();
    }
}

void exercicio3()
{
    GraphViewer* gv = new GraphViewer(600,600,false);
    gv->createWindow(600,600);

    ifstream nodeFile;
    nodeFile.open("../resources/mapa1/nos.txt");

    string line;
    while (!nodeFile.eof())
    {
        getline(nodeFile,line);
        int nodeID,nodeX,nodeY;
        sscanf(line.c_str(), "%d;%d;%d", &nodeID, &nodeX, &nodeY);
        gv->addNode(nodeID,nodeX,nodeY);
    }
    nodeFile.close();

    ifstream edgeFile;
    edgeFile.open("../resources/mapa1/arestas.txt");

    while(!edgeFile.eof())
    {
        getline(edgeFile,line);
        int edgeID,nodeOrigin,nodeDest;
        sscanf(line.c_str(),"%d;%d;%d",&edgeID,&nodeOrigin,&nodeDest);
        gv->addEdge(edgeID,nodeOrigin,nodeDest,EdgeType::DIRECTED);
    }
    edgeFile.close();
    gv->rearrange();
}

int main() {
    /*
     * Uncomment the line below to run Exercise 1
     */
    //exercicio1();

    /*
      * Uncomment the line below to run Exercise 2
      */
    //exercicio2();

    /*
      * Uncomment the line below to run Exercise 3
      */
	//
	exercicio3();

	getchar();
	return 0;
}
