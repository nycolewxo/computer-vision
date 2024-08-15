#include<iostream>
#include<fstream>
using namespace std;

class ImageCompression{
    public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMinVal;
    int newMaxVal;
    int** ZFAry;
    int** skeletonAry;
    ImageCompression(ifstream &inFile){
        inFile>>numRows;
        inFile>>numCols;
        inFile>>minVal;
        inFile>>maxVal;

        newMinVal=minVal;
        newMaxVal=maxVal;

        ZFAry=new int*[numRows+2];
        skeletonAry=new int*[numRows+2];
        for(int i=0;i<numRows+2;i++){
            ZFAry[i]=new int[numCols+2];
            skeletonAry[i]=new int[numCols+2];
        }
    }
    void setZero(int** ary){
        for(int i=0;i<numRows+2;i++){
            for(int j=0;j<numCols+2;j++){
                ary[i][j]=0;
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
    void loadSkeleton(ifstream &skeletonFile,int** ZFAry,ofstream &deBugFile){
        skeletonFile>>numRows;
        skeletonFile>>numCols;
        skeletonFile>>newMinVal;
        skeletonFile>>newMaxVal;
        int i;
        int j;
        int val;
        while(skeletonFile>>i&&skeletonFile>>j&&skeletonFile>>val){
            ZFAry[i][j]=val;
        }
    }
    int myMin4(int a,int b,int c,int d){
        return min(min(min(a,b),c),d);
    }

    void distance8Pass(int** ZFAry,ofstream &outFile,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering distance8Pass() method\n";
        //step 1
        distance8Pass1(ZFAry,deBugFile);
        //step 2
        outFile<<"1st Pass Distance Transform: \n";
        imgReformat(ZFAry,outFile);
        //step 3
        distance8Pass2(ZFAry,deBugFile);
        //step 4
        outFile<<"2nd Pass Distance Transform: \n";
        imgReformat(ZFAry,outFile);
        //step 5
        deBugFile<<"leaving distance8() method\n";
    }
    void distance8Pass1(int** ZFAry,ofstream &deBugFile){
        newMinVal=999;
        newMaxVal=0;
        deBugFile<<"entering distance8pass1() method\n";
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                if(ZFAry[i][j]>0){
                    ZFAry[i][j]=1+myMin4(ZFAry[i-1][j-1],ZFAry[i-1][j],ZFAry[i-1][j+1],ZFAry[i][j-1]);
                }
                if(newMinVal>ZFAry[i][j]){
                    newMinVal=ZFAry[i][j];
                }
                if(newMaxVal<ZFAry[i][j]){
                    newMaxVal=ZFAry[i][j];
                }
            }
        }
        deBugFile<<"leaving distance8pass1() method\n";
    }
    void distance8Pass2(int** ZFAry,ofstream &deBugFile){
        newMinVal=999;
        newMaxVal=0;
        deBugFile<<"entering distanc8pass2() method\n";
        for(int i=numRows;i>0;i--){
            for(int j=numCols;j>0;j--){
                if(ZFAry[i][j]>0){
                    ZFAry[i][j]=min(myMin4(ZFAry[i][j+1],ZFAry[i+1][j-1],ZFAry[i+1][j],ZFAry[i+1][j+1])+1,ZFAry[i][j]);
                }
                if(newMinVal>ZFAry[i][j]){
                    newMinVal=ZFAry[i][j];
                }
                if(newMaxVal<ZFAry[i][j]){
                    newMaxVal=ZFAry[i][j];
                }
            }
        }
        deBugFile<<"leaving distance8pass2() method\n";

    }
    void imageCompression(int** ZFAry,int** skeletonAry,ofstream &skeletonFile,ofstream &outFile,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering imageCompression() method\n";
        //step 1
        computeLocalMaxima(ZFAry,skeletonAry,deBugFile);
        //step 2
        outFile<<"Local Maxima Skeleton Array: \n";
        imgReformat(skeletonAry,outFile);
        //step 3
        skeletonFile<<numRows<<" "<<numCols<<" "<<newMinVal<<" "<<newMaxVal<<"\n";
        extractSkeleton(skeletonAry,skeletonFile,deBugFile);
        //step 4
        deBugFile<<"leaving imageCompression() method\n";
    }
    int myMaxNeighbors(int** ZFAry,int i,int j){
        int a=ZFAry[i-1][j-1];
        int b=ZFAry[i-1][j];
        int c=ZFAry[i-1][j+1];
        int d=ZFAry[i][j-1];
        int e=ZFAry[i][j+1];
        int f=ZFAry[i+1][j-1];
        int g=ZFAry[i+1][j];
        int h=ZFAry[i+1][j+1];
        return max(max(max(max(max(max(max(a,b),c),d),e),f),g),h);
    }
    bool isLocalMaxima(int** ZFAry,int i,int j){
        if(ZFAry[i][j]>=myMaxNeighbors(ZFAry,i,j)){
            return true;
        }
        return false;
    }
    void computeLocalMaxima(int** ZFAry,int** skeletonAry,ofstream &deBugFile){
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                if(isLocalMaxima(ZFAry,i,j)){
                    skeletonAry[i][j]=ZFAry[i][j];
                }
                else{
                    skeletonAry[i][j]=0;
                }
            }
        }
    }
    void extractSkeleton(int** skeletonAry,ofstream &skeletonFile,ofstream &deBugFile){
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                if(skeletonAry[i][j]>0){
                    skeletonFile<<i<<" "<<j<<" "<<skeletonAry[i][j]<<"\n";
                }
            }
        }
    }
    void imageDecompression(int** ZFAry,ofstream &outFile,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering imageCompression() method\n";
        //step 1
        expansionPass1(ZFAry,deBugFile);
        //step 2
        outFile<<"1st Pass Expansion: \n";
        imgReformat(ZFAry,outFile);
        //step 3
        expansionPass2(ZFAry,deBugFile);
        //step 4
        outFile<<"2nd Pass Expansion: \n";
        imgReformat(ZFAry,outFile);
        //step 5
        deBugFile<<"leaving imageDecompression() method\n";
    }
    void expansionPass1(int** ZFAry,ofstream &deBugFile){
        deBugFile<<"entering expansionPass1() method\n";
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                if(ZFAry[i][j]==0){
                    ZFAry[i][j]=max(myMaxNeighbors(ZFAry,i,j)-1,ZFAry[i][j]);
                }
            }
        }
        deBugFile<<"leaving expansionPass1() method\n";
    }
    void expansionPass2(int** ZFAry,ofstream &deBugFile){
        deBugFile<<"entering expansionPass2() method\n";
        for(int i=numRows;i>0;i--){
            for(int j=numCols;j>0;j--){
                int newMax=max(myMaxNeighbors(ZFAry,i,j)-1,ZFAry[i][j]);
                if(ZFAry[i][j]<newMax){
                    ZFAry[i][j]=newMax;
                }
            }
        }
        deBugFile<<"leaving expansionPass2() method\n";
    }
    void threshold(int** ZFAry,ofstream &deCompressFile,int thr){
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                if(ZFAry[i][j]>=thr){
                    deCompressFile<<"1 ";
                }
                else{
                    deCompressFile<<"0 ";
                }
            }
            deCompressFile<<"\n";
        }
    }
    void imgReformat(int** ZFAry,ofstream &outFile){
        //step 1
        outFile<<numRows<<" "<<numCols<<" "<<newMinVal<<" "<<newMaxVal<<"\n";
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
                outFile<<ZFAry[r][c];
                //step 6
                str=to_string(ZFAry[r][c]);
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
        outFile<<"\n";
    }
};
int main(int argc,char** argv){
    //step 0
    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);
    ofstream deBugFile(argv[3]);
    ImageCompression* iC=new ImageCompression(inFile);

    //step 1
    string skeletonFileName=argv[1];
    skeletonFileName+="_skeleton.txt";
    //step 2
    ofstream skeletonFile(skeletonFileName);
    //step 3
    string deCompressedFileName=argv[1];
    deCompressedFileName+="_deCompressed.txt";
    //step 4
    ofstream deCompressedFile(deCompressedFileName);
    //step 5
    iC->setZero(iC->ZFAry);
    iC->setZero(iC->skeletonAry);
    //step 6
    iC->loadImage(inFile,iC->ZFAry);
    //step 7
    iC->distance8Pass(iC->ZFAry,outFile,deBugFile);
    //step 8
    iC->imageCompression(iC->ZFAry,iC->skeletonAry,skeletonFile,outFile,deBugFile);
    //step 9
    skeletonFile.close();
    //step 10
    ifstream skeletonFile1(skeletonFileName);
    //step 11
    iC->setZero(iC->ZFAry);
    //step 12
    iC->loadSkeleton(skeletonFile1,iC->ZFAry,deBugFile);
    //step 13
    iC->imageDecompression(iC->ZFAry,outFile,deBugFile);
    //step 14
    deCompressedFile<<iC->numRows<<" "<<iC->numCols<<" "<<iC->minVal<<" "<<iC->maxVal<<"\n";
    //step 15
    iC->threshold(iC->ZFAry,deCompressedFile,1);
    //step 16
    inFile.close();
    outFile.close();
    deBugFile.close();
    skeletonFile.close();
    deCompressedFile.close();
}