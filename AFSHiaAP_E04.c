#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>

static const char *dirpath = "/home/arisatox/shift4";
char cipher[] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
pthread_t tid;

char decrypt(char *x)
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

char encrypt(char *y)
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

static int xmp_mkdir(const char *path, mode_t mode)
{
	char fpath[1000], temp[1000];
	strcpy(temp, path);
	decrypt(temp);

	if(strcmp(temp,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,temp);

	int res;

	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
	char fpath[1000], temp[1000];

	strcpy(temp, path);

	decrypt(temp);
	
	sprintf(fpath,"%s%s",dirpath,temp);

	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  	char fpath[1000], temp[1000];
	strcpy(temp, path);
	decrypt(temp);

	if(strcmp(temp,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,temp);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		encrypt(de->d_name);

		res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  char fpath[1000], temp[1000];
	strcpy(temp, path);
	decrypt(temp);

	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,temp);
	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.mkdir		= xmp_mkdir,
};

int getLastCharPos(char *str, char chr){
	char *posChar = NULL;
	char *tempPosChar = strchr(str, chr);
	
	while(tempPosChar != NULL){
		posChar = tempPosChar;

		tempPosChar = strchr(tempPosChar+1, chr);
	}
	if(posChar==NULL)
		return 0;

	return (int) (posChar-str);
}

void* joinVideo(){
	char videoPath[1000], filePath[1000], ch;
	sprintf(videoPath, "%s/g[xO#",dirpath);
	mkdir(videoPath, 0777);

	DIR *dirvideo;
	struct dirent *de, **fileList;
	dirvideo = opendir(videoPath);
	if (dirvideo == NULL) {
		return NULL;
	}
	int n = scandir(dirpath, &fileList, 0, alphasort);
	int i = 0;
	while(i < n){
		de = fileList[i];
		i++;
		int lastDotChar = getLastCharPos(de->d_name, '.');
		if (de->d_type != DT_REG)
			continue;
		
		if (lastDotChar==0 || strlen(de->d_name)<4 || !((de->d_name[lastDotChar-3]=='m' && de->d_name[lastDotChar-2]=='k' && de->d_name[lastDotChar-1]=='v') || 
			(de->d_name[lastDotChar-3]=='m' && de->d_name[lastDotChar-2]=='p' && de->d_name[lastDotChar-1]=='4')))
			continue;

		sprintf(filePath, "%s/%s", dirpath, de->d_name);
		FILE* source = fopen(filePath, "r");

		de->d_name[lastDotChar] = '\0';
		sprintf(filePath, "%s/%s", videoPath, de->d_name);
		FILE* target = fopen(filePath, "a");

		while ((ch = fgetc(source)) != EOF)
			//if (ch!='\n') 
				fputc(ch, target);

		fclose(source);
		fclose(target);
	}
	
	return NULL;
}

void deleteVideo(){
	char videoPath[1000], filePath[1000];
	sprintf(videoPath, "%s/g[xO#",dirpath);

	DIR *dirVideo;
	struct dirent *de;
	dirVideo = opendir(videoPath);
	if (dirVideo == NULL) {
		return;
	}

	while((de = readdir(dirVideo)) != NULL){
		if (de->d_type == DT_REG) {
			sprintf(filePath, "%s/%s", videoPath, de->d_name);
			remove(filePath);
		}
	}
	remove(videoPath);
}

int main(int argc, char *argv[])
{
	umask(0);
	pthread_create(&tid,NULL,&joinVideo,NULL);
	fuse_main(argc, argv, &xmp_oper, NULL);
	deleteVideo();
	return 1;
}