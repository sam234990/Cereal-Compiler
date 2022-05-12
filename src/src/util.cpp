#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void write_file(string file_name, string file_content) {
    string write_file_name = file_name;
    ofstream os;     //创建一个文件输出流对象
    os.open(write_file_name, ios::out);//将对象与文件关联
    cout<<"向文本中写入内容！"<<endl;
    os<<file_content;   //将输入的内容放入txt文件中
    os.close();
    return;
}
