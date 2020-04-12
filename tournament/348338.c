#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
struct takimlar{
    char nickName;//takimin takma adi
    char name[12];//takim ismi max 12 karakter
    int score[8];//{OM,G,B,M,AG,YG,A,P}
    char *dep_gelenler;//deplasmana gelen takimlari tutan dizi
}*takim;  // :Dosyadan okunan takim sayisi degeri

void set_nickName(int t_sayi){//takimlara takma isim atayan fonksiyon
    int i;
    char harf='A';
    for(i=0;i<t_sayi;i++) takim[i].nickName='\0';//nickName'leri temizler
    for(i=0;i<t_sayi;i++){
        takim[i].nickName=harf;//nickName lere harf atar
        harf++;
    }
}
void set_name(int t_sayi){//dosyadan okudugu isimleri sirasiyla takimlara setleyen fonksiyon
    int i;
    FILE *takimName=fopen("takimlar.txt","r");
        for(i=0;i<t_sayi;i++){
            fscanf(takimName,"%s\n",takim[i].name);
        }
    fclose(takimName);
}
int ErrorDedection(char *string,int t_sayi){//hata tespiti yapan fonksiyon
    int i,j;
    int error;
    for(i=0;i<t_sayi;i++){
        if(takim[i].nickName==string[0]){//ev sahibi takimi arar
            for(j=0;j<sizeof(takim[i].dep_gelenler[j]);j++){//ev sahibi takimin deplasmana gelenler dizisinde gezinir
                if(takim[i].dep_gelenler[j]==0||takim[i].dep_gelenler[j]!=string[2]){//deplasmana gelenler dizisi boþmu veya indisleri string[2] den farklý mý bakar
                     error=0;//hata yok!
                }
                else{
                    error= 1;//hata var!
                    break;//hatayi buldugunda fonksiyondan cikar
                }
            }
        }
    }
    return error;
}
void skor_guncelle(char *string,int t_sayi){//score degerlerini güncelleyen fonksiyon
    int ev_gol=string[1]-'0'; // stringden 'char' olarak çekilen ev sahibinin gol sayisini int'e donustutur
    int dep_gol=string[3]-'0';// stringden 'char' olarak çekilen deplasman takiminin gol sayisini int'e donustutur
    int i,j;//i->struct içi degerler,j->takimlarin numaralari
    for(j=0;j<t_sayi;j++){
        if(string[0]==takim[j].nickName){ //ev sahibi takimlari taramak için
            takim[j].score[0]+=1; //OM günceller
            takim[j].score[4]+=ev_gol; //AG günceller
            takim[j].score[5]+=dep_gol;//YG günceller

            for(i=0;takim[j].dep_gelenler[i]!='\0';i++);//dep_gelenler dizinsindeki ilk bos indisi bulur
            takim[j].dep_gelenler[i]=string[2];// DEPLASMANA GELEN TAKIMLARi dep_gelenler dizisine atar
            if(ev_gol>dep_gol) takim[j].score[1]+=1;//GALIBIYET SAYIsini gunceller
            else if(ev_gol<dep_gol) takim[j].score[3]+=1;//MALUBIYET SAYISIni gunceller
            else takim[j].score[2]+=1;//BERABRLIK SAYISIni gunceller
        }
    }

    for(j=0;j<t_sayi;j++){
        if(string[2]==takim[j].nickName){ //deplasman takimlarini taramak için
            takim[j].score[0]+=1; //OM gunceller
            takim[j].score[4]+=dep_gol; //AG gunceller
            takim[j].score[5]+=ev_gol; // YG gunceller
            if(ev_gol>dep_gol) takim[j].score[3]+=1; //MALUBIYET SAYISIni gunceller
            else if(ev_gol<dep_gol) takim[j].score[1]+=1;//GALIBIYET SAYIsini gunceller
            else takim[j].score[2]+=1; //BERABRLIK SAYISIni gunceller
        }
    }
}
void skor_guncelle2(int g_puan,int b_puan,int m_puan,int t_sayi){ //AVERAJ - PUAN degerlerini hesaplar
    int i,k;
    for(i=0;i<t_sayi;i++){
            takim[i].score[6]=takim[i].score[4]-takim[i].score[5]; //AVERAJ hesaplar
    }
    for(i=0;i<t_sayi;i++){
        for(k=1;k<4;k++){
        takim[i].score[7]=takim[i].score[1]*g_puan+takim[i].score[2]*b_puan+takim[i].score[3]*m_puan; //PUAN hesaplar
        }
    }
}
void mystrcpy(char *s1, const char *s2) {//stringleri kopyalayan fonksiyon
  while (*(s1++ )= *(s2++));
}
int myStrcmp(const char *s1, const char *s2)//string karþýlaþtýran fonksiyon
{
        int retValue = 0;
        while( *s1 == *s2 )//iki stringinde ayný insidindeki harfleri eþit oldugu muddetce döner
        {
            if(*s1 == '\0')
            {
                retValue = 0;
                break;
            }
                ++s1;
                ++s2;
        }
        retValue = *s1 - *s2;// iki karakterin farki ile geri döner.

        return retValue;
}
void fiksturYazdir(int t_sayi){//fikstur tablosunu yazdýran fonksiyon
    int i, j,k,secim=1;
    printf("\nPuana gore siralama: 1\nTakma isme gore siralama: 2\nIsme gore siralama: 3\n");
    scanf("%d",&secim);
    printf("*********************************PUAN DURUM TABLOSU**********************************\n\n");
    printf("Nick     Name          OM  \tG\tB\tM\tAG\tYG\tA\tP\n");

    if(secim==1){//puana gore siralar
        int max=takim[0].score[7];
        int min=takim[0].score[7];
        for(i=1;i<t_sayi;i++){
            if(max<takim[i].score[7]){//max puan degerini bulur.
                max=takim[i].score[7];
            }
            if(min>takim[i].score[7]){//min puan degerini bulur.
                min=takim[i].score[7];
            }
        }
        while(min<=max){//max-min puanlar arasinda gezinerek büyükten kücüge puan tablosunu yazdirir
            for(j=0;j<t_sayi;j++){
                if(takim[j].score[7]==max){
                    printf("%c\t",takim[j].nickName);
                    printf("%-15s",takim[j].name);
                    for(k=0;k<8;k++){
                        printf("%-5d\t",takim[j].score[k]);
                    }

                    printf("\n");
                }
            }
            max--;
        }
    }
    else if(secim==2){//takma isme gore siralar
        for(i=0;i<t_sayi;i++){ //butun harfler sirayla atandigindan takimlar sirayla basilir
            printf("%c\t",takim[i].nickName);
            printf("%-15s",takim[i].name);
            for(j=0;j<8;j++){
                printf("%-5d\t",takim[i].score[j]);
            }
	    printf("\n");

        }
    }
    else if(secim==3){//isme gore sýralar
        char teamName[t_sayi][30],temp[30];
        for(i=0;i<t_sayi;i++){
                mystrcpy(teamName[i],takim[i].name);//tüm name'ler diziye atilir
            }
        for(i=0;i<t_sayi - 1;i++){//dizinin elemanlari karsilastirilip siralanir
            for(j=0;j<t_sayi - i - 1;j++){
                if(myStrcmp(teamName[j],teamName[j + 1])>0){
                    mystrcpy(temp,teamName[j]);
                    mystrcpy(teamName[j],teamName[j + 1]);
                    mystrcpy(teamName[j + 1],temp);
                }
            }
        }
        for(i=0;i<t_sayi;i++){//siralanan isimler dizisindeki isimlere göre takimlar basilir
                for(j=0;j<t_sayi;j++){
                    if(myStrcmp(teamName[i],takim[j].name) == 0){
                        printf("%c\t",takim[j].nickName);
			printf("%-15s",takim[j].name);
                        for(k=0;k<8;k++){
                            printf("%-5d\t",takim[j].score[k]);
                        }
			printf("\n");
                    }

                }
        }
    }
        printf("\n*************************************************************************************\n");
}

void girdiler(){//girdilerin alindigi fonksiyon
    int i,j;
    int t_sayi, g_puan, b_puan, m_puan;
    int secim;
    char string[4],secim1;
    FILE *ayarlar=fopen("ayarlar.txt","r");
    if(ayarlar!=NULL){//Ayarlar text dosyasyndaki galibiyet-beraberlik-malubiyet puanlarini ve toplam takim sayisini okur
        fscanf(ayarlar,"%d\n%d\n%d\n%d",&t_sayi,&g_puan,&b_puan,&m_puan);
    }
    else printf("Dosya Bulunamadi!");
    fclose(ayarlar);

    takim=(int*)malloc(sizeof (int)*t_sayi);//takim struct una boyut atanir
    for(i=0;i<t_sayi;i++)  takim[i].dep_gelenler=(int*)malloc(sizeof (char)*20);//dep_gelenler e boyut atanir


    for(i = 0;i < t_sayi;i++){//score degerleri 0 lanýr
        for(j = 0;j < 8;j++){
            takim[i].score[j] = 0;
        }
    }

    set_nickName(t_sayi);//nickName ler setlenir
    printf("Mac sonuclarini manuel(1) mi girmek istersiniz yoksa dosyadan okutmak(0) mi istersiniz? :");
    scanf("%d",&secim);
    set_name(t_sayi);//name ler setlenir

    if(secim==1){
    for(i=0;i<t_sayi;i++){//dep gelenler o lanir
        for(j = 0;j < sizeof(takim[i].dep_gelenler);j++){
            takim[i].dep_gelenler[j] = '\0';
        }
    }

        while(1){//klavyeden mac sonucu girme
                fflush(stdin);
                printf("\nMac sonucunu giriniz:");
                scanf(" %c %c %c %c",&string[0],&string[1],&string[2],&string[3]);
		//fflush(stdin);
                if(ErrorDedection(string,t_sayi)==0){
                    skor_guncelle(string,t_sayi);
                }
                else printf("Girisiniz kabul edilmedi!\n");

                printf("Tekrar girdi yapacak misiniz?(E/H) :");
                scanf(" %c",&secim1);
		fflush(stdin);
                if(secim1=='H') break;
            }
    }
    else if(secim==0){//dosyadan mac sonucu okuma
        for(i=0;i<t_sayi;i++){//dep_gelenler 0 lanir
            for(j = 0;j < sizeof(takim[i].dep_gelenler);j++){
                takim[i].dep_gelenler[j] = '\0';
            }
        }

        FILE *maclar=fopen("maclar.txt","r");
        do{
            fscanf(maclar,"%c %c %c %c\n",&string[0],&string[1],&string[2],&string[3]);
            if(ErrorDedection(string,t_sayi)==0){
                    skor_guncelle(string,t_sayi);
                }
            else printf("%c %c %c %c Girisiniz kabul edilmedi!\n",string[0],string[1],string[2],string[3]);
        }while(!feof(maclar));
    }

    skor_guncelle2(g_puan,b_puan,m_puan,t_sayi);
    fiksturYazdir(t_sayi);
}

int main()
{
    setlocale(LC_ALL,"Turkish");
    girdiler();
    free(takim);

    return 0;
}
