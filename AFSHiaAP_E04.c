#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <pthread.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int key;
static const char *dirpath = "/home/arisatox/shift4";
char cipher[] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
pthread_t tid;

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

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
	char fpath[1000], temp[1000];
	strcpy(temp, path);
	if(strstr(path,"/YOUTUBER") != 0 && strstr(path,".iz1") != 0)
	{
		printf("----------------------------%s---------------------------\n", temp);
		printf("----------------------------MASUK YOUTUBER---------------------------\n");

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
	
	encrypt(temp);

	if(strcmp(temp,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,temp);

	res = chmod(fpath, mode);
	
	if (res == -1)
		return -errno;

	return 0;
}

 static int xmp_create(const char *path, mode_t mode,
                       struct fuse_file_info *fi)
 {
        char fpath[1000], temp[1000];

		if(strstr(path,"/YOUTUBER") != 0)
		{
			strcat(path,".iz1");
		}
		
		strcpy(temp, path);
		encrypt(temp);

		if(strcmp(temp,"/") == 0)
		{
			path=dirpath;
			sprintf(fpath,"%s",path);
		}
		else sprintf(fpath, "%s%s",dirpath,temp);
			
		int res;

		if(strstr(path,"/YOUTUBER") != 0)
		{
			res = open(fpath, fi->flags, 0640 );
		}
		else
		{
			res = open(fpath, fi->flags, mode);
		}
 
        if (res == -1)
                return -errno;
 
        fi->fh = res;
        return 0;
 }

static int xmp_truncate(const char *path, off_t size)
{
	char fpath[1000], temp[1000];
	strcpy(temp, path);
	encrypt(temp);

	if(strcmp(temp,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,temp);
	
	int res;

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	char fpath[1000], temp[1000];

		if(strstr(path,"/YOUTUBER") != 0)
		{
			strcat(path,".iz1");
		}
		
		strcpy(temp, path);
		encrypt(temp);

	if(strcmp(temp,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,temp);

	int res;

	res = utimensat(0, fpath, ts, AT_SYMLINK_NOFOLLOW);
	
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	char fpath[1000], temp[1000];
	strcpy(temp, path);
	encrypt(temp);

	if(strcmp(temp,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,temp);

	int res;

	if(strstr(path,"/YOUTUBER") != 0)
	{
		res = mkdir(fpath, 0750);
	}
	else
	{
		res = mkdir(fpath, mode);
	}
	
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
	char fpath[1000], temp[1000];
	strcpy(temp, path);
	encrypt(temp);

	if(strcmp(temp,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,temp);
	
	int res;

	res = rmdir(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	char fpath[1000], temp[1000];
	strcpy(temp, path);
	encrypt(temp);

	if(strcmp(temp,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,temp);

	int res, isFile, status;


	//-----------------------SOAL NO 5

	isFile = access(fpath, F_OK);


	//printf("====================UNLINK=======%s  ====  %s\n", path, fpath);
	if(isFile<0)				//JIKA BUKAN FILE
		return 0;
	printf("====================UNLINK=======%s  ====  %s\n", path, fpath);
	char backup[] = "Backup", pathBackup[1000];
	char command[1000], timestamp[100], namaFileZip[1000], namaFile[1000], ext[1000],
			namaFileWithoutExt[1000], pathNow[1000], namaRecycleBin[]="RecycleBin";
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(timestamp, "%04d-%02d-%02d_%02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);


	encrypt(backup);
	strncpy(pathBackup, path, getLastCharPos(path, '/'));
	pathBackup[getLastCharPos(path, '/')] = '\0';
	sprintf(temp, "%s/%s", pathBackup, backup);
	strcpy(pathBackup, temp);
	//sprintf(temp, "%s%s", dirpath, pathBackup);
	
	//decrypt(path);
	int posSlash = getLastCharPos(path, '/');
	int posDot = getLastCharPos(path, '.');
	
	if (posDot==0)
		posDot = strlen(path);
	else{
		strcpy(ext, path+posDot);
		if (strcmp(ext, ".swp")==0)		//PREVENT .swp file to load
		{
			res = unlink(fpath);
			
			if (res == -1)
				return -errno;
			return 0;
		}
	}
	strcpy(namaFile, path+posSlash+1);
	strncpy(namaFileWithoutExt, path+posSlash+1, posDot-(posSlash+1));
	namaFileWithoutExt[posDot-(posSlash+1)] = '\0';

	strncpy(temp, path, getLastCharPos(path, '/'));
	temp[getLastCharPos(path, '/')] = '\0';
	sprintf(pathNow, "%s%s", dirpath, temp);


	sprintf(namaFileZip, "%s_deleted_%s.zip\0", namaFileWithoutExt, timestamp);
	encrypt(namaFileZip);
	encrypt(namaRecycleBin);
	encrypt(namaFile);
	encrypt(namaFileWithoutExt);
	sprintf(command, "cd %s && mkdir -p '%s' && zip '%s/%s' '%s' '%s/%s'* && rm -f '%s/%s'*", pathNow, namaRecycleBin,namaRecycleBin, namaFileZip, namaFile, backup, namaFileWithoutExt, backup, namaFileWithoutExt);

	printf("=================COMMAND=======%s\n", command);
	if (fork()==0)
		execl("/bin/sh","/bin/sh", "-c", command, NULL);

	while((wait(&status))>0);
	//-----------------------SOAL NO 5

	res = unlink(fpath);
	
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  	int res; 
	//int len;
	char fpath[1000], temp[1000];

	strcpy(temp, path);

	/*
	len = strlen(temp);
	//printf("TEMP2 ---------------------- %s %d\n", temp2, (int)strlen(temp2));	
	
	if (temp[len-1] == '1' && temp[len-2] == 'z' && temp[len-3] == 'i' && temp[len-4] == '.')
	{
		temp[len-4] = '\0';
		//printf("MASUKKKKK SIIIINIIIIIIIIIIIIIIII -------------------------------- \n");
		//printf("TEMP2 MODIFIED---------------------- %s %d\n", temp2, (int)strlen(temp2));
	} 
	 //printf("ORIGINAL ----------------------- %s %d\n", fpath, len);	
	*/

	encrypt(temp);
	sprintf(fpath,"%s%s",dirpath,temp);
	res = lstat(fpath, stbuf);

	if (res == -1)
	{
		return -errno;
	}
		
	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  	char fpath[1000], temp[1000];
	strcpy(temp, path);
	encrypt(temp);

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

	char pathFileMiris[200], nameFileMiris[] = "filemiris.txt";
	encrypt(nameFileMiris);
	sprintf(pathFileMiris, "%s/%s", dirpath, nameFileMiris);
	FILE *filemiris = fopen(pathFileMiris, "a");

	while ((de = readdir(dp)) != NULL) {
		struct stat st, info;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		if ( strcmp(de->d_name,".") == 0 || strcmp(de->d_name,"..") == 0 )
		{
			continue;
		}

		sprintf(temp, "%s/%s", fpath, de->d_name);
		stat(temp, &info);
		struct passwd *pw = getpwuid(info.st_uid);
		struct group  *gr = getgrgid(info.st_gid);
		int readable = access(temp, R_OK);			//return 0 if it is readable
		char date[30];

 		if (de->d_type == DT_REG && strcmp(temp, pathFileMiris)!=0 && readable!=0 && (strcmp(pw->pw_name, "chipset") || strcmp(pw->pw_name, "ic_controller")) && strcmp(gr->gr_name, "rusak")) {
			strftime(date, 30, "%Y-%m-%d %H:%M:%S", localtime(&(info.st_atime)));
			decrypt(de->d_name);
			fprintf(filemiris, "%s\t\t%d\t\t%d\t\t%s\n", de->d_name, gr->gr_gid, pw->pw_uid, date);
			remove(temp);
			continue;
		}

		decrypt(de->d_name);

		/*
		printf("-----------------DNAME %s-------------------\n", de->d_name);
		printf("-----------------FPATH %s-------------------\n", fpath);
		/*
		if(strstr(path,"/YOUTUBER") != 0)
		{
			if(S_ISREG(st.st_mode))
        		strcat(de->d_name,".iz1");
		}
		*/

		res = (filler(buf,de->d_name, &st, 0));
		if(res!=0) break;
		
	}

	fclose(filemiris);
	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  	char fpath[1000], temp[1000];
	strcpy(temp, path);
	encrypt(temp);

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

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000], temp[1000];
	
	if(strstr(path,"/YOUTUBER") != 0)
	{
		strcat(path,".iz1");
	}
	strcpy(temp, path);	
	encrypt(temp);

	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,temp);

	int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

	encrypt(path);
	sprintf(temp, "%s/%s", dirpath,path);
	if(access(temp, R_OK)<0)				//JIKA FILE TIDAK ADA
		return res;

	printf("===============FILEPATH========%s\n", path);
	char backup[] = "Backup", pathBackup[1000];
	encrypt(backup);
	strncpy(pathBackup, path, getLastCharPos(path, '/'));
	pathBackup[getLastCharPos(path, '/')] = '\0';
	sprintf(temp, "%s/%s", pathBackup, backup);
	strcpy(pathBackup, temp);
	sprintf(temp, "%s%s", dirpath, pathBackup);
	printf("==================PATH BACKUP ===========%s\n", pathBackup);
	mkdir(temp, 0777);


	decrypt(path);
	char filePathWithoutExt[1000], ext[100], timestamp[1000], fileNameBackup[1000], ch;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(timestamp, "%04d-%02d-%02d_%02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	int posSlash = getLastCharPos(path, '/');
	int posDot = getLastCharPos(path, '.');
	
	if (posDot==0) {
		posDot = strlen(path);
		ext[0] = '\0';
	}
	else{
		strcpy(ext, path+posDot);
		if (strcmp(ext, ".swp")==0)		//PREVENT .swp file to load
			return res;
	}
	strncpy(filePathWithoutExt, path+posSlash+1, posDot-(posSlash+1));
	filePathWithoutExt[posDot-(posSlash+1)] = '\0';
	
	sprintf(fileNameBackup,"%s_%s%s", filePathWithoutExt, timestamp, ext);
	printf("===============PATH========%s\n", path);
	printf("===============PATH========%d=====%d\n", posSlash, posDot);
	printf("===============FILEPATH EDIT=======%s\n", fileNameBackup);
	encrypt(fileNameBackup);
	encrypt(path);
	sprintf(temp, "%s%s", dirpath, path);
	printf("==========DIR SOURCE========%s\n", temp);
	FILE *source = fopen(temp, "r");

	sprintf(temp, "%s%s/%s", dirpath, pathBackup, fileNameBackup);
	printf("==========DIR TARGET========%s\n", temp);
	FILE *target = fopen(temp, "w");

	while ((ch = fgetc(source)) != EOF)
		fprintf(target, "%c", ch);

	fclose(target);
	fclose(source);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.mkdir		= xmp_mkdir,
	.create		= xmp_create,
	.chmod		= xmp_chmod,
	.utimens	= xmp_utimens,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.write		= xmp_write,
	.truncate	= xmp_truncate,
};

void* joinVideo(){
	char video[] = "Video";
	char videoPath[1000], filePath[1000], ch;

	encrypt(video);
	sprintf(videoPath, "%s/%s",dirpath, video);

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
		decrypt(de->d_name);
		int lastDotChar = getLastCharPos(de->d_name, '.');
		if (de->d_type != DT_REG)
			continue;

 		if (lastDotChar==0 || strlen(de->d_name)<4 || !((de->d_name[lastDotChar-3]=='m' && de->d_name[lastDotChar-2]=='k' && de->d_name[lastDotChar-1]=='v') || 
			(de->d_name[lastDotChar-3]=='m' && de->d_name[lastDotChar-2]=='p' && de->d_name[lastDotChar-1]=='4')))
			continue;
		
 		de->d_name[lastDotChar] = '\0';
		encrypt(de->d_name);
		sprintf(filePath, "%s/%s", videoPath, de->d_name);
		printf("==========FILEPATHNYA=========%s\n", filePath);
		if (access(filePath, F_OK) != -1)
			continue;
		
		FILE* target = fopen(filePath, "a");

		decrypt(de->d_name);
		for(int i = 0; i <= 999; i++)
		{
			char namaFile[1000];
			sprintf(namaFile, "%s.%03d",de->d_name, i);
			encrypt(namaFile);

 			sprintf(filePath, "%s/%s", dirpath, namaFile);
			if (access(filePath, F_OK) < 0)
				break;

			FILE* source = fopen(filePath, "r");

			while ((ch = fgetc(source)) != EOF)
				fprintf(target, "%c", ch);

			fclose(source);
		}
		fclose(target);
	}

 	return NULL;
}

void deleteVideo(){
	char video[] = "Video";
	char videoPath[1000], filePath[1000];

	encrypt(video);
	sprintf(videoPath, "%s/%s",dirpath, video);

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
	printf("Input key: ");
	scanf("%d", &key);
	umask(0);
	pthread_create(&tid,NULL,&joinVideo,NULL);
	fuse_main(argc, argv, &xmp_oper, NULL);
	deleteVideo();
	return 1;
}
