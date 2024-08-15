import java.io.*;
import java.util.*;
public class ConnectedComponent{
    static class Property{
        int label;
        int numPixels;
        int minR;
        int minC;
        int maxR;
        int maxC;
        Property(){

        }
    }
    static class ccLabel{
        int numRows;
        int numCols;
        int minVal;
        int maxVal;
        int newLabel;
        int trueNumCC;
        int newMin;
        int newMax;
        int zeroFramedAry[][];
        int nonZeroNeighborAry[];
        int EQAry[];
        char option;
        Property CCproperty[];
        ccLabel(Scanner inFile) throws FileNotFoundException{
            numRows=inFile.nextInt();
            numCols=inFile.nextInt();
            minVal=inFile.nextInt();
            maxVal=inFile.nextInt();
            newLabel=0;
            trueNumCC=0;
            zeroFramedAry=new int[numRows+2][numCols+2];
            nonZeroNeighborAry=new int[5];
            EQAry=new int[(numRows*numCols)/4];
            for(int i=0;i<EQAry.length;i++){
                EQAry[i]=i;
            }
        }
        public void zero2D(int[][] zeroFramedAry){
            for(int i=0;i<zeroFramedAry.length;i++){
                for(int j=0;j<zeroFramedAry[0].length;j++){
                    zeroFramedAry[i][j]=0;
                }
            }
        }
        public void negative1D(int[] nonZeroNeighborAry){
            for(int i=0;i<nonZeroNeighborAry.length;i++){
                nonZeroNeighborAry[i]=-1;
            }
        }
        public void loadImage(Scanner inFile,int[][] zeroFramedAry) throws FileNotFoundException{
            for(int i=1;i<zeroFramedAry.length-1;i++){
                for(int j=1;j<zeroFramedAry[0].length-1;j++){
                    zeroFramedAry[i][j]=inFile.nextInt();
                }
            }
        }
        public void imgReformat(int[][] zeroFramedAry,FileWriter outFile) throws IOException{
            String str=String.valueOf(newLabel);
            int width=str.length();
            int r=1;
            while(r<numRows+1){
                int c=1;
                while(c<numCols+1){
                    if(zeroFramedAry[r][c]==0){
                        outFile.write(".");
                    }
                    else{
                        outFile.write(String.valueOf(zeroFramedAry[r][c]));
                    }
                    str=String.valueOf(zeroFramedAry[r][c]);
                    int ww=str.length();
                    while(ww<=width){
                        outFile.write(" ");
                        ww++;
                    }
                    c++;
                }
                r++;
                outFile.write("\n");
            }
            outFile.write("\n");
        }
        public int getMin(int[] nonZeroNeighborAry){
            int min=999;
            int index=0;
            while(index<5){
                if((nonZeroNeighborAry[index]!=-1)&&(nonZeroNeighborAry[index]<min)){
                    min=nonZeroNeighborAry[index];
                }
                index++;
            }
            return min;
        }
        public int whichCase(int[] nonZeroNeighborAry){
            int index=0;
            int first=nonZeroNeighborAry[index];
            if(first==-1){
                return 1;
            }
            while(nonZeroNeighborAry[index]!=-1&&index+1<5){
                if(first!=nonZeroNeighborAry[index+1]){
                    return 3;
                }
                index++;
            }
            return 2;
        }
        public void getNeighbors8Pass1(int[][]zeroFramedAry,int i,int j,int[] nonZeroNeighborAry){
            negative1D(nonZeroNeighborAry);
            int index=0;
            if(zeroFramedAry[i-1][j-1]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i-1][j-1];
                index++;
            }
            if(zeroFramedAry[i-1][j]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i-1][j];
                index++;
            }
            if(zeroFramedAry[i-1][j+1]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i-1][j+1];
                index++;
            }
            if(zeroFramedAry[i][j-1]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i][j-1];
                index++;
            }
        }
        public void connect8Pass1(int[][] zeroFramedAry,int newLabel,int[] EQAry){
            for(int i=1;i<zeroFramedAry.length-1;i++){
                for(int j=1;j<zeroFramedAry[0].length-1;j++){
                    int pixel=zeroFramedAry[i][j];
                    getNeighbors8Pass1(zeroFramedAry,i,j,nonZeroNeighborAry);
                    if(pixel>0){
                        //case 1
                        if(whichCase(nonZeroNeighborAry)==1){
                            this.newLabel++;
                            zeroFramedAry[i][j]=this.newLabel;
                            updateEQ(EQAry,this.newLabel);
                        }
                        //case 2
                        else if(whichCase(nonZeroNeighborAry)==2){
                            zeroFramedAry[i][j]=nonZeroNeighborAry[0];
                        }
                        //case 3
                        else if(whichCase(nonZeroNeighborAry)==3){
                            zeroFramedAry[i][j]=getMin(nonZeroNeighborAry);
                            updateEQ(EQAry,getMin(nonZeroNeighborAry));
                        }
                    }
                }
            }
        }
        public void getNeighbors8Pass2(int[][]zeroFramedAry,int i,int j,int[] nonZeroNeighborAry){
            negative1D(nonZeroNeighborAry);
            int index=0;
            if(zeroFramedAry[i][j]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i][j];
                index++;
            }
            if(zeroFramedAry[i][j+1]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i][j+1];
                index++;
            }
            if(zeroFramedAry[i+1][j-1]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i+1][j-1];
                index++;
            }
            if(zeroFramedAry[i+1][j]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i+1][j];
                index++;
            }
            if(zeroFramedAry[i+1][j+1]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i+1][j+1];
                index++;
            }
        }
        public void connect8Pass2(int[][] zeroFramedAry,int newLabel,int[] EQAry){
            for(int i=zeroFramedAry.length-2;i>0;i--){
                for(int j=zeroFramedAry[0].length-2;j>0;j--){
                    int pixel=zeroFramedAry[i][j];
                    getNeighbors8Pass2(zeroFramedAry,i,j,nonZeroNeighborAry);
                    if(pixel>0){
                        //case 1
                        if(whichCase(nonZeroNeighborAry)==1){

                        }
                        //case 2
                        else if(whichCase(nonZeroNeighborAry)==2){

                        }
                        //case 3
                        else if(whichCase(nonZeroNeighborAry)==3){
                            int minLabel=getMin(nonZeroNeighborAry);
                            if(pixel>minLabel){
                                EQAry[pixel]=minLabel;
                                zeroFramedAry[i][j]=minLabel;
                                zeroFramedAry[i][j]=EQAry[pixel];
                            }
                        }
                    }
                }
            }
        }
        public void getNeighbors4Pass1(int[][]zeroFramedAry,int i,int j,int[] nonZeroNeighborAry){
            negative1D(nonZeroNeighborAry);
            int index=0;
            if(zeroFramedAry[i-1][j]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i-1][j];
                index++;
            }
            if(zeroFramedAry[i][j-1]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i][j-1];
                index++;
            }
        }
        public void connect4Pass1(int[][] zeroFramedAry,int newLabel,int[] EQAry){
            for(int i=1;i<zeroFramedAry.length-1;i++){
                for(int j=1;j<zeroFramedAry[0].length-1;j++){
                    int pixel=zeroFramedAry[i][j];
                    getNeighbors4Pass1(zeroFramedAry,i,j,nonZeroNeighborAry);
                    if(pixel>0){
                        //case 1
                        if(whichCase(nonZeroNeighborAry)==1){
                            this.newLabel++;
                            zeroFramedAry[i][j]=this.newLabel;
                            updateEQ(EQAry,this.newLabel);
                        }
                        //case 2
                        else if(whichCase(nonZeroNeighborAry)==2){
                            zeroFramedAry[i][j]=nonZeroNeighborAry[0];
                        }
                        //case 3
                        else if(whichCase(nonZeroNeighborAry)==3){
                            zeroFramedAry[i][j]=getMin(nonZeroNeighborAry);
                            updateEQ(EQAry,getMin(nonZeroNeighborAry));
                        }
                    }
                }
            }
        }
        public void getNeighbors4Pass2(int[][]zeroFramedAry,int i,int j,int[] nonZeroNeighborAry){
            negative1D(nonZeroNeighborAry);
            int index=0;
            if(zeroFramedAry[i][j]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i][j];
                index++;
            }
            if(zeroFramedAry[i][j+1]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i][j+1];
                index++;
            }
            if(zeroFramedAry[i+1][j]!=0){
                nonZeroNeighborAry[index]=zeroFramedAry[i+1][j];
                index++;
            }
        }
        public void connect4Pass2(int[][] zeroFramedAry,int newLabel,int[] EQAry){
            for(int i=zeroFramedAry.length-2;i>0;i--){
                for(int j=zeroFramedAry[0].length-2;j>0;j--){
                    int pixel=zeroFramedAry[i][j];
                    getNeighbors4Pass2(zeroFramedAry,i,j,nonZeroNeighborAry);
                    if(pixel>0){
                        //case1 1
                        if(whichCase(nonZeroNeighborAry)==1){

                        }
                        //case 2
                        else if(whichCase(nonZeroNeighborAry)==2){

                        }
                        //case 3
                        else if(whichCase(nonZeroNeighborAry)==3){
                            int minLabel=getMin(nonZeroNeighborAry);
                            if(pixel>minLabel){
                                EQAry[pixel]=minLabel;
                                zeroFramedAry[i][j]=minLabel;
                                zeroFramedAry[i][j]=EQAry[pixel];
                            }
                        }
                    }
                }
            }
        }
        public void connectPass3(int[][] zeroFramedAry,int[] EQAry,int trueNumCC,FileWriter deBugFile) throws IOException{
            //step 0
            deBugFile.write("entering connectPass3 method\n");
            CCproperty=new Property[trueNumCC+1];
            //step 1
            for(int i=0;i<=trueNumCC;i++){
                CCproperty[i]=new Property();
                CCproperty[i].label=i;
                CCproperty[i].numPixels=0;
                CCproperty[i].minR=numRows;
                CCproperty[i].maxR=0;
                CCproperty[i].minC=numCols;
                CCproperty[i].maxC=0;
            }
            //step 4
            //step 2
            for(int r=0;r<zeroFramedAry.length;r++){
                for(int c=0;c<zeroFramedAry[0].length;c++){
                    int pixel=zeroFramedAry[r][c];
                    //step 3
                    if(pixel>0){
                        zeroFramedAry[r][c]=EQAry[pixel];
                        int k=zeroFramedAry[r][c];
                        CCproperty[k].numPixels++;
                        if(r<CCproperty[k].minR){
                            CCproperty[k].minR=r-1;
                        }
                        if(r>CCproperty[k].maxR){
                            CCproperty[k].maxR=r-1;
                        }
                        if(c<CCproperty[k].minC){
                            CCproperty[k].minC=c-1;
                        }
                        if(c>CCproperty[k].maxC){
                            CCproperty[k].maxC=c-1;
                        }
                    }
                }
            }
            deBugFile.write("leaving connectPass3 method");
        }
        public void updateEQ(int[] EQAry,int newVal){
            int index=0;
            while(nonZeroNeighborAry[index]!=-1&&index<5){
                if(EQAry[nonZeroNeighborAry[index]]<newVal){

                }
                else{
                    EQAry[nonZeroNeighborAry[index]]=newVal;
                }
                index++;
            }
        }
        public int manageEQAry(int[] EQAry,int newLabel){
            //step 0
            int readLabel=0;
            //step 1
            int index=1;
            //step 4
            while(index<=newLabel){
                //step 2
                if(index!=EQAry[index]){
                    EQAry[index]=EQAry[EQAry[index]];
                }
                else{
                    readLabel++;
                    EQAry[index]=readLabel;
                }
                //step 3
                index++;
            }
            return readLabel;
        }
        public void printCCProperty(FileWriter outFile) throws IOException{
            newMin=0;
            newMax=trueNumCC;
            outFile.write(numRows+" "+numCols+" "+newMin+" "+newMax+"\n");
            outFile.write(trueNumCC+"\n");
            for(int i=1;i<=trueNumCC;i++){
                outFile.write(CCproperty[i].label+"\n");
                outFile.write(CCproperty[i].numPixels+"\n");
                outFile.write(CCproperty[i].minR+"\t"+CCproperty[i].minC+"\n");
                outFile.write(CCproperty[i].maxR+"\t"+CCproperty[i].maxC+"\n");
            }
        }
        public void printEQAry(int newLabel,FileWriter outFile) throws IOException{
            for(int i=1;i<=this.newLabel;i++){
                outFile.write(EQAry[i]+" ");
            }
            outFile.write("\n\n");
        }
        public void drawBoxes(int[][] zeroFramedAry,Property[] CCproperty,int trueNumCC,FileWriter deBugFile) throws IOException{
            //step 0
            deBugFile.write("entering drawBoxes method\n");
            //step 1
            int index=1;
            //step 5
            while(index<=trueNumCC){
                //step 2
                int minRow=CCproperty[index].minR+1;
                int minCol=CCproperty[index].minC+1;
                int maxRow=CCproperty[index].maxR+1;
                int maxCol=CCproperty[index].maxC+1;
                int label=CCproperty[index].label;
                //step 3
                for(int i=minCol;i<=maxCol;i++){
                    zeroFramedAry[minRow][i]=label;
                    zeroFramedAry[maxRow][i]=label;
                }
                for(int j=minRow;j<=maxRow;j++){
                    zeroFramedAry[j][minCol]=label;
                    zeroFramedAry[j][maxCol]=label;
                }
                //step 4
                index++;
            }
            //step 6
            deBugFile.write("leaving drawBoxes method\n");
        }
        public void printImg(int[][] zeroFramedAry,FileWriter outFile) throws IOException{
            newMax=trueNumCC;
            outFile.write(numRows+" "+numCols+" "+newMin+" "+newMax+"\n");
            for(int i=1;i<zeroFramedAry.length-1;i++){
                for(int j=1;j<zeroFramedAry[0].length-1;j++){
                    outFile.write(zeroFramedAry[i][j]+" ");
                }
                outFile.write("\n");
            }
            outFile.write("\n");
        }
        public void connected4(int[][] zeroFramedAry,int newLabel,int[] EQAry,FileWriter RFprettyPrintFile,FileWriter deBugFile) throws IOException{
            //step 0
            deBugFile.write("entering connect4 method\n");
            //step 1
            connect4Pass1(zeroFramedAry,this.newLabel,EQAry);
            deBugFile.write("after connect4Pass1, new Label= "+this.newLabel+"\n");
            RFprettyPrintFile.write("Result of: Pass 1\n");
            imgReformat(zeroFramedAry,RFprettyPrintFile);
            RFprettyPrintFile.write("Equivalency Table after: Pass 1 (indexing starts from 1)\n");
            printEQAry(this.newLabel,RFprettyPrintFile);
            //step 2
            connect4Pass2(zeroFramedAry,this.newLabel,EQAry);
            deBugFile.write("after connect4Pass2, new Label= "+this.newLabel+"\n");
            RFprettyPrintFile.write("Result of: Pass 2\n");
            imgReformat(zeroFramedAry,RFprettyPrintFile);
            RFprettyPrintFile.write("Equivalency Table after: Pass 2 (indexing starts from 1)\n");
            printEQAry(this.newLabel,RFprettyPrintFile);
            //step 3
            trueNumCC=manageEQAry(EQAry,this.newLabel);
            RFprettyPrintFile.write("Equivalency Table after: EQ Table Management (indexing starts from 1)\n");
            printEQAry(this.newLabel,RFprettyPrintFile);
            //step 4
            connectPass3(zeroFramedAry,EQAry,trueNumCC, deBugFile);
            //step 5
            RFprettyPrintFile.write("Result of: Pass 3\n");
            imgReformat(zeroFramedAry,RFprettyPrintFile);
            //step 6
            RFprettyPrintFile.write("Equivalency Table after: Pass 3 (indexing starts from 1)\n");
            printEQAry(this.newLabel,RFprettyPrintFile);
            //step 7
            deBugFile.write("leaving connected4 method\n");
        }
        public void connected8(int[][] zeroFramedAry,int newLabel,int[] EQAry,FileWriter RFprettyPrintFile,FileWriter deBugFile) throws IOException{
            //step 0
            deBugFile.write("entering connect8 method\n");
            //step 1
            connect8Pass1(zeroFramedAry,this.newLabel,EQAry);
            deBugFile.write("after connect8Pass1, new Label= "+this.newLabel+"\n");
            RFprettyPrintFile.write("Result of: Pass 1\n");
            imgReformat(zeroFramedAry,RFprettyPrintFile);
            RFprettyPrintFile.write("Equivalency Table after: Pass 1 (indexing starts from 1)\n");
            printEQAry(this.newLabel,RFprettyPrintFile);
            //step 2
            connect8Pass2(zeroFramedAry,this.newLabel,EQAry);
            deBugFile.write("after connect4Pass2, new Label= "+this.newLabel+"\n");
            RFprettyPrintFile.write("Result of: Pass 2\n");
            imgReformat(zeroFramedAry,RFprettyPrintFile);
            RFprettyPrintFile.write("Equivalency Table after: Pass 2 (indexing starts from 1)\n");
            printEQAry(this.newLabel,RFprettyPrintFile);
            //step 3
            trueNumCC=manageEQAry(EQAry,this.newLabel);
            RFprettyPrintFile.write("Equivalency Table after: EQ Table Management (indexing starts from 1)\n");
            printEQAry(this.newLabel,RFprettyPrintFile);
            //step 4
            connectPass3(zeroFramedAry,EQAry,trueNumCC, deBugFile);
            //step 5
            RFprettyPrintFile.write("Result of: Pass 3\n");
            imgReformat(zeroFramedAry,RFprettyPrintFile);
            //step 6
            RFprettyPrintFile.write("Equivalency Table after: Pass 3 (indexing starts from 1)\n");
            printEQAry(this.newLabel,RFprettyPrintFile);
            //step 7
            deBugFile.write("leaving connected4 method\n");
        }
    }
    public static void main(String args[]) throws IOException,FileNotFoundException{
        //step 0
        Scanner inFile=new Scanner(new FileReader(args[0]));
        int connectness=Integer.parseInt(args[1]);
        FileWriter RFprettyPrintFile=new FileWriter(args[2]);
        FileWriter labelFile=new FileWriter(args[3]);
        FileWriter propertyFile=new FileWriter(args[4]);
        FileWriter deBugFile=new FileWriter(args[5]);
        
        ccLabel ccl=new ccLabel(inFile);

        //step 1
        ccl.zero2D(ccl.zeroFramedAry);
        //step 2
        ccl.loadImage(inFile,ccl.zeroFramedAry);
        //step 3
        if(connectness==4){
            ccl.connected4(ccl.zeroFramedAry,ccl.newLabel,ccl.EQAry,RFprettyPrintFile,deBugFile);
        }
        //step 4
        if(connectness==8){
            ccl.connected8(ccl.zeroFramedAry,ccl.newLabel,ccl.EQAry,RFprettyPrintFile,deBugFile);
        }
        //step 5
        //step 6
        ccl.printImg(ccl.zeroFramedAry,labelFile);
        //step 7
        ccl.printCCProperty(propertyFile);
        //step 8
        ccl.drawBoxes(ccl.zeroFramedAry,ccl.CCproperty,ccl.trueNumCC,deBugFile);
        //step 9
        RFprettyPrintFile.write("Bounding Boxes: \n");
        ccl.imgReformat(ccl.zeroFramedAry, RFprettyPrintFile);
        //step 10
        RFprettyPrintFile.write("Number of Connected Components: "+ccl.trueNumCC+"\n");
        //step 11
        inFile.close();
        RFprettyPrintFile.close();
        labelFile.close();
        propertyFile.close();
        deBugFile.close();
    }
}