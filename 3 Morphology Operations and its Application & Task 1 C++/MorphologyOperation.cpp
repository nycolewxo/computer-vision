#include<iostream>
#include<fstream>
using namespace std;

class Morphology{
    public:
    int numImgRows;
    int numImgCols;
    int imgMin;
    int imgMax;
    int numStructRows;
    int numStructCols;
    int structMin;
    int structMax;
    int rowOrigin;
    int colOrigin;
    int rowFrameSize;
    int colFrameSize;
    int extraRows;
    int extraCols;
    int rowSize;
    int colSize;
    int** zeroFramedAry;
    int** morphAry;
    int** tempAry;
    int** structAry;
    Morphology(ifstream &imgFile,ifstream &structFile){
        imgFile>>numImgRows;
        imgFile>>numImgCols;
        imgFile>>imgMin;
        imgFile>>imgMax;
        
        structFile>>numStructRows;
        structFile>>numStructCols;
        structFile>>structMin;
        structFile>>structMax;

        structFile>>rowOrigin;
        structFile>>colOrigin;
        
        rowFrameSize=numStructRows/2;
        colFrameSize=numStructCols/2;
        extraRows=rowFrameSize*2;
        extraCols=colFrameSize*2;
        rowSize=numImgRows+extraRows;
        colSize=numImgCols+extraCols;

        zeroFramedAry=new int*[rowSize];
        morphAry=new int*[rowSize];
        tempAry=new int*[rowSize];

        for(int i=0;i<rowSize;i++){
            zeroFramedAry[i]=new int[colSize];
            morphAry[i]=new int[colSize];
            tempAry[i]=new int[colSize];
        }

        structAry=new int*[numStructRows];
        for(int i=0;i<numStructRows;i++){
            structAry[i]=new int[numStructCols];
        }
    }
    void zero2DAry(int** ary,int nRows,int nCols){
        for(int i=0;i<nRows;i++){
            for(int j=0;j<nCols;j++){
                ary[i][j]=0;
            }
        }
    }
    void loadImg(ifstream &imgFile,int** zeroFramedAry){
        for(int i=rowOrigin;i<numImgRows+rowOrigin;i++){
            for(int j=colOrigin;j<numImgCols+colOrigin;j++){
                imgFile>>zeroFramedAry[i][j];
            }
        }
    }
    void loadStruct(ifstream &structFile,int** structAry){
        for(int i=0;i<numStructRows;i++){
            for(int j=0;j<numStructCols;j++){
                structFile>>structAry[i][j];
            }
        }
    }
    void computeDilation(int** inAry,int** outAry){
        //step 1
        int i=rowFrameSize;
        //step 7
        while(i<rowSize){
            //step 2
            int j=colFrameSize;
            //step 5
            while(j<colSize){
                //step 3
                if(inAry[i][j]>0){
                    onePixelDilation(i,j,inAry,outAry);
                }
                //step 4
                j++;
            }
            //step 6
            i++;
        }
    }
    void computeErosion(int** inAry,int** outAry){
        //step 1
        int i=rowFrameSize;
        //step 7
        while(i<rowSize){
            //step 2
            int j=colFrameSize;
            //step 5
            while(j<colSize){
                //step 3
                if(inAry[i][j]>0){
                    onePixelErosion(i,j,inAry,outAry);
                }
                //step 4
                j++;
            }
            //step 6
            i++;
        }
    }
    void computeOpening(int** zeroFramedAry,int** morphAry,int** tempAry){
        computeErosion(zeroFramedAry,tempAry);
        computeDilation(tempAry,morphAry);
    }
    void computeClosing(int** zeroFramedAry,int** morphAry,int** tempAry){
        computeDilation(zeroFramedAry,tempAry);
        computeErosion(tempAry,morphAry);
    }
    void onePixelDilation(int i,int j, int** inAry,int** outAry){
        //step 0
        int iOffSet=i-rowOrigin;
        int jOffSet=j-colOrigin;
        //step 1
        int rIndex=0;
        //step 7
        while(rIndex<numStructRows){
            //step 2
            int cIndex=0;
            //step 5
            while(cIndex<numStructCols){
                //step 3
                if(structAry[rIndex][cIndex]>0){
                    outAry[iOffSet+rIndex][jOffSet+cIndex]=1;
                }
                //step 4
                cIndex++;
            }
            //step 6
            rIndex++;
        }
    }
    void onePixelErosion(int i,int j, int** inAry,int** outAry){
        //step 0
        int iOffSet=i-rowOrigin;
        int jOffSet=j-colOrigin;
        bool matchFlag=true;
        //step 1
        int rIndex=0;
        //step 7
        while(matchFlag==true&&rIndex<numStructRows){
            //step 2
            int cIndex=0;
            //step 5
            while(matchFlag==true&&cIndex<numStructCols){
                //step 3
                if((structAry[rIndex][cIndex]>0)&&(inAry[iOffSet+rIndex][jOffSet+cIndex]<=0)){
                    matchFlag=false;
                }
                //step 4
                cIndex++;
            }
            //step 6
            rIndex++;
        }
        if(matchFlag==true){
            outAry[i][j]=1;
        }
        else{
            outAry[i][j]=0;
        }
    }
    void aryToFile(int** ary,ofstream &outFile){
        outFile<<numImgRows<<" "<<numImgCols<<" "<<imgMin<<" "<<imgMax<<"\n";
        for(int i=0;i<rowSize;i++){
            for(int j=0;j<colSize;j++){
                outFile<<ary[i][j]<<" ";
            }
            outFile<<"\n";
        }
    }
    void prettyPrint(int** ary,ofstream &outFile){
        outFile<<rowSize<<" "<<colSize<<" "<<imgMin<<" "<<imgMax<<"\n";
        for(int i=0;i<rowSize;i++){
            for(int j=0;j<colSize;j++){
                if(ary[i][j]==0){
                    outFile<<". ";
                }
                else{
                    outFile<<"1 ";
                }
            }
            outFile<<"\n";
        }
        outFile<<"\n";
    }
    void prettyPrintOriginal(int** ary,ofstream &outFile){
        outFile<<numImgRows<<" "<<numImgCols<<" "<<imgMin<<" "<<imgMax<<"\n";
        for(int i=rowOrigin;i<rowOrigin+numImgRows;i++){
            for(int j=colOrigin;j<colOrigin+numImgCols;j++){
                if(ary[i][j]==0){
                    outFile<<". ";
                }
                else{
                    outFile<<"1 ";
                }
            }
            outFile<<"\n";
        }
        outFile<<"\n";
    }
    void prettyPrintStruct(int** ary,ofstream &outFile){
        outFile<<numStructRows<<" "<<numStructCols<<" "<<structMin<<" "<<structMax<<"\n";
        for(int i=0;i<numStructRows;i++){
            for(int j=0;j<numStructCols;j++){
                if(ary[i][j]==0){
                    outFile<<". ";
                }
                else{
                    outFile<<"1 ";
                }
            }
            outFile<<"\n";
        }
        outFile<<"\n";
    }
    void objectExtractions(ifstream &structFile,ofstream &outFile){
        zero2DAry(structAry,numStructRows,numStructCols);
        loadStruct(structFile,structAry);
        outFile<<"Structuring Image\n";
        prettyPrintStruct(structAry,outFile);

        zero2DAry(morphAry,rowSize,colSize);
        zero2DAry(tempAry,rowSize,colSize);
        computeOpening(zeroFramedAry,morphAry,tempAry);
        outFile<<"Operation used is: Opening\n";
        prettyPrint(morphAry,outFile);
    }
    void fillHoles(ifstream &structFile,ofstream &outFile){
        zero2DAry(structAry,numStructRows,numStructCols);
        loadStruct(structFile,structAry);
        outFile<<"Structuring Image\n";
        prettyPrintStruct(structAry,outFile);

        zero2DAry(tempAry,rowSize,colSize);
        computeDilation(morphAry,tempAry);
        outFile<<"Operation used is: Dilation\n";
        prettyPrint(tempAry,outFile);
    }
    void objectDecompose(ifstream &structFile,ofstream &outFile){
        int** headbody=new int*[rowSize];
        int** armleg=new int*[rowSize];
        int** head=new int*[rowSize];
        int** body=new int*[rowSize];
        int** arm=new int*[rowSize];
        int** leg=new int*[rowSize];

        for(int i=0;i<rowSize;i++){
            headbody[i]=new int[colSize];
            armleg[i]=new int[colSize];
            head[i]=new int[colSize];
            body[i]=new int[colSize];
            arm[i]=new int[colSize];
            leg[i]=new int[colSize];
        }

        zero2DAry(headbody,rowSize,colSize);
        zero2DAry(armleg,rowSize,colSize);
        zero2DAry(head,rowSize,colSize);
        zero2DAry(body,rowSize,colSize);
        zero2DAry(arm,rowSize,colSize);
        zero2DAry(leg,rowSize,colSize);

        zero2DAry(structAry,numStructRows,numStructCols);
        loadStruct(structFile,structAry);
        outFile<<"Structuring Image\n";
        prettyPrintStruct(structAry,outFile);

        zero2DAry(tempAry,rowSize,colSize);
        computeOpening(zeroFramedAry,headbody,tempAry);
        outFile<<"Operation used is: Opening\n";
        prettyPrint(headbody,outFile);
        
        armleg=subtractElement(zeroFramedAry,headbody,armleg);
        outFile<<"Operation used is: Subtraction\n";
        prettyPrint(armleg,outFile);

        zero2DAry(structAry,numStructRows,numStructCols);
        loadStruct(structFile,structAry);
        outFile<<"Structuring Image\n";
        prettyPrintStruct(structAry,outFile);

        zero2DAry(tempAry,rowSize,colSize);
        computeOpening(headbody,body,tempAry);
        outFile<<"Operation used is: Opening\n";
        prettyPrint(body,outFile);

        head=subtractElement(headbody,body,head);
        outFile<<"Operation used is: Subtraction\n";
        prettyPrint(head,outFile);

        zero2DAry(structAry,numStructRows,numStructCols);
        loadStruct(structFile,structAry);
        outFile<<"Structuring Image\n";
        prettyPrintStruct(structAry,outFile);

        zero2DAry(tempAry,rowSize,colSize);
        computeOpening(armleg,leg,tempAry);
        outFile<<"Operation used is: Opening\n";
        prettyPrint(leg,outFile);

        arm=subtractElement(armleg,leg,arm);
        outFile<<"Operation used is: Subtraction\n";
        prettyPrint(arm,outFile);
    }
    int** subtractElement(int** fullImg,int** partImg,int** otherPartImg){
        for(int i=0;i<rowSize;i++){
            for(int j=0;j<colSize;j++){
                if(fullImg[i][j]!=partImg[i][j]){
                    otherPartImg[i][j]=1;
                }
            }
        }
        return otherPartImg;
    }
};
int main(int argc,char** argv){
    //step 0
    ifstream imgFile(argv[1]);
    ifstream structFile(argv[2]);
    ifstream img1(argv[3]);
    ifstream img2(argv[4]);
    ofstream outFile1(argv[5]);
    ofstream outFile2(argv[6]);
    ifstream taskStruct1(argv[7]);
    //ifstream taskStruct2(argv[8]);

    //step 1
    //step 2
    Morphology* m=new Morphology(imgFile,structFile);

    //step 3
    m->zero2DAry(m->zeroFramedAry,m->rowSize,m->colSize);

    //step 4
    m->loadImg(imgFile,m->zeroFramedAry);
    outFile1<<"Original Image\n";
    m->prettyPrintOriginal(m->zeroFramedAry,outFile1);
    
    
    //step 5
    m->zero2DAry(m->structAry,m->numStructRows,m->numStructCols);
    m->loadStruct(structFile,m->structAry);
    outFile1<<"Structuring Image\n";
    m->prettyPrintStruct(m->structAry,outFile1);
    
    //step 6
    m->zero2DAry(m->morphAry,m->rowSize,m->colSize);
    m->computeDilation(m->zeroFramedAry,m->morphAry);
    outFile1<<"Dilation\n";
    m->prettyPrint(m->morphAry,outFile1);
    
    //step 7
    m->zero2DAry(m->morphAry,m->rowSize,m->colSize);
    m->computeErosion(m->zeroFramedAry,m->morphAry);
    outFile1<<"Erosion\n";
    m->prettyPrint(m->morphAry,outFile1);
    
    //step 8
    m->zero2DAry(m->morphAry,m->rowSize,m->colSize);
    m->zero2DAry(m->tempAry,m->rowSize,m->colSize);
    m->computeOpening(m->zeroFramedAry,m->morphAry,m->tempAry);
    outFile1<<"Opening\n";
    m->prettyPrint(m->morphAry,outFile1);
    
    //step 9
    m->zero2DAry(m->morphAry,m->rowSize,m->colSize);
    m->zero2DAry(m->tempAry,m->rowSize,m->colSize);
    m->computeClosing(m->zeroFramedAry,m->morphAry,m->tempAry);
    outFile1<<"Closing\n";
    m->prettyPrint(m->morphAry,outFile1);
    
    //step 10
    Morphology* task1=new Morphology(img1,taskStruct1);
    task1->zero2DAry(task1->zeroFramedAry,task1->rowSize,task1->colSize);
    task1->loadImg(img1,task1->zeroFramedAry);
    outFile2<<"Task 1 Original Image\n";
    task1->prettyPrintOriginal(task1->zeroFramedAry,outFile2);

    task1->objectExtractions(taskStruct1,outFile2);
    task1->fillHoles(taskStruct1,outFile2);
    /*
    outFile2<<"\n";
    Morphology* task2=new Morphology(img2,taskStruct2);
    task2->zero2DAry(task2->zeroFramedAry,task2->rowSize,task2->colSize);
    task2->loadImg(img2,task2->zeroFramedAry);
    outFile2<<"Original Image Task 2\n";
    task2->prettyPrintOriginal(task2->zeroFramedAry,outFile2);

    task2->objectDecompose(taskStruct2,outFile2);
    */
    outFile1.close();
    outFile2.close();
}