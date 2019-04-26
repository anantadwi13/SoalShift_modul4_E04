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
						x[i] = cipher[(ind + 17) % strlen(cipher)];
				}
		}

		return *x;
}

char decrypt(char *y)
{
		int ind, i;
		char *ptr;

		for (i = 0; i < strlen(y); i++)
		{
				ptr = strchr(cipher, y[i]);

				if(ptr) 
				{
						ind = ptr - cipher - 17;
						if (ind < 0)
						{
								ind = ind + strlen(cipher);
						}
						y[i] = cipher[ind];
				}
		}

		return *y;
}

Fungsi encrypt dan decrypt ini akan dipakai di dalam fungsi-fungsi FUSE yang lainnya. Contohnya saja adalah create (untuk touch),
mkdir, utimens, rmdir, dsb. Tujuannya adalah untuk mengenkripsi nama-nama file pada saat tidak di mount. Kemudian pada saat di mount akan
di decrypt agar terlihat seperti semula.
