#include<iostream>
#include<fstream>
using namespace std;

class Edge{
    public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;

    int* histRobertAry;
    int* histSobelAry;

    int** mirrorFramedAry;
    int** RobertEdgeAry;
    int** SobelEdgeAry;

    int RobertVertical[2][2]={{1,-1},{1,-1}};
    int RobertHorizontal[2][2]={{1,1},{-1,-1}};
    int RobertRightDiag[2][2]={{1,-1},{-1,1}};
    int RobertLeftDiag[2][2]={{-1,1},{1,-1}};

    int SobelVertical[3][3]={{1,0,-1},{2,0,-2},{1,0,-1}};
    int SobelHorizontal[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
    int SobelRightDiag[3][3]={{2,1,0},{1,0,-1},{0,-1,-2}};
    int SobelLeftDiag[3][3]={{0,1,2},{-1,0,1},{-2,-1,0}};
    Edge(ifstream &inFile){
        inFile>>numRows;
        inFile>>numCols;
        inFile>>minVal;
        inFile>>maxVal;

        mirrorFramedAry=new int*[numRows+2];
        RobertEdgeAry=new int*[numRows+2];
        SobelEdgeAry=new int*[numRows+2];
        for(int i=0;i<numRows+2;i++){
            mirrorFramedAry[i]=new int[numCols+2];
            RobertEdgeAry[i]=new int[numCols+2];
            SobelEdgeAry[i]=new int[numCols+2];
        }
        for(int i=0;i<numRows+2;i++){
            for(int j=0;j<numCols+2;j++){
                mirrorFramedAry[i][j]=0;
                RobertEdgeAry[i][j]=0;
                SobelEdgeAry[i][j]=0;
            }
        }
    }
    void loadImage(ifstream &inFile,int** mirrorFramedAry){
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                inFile>>mirrorFramedAry[i][j];
            }
        }
    }
    void mirrorFraming(int** mirrorFramedAry){
        for(int c=0;c<numCols+2;c++){
            mirrorFramedAry[0][c]=mirrorFramedAry[1][c];
            mirrorFramedAry[numRows+1][c]=mirrorFramedAry[numRows][c];
        }
        for(int r=0;r<numRows+2;r++){
            mirrorFramedAry[r][0]=mirrorFramedAry[r][1];
            mirrorFramedAry[r][numCols+1]=mirrorFramedAry[r][numCols];
        }

    }
    void robertEdgeDetector(int** mirrorFramedAry,int** RobertEdgeAry,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering roberEdgeDetector method\n";
        int newMax=0;
        int newMin=99999;
        //step 1
        int i=1;
        //step 7
        while(i<=numRows){
            //step 2
            int j=1;
            //step 5
            while(j<=numCols){
                //step 3
                int tempV=abs(computeRobert(i,j,RobertVertical));
                int tempH=abs(computeRobert(i,j,RobertHorizontal));
                int tempR=abs(computeRobert(i,j,RobertRightDiag));
                int tempL=abs(computeRobert(i,j,RobertLeftDiag));
                RobertEdgeAry[i][j]=tempV+tempH+tempR+tempL;
                if(RobertEdgeAry[i][j]>newMax){
                    newMax=RobertEdgeAry[i][j];
                }
                if(RobertEdgeAry[i][j]<newMin){
                    newMin=RobertEdgeAry[i][j];
                }
                //step 4
                j++;
            }
            //step 6
            i++;
        }
        maxVal=newMax;
        minVal=newMin;
        //step 8
        histRobertAry=new int[maxVal+1];
        for(int i=0;i<maxVal+1;i++){
            histRobertAry[i]=0;
        }
        deBugFile<<"leaving roberEdgeDetector method\n";
    }
    int computeRobert(int i,int j,int mask[2][2]){
        int value=0;
        for(int r=0;r<2;r++){
            for(int c=0;c<2;c++){
                value+=(mirrorFramedAry[i+r][j+c]*mask[r][c]);
            }
        }
        return value;
    }
    void sobelEdgeDetector(int** mirrorFramedAry,int** SobelEdgeAry,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering sobelEdgeDetector\n";
        int newMax=0;
        int newMin=99999;
        //step 1
        int i=1;
        //step 7
        while(i<=numRows){
            //step 2
            int j=1;
            //step 5
            while(j<=numCols){
                //step 3
                int tempV=abs(computeSobel(i,j,SobelVertical));
                int tempH=abs(computeSobel(i,j,SobelHorizontal));
                int tempR=abs(computeSobel(i,j,SobelRightDiag));
                int tempL=abs(computeSobel(i,j,SobelLeftDiag));
                SobelEdgeAry[i][j]=tempV+tempH+tempR+tempL;
                if(SobelEdgeAry[i][j]>newMax){
                    newMax=SobelEdgeAry[i][j];
                }
                if(SobelEdgeAry[i][j]<newMin){
                    newMin=SobelEdgeAry[i][j];
                }
                //step 4
                j++;
            }
            //step 6
            i++;
        }
        maxVal=newMax;
        minVal=newMin;
        //step 8
        histSobelAry=new int[maxVal+1];
        for(int i=0;i<maxVal+1;i++){
            histSobelAry[i]=0;
        }
        deBugFile<<"leaving sobelEdgeDetector method\n";
    }
    int computeSobel(int i,int j,int mask[3][3]){
        int value=0;
        for(int r=0;r<3;r++){
            for(int c=0;c<3;c++){
                value+=(mirrorFramedAry[i-1+r][j-1+c]*mask[r][c]);
            }
        }
        return value;
    }
    void computeHist(int** inAry,int* histAry,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering computeHist method\n";
        //step 1
        int i=1;
        //step 7
        while(i<=numRows){
            //step 2
            int j=1;
            //step 5
            while(j<=numCols){
                //step 3
                histAry[inAry[i][j]]++;
                //step 4
                j++;
            }
            //step 6
            i++;
        }
        deBugFile<<"leaving computeHist method\n";
    }
    void printHist(int* histAry,ofstream &histFile,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering printHist method\n";
        histFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        //step 1
        int index=0;
        //step 4
        while(index<=maxVal){
            //step 2
            histFile<<index<<" "<<histAry[index]<<"\n";
            //step 3
            index++;
        }
        //step 5
        deBugFile<<"leaving printHist method\n";
    }
    void imgReformat(int** inAry,ofstream &outFile){
        //step 1
        outFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        //step 2
        string str=to_string(maxVal);
        int width=str.length();
        //step 3 fix
        int r=1;
        //step 12
        while(r<numRows+1){
            //step 4
            int c=1;
            //step 10
            while(c<numCols+1){
                //step 5
                outFile<<inAry[r][c];
                //step 6
                str=to_string(inAry[r][c]);
                int ww=str.length();
                while(ww<=width){
                    //step 7
                    outFile<<" ";
                    ww++;
                }
                //step 9
                c++;
            }
            //step 11
            r++;
            outFile<<"\n";
        }
    }
};

int main(int argc,char** argv){
    //step 0
    ifstream inFile(argv[1]);
    int choice=stoi(argv[2]);
    ofstream outFile(argv[3]);
    ofstream deBugFile(argv[4]);
    Edge* e=new Edge(inFile);
    //step 1
    e->loadImage(inFile,e->mirrorFramedAry);
    //step 2
    e->mirrorFraming(e->mirrorFramedAry);
    //step 3
    e->imgReformat(e->mirrorFramedAry,outFile);
    //step 4
    if(choice==1){
        e->robertEdgeDetector(e->mirrorFramedAry,e->RobertEdgeAry,deBugFile);
        e->computeHist(e->RobertEdgeAry,e->histRobertAry,deBugFile);
        string nameRobertEdge=argv[1];
        nameRobertEdge+="_Robert_edge.txt";
        ofstream RobertEdgeFile(nameRobertEdge);
        e->imgReformat(e->RobertEdgeAry,outFile);
        e->imgReformat(e->RobertEdgeAry,RobertEdgeFile);
        string nameRobertHist=argv[1];
        nameRobertHist+="_Robert_hist.txt";
        ofstream RobertHistFile(nameRobertHist);
        e->printHist(e->histRobertAry,RobertHistFile,deBugFile);
        RobertEdgeFile.close();
        RobertHistFile.close();
    }
    if(choice==2){
        e->sobelEdgeDetector(e->mirrorFramedAry,e->SobelEdgeAry,deBugFile);
        e->computeHist(e->SobelEdgeAry,e->histSobelAry,deBugFile);
        string nameSobelEdge=argv[1];
        nameSobelEdge+="_Sobel_edge.txt";
        ofstream SobelEdgeFile(nameSobelEdge);
        e->imgReformat(e->SobelEdgeAry,outFile);
        e->imgReformat(e->SobelEdgeAry,SobelEdgeFile);
        string nameSobelHist=argv[1];
        nameSobelHist+="_Sobel_hist.txt";
        ofstream SobelHistFile(nameSobelHist);
        e->printHist(e->histSobelAry,SobelHistFile,deBugFile);
        SobelEdgeFile.close();
        SobelHistFile.close();

    }
    inFile.close();
    outFile.close();
    deBugFile.close();
}