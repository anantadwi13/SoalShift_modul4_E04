# SoalShift_modul4_E04

1.
Semua nama file dan folder harus terenkripsi
Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:

qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0

Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:
“INI_FOLDER/halo” saat belum di-mount maka akan bernama “n,nsbZ]wio/QBE#”, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi “INI_FOLDER/halo” .
Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan

Jawab:

Jadi, pertama membuat dulu array untuk menampung character yang sudah ada.

char cipher[] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";

setelah itu, membuat fungsi untuk encrypt dan decrypt nama file. File akan terenkripsi jika dalam keadaan normal dan akan terdekripsi
saat di mount di FUSE.
Encrypt:
```
char encrypt(char *x)
{
		int ind, i;
		char *ptr;

		for (i = 0; i < strlen(x); i++)
		{
				ptr = strchr(cipher, x[i]);

				if(ptr) 
				{
						ind = ptr - cipher;
						x[i] = cipher[(ind + key) % strlen(cipher)];
				}
		}

		return *x;
}
```
Decrypt:
```
char decrypt(char *y)
{
		int ind, i;
		char *ptr;

		for (i = 0; i < strlen(y); i++)
		{
				ptr = strchr(cipher, y[i]);

				if(ptr) 
				{
						ind = ptr - cipher - key;
						if (ind < 0)
						{
								ind = ind + strlen(cipher);
						}
						y[i] = cipher[ind];
				}
		}

		return *y;
}
```

Fungsi encrypt dan decrypt ini akan dipakai di dalam fungsi-fungsi FUSE yang lainnya. Contohnya saja adalah create (untuk touch),
mkdir, utimens, rmdir, dsb. Tujuannya adalah untuk mengenkripsi nama-nama file pada saat tidak di mount. Kemudian pada saat di mount akan
di decrypt agar terlihat seperti semula.

Jadi cara encrypt nya adalah menggunakan strchr untuk mendapatkan index huruf yang akan dienkripsi. Kemudian akan dilihat key nya, dan pada saat mengenkripsi, index tersebut akan ditambah dengan key agar bergerak ke kanan. Apabila lebih dari batasnya, akan mulai dari paling kiri. Sehingga caranya adalah dengan menggunakan modulo. Hal yang sama dilakukan juga untuk decrypt hanya saja dikurangi dan tidak ditambah agar bergerak ke kiri.\


4.
Pada folder YOUTUBER, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.iz1”. File berekstensi “.iz1” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”

Jawab:

Untuk membuat folder youtuber, menggunakan fungsi mkdir yang sudah diimplementasikan sebelumnya. Pada saat mkdir, dilakukan enkripsi terlebih dahulu dengan cara:

```
char fpath[1000], temp[1000];
strcpy(temp, path);
encrypt(temp);

if(strcmp(temp,"/") == 0)
{
	path=dirpath;
	sprintf(fpath,"%s",path);
}
else sprintf(fpath, "%s%s",dirpath,temp);
```

Kemudian, jika di dalam folder YOUTUBER tersebut akan dibuat folder, permissionnya akan berubah otomatis menjadi 750. Di dalam mkdir, diberi kondisi if untuk mengecek apakah folder tersebut berada di dalam folder YOUTUBER atau tidak.

```
if(strstr(path,"/YOUTUBER") != 0)
{
	res = mkdir(fpath, 0750);
}
else
{
	res = mkdir(fpath, mode);
}
```

0750 disana yang dimaksud adalah permissionnya. Mkdir memiliki argument mode yang bertujuan untuk menentukan permission pada saat pertama kali dibuat. Jika dalam file YOUTUBER, maka otomatis pada saat dibuat, folder tersebut akan memiliki permission 750.

Kemudian untuk file, permissionnya otomatis dibuat menjadi 640 dan juga ditambahi .iz1 sebagai extensionnya. Untuk menambahi permission caranya adalah:
```
if(strstr(path,"/YOUTUBER") != 0)
{
	res = open(fpath, fi->flags, 0640 );
}
else
{
	res = open(fpath, fi->flags, mode);
}
```
Sama seperti sebelumnya, jika didalam folder YOUTUBER, maka file akan diganti permissionnya menjadi 0640.

Kemudian untuk menambahkan extension .iz1 akan digunakan pengecekan ini saat sebelum encrypt:

```
if(strstr(path,"/YOUTUBER") != 0)
{
	strcat(path,".iz1");
}
```
Tujuannya adalah untuk menambahkan .iz1 ke file yang ada di dalam folder YOUTUBER

Kemudian, dalam chmod ditambahi kondisi lagi agar file berekstensi .iz1 tidak bisa di chmod. Caranya adalah:

```
if(strstr(path,"/YOUTUBER") != 0 && strstr(path,".iz1") != 0)
{
	pid_t child_id;

	child_id = fork();

	if (child_id == 0) 
	{
		// this is child

		char *argv[] = {"zenity", "--error", "--text", "File ekstensi iz1 tidak boleh diubah permissionnya.", NULL};
		execv("/usr/bin/zenity", argv);
	} 
	else 
	{
		// this is parent

		return 0;
	}

}
```
Digunakan fork untuk bisa melakukan exec di child nya. Untuk menampilkan file error, maka digunakan zenity. Zenity nanti akan mengeluarkan popup error.
