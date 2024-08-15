#include<iostream>
#include<fstream>
using namespace std;

class Enhancement{
    public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int maskRows;
    int maskCols;
    int maskMin;
    int maskMax;
    int** mirrorFramedAry;
    int** avgAry;
    int** GaussAry;
    int neighborAry[25];
    int maskAry[25];
    int maskWeight;
    int* histAvgAry;
    int* histGaussAry;
    Enhancement(ifstream &inFile,ifstream &maskFile){
        inFile>>numRows;
        inFile>>numCols;
        inFile>>minVal;
        inFile>>maxVal;

        maskFile>>maskRows;
        maskFile>>maskCols;
        maskFile>>maskMin;
        maskFile>>maskMax;

        mirrorFramedAry=new int*[numRows+4];
        avgAry=new int*[numRows+4];
        GaussAry=new int*[numRows+4];
        for(int i=0;i<numRows+4;i++){
            mirrorFramedAry[i]=new int[numCols+4];
            avgAry[i]=new int[numCols+4];
            GaussAry[i]=new int[numCols+4];
        }

        histAvgAry=new int[maxVal+1];
        histGaussAry=new int[maxVal+1];
        for(int i=0;i<maxVal+1;i++){
            histAvgAry[i]=0;
            histGaussAry[i]=0;
        }
        
    }
    void loadImage(ifstream &inFile,int** mirrorFramedAry){
        for(int i=0;i<numRows;i++){
            for(int j=0;j<numCols;j++){
                inFile>>mirrorFramedAry[i+2][j+2];
            }
        }
    }
    void mirrorFraming(int** mirrorFramedAry){
        for(int i=0;i<numRows+4;i++){
            for(int j=1;j>=0;j--){
                mirrorFramedAry[i][j]=mirrorFramedAry[i][2];
                mirrorFramedAry[i][numCols+4-1-j]=mirrorFramedAry[i][numCols+4-3];
            }
        }
        for(int i=0;i<numCols+4;i++){
            for(int j=1;j>=0;j--){
                mirrorFramedAry[j][i]=mirrorFramedAry[2][i];
                mirrorFramedAry[numRows+4-1-j][i]=mirrorFramedAry[numRows+4-3][i];
            }
        }
    }
    int loadMaskAry(ifstream &maskFile,int* maskAry){
        int sum=0;
        for(int i=0;i<25;i++){
            maskFile>>maskAry[i];
            sum+=maskAry[i];
        }
        return sum;
    }
    
    void loadNeighborAry(int** mirrorFramedAry,int i,int j,int* neighborAry){
        int index=0;
        for(int r=-2;r<=2;r++){
            for(int c=-2;c<=2;c++){
                neighborAry[index]=mirrorFramedAry[i+r][j+c];
                index++;
            }
        }
    }
    void computeAvg5x5(int** mirrorFramedAry,int** avgAry,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering computeAvg5x5 method\n";
        //step 1
        int i=2;
        //step 7
        while(i<numRows+2){
            //step 2
            int j=2;
            while(j<numCols+2){
                //step 3
                avgAry[i][j]=avg5x5(mirrorFramedAry,i,j);
                //step 4
                j++;
            }
            //step 6
            i++;
        }
        deBugFile<<"leaving computeAvg5x5 method\n";
    }
    int avg5x5(int** mirrorFramedAry,int i,int j){
        int avg=0;
        loadNeighborAry(mirrorFramedAry,i,j,neighborAry);
        for(int i=0;i<25;i++){
            avg+=neighborAry[i];
        }
        avg/=25;
        return avg;
    }
    void computeGauss5x5(int** mirrorFramedAry,int** GaussAry,int* maskAry,int maskWeight,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering computeGauss method\n";
        //step 1
        int i=2;
        //step 8
        while(i<numRows+2){
            //step 2
            int j=2;
            //step 6
            while(j<numCols+2){
                //step 3
                loadNeighborAry(mirrorFramedAry,i,j,neighborAry);
                //step 4
                GaussAry[i][j]=convolution(neighborAry,maskAry,maskWeight,deBugFile);
                //step 5
                j++;
            }
            //step 7
            i++;
        }
        //step 9
        deBugFile<<"leaving computeGauss method\n";
    }
    int convolution(int* neighborAry,int* maskAry,int weight,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering convolution method\n";
        int result=0;
        //step 1
        int i=0;
        //step 4
        while(i<25){
            //step 2
            result+=neighborAry[i]*maskAry[i];
            //step 3
            i++;
        }
        //step 5
        deBugFile<<"in convolution method, result is: "<<result<<"\n";
        //step 6
        deBugFile<<"leaving convolution method\n";
        //step 7
        return (result/weight);
    }
    void computeHist(int** inAry,int* histAry,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering computeHist method\n";
        //step 1
        int i=2;
        //step 7
        while(i<numRows+2){
            //step 2
            int j=2;
            //step 5
            while(j<numCols+2){
                //step 3
                histAry[inAry[i][j]]++;
                //step 4
                j++;
            }
            //step 6
            i++;
        }
        //step 8
        deBugFile<<"leaving computeHist method\n";
    }
    void imgReformat(int** inAry,ofstream &outFile){
        //step 1
        outFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        //step 2
        string str=to_string(maxVal);
        int width=str.length();
        //step 3
        int r=2;
        //step 12
        while(r<numRows+2){
            //step 4
            int c=2;
            //step 10
            while(c<numCols+2){
                //step 5
                outFile<<inAry[r][c];
                //step 6
                str=to_string(inAry[r][c]);
                int ww=str.length();
                while(ww<width){
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
    void printHist(int* histAry,ofstream &histFile,int maxVal,ofstream &deBugFile){
        //step 0
        deBugFile<<"entering printHist method\n";
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

};

int main(int argc,char** argv){
    //step 0
    ifstream inFile(argv[1]);
    ifstream maskFile(argv[2]);
    ofstream outFile1(argv[4]);
    ofstream deBugFile(argv[5]);
    int choice=stoi(argv[3]);
    Enhancement* e=new Enhancement(inFile,maskFile);
    //step 1
    e->loadImage(inFile,e->mirrorFramedAry);
    //step 2
    e->mirrorFraming(e->mirrorFramedAry);
    //step 3
    e->imgReformat(e->mirrorFramedAry,outFile1);
    outFile1<<"\n";
    //step 4
    if(choice==1){
        e->computeAvg5x5(e->mirrorFramedAry,e->avgAry,deBugFile);
        e->computeHist(e->avgAry,e->histAvgAry,deBugFile);
        e->imgReformat(e->avgAry,outFile1);
        string nameAvg=argv[1];
        nameAvg+="_Avg5x5.txt";
        ofstream avgFile(nameAvg);
        avgFile<<e->numRows<<" "<<e->numCols<<" "<<e->minVal<<" "<<e->maxVal<<"\n";
        for(int i=2;i<e->numRows+2;i++){
            for(int j=2;j<e->numCols+2;j++){
                avgFile<<e->avgAry[i][j]<<" ";
            }
            avgFile<<"\n";
        }
        string avgHist=argv[1];
        avgHist+="_Avg5x5_hist.txt";
        ofstream histAvgFile(avgHist);
        histAvgFile<<e->numRows<<" "<<e->numCols<<" "<<e->minVal<<" "<<e->maxVal<<"\n";
        e->printHist(e->histAvgAry,histAvgFile,e->maxVal,deBugFile);
        histAvgFile.close();
        avgFile.close();

    }
    //step 5
    if(choice==2){
        e->maskWeight=e->loadMaskAry(maskFile,e->maskAry);
        e->computeGauss5x5(e->mirrorFramedAry,e->GaussAry,e->maskAry,e->maskWeight,deBugFile);
        e->computeHist(e->GaussAry,e->histGaussAry,deBugFile);
        e->imgReformat(e->GaussAry,outFile1);
        string nameGauss=argv[1];
        nameGauss+="_Gauss5x5.txt";
        ofstream GaussFile(nameGauss);
        GaussFile<<e->numRows<<" "<<e->numCols<<" "<<e->minVal<<" "<<e->maxVal<<"\n";
        for(int i=2;i<e->numRows+2;i++){
            for(int j=2;j<e->numCols+2;j++){
                GaussFile<<e->GaussAry[i][j]<<" ";
            }
            GaussFile<<"\n";
        }
        string GaussHist=argv[1];
        GaussHist+="_Gauss5x5_hist.txt";
        ofstream histGaussFile(GaussHist);
        histGaussFile<<e->numRows<<" "<<e->numCols<<" "<<e->minVal<<" "<<e->maxVal<<"\n";
        e->printHist(e->histGaussAry,histGaussFile,e->maxVal,deBugFile);
        histGaussFile.close();
        GaussFile.close();
    }
    inFile.close();
    maskFile.close();
    outFile1.close();
    deBugFile.close();
}