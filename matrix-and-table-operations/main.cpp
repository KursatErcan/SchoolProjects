//A "Matrix, Image and Table Operations" Program //

// Author      : Kürşat Ercan
// Student ID  : 348338
// e-mail      : kursat_ercan98@hotmail.com - 348338@ogr.ktu.edu.tr
// Mobile Phone: 05077939577

#include<iostream>
#include<ctime>
#include<fstream>
#include <string.h>
#include <cmath>

using namespace std;

int i, j, k;
template <typename K> class Matrix {
    protected:
        K **array;
        int row;	//satir
        int column;	//sutun
    public:
        Matrix();
        Matrix(int,int, K );
        Matrix(int,int,char);
        Matrix operator=(Matrix);
        Matrix operator*(int);
        Matrix operator*(Matrix);
        Matrix operator+(int);
        Matrix operator+(Matrix);
        Matrix operator-(int);
        Matrix operator-(Matrix);
        Matrix operator/(int);
        Matrix operator%(int);
        Matrix operator^(int);
        Matrix inv();
        Matrix emul(Matrix);
        void matrixCreator();
        Matrix T();
        int det();
        void resize(int,int);
        void print();
        void print(string);
};
template <typename K> class Image:public Matrix<K>{
    private:
        int imageHeader[54];
    public:
        Image();
        Image(int,int);
        Image(string,string);
        void imread(string,string);
        void imwrite(string,string);
        void color2gray();
        void gray2binary(int);
        void erosion();
        void dilation();
        void opening();
        void closing();
};
template <typename K> class Table:public Matrix<K>{
    private:
        string *rowNames;
        string *columnNames;
    public:
        Table();
        Table(int,int,int);
        Table(int,int,char);
        int itemAt(int,int);
        int itemAt(string);
        int itemAt(string,string);
        void setRowNames(string *,int);
        void setColNames(string *,int);
        void print();
        bool inArray(string *, string,int);
};
template<typename K> Matrix<K>::Matrix() {  //default constructor
		this->row = 10;
		this->column = 10;
		matrixCreator();
		for (i = 0; i < row; i++)			//diziye değer ataması
			for (j = 0; j < column; j++)
				array[i][j] = 0;			//dizinin bütün elemanları 0 olacak
}
template<typename K> Matrix<K>::Matrix(int row, int column, K value) {
    this->row = row;
    this->column = column;
    matrixCreator();
	for (i = 0; i < row; i++)
		for (j = 0; j < column; j++)
			array[i][j] = value;
}
template<typename K> Matrix<K>::Matrix(int row, int column, char ch) {
    this->row = row;
	this->column = column;

	matrixCreator();

	if (ch == 'e') {				//birim matris
		for (i = 0; i < row; i++)
			for (j = 0; j < column; j++)
				if (i == j) array[i][j] = 1.0;
				else array[i][j] = 0.0;
	}
    else if (ch == 'r') {			//matrise random deger atama
        srand(time(NULL));
		for (i = 0; i < row; i++)
			for (j = 0; j < column; j++)
				array[i][j] = rand() % 256;	//0-255 arasında random sayılar üretir
	}
	else {
		cout << "Hatali giris!" << endl;
	}
}
template<typename K>void Matrix<K>::matrixCreator() {						//iki boyutlu matrix oluşturma
	array = new K * [row];
	for (i = 0; i < row; i++)
		array[i] = new K [column];
}
template<typename K>void Matrix<K>::print() {								//ekrana bastır
    for (i = 0; i < this->row; i++) {
		for (j = 0; j < this->column; j++) {
            std::cout.width(5);
			cout << array[i][j];
		}
		cout << endl;
	}
}
template<typename K>void Matrix<K>::print(string txt) {						//dosyaya yazar
    fstream file;
	file.open(txt, ios::out);					//txt dosyasını yazma modunda açar

	if (file.is_open() == true) {
		for (i = 0; i < this->row; i++) {
			for (j = 0; j < this->column; j++) {
				file << array[i][j] << "  ";
			}
			file << endl;
		}
	}
	file.close();			//dosyayı kapat
}
template<typename K> Matrix<K> Matrix<K>::T() {                                   //transpoze hesaplar
    Matrix* trans = new Matrix(this->row, this->column, 0);
    for (i = 0; i < this->row; ++i) {
        for (j = 0; j < this->column; ++j)
        {
			trans->array[i][j] = this->array[j][i];
		}
    }
	return *trans;
}
template<typename K> Matrix<K> Matrix<K>::emul(Matrix c) {                        //eleman düzeyinde çarpma
    if (row != c.row || column != c.column){
		cout << "\n **Multiplication is not possible***" << endl;
		exit(1);
	}
	else {
		Matrix* temp = new Matrix(row, column, 0);
		for (i = 0; i < row; i++)
			for (j = 0; j < column; j++)
				temp->array[i][j] = array[i][j] * c.array[i][j];
        return *temp;
	}
}
template<typename K>int Matrix<K>::det() {                                         //determinant hesaplar
    if (row != column)
		cout << "\n *** Calculate determinant is not possible***" << endl;
	else {
        int det=0,l,m;
		if (row == 1) det = array[0][0];
		else{
			for(m=0;m<row;m++){
				Matrix<K>* temp = new Matrix<K>(row-1, column-1, 0.);
				k=0;
				for(i=0;i<row;i++){
                    if(i==m) continue;
                    l=0;
                    for(j=0;j<column;j++){
                        if(j==0) continue;
                        temp->array[k][l]=array[i][j];
                        l++;
                    }
                    k++;
				}
				det+=array[m][0]*temp->det()*((m%2==0)?1:-1);
			}
		}
		return det;
	}
}
template<typename K> Matrix<K> Matrix<K>::inv() {//kullanacak nesneler float tipinde tanımlamış olamalı
	if (this->row != this->column) {
		cout << "\n***To inverse the matrix is not possible***" << endl;
		exit(1);
	}
	else {
		//Matrix* unitMatrix = new Matrix(this->row, this->column, 'e');
		Matrix<K> *unitMatrix=new Matrix<K>(row,column,'e');

        Matrix<K>* temp = new Matrix<K>(row, column, 0.);
        float x, y;
        for (i = 0; i < row; i++)
            for (j = 0; j < column; j++)
                temp->array[i][j] = array[i][j];
        for (i = 0; i < row; i++) {
			x = temp->array[i][i];
			for (j = 0; j < row; j++) {
				temp->array[i][j] = temp->array[i][j] / x;
				unitMatrix->array[i][j] = unitMatrix->array[i][j] / x;
			}
			for (k = 0; k < row; k++) {
				if (k != i) {
					y = temp->array[k][i];
					for (j = 0; j < row; j++) {
						temp->array[k][j] = temp->array[k][j] - (temp->array[i][j] * y);
						unitMatrix->array[k][j] = unitMatrix->array[k][j] - (unitMatrix->array[i][j] * y);
					}
				}
			}
		}
        return *unitMatrix;
	}
}
template<typename K>void Matrix<K>::resize(int r, int c) {                  //yeniden boyutlandırma yapar
    if (row < 0 || column < 0) {
		cout << "***Invalid resize***";
		return;
	}
	else {
		int temp[r][c];
		for (i=0; i < r; i++) {
			for (j=0; j < c; j++) {
                if(i>=row||j>column) temp[i][j]=0;
                else temp[i][j] = array[i][j];
			}
		}
		row=r;
		column=c;
		this->matrixCreator();
		for (i = 0; i < r; i++) {
			for (j = 0; j < c; j++) {
				this->array[i][j] = temp[i][j];
			}
		}
	}
}
template<typename K> Matrix<K> Matrix<K>::operator=(Matrix c) {
		if (row != c.row || column != c.column)
		{
			cout << "\n***Loading failed***" << endl;
			exit(1);
		}
		else {
			row=c.row;
			column=c.column;
			for (i = 0; i < row; i++)
				for (j = 0; j < column; j++)
					array[i][j] = c.array[i][j];
            return *this;
		}
}
/*template<typename K> Matrix<K> Matrix<K>::operator+(Matrix c) {
		if (row != c.row || column != c.column)
		{
			cout << "\n ***Addition is not possible***" << endl;
			exit(1);
		}
		else {
            Matrix<K> *temp=new Matrix<K>(row,column,0.);
			for (i = 0; i < row; i++)
				for (j = 0; j < column; j++)
					temp->array[i][j] = array[i][j] + c.array[i][j];
			return *temp;
		}
}*/
template<typename K> Matrix<K> Matrix<K>::operator-(Matrix c) {
    if (row != c.row || column != c.column)
	{
		cout << "\n ***Subtraction is not possible***" << endl;
		exit(1);
	}
	else {
        Matrix<K> *temp=new Matrix<K>(row,column,0.);
        for (i = 0; i < row; i++)
			for (j = 0; j < column; j++)
				temp->array[i][j] = array[i][j] - c.array[i][j];
		return *temp;
	}
}
template<typename K> Matrix<K> Matrix<K>::operator*(Matrix c) {
	if (column != c.row)
	{
		cout << "\n ***Multiplication is not possible***" << endl;
		exit(1);
	}
	else {
		Matrix<K> *temp=new Matrix<K>(row,c.column,0.);
		for (i = 0; i < row; i++){
            for (j = 0; j < c.column; j++){
                temp->array[i][j] = 0;
                for (k = 0; k < c.row; k++){
                    temp->array[i][j] += array[i][k] * c.array[k][j];
                }
            }
        }
        return *temp;
	}
}
template<typename K> Matrix<K> Matrix<K>::operator+(int c){
	Matrix<K> *temp=new Matrix<K>(row,column,0.);
        for (i = 0; i < row; i++)
            for (j = 0; j < column; j++)
                temp->array[i][j] = array[i][j] + c;
        return *temp;
}
template<typename K> Matrix<K> Matrix<K>::operator-(int c){
	Matrix<K> *temp=new Matrix<K>(row,column,0.);
        for (i = 0; i < row; i++)
            for (j = 0; j < column; j++)
                temp->array[i][j] = array[i][j] - c;
        return *temp;
}
/*template<typename K> Matrix<K> Matrix<K>::operator*(int c){
	Matrix<double> *temp=new Matrix<double>(row,column,0.0);
        for (i = 0; i < row; i++)
            for (j = 0; j < column; j++)
                temp->array[i][j] = array[i][j] * c;
        return *temp;
}*/
template<typename K> Matrix<K> Matrix<K>::operator/(int c){
	Matrix<K> *temp=new Matrix<K>(row,column,0.);
        for (i = 0; i < row; i++)
            for (j = 0; j < column; j++)
                temp->array[i][j] = array[i][j] / c;
        return *temp;
}
template<typename K> Matrix<K> Matrix<K>::operator%(int c){
	Matrix<K> *temp=new Matrix<K>(row,column,0.);
        for (i = 0; i < row; i++)
            for (j = 0; j < column; j++)
                temp->array[i][j] = array[i][j] % c;
        return *temp;
}
template<typename K> Matrix<K> Matrix<K>::operator^(int c){
	Matrix<K> *temp=new Matrix<K>(row,column,0.);
        for (i = 0; i < row; i++)
            for (j = 0; j < column; j++)
                temp->array[i][j] = pow(array[i][j],c);
        return *temp;
}
template<typename K> Image<K>::Image():Matrix<K>(255,255,0){}
template<typename K> Image<K>::Image(int width, int height):Matrix<K>(width,height,0){}
template<typename K> Image<K>::Image(string filename, string format):Matrix<K>(){
    if(format=="bmp"){              //bmp file read
        ifstream iFile;
        iFile.open(filename,ios::binary);

        if(iFile.is_open()){
           for(int i=0;i<54;i++){
                imageHeader[i]=iFile.get();
            }
            int r=imageHeader[18];
            int c=imageHeader[22];
            this->resize(r,c);
            for(i=0;i<(this->row);i++){
                for(j=0;j<(this->column);j++){
                   this->array[i][j]=iFile.get();
                }
            }
            iFile.close();
            //view imageHeader
            //for(i=0;i<54;i++) cout<<std::hex<<imageHeader[i]<<" ";
           /* view pixels
            for(i=0;i<this->row;i++){
                for(j=0;this->column;j++){
                    cout<<std::hex<<this->array[i][j]<<" ";
                }
                cout<<endl;
            }
            */
        }
        else cout<<"File opening failed."<<endl;
    }
    else if(format=="bin"){              //bin file read
        ifstream iFile;
        iFile.open(filename,ios::in);

         if(iFile.is_open()){
            int r=iFile.get();
            int c=iFile.get();
            this->resize(r,c);

           for(int i=0;i<r;i++){
                for(j=0;j<c;j++)
                    this->array[i][j]=iFile.get();
            }
            iFile.close();
            //view file contents
        /*
            for(i=0;i<this->row;i++){
                for(j=0;j<this->column;j++){
                    cout<<std::hex<<this->array[i][j]<<" ";
                }
                cout<<endl;
            }
        */
        }
        else cout<<"File opening failed."<<endl;
    }
    else cout<< "Unknown file format!"<<endl;
}
template<typename K>void Image<K>::imread(string filename, string format){

}
template<typename K>void Image<K>::imwrite(string filename, string format){
    if(format=="bmp"){          //bmp file write
        ofstream oFile;
        oFile.open(filename,ios::binary);
        if(oFile.is_open()){
            for(int i=0;i<54;i++){
                oFile<<std::hex<<imageHeader[i]<<" ";
            }
            for(i=0;i<this->row;i++){
                for(j=0;j<this->column;j++){
                    oFile<<std::hex<<this->array[i][j]<<" ";
                }
            }
            oFile.close();
            cout<<"File write successful!"<<endl;
        }
        else cout<<"File opening failed."<<endl;
    }
    else if(format=="bin"){                                //bin file write
        ofstream oFile;
        oFile.open(filename,ios::out);
        if(oFile.is_open()){
            oFile<<std::hex<<this->row<<" "<<std::dec<<this->column;
            for(i=0;i<this->row;i++){
                for(j=0;j<this->column;j++){
                    oFile<<std::hex<<this->array[i][j]<<" ";
                }
            }
            oFile.close();
            cout<<"File write successful!"<<endl;
        }
        else cout<<"File opening failed."<<endl;
    }
    else cout<< "Unknown file format!"<<endl;
}
template<typename K>void Image<K>::color2gray() {}
template<typename K> Table<K>::Table():Matrix<K>(10,10,0){}
template<typename K> Table<K>::Table(int r, int w,int d):Matrix<K>(r,w,d){}
template<typename K> Table<K>::Table(int r, int w,char ch):Matrix<K>(r,w,ch){}
template<typename K> int Table<K>::itemAt(int r,int c){return this->array[r][c];}
template<typename K> int Table<K>::itemAt(string ch){
    int c=int(ch[0])-65;
    int r=0;
    for(i=1;i<ch.length();i++){
        r+=(ch[i]-'0')*pow(10,(ch.length()-i-1));
    }
    return this->array[r][c];
}
template<typename K> int Table<K>::itemAt(string rs,string cs){
    int c=0,r=0;
    if(inArray(rowNames,rs,this->row)){     // rs in rowNames
        for(i=0;i<this->row;i++){
            if(rowNames[i]==rs){
            r=i;
            continue;
            }
        }
    }
    else{                                   // rs not in rowNames
         for(i=0;i<rs.length();i++){
            r+=(rs[i]-'0')*pow(10,(rs.length()-i-1));
        }
    }
    if(inArray(columnNames,cs,this->column)){   // cs in columnNames
        for(i=0;i<this->column;i++){
            if(columnNames[i]==cs){
            c=i;
            break;
            }
        }
    }
    else{                                       // cs not in columnNames
        c=int(cs[0])-65;
    }
    return this->array[r][c];
}
template<typename K> void Table<K>::setRowNames(string s[],int n){    //row name assignment
    rowNames = new string;
    for (i = 0; i < n; i++)
        if(s[i].length()>8){
            cout<<"Row length must not exceed 8 characters!";
        }
        else rowNames[i]=s[i];
}
template<typename K> void Table<K>::setColNames(string s[],int n){    //column name assignment
    columnNames = new string;
    for (i = 0; i < n; i++)
        if(s[i].length()>4){
            cout<<"Column length should not exceed 4 characters!";
        }
        else columnNames[i]=s[i];
}
template<typename K> void Table<K>::print(){        //print with row and column names
    cout<<"        ";
    for(i=0;i<this->column;i++){
        std::cout.width(8);
        cout<<columnNames[i];
    }
    cout<<endl;
    for(i=0;i<this->row;i++){
        cout<<endl;
        std::cout.width(8);
        cout<<rowNames[i];
        for(j=0;j<this->column;j++){
            std::cout.width(8);
            cout<<this->array[i][j];
        }
    }
}
template<typename K> bool Table<K>::inArray(string sArray[], string s,int n){   //my inArray func. (return bool)
    for(i=0;i<n;i++){
        if(sArray[i]==s) return true;
    }
    return false;
}

int main(){
	//Matrix<int>* m1 = new Matrix<int>(5, 4, "e");
	//m1->print();
	//cout << endl;
	/*
	Table<int> *m2= new Matrix<int>(4,4,"e");
	string s1 = { "satir1","satir2","satir3","satir4" };
	string s2 = { "sutun1","sutun2","sutun3","sutun4" };
	m2->setRowNames(s1, 4);
	m2->setColNames(s2, 4);
	cout << m2->itemAt("satir1", "sutun2");

	cout << endl << endl;
	m2->print();
	*/
	int sec=1;
	  switch(sec){
	    case 1 :{
	      Matrix<int> a(2,3,1);
	      a.print();
	      Matrix<int> b(2,3,9);
	      b.print();
	      a = b + a*1;
	      a.print("file.txt"); // iÃ§inde 10 olan matris olmalÄ±
	      break;
	}
	    case 2 :{
	      Matrix<double> m1(4,4,'e');
	      Matrix<double> m2(4,4,'r'); // (5,5) boyutu dene
	      m2.print();
	      m1 = m1 * m2;
	      m1.print();
	      break;}
	    case 3 :{
	      // STL vector kullanan buradan puan alamaz
	      Matrix<double> m3(5,4,'r');
	      m3.print();
	      // aÅŸaÄŸÄ±daki durumlarÄ± ayrÄ± ayrÄ± dene
	      //m3.resize(2,6);
	      m3.resize(10,6);
		cout<<endl;
		m3.print();
	      //m3.resize(10,3);
		cout<<endl;
	      m3.print();
	      break;}
	    case 4 :{
	      // lineer matris oluÅŸtur
	      Matrix<double> m4(2,2,'r');
	      Matrix<double> m5 = m4 * m4.inv();
	      m5.print(); // birim matris olmalÄ±

	      Matrix<double> m6(3,3,'r');
	      cout<<m6.det(); // sonuÃ§ 0 olmalÄ±
	      break;
	    }
	    case 9 :{
	      /*
	      Matrix sÄ±nÄ±fÄ±ndan miras alÄ±nacak,
	      matris, geniÅŸlik, yÃ¼kseklik verisi Table sÄ±nÄ±fÄ± iÃ§inde olmayacak, miras alÄ±nacak
	      */
	      Table<int> t1(4,4,1);
	      t1.print(); // satÄ±r ve sÃ¼tun etiketleri de yazÄ±lmalÄ±
	      cout<<endl;
	      string s[] = {"Sa1","Sa2"};

	      t1.setRowNames(s,2);
	      t1.setColNames(s,2);
	      t1.print();
	      break;}
	    case 10 :{
	      Table<int> t2(3,4,'r');
	      t2.print();
		cout<<endl;
	      cout<<t2.itemAt(1,1)<<endl; // (1,1) deki eleman
	      cout<<t2.itemAt("A2")<<endl;  // (2,0) deki eleman
	      string s2[] = {"S1","S2","S3"};
	      t2.setColNames(s2,3);
	      cout<<t2.itemAt("1","S2")<<endl;  // (1,1) deki eleman
	      break;}

	  }
    return 0;
}







