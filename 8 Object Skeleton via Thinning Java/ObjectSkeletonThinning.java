import java.io.*;
import java.util.*;

public class ObjectSkeletonThinning{
    static class Thinning{
        int numRows;
        int numCols;
        int minVal;
        int maxVal;
        int changeCount;
        int cycleCount;
        int[][] aryOne;
        int[][] aryTwo;
        int[] neighborAry;
        Thinning(Scanner inFile)throws FileNotFoundException{
            numRows=inFile.nextInt();
            numCols=inFile.nextInt();
            minVal=inFile.nextInt();
            maxVal=inFile.nextInt();
            changeCount=0;
            cycleCount=0;
            aryOne=new int[numRows+2][numCols+2];
            aryTwo=new int[numRows+2][numCols+2];
            neighborAry=new int[9];
        }
        void zeroFramed(int[][] inAry){
            for(int i=0;i<inAry.length;i++){
                for(int j=0;j<inAry[0].length;j++){
                    inAry[i][j]=0;
                }
            }
        }
        void loadImage(Scanner inFile,int[][] aryOne){
            for(int i=1;i<numRows+1;i++){
                for(int j=1;j<numCols+1;j++){
                    aryOne[i][j]=inFile.nextInt();
                }
            }
        }
        int loadNeighborAry(int[] neighborAry,int[][] aryOne,int i,int j){
            int count=0;
            int index=0;
            for(int rowOffSet=-1;rowOffSet<=1;rowOffSet++){
                for(int colOffSet=-1;colOffSet<=1;colOffSet++){
                    if(rowOffSet==0&&colOffSet==0){

                    }
                    else{
                        if(aryOne[i+rowOffSet][j+colOffSet]>0){
                            count++;
                        }
                    }
                    neighborAry[index]=aryOne[i+rowOffSet][j+colOffSet];
                    index++;
                }
            }
            if(count==0){
                return -1;
            }
            return count;
        }
        void copyArys(int[][] aryTwo,int[][] aryOne){
            for(int i=0;i<aryOne.length;i++){
                for(int j=0;j<aryOne[0].length;j++){
                    aryOne[i][j]=aryTwo[i][j];
                }
            }
        }
        void thinning(int[][] aryOne,int[][] aryTwo,FileWriter deBugFile)throws IOException{
            //step 0
            deBugFile.write("entering thinning() method before thinning four sides; aryOne is below: \n");
            reformatPrettyPrint(aryOne,deBugFile);
            changeCount=0;
            //step 1
            northThinning(aryOne, aryTwo, deBugFile);
            deBugFile.write("after northThinning() method; aryTwo is below: \n");
            reformatPrettyPrint(aryTwo,deBugFile);
            copyArys(aryTwo,aryOne);
            //for me
            deBugFile.write("aryOne now:\n");
            reformatPrettyPrint(aryOne,deBugFile);
            //step 2
            southThinning(aryOne, aryTwo, deBugFile);
            deBugFile.write("after southThinning() method; aryTwo is below: \n");
            reformatPrettyPrint(aryTwo,deBugFile);
            copyArys(aryTwo,aryOne);
            //step 3
            westThinning(aryOne, aryTwo, deBugFile);
            deBugFile.write("after westThinning() method; aryTwo is below: \n");
            reformatPrettyPrint(aryTwo,deBugFile);
            copyArys(aryTwo,aryOne);
            //step 4
            eastThinning(aryOne, aryTwo, deBugFile);
            deBugFile.write("after eastThinning() method; aryTwo is below: \n");
            reformatPrettyPrint(aryTwo,deBugFile);
            copyArys(aryTwo,aryOne);
            //step 5
            deBugFile.write("leaving thinning() method, cycleCount="+cycleCount+";changeCount="+changeCount+"\n");
        }
        void northThinning(int[][] aryOne,int[][] aryTwo,FileWriter deBugFile)throws IOException{
            //step 0
            deBugFile.write("entering northThinning() method, cycleCount="+cycleCount+";changeCount="+changeCount+"\n");
            //step 1
            int i=1;
            //step 8
            while(i<=numRows+1){
                //step 2
                int j=1;
                //step 6
                while(j<=numCols+1){
                    int nonZeroCount=-1;
                    boolean flag=false;
                    //step 3
                    if(aryOne[i][j]>0&&aryOne[i-1][j]==0){
                        nonZeroCount=loadNeighborAry(neighborAry,aryOne,i,j);
                        flag=checkConnector(neighborAry);
                        deBugFile.write("in northThinning() method, i="+i+";j="+j+";nonZeroCount="+nonZeroCount+";flag="+flag+"\n");
                    }
                    //step 4
                    if(nonZeroCount>=4&&flag==false){
                        aryTwo[i][j]=0;
                        changeCount++;
                    }
                    else{
                        aryTwo[i][j]=aryOne[i][j];
                    }
                    //step 5
                    j++;
                }
                //step 7
                i++;
            }
            //step 9
            deBugFile.write("leaving northThinning() method, cycleCount="+cycleCount+";changeCount="+changeCount+"\n");
        }
        void southThinning(int[][] aryOne,int[][] aryTwo,FileWriter deBugFile)throws IOException{
            //step 0
            deBugFile.write("entering southThinning() method, cycleCount="+cycleCount+";changeCount="+changeCount+"\n");
            //step 1
            int i=1;
            //step 8
            while(i<=numRows+1){
                //step 2
                int j=1;
                //step 6
                while(j<=numCols+1){
                    int nonZeroCount=-1;
                    boolean flag=false;
                    //step 3
                    if(aryOne[i][j]>0&&aryOne[i+1][j]==0){
                        nonZeroCount=loadNeighborAry(neighborAry,aryOne,i,j);
                        flag=checkConnector(neighborAry);
                        deBugFile.write("in southThinning() method, i="+i+";j="+j+";nonZeroCount="+nonZeroCount+";flag="+flag+"\n");
                    }
                    //step 4
                    if(nonZeroCount>=4&&flag==false){
                        aryTwo[i][j]=0;
                        changeCount++;
                    }
                    else{
                        aryTwo[i][j]=aryOne[i][j];
                    }
                    //step 5
                    j++;
                }
                //step 7
                i++;
            }
            //step 9
            deBugFile.write("leaving southThinning() method, cycleCount="+cycleCount+";changeCount="+changeCount+"\n");
        }
        void eastThinning(int[][] aryOne,int[][] aryTwo,FileWriter deBugFile)throws IOException{
            //step 0
            deBugFile.write("entering eastThinning() method, cycleCount="+cycleCount+";changeCount="+changeCount+"\n");
            //step 1
            int i=1;
            //step 8
            while(i<=numRows+1){
                //step 2
                int j=1;
                //step 6
                while(j<=numCols+1){
                    int nonZeroCount=-1;
                    boolean flag=false;
                    //step 3
                    if(aryOne[i][j]>0&&aryOne[i][j+1]==0){
                        nonZeroCount=loadNeighborAry(neighborAry,aryOne,i,j);
                        flag=checkConnector(neighborAry);
                        deBugFile.write("in eastThinning() method, i="+i+";j="+j+";nonZeroCount="+nonZeroCount+";flag="+flag+"\n");
                    }
                    //step 4
                    if(nonZeroCount>=4&&flag==false){
                        aryTwo[i][j]=0;
                        changeCount++;
                    }
                    else{
                            aryTwo[i][j]=aryOne[i][j];
                    }
                    //step 5
                    j++;
                }
                //step 7
                i++;
            }
            //step 9
            deBugFile.write("leaving eastThinning() method, cycleCount="+cycleCount+";changeCount="+changeCount+"\n");
        }
        void westThinning(int[][] aryOne,int[][] aryTwo,FileWriter deBugFile)throws IOException{
            //step 0
            deBugFile.write("entering westThinning() method, cycleCount="+cycleCount+";changeCount="+changeCount+"\n");
            //step 1
            int i=1;
            //step 8
            while(i<=numRows+1){
                //step 2
                int j=1;
                //step 6
                while(j<=numCols+1){
                    int nonZeroCount=-1;
                    boolean flag=false;
                    //step 3
                    if(aryOne[i][j]>0&&aryOne[i][j-1]==0){
                        nonZeroCount=loadNeighborAry(neighborAry,aryOne,i,j);
                        flag=checkConnector(neighborAry);
                        deBugFile.write("in westThinning() method, i="+i+";j="+j+";nonZeroCount="+nonZeroCount+";flag="+flag+"\n");
                        //step 4
                        if(nonZeroCount>=4&&flag==false){
                            aryTwo[i][j]=0;
                            changeCount++;
                        }
                        else{
                            aryTwo[i][j]=aryOne[i][j];
                        }
                    }
                    //step 4
                    if(nonZeroCount>=4&&flag==false){
                        aryTwo[i][j]=0;
                        changeCount++;
                    }
                    else{
                        aryTwo[i][j]=aryOne[i][j];
                    }
                    //step 5
                    j++;
                }
                //step 7
                i++;
            }
            //step 9
            deBugFile.write("leaving westThinning() method, cycleCount="+cycleCount+";changeCount="+changeCount+"\n");
        }
        boolean checkConnector(int[] neighborAry){
            if(neighborAry[1]==0&&neighborAry[7]==0){
                return true;
            }
            else if(neighborAry[3]==0&&neighborAry[5]==0){
                return true;
            }
            else if(neighborAry[0]==1&&neighborAry[1]==0&&neighborAry[3]==0){
                return true;
            }
            else if(neighborAry[1]==0&&neighborAry[2]==1&&neighborAry[5]==0){
                return true;
            }
            else if(neighborAry[3]==0&&neighborAry[6]==1&&neighborAry[7]==0){
                return true;
            }
            else if(neighborAry[5]==0&&neighborAry[7]==0&&neighborAry[8]==1){
                return true;
            }
            else{
                return false;
            }
        }
        void reformatPrettyPrint(int[][] inAry,FileWriter outFile)throws IOException{
            outFile.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
            for(int i=1;i<numRows+1;i++){
                for(int j=1;j<numCols+1;j++){
                    if(inAry[i][j]==0){
                        outFile.write(". ");
                    }
                    else{
                        outFile.write(inAry[i][j]+" ");
                    }
                }
                outFile.write("\n");
            }
            outFile.write("\n");
        }
        void imgReformat(int[][] inAry,FileWriter outFile)throws IOException{
            outFile.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
            for(int i=1;i<numRows+1;i++){
                for(int j=1;j<numCols+1;j++){
                    outFile.write(inAry[i][j]+" ");
                }
                outFile.write("\n");
            }
            outFile.write("\n");
        }
    }
    public static void main(String args[])throws IOException,FileNotFoundException{
        //step 0
        Scanner inFile=new Scanner(new FileReader(args[0]));
        FileWriter outFile=new FileWriter(args[1]);
        FileWriter deBugFile=new FileWriter(args[2]);
        Thinning t=new Thinning(inFile);
        //step 1
        t.loadImage(inFile,t.aryOne);
        //step 2
        outFile.write("in main(), before thinning, changeCount="+t.changeCount+"; cycleCount="+t.cycleCount+"\n");
        t.reformatPrettyPrint(t.aryOne,outFile);
        boolean firstRun=true;
        //step 6
        while(t.changeCount>0||firstRun==true){
            firstRun=false;
            //step 3
            t.thinning(t.aryOne,t.aryTwo,deBugFile);
            //step 4
            t.cycleCount++;
            //step 5
            outFile.write("in main(), inside iteration, changeCount="+t.changeCount+"; cycleCount="+t.cycleCount+"\n");
            t.reformatPrettyPrint(t.aryOne,outFile);
        }
        //step 7
        outFile.write("in main(), the final skeleton, changeCount="+t.changeCount+"; cycleCount="+t.cycleCount+"\n");
        t.imgReformat(t.aryOne,outFile);
        //step 8
        inFile.close();
        outFile.close();
        deBugFile.close();
    }
}    

