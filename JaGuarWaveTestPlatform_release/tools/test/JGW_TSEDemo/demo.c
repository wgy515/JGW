#include <stdio.h>


// int get_file_size_to_file(FILE* fp)
// {
//     int file_size;
// 
//     if (!fp) return 0;
//     fseek (fp, 0, SEEK_END);  // seek to end of file
//     file_size = ftell (fp); // get current file pointer
//     fseek (fp, 0, SEEK_SET);  // seek back to beginning of file
// 
//     return file_size;
// }
// 
// bool add_mac(char* mac)
// {
//     FILE* fp = NULL;
//     int file_size = 0;
//     char* file_buf = NULL;
//     const char* p = NULL;
// 
//     if ((fp = fopen("d:\\net","a+")) == NULL)
//     {
//         return false;
//     }
// 
//     file_size = get_file_size_to_file(fp);
//     //! 文件不为空的时候
//     if (0 != file_size)
//     {
//         file_buf = (char*)calloc(file_size + 1, 1);
//         //! 如果文件读取错误 则这直接返回false
//         if (1 != fread (file_buf, file_size, 1,fp))
//         {
//             free(file_buf);
//             return false;
//         }
//         //! 查找当前mac是否已经在当前文件中
//         p = strstr(file_buf,mac);
//         //! 如果存在则直接返回否则
//         if (NULL != p)
//         {
//             free(file_buf);
//             fclose(fp);
//             return true;
//         }
//         else
//         {
//             fseek (fp, 0, SEEK_END);  // seek to end of file
//         }
//     }
//     //! 写mac
//     if (1 != fwrite(mac,strlen(mac),1,fp))
//     {
//         return false;
//     }
//     //! 写入空格分隔符
//     if (1 != fwrite(" ",1,1,fp))
//     {
//         return false;
//     }
//     //! 
//     if(NULL != file_buf)
//     {
//         free(file_buf);
//     }
//     fclose(fp);
//     return true;
// }
// 
// bool dec_mac(char* mac)
// {
//     FILE* fp = NULL;
//     int file_size = 0;
//     char* file_buf = NULL;
//     char* file_write_buf = NULL;
//     const char* p = NULL;
//     int i = 0;
//     int index = 0;
// 
//     if ((fp = fopen("d:\\net","a+")) == NULL)
//     {
//         return false;
//     }
// 
//     file_size = get_file_size_to_file(fp);
//     //! 文件为空的时候
//     if (0 == file_size)
//     {
//         fclose(fp);
//         return true;
//     }
// 
//     file_buf = (char*)calloc(file_size + 1, 1);
//     //! 如果文件读取错误 则这直接返回false
//     if (1 != fread (file_buf, file_size, 1,fp))
//     {
//         free(file_buf);
//         return false;
//     }
//     //! 查找当前mac是否已经在当前文件中
//     p = strstr(file_buf,mac);
//     //! 如果不存在则直接返回
//     if (NULL == p)
//     {
//         free(file_buf);
//         fclose(fp);
//         return true;
//     }
// 
//     file_write_buf = (char*)calloc(file_size + 1, 1);
//     //! 复制前面部分
//     for (i = 0;i < p - file_buf;i ++,index ++)
//     {
//         file_write_buf[index] = file_buf[i];
//     }
//     //! 复制后面部分
//     for (i = p - file_buf + strlen(mac) + 1;i < file_size + 1;i ++)
//     {
//         file_write_buf[index ++] = file_buf[i];        
//     }
//     fclose(fp);
//     i = 0;
// 
//     fp = fopen("d:\\net","w+");
// 
//     //! 写mac
//     if (NULL != fp)
//         i = fwrite(file_write_buf,strlen(file_write_buf),1,fp);
//     //! 
//     if(NULL != file_buf)
//     {
//         free(file_buf);
//     }
// 
//     if(NULL != file_write_buf)
//     {
//         free(file_write_buf);
//     }
//     fclose(fp);
// 
//     return (1 == i);
// }

/**
 * 编码
 */
int encodeFile(char *oldpath, char *newpath){
	FILE *pfr, *pfw;
    int len = 0,i = 0;
	pfr = fopen(oldpath, "rb");
	pfw = fopen(newpath, "wb");
	if (pfr == NULL || pfw == NULL)
	{
		fclose(pfr);
		fclose(pfw);
		return 0;
	}else{
        char p[1024] = {0};
        len = fread(p, sizeof(char), 1024, pfr);
        while (0 != len) {
            for (i = 0; i < len; i++){
                p[i] ^= 'S';
            }
            fwrite(p, sizeof(char), len, pfw);
            len = fread(p, sizeof(char), 1024, pfr);
        }
        fclose(pfr);
        fclose(pfw);
        return 1;
	}
}

int main()
{
    encodeFile("c:/update.bin","c:/update.zip");
    return 0;
}