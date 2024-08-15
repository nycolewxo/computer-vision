#include<iostream>
#include<fstream>
using namespace std;

class point{
    public:
    int row;
    int col;
    point(int r,int c){
        row=r;
        col=c;
    }
};

class chainCode{
    public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int label;
    int** ZFAry;
    int** reconstructAry;
    point coordinateOffSet[8]={point(0,1),point(-1,1),point(-1,0),point(-1,-1),point(0,-1),point(1,-1),point(1,0),point(1,1)};
    int zeroTable[8]={6,0,0,2,2,4,4,6};
    point* startP=new point(0,0);
    point* currentP=new point(0,0);
    point* nextP=new point(0,0);
    int lastQ;
    int chainDir;
    chainCode(ifstream &inFile){
        inFile>>numRows;
        inFile>>numCols;
        inFile>>minVal;
        inFile>>maxVal;
        
        ZFAry=new int*[numRows+2];
        reconstructAry=new int*[numRows+2];
        for(int i=0;i<numRows+2;i++){
            ZFAry[i]=new int[numCols+2];
            reconstructAry[i]=new int[numCols+2];
        }

        for(int i=0;i<numRows+2;i++){
            for(int j=0;j<numCols+2;j++){
                ZFAry[i][j]=0;
                reconstructAry[i][j]=0;
            }
        }

    }
    void loadImage(ifstream &inFile,int** ZFAry){
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                inFile>>ZFAry[i][j];
            }
        }
    }
    void getChainCode(int** ZFAry,ofstream &chainCodeFile,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering getChainCode method\n";
        chainCodeFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        int iRow;
        int jCol;
        //step 1
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                if(ZFAry[i][j]>0){
                    iRow=i;
                    jCol=j;
                    goto chainPixelFound;
                }
            }
        }
        chainPixelFound:
        //step 2
        if(ZFAry[iRow][jCol]>0){
            label=ZFAry[iRow][jCol];
            chainCodeFile<<label<<" "<<iRow<<" "<<jCol<<"\n";
            startP->row=iRow;
            startP->col=jCol;
            currentP->row=iRow;
            currentP->col=jCol;
            lastQ=4;
            bool firstRun=true;
            while(firstRun==true||(currentP->row!=startP->row)||(currentP->col!=startP->col)){
                firstRun=false;
                //step 3
                lastQ=(lastQ+1)%8;
                //step 4
                chainDir=findNextP(currentP,lastQ,label,deBugFile);
                //step 5
                chainCodeFile<<chainDir<<" ";
                //step 6
                nextP->row=currentP->row+coordinateOffSet[chainDir].row;
                nextP->col=currentP->col+coordinateOffSet[chainDir].col;
                ZFAry[nextP->row][nextP->col]=label+4;
                currentP=nextP;
                //step 7
                if(chainDir==0){
                    lastQ=zeroTable[7];
                }
                else{
                    lastQ=zeroTable[chainDir-1];
                }
                //step 8
                deBugFile<<"lastQ="<<lastQ<<"; currentP.row="<<currentP->row<<"; currentP.col="<<currentP->col<<"; nextP.row="<<nextP->row<<"; nextP.col="<<nextP->col<<"\n";
            }
        }
        deBugFile<<"leaving getChainCode method\n";
    }
    int findNextP(point* currentP,int lastQ,int label,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering findNextP method\n";
        //step 1
        int index=lastQ;
        bool found=false;
        //step 4
        while(found!=true){
            //step 2
            int iRow=currentP->row+coordinateOffSet[index].row;
            int jCol=currentP->col+coordinateOffSet[index].col;
            //step 3
            if(ZFAry[iRow][jCol]==label||ZFAry[iRow][jCol]==(label+4)){
                chainDir=index;
                found=true;
            }
            else{
                index=(index+1)%8;
            }
        }
        //step 5
        deBugFile<<"leaving findNextP method\n";
        deBugFile<<"chainDir="<<chainDir<<"\n";
        //step 6
        return chainDir;
    }
    void constructBoundary(int** reconstructAry,ifstream &chainCodeFile){
        chainCodeFile>>numRows;
        chainCodeFile>>numCols;
        chainCodeFile>>minVal;
        chainCodeFile>>maxVal;
        chainCodeFile>>label;
        int iRow;
        int jCol;
        chainCodeFile>>iRow;
        chainCodeFile>>jCol;
        reconstructAry[iRow][jCol]=label;
        while(chainCodeFile>>chainDir){
            iRow+=coordinateOffSet[chainDir].row;
            jCol+=coordinateOffSet[chainDir].col;
            reconstructAry[iRow][jCol]=label;
        }
    }
    void reformatPrettyPrint(int** inAry,ofstream &outFile){
        //step 1
        outFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        //step 2
        string str=to_string(maxVal);
        int width=str.length();
        //step 3
        int r=1;
        //step 12
        while(r<numRows+1){
            //step 4
            int c=1;
            //step 10
            while(c<numCols+1){
                //step 5
                if(inAry[r][c]==0){
                    outFile<<".";
                }
                else{
                    outFile<<inAry[r][c];
                }
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
    void imgReformat(int** inAry,ofstream &outFile){
        //step 1
        outFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        //step 2
        string str=to_string(maxVal);
        int width=str.length();
        //step 3
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
    ofstream outFile(argv[2]);
    ofstream deBugFile(argv[3]);
    chainCode* c=new chainCode(inFile);

    //step 1
    string chainCodeFileName=argv[1];
    chainCodeFileName+="_chainCode.txt";
    string boundaryFileName=argv[1];
    boundaryFileName+="_boundary.txt";

    //step 2
    ofstream chainCodeFile(chainCodeFileName);
    ofstream boundaryFile(boundaryFileName);

    //step 3
    c->loadImage(inFile,c->ZFAry);
    deBugFile<<"after loadImage method; ZFAry as below: \n";
    c->reformatPrettyPrint(c->ZFAry,outFile);

    //step 4
    c->getChainCode(c->ZFAry,chainCodeFile,deBugFile);
    deBugFile<<"after getChainCode method; ZFAry as below: \n";
    c->reformatPrettyPrint(c->ZFAry,outFile);

    //step 5
    chainCodeFile.close();
    //step 6
    ifstream chainCodeFile1(chainCodeFileName);

    //step 7
    c->constructBoundary(c->reconstructAry,chainCodeFile1);
    c->reformatPrettyPrint(c->reconstructAry,outFile);

    //step 8
    c->imgReformat(c->reconstructAry,boundaryFile);
    
    //step 9
    chainCodeFile1.close();
    boundaryFile.close();
    deBugFile.close();
    outFile.close();
    inFile.close();
}