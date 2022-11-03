#include <iostream>
#include <fstream>
using namespace std;

class ImageCompression{
    public:
        int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal;
        int **ZFAry;
        int **skeletonAry;

    ImageCompression(int rows, int cols, int minv, int maxv){
        numRows = rows;
        numCols = cols;
        minVal = minv;
        maxVal = maxv;
        newMinVal = 9999;
        newMaxVal = 0;
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

    void setZero(int **arr){

        for(int i=0; i<=numRows+1; i++){
            for(int j=0; j<=numCols+1; j++){
                arr[i][j] = 0;
            }
        }
    }

    void loadImage(ifstream& in, int **zfa){
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                in >> zfa[i][j];
            }
        }
    }

    void loadSkeleton(ifstream& in, int **arr){
        int i, j, val;
        while(!in.eof()){
            in>>i;
            in>>j;
            in>>val;
            arr[i][j] = val;
        }
    }

    void compute8Distance(int **zfa, ofstream& out){
        //algo in specs
        
        firstPass8Distance(zfa);
        out << "Array after first pass: " << endl;
        reformatPrettyPrint(zfa, out); //proper caption
        secondPass8Distance(zfa);
        out << "Array after second pass: " << endl;
        reformatPrettyPrint(zfa, out);  //proper caption
    }

    void firstPass8Distance(int **ZFAry){
        //algo in notes

        int a,b,c,d, px;
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                px = ZFAry[i][j];
                if(px > 0){
                    a = ZFAry[i-1][j-1];
                    b = ZFAry[i-1][j];
                    c = ZFAry[i-1][j+1];
                    d = ZFAry[i][j-1];

                    ZFAry[i][j] = min(min(min(a, b), c), d) + 1;
                }
            }
        }
    }

    void secondPass8Distance(int **ZFAry){
        //algo in notes

        int e,f,g,h,px;
        for(int i=numRows; i>=1; i--){
            for(int j=numCols; j>=1; j--){
                px = ZFAry[i][j];
                if(px > 0){
                    e = ZFAry[i][j+1] + 1;
                    f = ZFAry[i+1][j-1] + 1;
                    g = ZFAry[i+1][j] + 1;
                    h = ZFAry[i+1][j+1] + 1;

                    ZFAry[i][j] = min(min(min(min(e, f), g), h), px);

                    if(ZFAry[i][j] > newMaxVal){
                        newMaxVal = ZFAry[i][j];
                    }
                    if(ZFAry[i][j] < newMinVal){
                        newMinVal = ZFAry[i][j];
                    }
                }
            }
        }
    }

    void imageCompression(int **zfa, int **sk, ofstream& skFile, ofstream& out){
        //algo in specs

        computeLocalMaxima(zfa, sk);
        out << "Local Maxima, skeletonAry:" << endl;
        reformatPrettyPrint(sk, out);
        extractSkelecton(sk, skFile);
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
        out << numRows << " ";
        out << numCols << " ";
        out << minVal << " ";
        out << maxVal << endl;
        
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(sk[i][j] > 0){
                    out << i << " " << j << " " << sk[i][j] << endl;
                }
            }
        }
    }

    void imageDeCompression(int **zfa, ofstream& out){
        //aglo in specs

        firstPassExpansion(zfa);
        out << "First pass expansion: " << endl;
        reformatPrettyPrint(zfa, out);
        secondPassExpansion(zfa);
        out << "Second pass expansion: " << endl;
        reformatPrettyPrint(zfa, out);
    }

    void firstPassExpansion(int **zfa){
        //algo in notes

        int a,b,c,d,e,f,g,h,px;
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                px = zfa[i][j];
                if(px == 0){
                    a = zfa[i-1][j-1] - 1;
                    b = zfa[i-1][j] - 1;
                    c = zfa[i-1][j+1] - 1;
                    d = zfa[i][j-1] - 1;
                    e = zfa[i][j+1] - 1;
                    f = zfa[i+1][j-1] - 1;
                    g = zfa[i+1][j] - 1;
                    h = zfa[i+1][j+1] - 1;

                    zfa[i][j] = max(max(max(max(max(max(max(max(a, b), c), d), e), f), g), h), px);
                }
            }
        }
    }

    void secondPassExpansion(int **zfa){
        //algo in notes

        int a,b,c,d,e,f,g,h,px, currMax;
        for(int i=numRows; i>=1; i--){
            for(int j=numCols; j>=1; j--){
                px = zfa[i][j];
                a = zfa[i-1][j-1] - 1;
                b = zfa[i-1][j] - 1;
                c = zfa[i-1][j+1] - 1;
                d = zfa[i][j-1] - 1;
                e = zfa[i][j+1] - 1;
                f = zfa[i+1][j-1] - 1;
                g = zfa[i+1][j] - 1;
                h = zfa[i+1][j+1] - 1;

                currMax = max(max(max(max(max(max(max(max(a, b), c), d), e), f), g), h), px);

                if(zfa[i][j] < currMax){
                    zfa[i][j] = currMax;
                }
            }
        }
    }

    void threshold(int **zfa, ofstream& out, int th){
        //do threshold on px ZFAry with TH value 1
        //if ZFAry[i][j] >= 1, then output 1 and a blank space to decompressed file
        //else, output 0 and a blank space to decompressed file.

        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(zfa[i][j] >= th){
                    out << "1 ";
                }
                else{
                    out << "0 ";
                }
            }
            out << endl;
        }
    }

    void reformatPrettyPrint(int **arr, ofstream& out){
        //algo in previous projects

        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(arr[i][j] == 0){
                    out << ". ";
                }else{
                    out << arr[i][j] << " ";
                }
            }
            out << endl;
        }
    }
};

int main(int argc, char *argv[]){

    if(argc != 3){
        cout << "Invalid arguments, ending program..." << endl;
        return 1;
    }
    
    //step 0
    ifstream inFile;
    inFile.open(argv[1]);
    
    ofstream outFile1;
    outFile1.open(argv[2]);

    int rows, cols, minv, maxv;
    if(inFile.is_open()){
        inFile >> rows;
        inFile >> cols;
        inFile >> minv;
        inFile >> maxv;
    }
    
    ImageCompression obj = ImageCompression(rows, cols, minv, maxv);
    
    //step 1
    string skeletonFileName = string(argv[1]) + "_skeleton.txt";

    //step 2
    ofstream skeletonFile;
    skeletonFile.open(skeletonFileName);

    //step 3
    string deCompressedFileName = string(argv[1]) + "_deCompressed.txt";

    //step 4
    ofstream deCompressFile;
    deCompressFile.open(deCompressedFileName);

    //step 5
    obj.setZero(obj.ZFAry);
    obj.setZero(obj.skeletonAry);

    //step 6
    obj.loadImage(inFile, obj.ZFAry);

    //step 7
    obj.compute8Distance(obj.ZFAry, outFile1);

    //step 8
    obj.imageCompression(obj.ZFAry, obj.skeletonAry, skeletonFile, outFile1);

    //step 9
    skeletonFile.close();

    //step 10
    ifstream skeletonFile2;
    skeletonFile2.open(skeletonFileName);

    //step 11
    obj.setZero(obj.ZFAry);

    //step 12
    obj.loadSkeleton(skeletonFile2, obj.ZFAry);

    //step 13
    obj.imageDeCompression(obj.ZFAry, outFile1);

    //step 14
    deCompressFile << obj.numRows << " ";
    deCompressFile << obj.numCols << " ";
    deCompressFile << obj.newMinVal << " ";
    deCompressFile << obj.newMaxVal << " ";
    deCompressFile << endl;

    //step 15
    obj.threshold(obj.ZFAry, deCompressFile, 1);

    //step 16
    skeletonFile2.close();
    inFile.close();
    outFile1.close();

    return 0;
}