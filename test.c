/*
 * @Author:
 * @Date: 2022-09-28 06:50:50
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-11-08 06:54:04
 * @FilePath: /rootfs/test/test.c
 * @Description:
 * @SAMT_copyright: All Rights Reserved.
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_KV_KEY_SIZE 16
#define __NR_csd_syscall_temp 436
#define __NR_csd_syscall_hello 437
#define SINGLE_USER_TYPE 1
#define MUTILE_USER_TYPE 2

// command code definition
#define INITIALIZE_BUCKET 'a'
#define GET_BUCKET_INFO 'b'
#define PUT_OBJECT 'p'
#define GET_OBJECT 'g'
#define DEL_OBJECT 'e'
#define INITIALIZE_FS 'f'
#define WRITE_TEST 'w'
#define READ_TEST 'r'
#define TEST_TEMP 'd'
#define TEST_KERNEL_SOCKET 'k'
#define PUT_OBJ_FROM_REMOTE 'x'
#define GET_OBJ_TO_REMOTE 'y'
#define BASE_BUFFER_SIZE 1024 * 1024 * 16 // 16MB
#define TEST_INPUT_FILE "/board/input/test.txt"
#define TEST_OUTPUT_FILE "/board/output/test.txt"
struct buff
{
    char command[4]; // user command code
};

struct csd_params
{
    int ObjectID;
    char *data_pointer;
    char *key_name;
    int data_len;
    struct buff buffer;
    char *from_ip_address;
    int from_port;
};

struct thread_arguments
{
    char *key_name;
    char *file_path;
    int buffer_size;
    int file_size;
    char command;
};

int main(int argc, char **argv)
{
    // call_socket_client();
    multi_user_test();
    sleep(5);
    singel_user_test();
    return 0;
}

int call_socket_client(void)
{
    if (!syscall(__NR_csd_syscall_hello))
    {
        printf("kernel call the userspace's server socket");
        return 0;
    }
    else
    {
        printf("Error systemcall\n");
        return -1;
    }
}
void *thread(void *arg)
{
    struct thread_arguments *arguments = (struct thread_arguments *)arg;
    char command = arguments->command;

    if(val_size>max_val_size)
    {
            switch (command)
            {
                case 'p':
                    putobject(arguments->key_name, arguments->file_path, arguments->buffer_size, arguments->file_size);
                    break;
                case 'g':
                    getobject(arguments->key_name, arguments->file_path, arguments->buffer_size, arguments->file_size);
                    break;
                default:
                    break;
            }
    }
    else
    {
        rocksdb::Options options;
        options.create_if_missing = true;
        options.env = new rocksdb::OSLEnv();

        rocksdb::DB* db;
        rocksdb::Status status = rocksdb::DB::Open(options, kDBPath, &db);
        if (!status.ok()) {
                std::cerr << "Failed to open database: " << status.ToString() << std::endl;
                return 1;
        }

        std::string key = "my_key";
        std::string value = "my_value";

        rocksdb::Slice key_slice(key);
        rocksdb::Slice value_slice(value);

        status = db->Put(rocksdb::WriteOptions(), key_slice, value_slice);
        if (!status.ok()) {
                std::cerr << "Failed to put object: " << status.ToString() << std::endl;
        }

        delete db;
        delete options.env;


    }
    
}

void multi_user_test(void)
{
    int i;
    char *obj[] = {"1", "2", "3", "4", "5"};
    for (i = 0; i < 5; ++i)
    {
        pthread_t th;
        int ret;
        struct thread_arguments *arguments = malloc(1024 * 1024 * 512);
        arguments->command = 'p';
        arguments->key_name = obj[i];
        arguments->file_path = TEST_INPUT_FILE;
        arguments->buffer_size = 1024 * 1024 * 128;
        arguments->file_size = 94167;
        ret = pthread_create(&th, NULL, thread, arguments);
        if (ret != 0)
        {
            printf("Create thread error!\n");
            return -1;
        }
        // pthread_join(th,NULL);
    }
}

void singel_user_test(void)
{
    // get
    getobject("1", TEST_OUTPUT_FILE, 1024 * 1024 * 1024, 94167);
}

int getobject(char *key_name, char *filepath, int buffer_size, int file_char_size) //将读取到的char*写入到文件中
{
    char *value = (char *)malloc(buffer_size); //新开一个value2
    struct csd_params user_para;
    user_para.buffer.command[0] = 'g';
    user_para.key_name = key_name;
    user_para.data_pointer = value;
    user_para.key_name = key_name;
    if (!syscall(__NR_csd_syscall_temp, (void *)&user_para))
    {
        printf("getobject successfull\n");
        FILE *fp = fopen(filepath, "wb+");
        if (fp == NULL)
        {
            printf("打开文件失败 %s\n", filepath);
            return -1;
        }
        //implement a value writing way after getting the object.
        return 0;
    }
    else
    {
        printf("Error systemcall\n");
        free(value);
        return -1;
    }
}

int putobject(char *key_name, char *filepath, int buffer_size, int file_char_size)
{
    char *value;
    value = (char *)malloc(buffer_size); // 128M
    value = argv[0]; // take input file from command line and pass as value
    struct csd_params user_para;
    user_para.buffer.command[0] = 'p';
    user_para.data_pointer = value;
    user_para.data_len = file_char_size;
    user_para.key_name = key_name;
    fclose(fp);
    if (!syscall(__NR_csd_syscall_temp, (void *)&user_para))
    {
        printf("putobj successfull of file %s \n", filepath);
        free(value);
        return 0;
    }
    else
    {
        printf("Error systemcall\n");
        free(value);
        return -1;
    }
}
