#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int dataTraining[4000][6];
int dataTesting[1000][6];
float hasilDistance[4000][2];
int hasilAkhir[1000];
int i = 0;
float vote1, vote2 = 0;

string berita;
string like;
string provokasi;
string komentar;
string emosi;
string hoax;

void tulisHasil(){
    ofstream myfile;
    myfile.open ("hasilAkhir.csv");
    for(int i= 0; i < 1000; i++){
        myfile << dataTesting[i][0];
        myfile << ";";
        myfile << hasilAkhir[i];
        if(i < 999){
            myfile << "\n";
        }
    }
    myfile.close();
}


void bacaDataTraining(){
    ifstream ip("dataTraining.csv");
	if(!ip.is_open()){
		cout << "File Data Training tidak dapat dibuka!" << endl;
	}else{
	    int i = 0;
	    cout << "Proses pembacaan data training.." << endl;
        while(ip.good()){
            getline(ip,berita,';');
            getline(ip,like,';');
            getline(ip,provokasi,';');
            getline(ip,komentar,';');
            getline(ip,emosi,';');
            getline(ip,hoax,'\n');

            dataTraining[i][0] = i+1;
            dataTraining[i][1] = atoi(like.c_str());
            dataTraining[i][2] = atoi(provokasi.c_str());
            dataTraining[i][3] = atoi(komentar.c_str());
            dataTraining[i][4] = atoi(emosi.c_str());
            dataTraining[i][5] = atoi(hoax.c_str());
            i++;
        }
        ip.close();
	}
}

void bacaDataTesting(){
    ifstream ip("dataTesting.csv");
	if(!ip.is_open()){
		cout << "File Data Testing tidak dapat dibuka!" << endl;
	}else{
	    cout << "Proses pembacaan data testing.." << endl;
	    i = 0;
        while(ip.good()){
            getline(ip,berita,';');
            getline(ip,like,';');
            getline(ip,provokasi,';');
            getline(ip,komentar,';');
            getline(ip,emosi,';');
            getline(ip,hoax,'\n');

            dataTesting[i][0] = i+4001;
            dataTesting[i][1] = atoi(like.c_str());
            dataTesting[i][2] = atoi(provokasi.c_str());
            dataTesting[i][3] = atoi(komentar.c_str());
            dataTesting[i][4] = atoi(emosi.c_str());
            dataTesting[i][5] = atoi(hoax.c_str());
            i++;
        }
        ip.close();
	}
}
void hitung(int j){
    i = 0;
    while (i < 4000){
        float hasil;
        hasil = sqrt ( pow((dataTesting[j][1] - dataTraining[i][1]),2) + pow((dataTesting[j][2] - dataTraining[i][2]),2) + pow((dataTesting[j][3] - dataTraining[i][3]),2) + pow((dataTesting[j][4] - dataTraining[i][4]),2) );
        hasilDistance[i][0] = hasil;
        hasilDistance[i][1] = dataTraining[i][5];
        //cout << i << " - " << hasilDistance[i][0] << " - " <<  hasilDistance[i][1] << endl;
        i++;
    }
}

void sortDistance()
{
    float temp, temp2;
    for(int i = 1; i < 4000 ; i++)
    {
        for(int j = 0; j < 4000-i; j++)
        {
            if(hasilDistance[j][0] > hasilDistance[j+1][0])
            {
                temp = hasilDistance[j][0];
                temp2 = hasilDistance[j][1];
                hasilDistance[j][0] = hasilDistance[j+1][0];
                hasilDistance[j][1] = hasilDistance[j+1][1];
                hasilDistance[j+1][0] = temp;
                hasilDistance[j+1][1] = temp2;

            }
        }
    }
}

int main( int argc, char* argv[] )
{
        cout << "========== k-NN =========" << endl;
        cout << "========== AWAN =========" << endl;
        //membaca data training dan testing dan memindahkannya kesebuah variabble array
        bacaDataTraining();
        bacaDataTesting();

        //mengulang sebnyak 1000 kali
        int x = 0;
        cout << "Proses menghitung Euclidean Distance, Mengurutkan dan Menentukan hasil.." << endl;

        while (x < 1000){
            vote1 = 0;
            vote2 = 0;

            //menghitung distance
            hitung(x);

            //mengurutkan
            sortDistance();

            i=0;
            while (i < 1501){
                //cout << hasilDistance[i][0] << " " << hasilDistance[i][1] << endl;
                if(hasilDistance[i][1] == 0){
                    vote1 = vote1 + 1;
                }else if (hasilDistance[i][1] == 1){
                    vote2 = vote2 + 1;
                }
                i++;
            }
            //cout << "Voting: " << (vote1/2000)*100 << "% - " << (vote2/2000)*100 << "%" << endl;
            if(vote1 > vote2){
                hasilAkhir[x] = 0;
            }else if(vote1 < vote2){
                hasilAkhir[x] = 1;
            }
            x++;
        }
        cout << "Proses Membuat File Excel.." << endl;
        tulisHasil();

        cout << "Memuat hasil akhir.." << endl;
        cout << "Hasil: " << endl;

        ifstream ip("hasilAkhir.csv");
        if(!ip.is_open()){
            cout << "File tidak dapat dibuka!" << endl;
        }else{
            i = 0;
            while(ip.good()){
                getline(ip,berita,';');
                getline(ip,hoax,'\n');

                cout << "Berita: " << berita << " - ";
                cout << "Hoax: " << hoax << endl;

                i++;
            }
            ip.close();
        }
        return 0;
}
