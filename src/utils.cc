#include"utils.h"


std::string json2str(Json::Value root)
{
    std::ostringstream os;
    Json::StreamWriterBuilder writerBuilder;
    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root,&os);
    return os.str();
}

Json::Value str2json(std::string str)
{
    Json::Value root;
    Json::String err;
    Json::CharReaderBuilder readerBuilder;
    std::unique_ptr<Json::CharReader> jsonReader(readerBuilder.newCharReader());
    // jsonReader.get();
    bool res=jsonReader->parse(str.c_str(),str.c_str()+str.length(),&root,&err);
    if(!res||!err.empty())
    {
        printf("str2json(): %s\n",err.c_str());
    }
    // return root;
    // std::cout<<root<<std::endl;
    return root;
}