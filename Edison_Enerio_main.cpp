#include <iostream>
#include <fstream>
using namespace std;

class ImageCompression{
    private:
        int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal;
    public:
        int **ZFAry;
        int **skeletonAry;

    ImageCompression(int rows, int cols, int minv, int maxv){
        numRows = rows;
        numCols = cols;
        minVal = minv;
        maxVal = maxv;
        ZFAry = new int*[rows + 2];
        skeletonAry = new int*[rows + 2];
        for(int i = 0; i < rows+2; i++){
            ZFAry[i] = new int[cols + 2]();
            skeletonAry[i] = new int[cols + 2]();
        }
    }

    ~ImageCompression(){
        delete[] ZFAry;
        delete[] skeletonAry;

        for(int i=0; i<numRows+2; i++){
            delete[] ZFAry[i];
            delete[] skeletonAry[i];
        }
    }

    void compute8Distance(int **zfa, ofstream& out){
        //algo in specs
        
        firstPass8Distance(zfa);
        reformatPrettyPrint(zfa, out); //proper caption
        secondPass8Distance(zfa);
        reformatPrettyPrint(zfa, out);  //proper caption
    }

    void firstPass8Distance(int **zfa){
        //algo in notes
    }

    void secondPass8Distance(int **zfa){
        //algo in notes
    }

    void imageCompression(int **zfa, int **sk, ofstream& skFile, ofstream& out){
        //algo in specs

        computeLocalMaxima(zfa, sk);
        reformatPrettyPrint(zfa, out);
        extractSkelecton(skeletonAry, skFile);
    }

    bool isLocalMaxima(int **arr, int i, int j){
        //A px is local maxima if >= to all of its 8 neighbors
        for(int k=-1; k < 2; k++){
            if(arr[i+k][j] > arr[i][j] || arr[i+k][j-1] > arr[i][j] || arr[i+k][j+1] > arr[i][j]){
                return false;
            }

            if(arr[i-1][j+k] > arr[i][j] || arr[i][j+k] > arr[i][j] || arr[i+1][j+k] > arr[i][j]){
                return false;
            }
        }
        return true;
    }

    void computeLocalMaxima(int **zfa, int **sk){
        //Check all px ZFAry[i][j]
        //if isLocalMaxima(ZFAry[i][j]), then skeletonAry[i][j] = ZFAry[i][j]
        //else, skeletonAry[i][j] = 0

        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(isLocalMaxima(zfa, i, j)){
                    sk[i][j] = zfa[i][j];
                }
                else{
                    sk[i][j] = 0;
                }
            }
        }
    }

    void extractSkelecton(int **sk, ofstream& out){
        //if skeletonAry[i][j] > 0, then write the triplet (i,j,skeletonAry[i][j] to skeletonFile)
    }

    void imageDeCompression(int **zfa, ofstream& out){
        //aglo in specs

        firstPassExpansion(zfa);
        reformatPrettyPrint(zfa, out);
        secondPassExpansion(zfa);
        reformatPrettyPrint(zfa, out);
    }

    void firstPassExpansion(int **zfa){
        //algo in notes
    }

    void secondPassExpansion(int **zfa){
        //algo in notes
    }

    void threshold(int **zfa, ofstream& out){
        //do threshold on px ZFAry with TH value 1
        //if ZFAry[i][j] >= 1, then output 1 and a blank space to decompressed file
        //else, output 0 and a blank space to decompressed file.
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(zfa[i][j] >= 1){
                    out << "1 ";
                }
                else{
                    out << "0 ";
                }
            }
        }
    }

    void reformatPrettyPrint(int **arr, ofstream& out){
        //algo in previous projects
    }
};