#include <iostream>
#include <RCF/RCF.hpp>
#include <rocksdb/c.h>
#include "rocksdb/osl_env.h"

RCF_BEGIN(I_RocksDBService, "I_RocksDBService")
RCF_METHOD_V2(void, PutObject, const std::string&, const std::string&)
RCF_METHOD_R1(std::string, GetObject, const std::string&)
RCF_METHOD_V1(void, DeleteObject, const std::string &)
RCF_END(I_RocksDBService)


rocksdb_t* db;

void insert_large_file(const char *key, const char *filename) {

        FILE *file = fopen(filename, "rb");

        if(file) {

                char *err = NULL;
                const int buffer_size = 1024 * 1024;
                char buffer[buffer_size];

                rocksdb_put(db, rocksdb_writeoptions_create(), key, strlen(key), filename, strlen(filename), &err);

                size_t chunk_number = 0;
                while (!feof(file)) {
                        size_t bytes_read = fread(buffer, 1, buffer_size, file);
                        if (bytes_read > 0) {
                                char part_key[100];
                                snprintf(part_key, sizeof(part_key), "key_%d", chunk_number);
                                rocksdb_put(db, rocksdb_writeoptions_create(), part_key, strlen(part_key), buffer, bytes_read, &err);
                                chunk_number++;
                        }
                }

                fclose(file);
        } else {
                std::cout << "Error opening " << filename << std::endl;
        }
}

char* retrieve_large_file(const char *key) {

        char *err = NULL;
        char *buffer;
        size_t buffer_size;
        size_t chunk_number = 0;

        char *filename =  rocksdb_get(db, rocksdb_readoptions_create(), key, strlen(key), &buffer_size, &err);
        FILE *file = fopen(filename, "wb");

        if(file) {
                char part_key[100];
                while (1) {
                        snprintf(part_key, sizeof(part_key), "key_%d", chunk_number);
                        buffer = rocksdb_get(db, rocksdb_readoptions_create(), part_key, strlen(part_key), &buffer_size, &err);
                        if (buffer == NULL) {
                                break;
                        }

                        fwrite(buffer, 1, buffer_size, file);
                        free(buffer);
                        chunk_number++;
                }

                fclose(file);
                return filename;
        } else {
                std::cout << "Error opening " << filename << std::endl;
                return NULL;
        }
}

class RocksDBService
{
        public:
                void PutObject(std::string key, std::string uploadId)
                {
                        std::cout << "PutObject() is called" << std::endl;
                        RCF::Path pathToUpload = RCF::getCurrentRcfSession().getUploadPath(uploadId);

                        insert_large_file(key.c_str(), pathToUpload.c_str());
                        std::filesystem::remove(pathToUpload);
                }

                std::string GetObject(std::string key)
                {
                        std::cout << "GetObject() is called" << std::endl;
                        char *filename = retrieve_large_file(key.c_str());

                        if(filename) {
                                RCF::Path fileToDownload = filename;
                                std::string downloadId = RCF::getCurrentRcfSession().configureDownload(fileToDownload);

                                free(filename);
                                return downloadId;
                        } else {
                                return "";
                        }
                }

                void DeleteObject(const std::string & s)
                {
                        std::cout << "DeleteObject() is called" << std::endl;
                }

};
int main()
{
        try
        {
                rocksdb_options_t* options = rocksdb_options_create();
                rocksdb_options_set_create_if_missing(options, 1);

                options.env = new rocksdb::OSLEnv();

                char* err = NULL;
                db = rocksdb_open(options, "/tmp/rocksdb", &err);

                RCF::RcfInit rcfInit;
                RCF::RcfServer server(RCF::TcpEndpoint("0.0.0.0", 50001));
                RocksDBService rocksDBService;
                server.bind<I_RocksDBService>(rocksDBService);
                server.setUploadDirectory("/tmp");

                server.start();
                std::cout << "Listening on port 50001" << std::endl;
                std::cout << "Press Enter to exit..." << std::endl;
                std::cin.get();
                server.stop();

                rocksdb_options_destroy(options);
                rocksdb_close(db);
        }
        catch ( const RCF::Exception & e )
        {
                std::cout << "Error: " << e.getErrorMessage() << std::endl;
        }
        return 0;
}


