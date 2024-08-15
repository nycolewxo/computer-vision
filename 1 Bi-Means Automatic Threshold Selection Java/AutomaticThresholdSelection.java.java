import java.io.*;
import java.util.*;
import java.lang.Math;

public class AutomaticThresholdSelection{
    static class thresholdSelection{
        int numRows;
        int numCols;
        int minVal;
        int maxVal;
        int[] histAry;
        int[] GaussAry;
        int BiGaussThrVal;
        int maxHeight;

        thresholdSelection(){

        }

        int loadHist(int[] histAry,Scanner inFile)throws FileNotFoundException{
            int max=0;
            int i;
            while(inFile.hasNextInt()){
                i=inFile.nextInt();
                histAry[i]=inFile.nextInt();
                if(max<histAry[i]){
                    max=histAry[i];
                }
            }
            return max;
        }

        void dispHist(int[] histAry,FileWriter outFile)throws IOException{
            outFile.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
            PrintWriter format=new PrintWriter(outFile);
            for(int i=0;i<histAry.length;i++){
                format.printf("%-3d%s%3d)%2s",i,"(",histAry[i],":");
                for(int j=0;j<histAry[i];j++){
                    outFile.write("+");
                }
                outFile.write("\n");
            }
        }

        void setZero(int[] Ary){
            for(int i=0;i<Ary.length;i++){
                Ary[i]=0;
            }
        }

        int biGauss(int[] histAry,int[] GaussAry,int maxHeight,int minVal, int maxVal,FileWriter deBugFile)throws IOException{
            //step 0
            deBugFile.write("Entering biGaussian method\n");
            double sum1;
            double sum2;
            double total;
            double minSumDiff;
            int offSet=(int)(maxVal-minVal)/10;
            int dividePt=offSet;
            int bestThr=dividePt;
            minSumDiff=99999.0;

            //step 8
            while(dividePt<(maxVal-offSet)){
                //step 1
                setZero(GaussAry);
                //step 2
                sum1=fitGauss(0,dividePt,histAry,GaussAry,deBugFile);
                //step 3
                sum2=fitGauss(dividePt,maxVal,histAry,GaussAry,deBugFile);
                //step 4
                total=sum1+sum2;
                //step 5
                if(total<minSumDiff){
                    minSumDiff=total;
                    bestThr=dividePt;
                }
                //step 6
                deBugFile.write(dividePt+" "+sum1+" "+sum2+" "+total+" "+minSumDiff+" "+bestThr+"\n");
                //step 7
                dividePt++;
            }
            //step 9
            deBugFile.write("leaving biGauss method\n");
            deBugFile.write("minSumDiff: "+minSumDiff+"\n");
            deBugFile.write("bestThr: "+bestThr+"\n");
            //step 10
            return bestThr;
        }

        double computeMean(int leftIndex,int rightIndex,int maxHeight,int[] histAry,FileWriter deBugFile)throws IOException{
            //step 0
            deBugFile.write("Entering computeMean method\n");
            maxHeight=0;
            int sum=0;
            int numPixels=0;
            //step 1
            int index=leftIndex;
            //step 5
            while(index<rightIndex){
                //step 2
                sum+=(histAry[index]*index);
                numPixels+=histAry[index];
                //step 3
                if(histAry[index]>maxHeight){
                    maxHeight=histAry[index];
                }
                //step 4
                index++;
            }
            //step 6
            double result=(double)sum/(double)numPixels;
            //step 7
            deBugFile.write("Leaving compute mean method max Height is an result "+maxHeight+" "+result+"\n");
            //step 8
            return result;
        }

        double computeVar(int leftIndex,int rightIndex,double mean,int[] histAry,FileWriter deBugFile)throws IOException{
            //step 0
            deBugFile.write("Entering computeVar method\n");
            double sum=0.0;
            int numPixels=0;
            //step 1
            int index=leftIndex;
            //step 4
            while(index<rightIndex){
                //step 2
                sum+=(double)histAry[index]*(Math.pow((double)(index-mean),2));
                numPixels+=histAry[index];
                //step 3
                index++;
            }
            double result=sum/(double)numPixels;
            deBugFile.write("Leaving computeVar\n");
            deBugFile.write("result: "+result+"\n");
            return result;
        }

        double modifiedGauss(int x,double mean,double var,int maxHeight){
            double gx=maxHeight*Math.exp(-(double)(Math.pow(x-mean,2)/(2*var)));
            return gx;
        }

        double fitGauss(int leftIndex,int rightIndex,int[] histAry,int[] GaussAry,FileWriter deBugFile)throws IOException{
            //step 0
            deBugFile.write("Entering fitGauss method\n");
            double mean;
            double var;
            double sum=0.0;
            double Gval;
            double maxGval;
            //step 1
            mean=computeMean(leftIndex,rightIndex,maxHeight,histAry,deBugFile);
            var=computeVar(leftIndex,rightIndex,mean,histAry,deBugFile);
            //step 2
            int index=leftIndex;
            //step 7
            while(index<=rightIndex){
                //step 3
                Gval=modifiedGauss(index,mean,var,maxHeight);
                //step 4
                sum+=Math.abs(Gval-(double)histAry[index]);
                //step 5
                GaussAry[index]=(int)Gval;
                //step 6
                index++;
            }
            deBugFile.write("leaving fitGauss method\n");
            deBugFile.write("sum: "+sum+"\n");
            return sum;
        }
    }
    
    public static void main(String args[]) throws IOException,FileNotFoundException{
        //step 0
        Scanner inFile=new Scanner(new FileReader(args[0]));
        FileWriter outFile=new FileWriter(args[1]);
        FileWriter deBugFile=new FileWriter(args[2]);

        //step 1
        thresholdSelection tS=new thresholdSelection();
        tS.numRows=inFile.nextInt();
        tS.numCols=inFile.nextInt();
        tS.minVal=inFile.nextInt();
        tS.maxVal=inFile.nextInt();
        tS.histAry=new int[tS.maxVal+1];
        tS.maxHeight=tS.loadHist(tS.histAry,inFile);
        tS.GaussAry=new int[tS.maxVal+1];
        tS.setZero(tS.GaussAry);

        //step 2
        tS.dispHist(tS.histAry,outFile);

        //step 3
        tS.BiGaussThrVal=tS.biGauss(tS.histAry,tS.GaussAry,tS.maxHeight,tS.minVal,tS.maxVal,deBugFile);
        outFile.write("The Bi-Gaussian Auto-Selected Threshold Value: "+tS.BiGaussThrVal+"\n");

        //step 4
        inFile.close();
        outFile.close();
        deBugFile.close();
    }
}