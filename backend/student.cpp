#include <string>

using namespace std;

struct Student{
    private: 
        string name; //name
        float gpa_4; //GPA hệ 4
        float gpa_10;//GPA hệ 10
        bool gender; //true = nam, false = nữ
        string dateOfBirth; // string d/month/year sepparator bằng dấu "/"
        string major; //Tên ngành
        string MSSV; //MSSV, "aabbbccc". a = niên khóa, b = ngành, c = stt
        bool isNgu; //trong kỳ có rớt môn ko? 
        int credit; //Số tính chỉ
        int DRL; //Điểm rèn luyện

    public:
        Student(string name,
                float gpa_4, //GPA hệ 4
                float gpa_10,//GPA hệ 10
                bool gender, //true = nam, false = nữ
                string dateOfBirth, // string d/month/year sepparator bằng dấu "/"
                string major, //Tên ngành
                string MSSV, //MSSV, "aabbbccc". a = niên khóa, b = ngành, c = stt
                bool isNgu, //trong kỳ có rớt môn ko? 
                int credit, //Số tính chỉ
                int DRL){
            this->name = name;
            this->gpa_4 = gpa_4;
            this->gpa_10 = gpa_10;
            this->gender = gender;
            this->dateOfBirth = dateOfBirth;
            this->major = major;
            this->MSSV = MSSV;
            this->isNgu = isNgu;
            this->credit = credit;
            this->DRL = DRL;
        }
};