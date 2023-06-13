#include <iostream>
#include <RCF/RCF.hpp>
#include <RCF/Uuid.hpp>

// ./client <OPTION> <KEY> <VALUE>


   RCF_BEGIN(I_RocksDBService, "I_RocksDBService")
   RCF_METHOD_V2(void, PutObject, const std::string&, const std::string&)
   RCF_METHOD_R1(std::string, GetObject, const std::string&)
   RCF_METHOD_V1(void, DeleteObject, const std::string &)
   RCF_END(I_RocksDBService)


#define KEY          argv[2]
#define FILENAME     argv[3]

#define PUT_OBJECT 'c'
#define GET_OBJECT 'd'
#define DEL_OBJECT 'e'



int main(int argc, char** argv)
{
        try
        {
                RCF::RcfInit rcfInit;

                RcfClient<I_RocksDBService> client(RCF::TcpEndpoint("10.0.0.10", 50001));

                std::cout << "Connecting 10.0.0.10 on port 500001\n";

                switch(argv[1][0]) {
                        case PUT_OBJECT: {
                                                 std::string uploadId = RCF::generateUuid();
                                                 RCF::Path fileToUpload = FILENAME;
                                                 client.getClientStub().uploadFile(uploadId, fileToUpload);
                                                 client.PutObject(KEY, uploadId);
                                                 break;
                                         }
                        case GET_OBJECT: {
                                                 std::string downloadId = client.GetObject(KEY);
                                                 std::cout << downloadId << std::endl;
                                                 RCF::Path fileToDownloadTo = FILENAME;
                                                 client.getClientStub().downloadFile(downloadId, fileToDownloadTo);
                                                 break;
                                         }
                        case DEL_OBJECT: {
                                                 client.DeleteObject(KEY);
                                                 break;
                                         }
                        default:
                                         std::cout << "Invalid Option" << std::endl;
                }

        }
        catch ( const RCF::Exception & e )
        {
                std::cout << "Error: " << e.getErrorMessage() << std::endl;
        }
        return 0;
}
